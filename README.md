# Compression d'image via k-means en C++

Implémentation basique de k-means pour de la compression d'image par quantification vectorielle.

J'utilise ici OpenCV car c'est le standard de facto pour le traitement d'image en C++ et que je voulais donc me familiariser avec, mais je suppose qu'on pourrait faire plus léger et rapide avec CImg ou autre.

On n'utilisera évidemment pas l'implémentation déjà existante de k-means de OpenCV car cela n'aurait aucun intérêt. OpenCV sera utilisé pour la lecture et l'écriture d'image, et pour les typedefs assez pratiques (mais dont j'aimerais me débarasser, c.f. [plus bas](#limitations)).

## Installation

Télécharger le binaire précompilé dans les [releases](https://github.com/bakraw/k-means-compression/releases) ou compiler la source (**s'assurer que CMake, OpenCV et OpenMP soient installés sur votre machine**).

```bash
git clone https://github.com/bakraw/k-means-compression
cd k-means-compression
mkdir build && cd build
cmake ..
make
```

## Usage

```bash
./k-means-compression [Image à compresser] [Valeur de k]
```
L'image compressée sera enregistrée dans le même répertoire sous le nom `compressed.png`.

## Résultat

Avant :
![test](https://github.com/user-attachments/assets/19be53ca-1436-454f-b5df-01a1a1642c7a)


Après :
![compressed](https://github.com/user-attachments/assets/4c18187d-f6b2-45a1-8683-909a51e08652)

Avec k = 32 et un seuil à 0.75, on passe sur cette image de 1.5 MiB à ~650 KiB.

> N.B. : L'image compressée est sauvegardée au format PNG, car l'objet principal de ce projet est k-means, et je ne voulais donc pas que la compression affecte les couleurs obtenues par l'algorithme. **Si l'image originale est au format JPEG, le rendement de la compression sera négatif**.

## Limitations

Je compte continuer à améliorer ce programme qui est pour l'instant l'implémentation la plus basique possible de k-means, et qui est donc **atrocement lent** (préparez votre après-midi si vous voulez compresser une image 4K ou fixer k à n'importe quelle valeur supérieure à 40). Il n'y pour l'instant qu'un parallélisme très simpliste avec OpenMP (à base de ```#pragma omp parallel for``` à droite à gauche), mais j'ai l'intention de faire plusieurs modifications à l'avenir, nommément :
- implémenter ma propre version des typedefs de vecteurs de OpenCV (```cv::Vec3f```, etc.) et éventuellement de cv::Mat qui sont lents et incompatibles avec certaines fonctionnalités de OpenMP.
- utiliser l'algorithme de Elkan, k-means++, etc.
- utiliser CUDA ?