## **Cours : Gestion Avancée de la Mémoire**

---

### **1. Allocation Dynamique et Gestion Manuelle**

En C++, l'allocation dynamique repose principalement sur l’utilisation des opérateurs `new` et `delete`, qui offrent un contrôle direct sur la création et la destruction des objets. 

---

#### **1.1 `new` et `delete`**

- **`new`** :
  - Alloue de la mémoire sur le tas pour un objet ou un tableau.
  - Appelle automatiquement le **constructeur** de l'objet.
  - Retourne un pointeur typé vers l'objet ou le tableau créé.

- **`delete`** :
  - Détruit l'objet ou le tableau précédemment alloué avec `new`.
  - Appelle automatiquement le **destructeur** de l'objet.
  - Libère la mémoire utilisée.

- **Exemple** :
```cpp
#include <iostream>

class MyClass {
public:
    MyClass() { std::cout << "Constructeur\n"; }
    ~MyClass() { std::cout << "Destructeur\n"; }
};

int main() {
    MyClass* obj = new MyClass(); // Allocation dynamique
    delete obj; // Libération de la mémoire
    return 0;
}
```

#### **1.2 Problèmes courants d'allocation dynamique**

- **Fuites mémoire** : 
  - Se produisent lorsque l'on oublie d'utiliser `delete` pour libérer la mémoire.
  - Exemple : réassigner un pointeur sans libérer la mémoire précédente.

- **Double libération** :
  - Appeler `delete` plusieurs fois sur le même pointeur entraîne un comportement indéfini.

- **Accès à une mémoire libérée** :
  - Lire ou écrire sur un pointeur après avoir appelé `delete`.

- **Solutions** :
  - Toujours associer un `delete` à chaque `new`.
  - Utiliser des smart pointers pour éviter ces problèmes (voir section suivante).

---

### **2. RAII (Resource Acquisition Is Initialization)**

Le **RAII** est un paradigme fondamental en C++ qui associe la durée de vie d’un objet à la gestion d’une ressource. Ce principe garantit qu'une ressource acquise est correctement libérée lorsqu’elle n’est plus nécessaire. Cela rend la gestion de ressources (mémoire, fichiers, connexions réseau, etc.) plus sûre et plus simple, même en présence d’exceptions.

---

#### **2.1 Concept et Fondements**

Le RAII repose sur deux mécanismes fondamentaux en C++ :
- **Constructeurs** : Ils acquièrent la ressource et l’initialisent au moment de la création de l’objet.
- **Destructeurs** : Ils libèrent automatiquement la ressource lorsque l’objet sort de sa portée.

Cela signifie que les ressources sont gérées automatiquement en fonction du cycle de vie de l’objet, réduisant ainsi les risques d’oublier de libérer une ressource ou de la libérer plusieurs fois.

**Avantages** :
- Évite les fuites mémoire ou de ressources.
- Garantit la libération des ressources même en cas d’exception.
- Réduit la complexité liée à la gestion manuelle des ressources.

---

#### **2.2 Exemple Théorique**

Supposons que nous devons gérer l’ouverture et la fermeture d’un fichier. Avec une gestion manuelle, il est facile d’oublier de fermer le fichier, surtout si une exception est levée entre-temps. Le RAII assure que le fichier sera toujours correctement fermé.

**Gestion manuelle (sans RAII)** :
```cpp
#include <iostream>
#include <fstream>

void writeFileWithoutRAII(const std::string& filename, const std::string& data) {
    std::ofstream file;
    file.open(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Impossible d'ouvrir le fichier");
    }
    file << data;

    // Risque d'oubli de fermeture du fichier en cas d'exception
    file.close();
}

int main() {
    try {
        writeFileWithoutRAII("test.txt", "Bonjour !");
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
    }
    return 0;
}
```

**Gestion avec RAII** :
```cpp
#include <iostream>
#include <fstream>
#include <string>

class FileHandler {
    std::ofstream file;
public:
    FileHandler(const std::string& filename) {
        file.open(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Impossible d'ouvrir le fichier");
        }
    }
    ~FileHandler() {
        if (file.is_open()) {
            file.close(); // Fermeture automatique
        }
    }
    void write(const std::string& data) {
        file << data;
    }
};

int main() {
    try {
        FileHandler fh("test.txt");
        fh.write("Bonjour !");
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
    }
    return 0;
}
```

Dans cet exemple :
- Le fichier est automatiquement fermé lorsque l’objet `FileHandler` sort de portée.
- Le destructeur de `FileHandler` est garanti d’être appelé, même si une exception est levée.

---

#### **2.3 RAII et Smart Pointers**

Les **smart pointers** (comme `std::unique_ptr` et `std::shared_ptr`) sont un exemple concret d’application du RAII pour la gestion de la mémoire dynamique.

