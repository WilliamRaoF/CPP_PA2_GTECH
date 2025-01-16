# **Cours SDL en C++ : Développement Multimédia et Jeux**


## **Plan du cours**

### **1. Introduction à SDL**

#### **Qu'est-ce que SDL ?**
- SDL (**Simple DirectMedia Layer**) est une bibliothèque open-source en C/C++ permettant de :  
  - Créer des jeux 2D ou des interfaces graphiques.  
  - Gérer des fenêtres, des entrées utilisateur (clavier/souris), des graphiques, des sons, et plus encore.  
- Compatible avec Windows, Linux, macOS, Android, et iOS.  
- SDL est modulaire : plusieurs extensions existent pour des fonctionnalités spécifiques :  
  - **SDL_image** pour charger différents formats d’images.  
  - **SDL_mixer** pour gérer les sons et musiques.  
  - **SDL_ttf** pour le rendu de texte.  

**Applications pratiques :**  
- Développement de jeux 2D.  
- Simulation graphique et prototypes d'applications.  

---

#### **Installation et Configuration de SDL (de la même manière que SFML)**  

1. Téléchargez la version "Development Libraries".  
2. Décompressez l'archive dans un dossier (ex. : `C:/SDL2`).  
3. Configurez le projet :  
   - Ajoutez le chemin vers les headers (`include`) et les bibliothèques (`lib`).  
   - Liez les bibliothèques nécessaires (`SDL2.lib`, `SDL2main.lib`).  

---

### **2. Création d'une fenêtre SDL**

#### **Initialisation et Cycle de vie SDL**  
1. **Initialisation de SDL** :  
   - Utilisez `SDL_Init()` pour démarrer SDL et initialiser les sous-systèmes nécessaires.  
   - Exemple : `SDL_INIT_VIDEO` pour les graphiques et fenêtres.  
2. **Création d'une fenêtre** :  
   - Utilisez `SDL_CreateWindow()` pour ouvrir une fenêtre.  
   - Spécifiez la position, les dimensions, et les options (par ex. `SDL_WINDOW_SHOWN`).  
3. **Rendu graphique** :  
   - `SDL_CreateRenderer()` permet de dessiner des éléments dans la fenêtre.  
4. **Libération des ressources** :  
   - Détruisez les objets (`SDL_DestroyWindow`, `SDL_DestroyRenderer`) et fermez SDL avec `SDL_Quit()`.

---

**Code détaillé :**  

```cpp
#include <SDL.h>
#include <iostream>

int main(int argc, char* argv[]) {
    // Initialisation de SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "Erreur lors de l'initialisation de SDL : " << SDL_GetError() << std::endl;
        return 1;
    }

    // Création de la fenêtre
    SDL_Window* window = SDL_CreateWindow(
        "Première fenêtre SDL",       // Titre de la fenêtre
        SDL_WINDOWPOS_CENTERED,      // Position X
        SDL_WINDOWPOS_CENTERED,      // Position Y
        800,                         // Largeur
        600,                         // Hauteur
        SDL_WINDOW_SHOWN             // Options (visible à l'écran)
    );

    if (!window) {
        std::cerr << "Erreur lors de la création de la fenêtre : " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    // Garde la fenêtre ouverte pendant 5 secondes
    SDL_Delay(5000);

    // Nettoyage des ressources
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
```

---

### **3. Gestion des événements et boucle principale (1 heure)**

#### **La boucle d'événements**  
1. Une application SDL fonctionne autour d'une **boucle principale** qui :  
   - Gère les événements (fermeture de la fenêtre, clavier, souris).  
   - Met à jour les états de l'application.  
   - Rafraîchit l'écran.  
2. Les événements sont gérés via `SDL_Event`.  
   - Types courants :  
     - `SDL_QUIT` : Fermeture de la fenêtre.  
     - `SDL_KEYDOWN` et `SDL_KEYUP` : Entrées clavier.  
     - `SDL_MOUSEBUTTONDOWN` et `SDL_MOUSEBUTTONUP` : Boutons de la souris.  

---

#### **Implémentation d'une boucle d'événements**  

**Code détaillé :**  

```cpp
#include <SDL.h>
#include <iostream>

int main(int argc, char* argv[]) {
    // Initialisation de SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "Erreur lors de l'initialisation de SDL : " << SDL_GetError() << std::endl;
        return 1;
    }

    // Création de la fenêtre et du rendu
    SDL_Window* window = SDL_CreateWindow("Gestion des événements",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          800, 600, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    bool isRunning = true;
    SDL_Event event;

    // Boucle principale
    while (isRunning) {
        // Gestion des événements
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                isRunning = false; // Quitter l'application
            } else if (event.type == SDL_KEYDOWN) {
                std::cout << "Touche appuyée : " << SDL_GetKeyName(event.key.keysym.sym) << std::endl;
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                std::cout << "Clic souris : (" << event.button.x << ", " << event.button.y << ")" << std::endl;
            }
        }

        // Efface l'écran avec une couleur noire
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Met à jour l'écran
        SDL_RenderPresent(renderer);
    }

    // Nettoyage
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
```

---

### **4. Dessin et gestion des textures**

1. **Dessin avec SDL_Renderer** :  
   - Utilisez `SDL_SetRenderDrawColor` pour définir la couleur.  
   - Dessinez des rectangles, lignes, ou pixels.  

2. **Textures** :  
   - Chargez des images avec `SDL_Surface`.  
   - Convertissez les surfaces en textures pour le rendu.  
   - Utilisez **SDL_image** pour prendre en charge des formats comme PNG et JPEG.  

---

#### **Dessiner et afficher des images**  

**Code détaillé :**  

```cpp
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

int main(int argc, char* argv[]) {
    // Initialisation de SDL et SDL_image
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "Erreur SDL : " << SDL_GetError() << std::endl;
        return 1;
    }
    IMG_Init(IMG_INIT_PNG);

    SDL_Window* window = SDL_CreateWindow("Textures et Images",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          800, 600, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Charger une image
    SDL_Surface* surface = IMG_Load("chemin/vers/image.png");
    if (!surface) {
        std::cerr << "Erreur lors du chargement de l'image : " << IMG_GetError() << std::endl;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    // Boucle principale
    bool isRunning = true;
    SDL_Event event;

    while (isRunning) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) isRunning = false;
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }

    // Nettoyage
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}
```

---

### **5. Projet final : Mini-jeu simple**  

**Objectif :** Créer un jeu où un joueur contrôle un carré qui évite des obstacles ou collecte des objets.  

1. Dessinez un carré pour le joueur.  
2. Ajoutez des commandes clavier pour déplacer le carré.  
3. Ajoutez des obstacles ou cibles (dessinés comme des rectangles).  
