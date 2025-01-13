/**
 * @file main.cpp
 * @author bakraw@github.com
 * @version 0.1
 * @date 2025-01-13
 * 
 * Implémentation d'un algorithme de compression d'image via k-means,
 * réalisée pour un projet universitaire.
 * 
 * OpenCV est très lourde et franchement overkill pour l'usage qu'on en fait ici, mais je l'ai utilisé pour
 * me familiariser avec la librairie, qui est le standard. Objectivement, CImg aurait sans doute été plus adaptée
 * et aurait permis de gagner quelques secondes d'exécution au total.
 * 
 * Je sais qu'OpenCV possède déjà une implémentation de k-means ; on ne s'en servira évidemment pas ici, étant 
 * donné qu'il s'agit d'un projet d'apprentissage.
 **/


///////////////////////////////////// INCLUDES /////////////////////////////////////


// Standard
#include <iostream>
#include <vector>
#include <random>

// Externe
#include <opencv2/opencv.hpp>

// Interne
#include "Image.hpp"


///////////////////////////////////// CONSTANTES /////////////////////////////////////


// Seuil de convergence
// Si le déplacement moyen des centroïdes est inférieur à ce seuil, on break.
// Je ne recommande pas de la baisser plus, car cela peut rendre l'algorithme beaucoup plus lent pour
// un gain de précision minime.
static constexpr double EPSILON{75e-2};


// Nombre maximum d'itérations
// Etant donné la lenteur de l'algorithme, le garder assez bas.
// Une vingtaine d'itérations suffit généralement à obtenir un résultat correct.
static constexpr uint16_t MAX_ITERATIONS{20};


// Debug mode
// Si true, on affiche des infos telles que les centroïdes à chaque itération, etc.
static constexpr bool DEBUG{false};


///////////////////////////////////// MAIN /////////////////////////////////////


int main(const int argc, const char** argv) {
    // Vérification du nombre d'arguments
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << "[chemin vers l'image] [valeur de k]" << std::endl;
        return -1;
    }

    // Chargement de l'image
    Kmeans::Image image(argv[1], DEBUG);
    if (!image.isValid()) {
        std::cerr << "ERREUR -> Impossible de charger l'image." << std::endl;
        return EXIT_FAILURE;
    }

    // Vérification de la valeur de k
    int k;
    try {
        k = std::stoi(argv[2]);
        if (k <= 0) {
            std::cerr << "ERREUR -> k doit être positif non nul." << std::endl;
            return EXIT_FAILURE;
        }
    } catch (const std::exception& e) {
        std::cerr << "ERREUR -> Valeur de k invalide. Doit être un entier positif non nul." << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << ">>> Compression..." << std::endl;
    image.compress(k, EPSILON, MAX_ITERATIONS);

    std::cout << ">>> Affichage..." << std::endl;
    image.draw();

    std::cout << ">>> Sauvegarde..." << std::endl;
    image.save("compressed.png");

    return EXIT_SUCCESS;
}