**Exemple avec `std::unique_ptr`** :
```cpp
#include <iostream>
#include <memory>

class MyClass {
public:
    MyClass() { std::cout << "Constructeur\n"; }
    ~MyClass() { std::cout << "Destructeur\n"; }
};

int main() {
    std::unique_ptr<MyClass> ptr = std::make_unique<MyClass>();
    // L'objet est automatiquement détruit lorsque ptr sort de portée
    return 0;
}
```

Avantages :
- Plus besoin d’appeler `delete` pour libérer la mémoire.
- Sécurise la gestion des pointeurs dans des codes complexes.

---

#### **2.4 Bonnes Pratiques avec le RAII**

1. **Toujours encapsuler les ressources** dans des classes ou des structures.
2. **Assurez-vous que le destructeur libère toutes les ressources acquises.**
3. Utilisez des **classes standard** (comme  `std::unique_ptr`) autant que possible pour simplifier l’implémentation.
4. Vérifiez les erreurs d’acquisition des ressources (par exemple, échec d’ouverture d’un fichier).

---
### **3. Gestion Automatique avec les Smart Pointers**

La gestion manuelle de la mémoire peut être complexe et source d'erreurs, notamment en cas de fuites mémoire, de double suppression ou d'accès à une mémoire déjà libérée. Les **smart pointers** de C++ standard (introduits avec C++11 et améliorés depuis) offrent une gestion automatique et sécurisée des ressources dynamiques en suivant les principes du RAII.

---

#### **3.1 Les Smart Pointers : Introduction**

Un smart pointer est une classe qui agit comme un wrapper pour un pointeur brut. Il assure que la mémoire allouée est automatiquement libérée lorsqu'elle n'est plus utilisée. Ces classes sont définies dans le fichier d'en-tête `<memory>`.

Les principaux types de smart pointers sont :
1. **`std::unique_ptr`** : Propriétaire exclusif d’une ressource.
2. **`std::shared_ptr`** : Propriétaire partagé d’une ressource (référence comptée).
3. **`std::weak_ptr`** : Référence non-propriétaire pour éviter les cycles dans les références partagées.

---

#### **3.2 `std::unique_ptr` : Gestion Exclusive**

Un `std::unique_ptr` est un pointeur unique qui possède **seul** la ressource qu'il gère. Une fois détruit ou réassigné, la ressource est automatiquement libérée.

##### **Caractéristiques :**
- Non copiable : Un `std::unique_ptr` ne peut pas être copié, ce qui garantit que la ressource est toujours gérée par un seul smart pointer.
- Déplaçable : Permet de transférer la propriété à un autre `std::unique_ptr` via `std::move`.

##### **Exemple Pratique :**
```cpp
#include <iostream>
#include <memory>

class MyClass {
public:
    MyClass() { std::cout << "Constructeur\n"; }
    ~MyClass() { std::cout << "Destructeur\n"; }
};

int main() {
    std::unique_ptr<MyClass> ptr1 = std::make_unique<MyClass>(); // Allocation
    // Transfert de propriété
    std::unique_ptr<MyClass> ptr2 = std::move(ptr1);

    if (!ptr1) {
        std::cout << "ptr1 ne possède plus la ressource\n";
    }

    return 0;
}
```

##### **Avantages de `std::unique_ptr` :**
- Léger, rapide, et sûr pour des ressources uniques.
- Simplifie la gestion de mémoire, aucun besoin explicite de `delete`.

---

#### **3.3 `std::shared_ptr` : Gestion Partagée**

Un `std::shared_ptr` permet de partager une ressource entre plusieurs smart pointers. Il utilise un mécanisme de comptage de références pour déterminer quand libérer la ressource.

##### **Caractéristiques :**
- Partage une ressource avec d'autres `std::shared_ptr`.
- Libère automatiquement la ressource lorsque le dernier propriétaire est détruit.
- Compatible avec `std::weak_ptr` pour éviter les cycles de références.

##### **Exemple Pratique :**
```cpp
#include <iostream>
#include <memory>

class MyClass {
public:
    MyClass() { std::cout << "Constructeur\n"; }
    ~MyClass() { std::cout << "Destructeur\n"; }
};

int main() {
    std::shared_ptr<MyClass> ptr1 = std::make_shared<MyClass>();
    {
        std::shared_ptr<MyClass> ptr2 = ptr1; // Partage de la ressource
        std::cout << "Nombre de références : " << ptr1.use_count() << '\n';
    } // ptr2 est détruit ici, mais la ressource n'est pas libérée

    std::cout << "Nombre de références : " << ptr1.use_count() << '\n';
    // La ressource est libérée ici lorsque ptr1 est détruit

    return 0;
}
```

##### **Avantages de `std::shared_ptr` :**
- Pratique pour les ressources partagées entre plusieurs parties d’un programme.
- Comptage automatique des références.

##### **Limites :**
- Léger surcoût lié au mécanisme de comptage.
- Risque de **cycles de références** qui empêchent la libération d’une ressource (résolu avec `std::weak_ptr`).

