/**
 * @file Image.cpp
 * @author @bakraw
 * @version 0.1
 * @date 2025-01-13
 * 
 * Définition de la classe Image.
 **/


///////////////////////////////////// HEADER /////////////////////////////////////


#include "Image.hpp"


///////////////////////////////////// CONSTRUCTEUR & DESTRUCTEUR /////////////////////////////////////


Kmeans::Image::Image(std::string_view path, const bool &debug) 
                    : debug_{debug} {
    // Chargement de l'image. Pas de check ici, on le fait dans isValid().
    this->data_ = cv::imread(path.data());

    // Mise en place de l'aléatoire
    std::random_device rd;
    std::mt19937 rng(rd());
    this->rng_ = rng;
}


Kmeans::Image::~Image() {
    this->data_.release();
    cv::destroyAllWindows();
}


///////////////////////////////////// MÉTHODES /////////////////////////////////////


bool Kmeans::Image::isValid() const {
    return !this->data_.empty();
}


std::vector<cv::Vec3f> Kmeans::Image::generateCentroids(const uint16_t k) {
    std::vector<cv::Vec3f> centroids;

    for (unsigned int i{0}; i < k; i++) {
        // Distribution uniforme selon les dimensions de l'image
        std::uniform_int_distribution<int> heightDistribution(0, this->data_.rows - 1);
        std::uniform_int_distribution<int> widthDistribution(0, this->data_.cols - 1);

        int y = heightDistribution(this->rng_);
        int x = widthDistribution(this->rng_);

        // Récupération de la couleur du pixel à ces coordonnées
        cv::Vec3b pixelColor = this->data_.at<cv::Vec3b>(y, x);
        if (pixelColor != cv::Vec3b{0, 0, 0}) {
            centroids.push_back(pixelColor);
        } else {
            i--; // On réessaie si le pixel est noir
        }
    }

    if (this->debug_) {
        std::cout << "-------------GENERATION DE CENTROIDS-------------\n";
        for (const cv::Vec3f& centroid : centroids) {
            std::cout << "Centroid: " << centroid << "\n";
        }
    }

    return centroids;
}


double Kmeans::Image::distanceToCentroid(const cv::Vec3b &color, const cv::Vec3f &centroid) const {
    return cv::norm(static_cast<cv::Vec3f>(color) - centroid); // Norme du vecteur entre les deux couleurs (-> distance euclidienne)
}


cv::Vec3f Kmeans::Image::getClosestCentroid(const cv::Vec3b &color, const std::vector<cv::Vec3f> &centroids) const {
    double minDistance = std::numeric_limits<double>::max(); // On prend la plus grande valeur possible
    cv::Vec3f closestCentroid;

    // Détermination du centroïde le plus proche
    for (const cv::Vec3f& centroid : centroids) {
        double distance = this->distanceToCentroid(color, centroid);
        if (distance < minDistance) {
            minDistance = distance;
            closestCentroid = centroid;
        }
    }

    return closestCentroid;
}


void Kmeans::Image::updatePixels(const std::vector<cv::Vec3f> &centroids) {
    #pragma omp parallel for collapse (2) schedule(dynamic) // Parallélisation (OpenMP)
    for (int y = 0; y < this->data_.rows; y++) { 
        for (int x = 0; x < this->data_.cols; x++) {
            // La couleur de chaque pixel est remplacée par le centroïde le plus proche
            cv::Vec3b& pixel = this->data_.at<cv::Vec3b>(y, x);  // La référence évite une copie
            pixel = this->getClosestCentroid(pixel, centroids);
        }
    }
}


double Kmeans::Image::updateCentroids(std::vector<cv::Vec3f> &centroids) {
    std::vector<cv::Vec3f> previousCentroids{centroids};
    for (cv::Vec3f& centroid : centroids) {
        // Pour chaque centroïde, on calcule la somme des couleurs des pixels qui lui sont assignés
        cv::Vec3f sum{0.0, 0.0, 0.0};
        int count = 0;
        
        #pragma omp parallel for collapse (2) schedule(dynamic) 
        for (int y = 0; y < this->data_.rows; y++) {
            for (int x = 0; x < this->data_.cols; x++) {
                cv::Vec3b pixel = this->data_.at<cv::Vec3b>(y, x);
                if (this->getClosestCentroid(pixel, centroids) == centroid) {
                    #pragma omp atomic // Évite les problèmes de concurrence
                    sum[0] += static_cast<double>(pixel[0]); // Pas trouvé mieux pour conserver la précision
                    #pragma omp atomic
                    sum[1] += static_cast<double>(pixel[1]);
                    #pragma omp atomic
                    sum[2] += static_cast<double>(pixel[2]);
                    #pragma omp atomic
                    count++;
                }
            }
        }

        // On fait la moyenne des couleurs pour obtenir le nouveau centroïde
        if (count > 0) {
            sum /= count;
            centroid = sum;
        }
    }

    double totalShift = 0.0;
    for (size_t i = 0; i < centroids.size(); ++i) {
        totalShift += cv::norm(centroids[i] - previousCentroids[i]);
    }

    if (this->debug_) {
        std::cout << "Déplacement moyen: " << totalShift / centroids.size() << "\n"; // Pas opti parce qu'on refait le calcul après mais osef
    }

    return totalShift / centroids.size(); // Déplacement moyen des centroïdes
}


void Kmeans::Image::compress(const uint16_t k, const double epsilon, const uint16_t maxIterations) {
    std::vector<cv::Vec3f> centroids{this->generateCentroids(k)};
    double initialCentroidShift{};

    // Boucle principale
    for (unsigned int i{}; i < maxIterations; i++) {
        if (this->debug_) {
            for (const cv::Vec3f& centroid : centroids) {
                std::cout << centroid << "\n";
            }
        }

        double centroidShift = this->updateCentroids(centroids);

        if (i == 0) {
            initialCentroidShift = centroidShift;  // On stocke le déplacement initial pour calculer la progression
        }

        std::cout << ">> Iteration " << i+1 << " / " << maxIterations << " - " << centroidShift
                  << " (" << static_cast<int>((initialCentroidShift - centroidShift) / (initialCentroidShift - epsilon) * 100) << "%)\n";
        // On met à jour les centroïdes et vérifie s'il y a convergence
        if (epsilon >= centroidShift) {
            break;
        }
    }

    this->updatePixels(centroids); // On change les couleurs des pixels une fois que les centroïdes sont déterminés
}


void Kmeans::Image::draw() const {
    cv::imshow("Image", this->data_);
    cv::waitKey(0);
}


void Kmeans::Image::save(const std::string &path) const {
    cv::imwrite(path, data_, {cv::IMWRITE_PNG_COMPRESSION, 9}); // Compression PNG max pour profiter de la quantification
}