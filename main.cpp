/**
 * @file main.cpp
 * @author bakraw@github.com
 * @version 1.0
 * @date 2025-01-02
 * 
 * Implémentation d'un algorithme de compression d'image via k-means,
 * réalisée pour un projet universitaire.
 * 
 * OpenCV est très lourde et franchement overkill pour l'usage qu'on en fait ici 
 * (il ne sert qu'à lire une image), mais je l'ai utilisé pour me familiariser avec la librairie,
 * qui est le standard. Objectivement, CImg aurait été plus adaptée.
 * 
 * Je sais qu'OpenCV possède déjà une implémentation de k-means ; on ne s'en servira 
 * évidemment pas ici, étant donné qu'il s'agit d'un projet d'apprentissage.
 **/


///////////////////////////////////// INCLUDES /////////////////////////////////////


#include <iostream>
#include <vector>

#include <opencv2/opencv.hpp>


///////////////////////////////////// MAIN /////////////////////////////////////


int main(const int argc, const char** argv) {
    // Lecture de l'image
    cv::Mat image = cv::imread(argv[1]);

    if(image.empty()) {
        std::cerr << "ERREUR -> L'image n'a pas pu être chargée" << std::endl;
        return -1;
    }

    cv::imshow("Image", image);
    cv::waitKey(0);

    return 0;
}