---

#### **3.4 `std::weak_ptr` : Références Non-Propriétaires**

Un `std::weak_ptr` est un smart pointer qui n’affecte pas le comptage des références d’un `std::shared_ptr`. Il est souvent utilisé pour éviter les cycles de références dans des graphes ou des structures fortement liées.

##### **Caractéristiques :**
- Ne possède pas la ressource, agit comme une "vue".
- Permet de vérifier si la ressource existe toujours via `expired()`.

##### **Exemple Pratique :**
```cpp
#include <iostream>
#include <memory>

class MyClass {
public:
    MyClass() { std::cout << "Constructeur\n"; }
    ~MyClass() { std::cout << "Destructeur\n"; }
};

int main() {
    std::shared_ptr<MyClass> sharedPtr = std::make_shared<MyClass>();
    std::weak_ptr<MyClass> weakPtr = sharedPtr;

    if (auto locked = weakPtr.lock()) { // Vérifie et accède à la ressource
        std::cout << "La ressource est toujours disponible\n";
    }

    sharedPtr.reset(); // Libère la ressource
    if (weakPtr.expired()) {
        std::cout << "La ressource a été libérée\n";
    }

    return 0;
}
```

##### **Avantages de `std::weak_ptr` :**
- Évite les cycles de références, notamment dans les structures en graphe.
- Permet un accès sécurisé à une ressource partagée.

---

#### **3.5 Comparaison des Smart Pointers**

| Type             | Propriété              | Utilisation principale                     | Limitation                     |
|-------------------|------------------------|--------------------------------------------|--------------------------------|
| `std::unique_ptr` | Propriété exclusive    | Gestion simple de ressources uniques       | Non copiable                  |
| `std::shared_ptr` | Propriété partagée     | Gestion de ressources partagées            | Surcoût du comptage           |
| `std::weak_ptr`   | Référence non-propriétaire | Évite les cycles dans les références partagées | Nécessite un `std::shared_ptr` |

---

#### **3.6 Applications Pratiques**

1. **Gestion de ressources dynamiques dans des systèmes complexes** :
   Les smart pointers simplifient la gestion de ressources telles que la mémoire, les fichiers, ou les connexions réseau.

2. **Structures de données en graphe** :
   Utiliser `std::shared_ptr` et `std::weak_ptr` permet de créer des structures de données complexes sans risque de fuites mémoire dues à des cycles.

---

#### **3.7 Bonnes Pratiques**

1. **Utilisez `std::make_unique` et `std::make_shared`** :
   Préférer ces fonctions pour créer des smart pointers, car elles sont plus performantes et évitent des erreurs de code.
   ```cpp
   auto ptr = std::make_shared<MyClass>();
   ```

2. **Évitez les pointeurs bruts autant que possible** :
   Limitez leur usage à des cas spécifiques, comme dans les bibliothèques ou les APIs non-C++.

3. **Combinez avec RAII** :
   Utilisez les smart pointers pour encapsuler les ressources, en particulier dans des classes ou des structures.

---

### Liste d'Exercices : Gestion Avancée de la Mémoire

Voici une série d'exercices pour pratiquer les concepts abordés dans les sections 1, 2 et 3.

---

### **1. Allocation Dynamique et Gestion Manuelle**

1. **Manipulation de Pointeurs Dynamiques**  
   - Allouez dynamiquement un tableau d'entiers de taille donnée par l'utilisateur.  
   - Initialisez chaque case du tableau avec un multiple de son index (ex. : tableau\[i\] = i \* 2).  
   - Affichez le contenu du tableau.  
   - Libérez correctement la mémoire.

2. **Double Suppression**  
   - Créez une fonction qui alloue dynamiquement une ressource (par exemple, un entier).  
   - Tentez de supprimer la ressource deux fois de suite et observez le comportement du programme.

---

### **2. Gestion Automatique avec les Smart Pointers**

1. **Gestion Exclusive avec `std::unique_ptr`**  
   - Créez une classe simple `MyClass` avec un constructeur et un destructeur qui affichent des messages.  
   - Allouez un objet de cette classe en utilisant un `std::unique_ptr`.  
   - Transférez la propriété de l'objet à un autre `std::unique_ptr` à l'aide de `std::move`.

2. **Gestion Partagée avec `std::shared_ptr`**  
   - Créez une structure `Node` représentant un nœud dans une liste chaînée simple.  
   - Utilisez `std::shared_ptr` pour relier les nœuds.  
   - Implémentez des méthodes pour ajouter et afficher les éléments de la liste.

3. **Éviter les Cycles avec `std::weak_ptr`**  
   - Implémentez un système de nœuds où chaque nœud a :
     - Un parent (`std::shared_ptr`).
     - Une liste de fils (contenus dans des `std::weak_ptr` pour éviter des cycles).  
   - Ajoutez des fonctions pour construire un arbre simple et afficher les relations entre les nœuds.

---
