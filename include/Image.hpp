/**
 * @file Image.hpp
 * @author @bakraw
 * @version 0.1
 * @date 2025-01-13
 * 
 * Déclaration de la classe Image, qui contient à peu près toute la logique.
 **/

#pragma once


///////////////////////////////////// INCLUDES /////////////////////////////////////


// Standard
#include <vector>
#include <random>

// Externe
#include <opencv2/opencv.hpp>
#include <omp.h>


///////////////////////////////////// DECLARATIONS /////////////////////////////////////


/**
 * Le namespace Kmeans n'est pas techniquement nécessaire, mais il permet de savoir directement
 * qu'une classe ou fonction appartient à ce projet et pas à un librairie externe.
 */
namespace Kmeans {
    /**
     * La classe Image contient les données d'une image, ainsi que les méthodes 
     * nécessaires à l'algorithme de k-means.
     * 
     * @param path (std::string_view) : Chemin vers l'image
     **/
    class Image {
    public:
        // Constructeur & destructeur
        Image(std::string_view path, const bool &debug = false);;
        ~Image();

        // Méthodes publiques
        /**
         * Vérifie que l'image est correctement chargée. Passer par une méthode est plus simple que de se battre avec 
         * les scopes de try/catch.
         * 
         * @return true si l'image est valide, false sinon.
         */
        bool isValid() const;


        /**
         * Comprime l'image en utilisant k-means.
         * 
         * @param k (const uint16_t) : Nombre de clusters
         * @param epsilon (const double) : Seuil de convergence
         * @param maxIterations (const uint16_t) : Nombre maximum d'itérations
         */
        void compress(const uint16_t k, const double epsilon, const uint16_t maxIterations = 25);


        /**
         * Affiche l'image dans une fenêtre.
         **/
        void draw() const;


        /**
         * Sauvegarde l'image au chemin spécifié. Format PNG pour préserver les couleurs obtenues par l'algorithme.
         * 
         * @param path (const std::string&) : Chemin de sauvegarde (on ne peut pas utiliser un string_view,
         * car cv::imwrite() ne prend les prend pas en charge)
         **/
        void save(const std::string &path) const;


    private:
        // Membres
        cv::Mat data_; // Image à traiter
        std::mt19937 rng_; // RNG pour la génération des centroïdes
        const bool debug_; // Mode debug


        // Méthodes privées
        /**
         * Choisit k pixels aléatoires selon une distribution uniforme, et renvoie leurs couleurs sous forme de vecteur.
         * 
         * @param k (const uint16_t) : Nombre de clusters
         * @return Les centroïdes générés sous forme de vecteur de couleurs (std::vector<cv::Vec3f>).
         **/
        std::vector<cv::Vec3f> generateCentroids(const uint16_t k);


        /**
        * Calcule la distance euclidienne entre la couleur d'un point et celle du centroïde donné.
        * 
        * @param color (const cv::Vec3b&) : Couleur du pixel
        * @param centroid (const cv::Vec3f&) : Centroïde à comparer
        * @return La distance (double).
        **/
        double distanceToCentroid(const cv::Vec3b &color, const cv::Vec3f &centroid) const;


        /**
         * Renvoie le centroïde le plus proche de la couleur donnée.
         * 
         * @param color (const cv::Vec3b&) : Couleur du pixel à comparer
         * @param centroids (const std::vector<cv::Vec3b>&) : Centroïdes à comparer
         * @return Le centroïde le plus proche sous forme de couleur (cv::Vec3f)
        **/
        cv::Vec3f getClosestCentroid(const cv::Vec3b &color, const std::vector<cv::Vec3f> &centroids) const;


        /**
         * Change la couleur de tous les pixels de l'image en fonction du centroïde le plus proche.
         * 
         * @param centroids (const std::vector<cv::Vec3f>&) : Centroïdes à comparer
         **/
        void updatePixels(const std::vector<cv::Vec3f> &centroids);


        /**
         * Met à jour les centroïdes selon la nouvelle couleur des pixels.
         * 
         * @param centroids (std::vector<cv::Vec3f>&) : Centroïdes à mettre à jour
         * @return Le déplacement moyen des centroïdes (double), pour savoir si l'algorithme a convergé.
         */
        double updateCentroids(std::vector<cv::Vec3f> &centroids);
    };
}