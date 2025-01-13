# **1.1 Héritage et Polymorphisme**

### **1.1.1 Rappel sur l’héritage**

#### **Concept de base**
L’héritage permet à une classe dérivée (ou enfant) d’hériter des attributs et méthodes d’une classe de base (ou parent). Cela favorise la réutilisation du code et établit une relation hiérarchique.

```cpp
class Animal {
public:
    void eat() {
        std::cout << "L'animal mange.\n";
    }
};

class Dog : public Animal {
public:
    void bark() {
        std::cout << "Woof!\n";
    }
};
```

#### **Types d’héritage**
- **Public** : Les membres publics et protégés de la classe de base conservent leur visibilité.
- **Protected** : Les membres publics deviennent protégés dans la classe dérivée.
- **Private** : Tous les membres deviennent privés dans la classe dérivée.

Exemple :
```cpp
class Base {
protected:
    int x;
};

class Derived : public Base {
public:
    void setX(int value) { x = value; }
};
```

**Points à noter :**
- Utilisez `public` pour les relations "est-un" (e.g., un chien est un animal).
- Évitez l’héritage par défaut (`private`) sauf si justifié.

---

### **1.1.2 Héritage multiple et problème du diamant**

#### **Le problème du diamant**
L’héritage multiple peut créer des ambiguïtés, surtout lorsque deux classes parent partagent une base commune. Cela donne naissance au **problème du diamant**.

Imaginez une hiérarchie de classes suivante :
- Une classe de base appelée `A`.
- Deux classes dérivées, `B` et `C`, qui héritent de `A`.
- Une quatrième classe, `D`, qui hérite à la fois de `B` et `C`.

```cpp
class A {
public:
    int value;
};

class B : public A {};

class C : public A {};

class D : public B, public C {};
```

Dans cette configuration, la classe `D` possède **deux copies distinctes** de la classe `A` : une héritée via `B` et une via `C`. Cela crée des ambiguïtés :
- Si vous essayez d'accéder à `value` à partir de `D`, le compilateur ne sait pas quelle copie de `A::value` utiliser (celle héritée via `B` ou via `C`).
- Cela augmente également la redondance mémoire, car `D` stocke deux instances de `A`.

---

#### **L'héritage virtuel comme solution :**
Pour éviter ce problème, vous pouvez déclarer l'héritage de `A` comme **virtuel** dans les classes intermédiaires `B` et `C` :

```cpp
class A {
public:
    int value;
};

class B : virtual public A {};

class C : virtual public A {};

class D : public B, public C {};
```

Avec cette modification, une seule instance de `A` sera partagée entre `B` et `C` lorsqu'elles sont combinées dans `D`. Cela élimine les ambiguïtés et économise de la mémoire.

---

#### **Effets de l'héritage virtuel :**
1. **Une seule instance partagée :** Les classes intermédiaires (`B` et `C`) ne possèdent pas leurs propres copies indépendantes de `A`. Au lieu de cela, elles font référence à une seule instance commune de `A`, qui est finalement gérée par la classe dérivée (`D` dans cet exemple).
2. **Ambiguïtés résolues :** L'accès aux membres de `A` à partir de `D` est désormais non ambigu, car il n'y a qu'une seule instance.

Exemple d'utilisation dans `D` :
```cpp
D d;
d.value = 42;  // Pas d'ambiguïté
```

---

#### **Scénarios d'utilisation :**
L'héritage virtuel est particulièrement utile dans des cas où :
- Vous travaillez avec des hiérarchies complexes où l'héritage multiple peut entraîner des redondances ou des ambiguïtés.
- Vous souhaitez minimiser l'utilisation de mémoire en évitant des copies multiples d'une classe de base commune.

---

#### **Inconvénients :**
- L'héritage virtuel peut introduire une légère surcharge en termes de complexité d'implémentation. Par exemple, les constructeurs des classes intermédiaires ne peuvent pas initialiser directement la classe de base virtuelle ; cela doit être fait par la classe dérivée la plus dérivée (dans notre exemple, `D`).

---

### **1.1.3 Polymorphisme**

#### **Polymorphisme statique**
Le polymorphisme statique se produit au moment de la compilation, par exemple, via la surcharge de fonctions.

```cpp
class Calculator {
public:
    int add(int a, int b) {
        return a + b;
    }

    double add(double a, double b) {
        return a + b;
    }
};
```

#### **Polymorphisme dynamique**
Le polymorphisme dynamique repose sur des fonctions virtuelles et est résolu à l’exécution.

Exemple avec une méthode virtuelle :
```cpp
class Animal {
public:
    virtual void sound() {
        std::cout << "Un animal fait un son.\n";
    }
};

class Dog : public Animal {
public:
    void sound() override {
        std::cout << "Woof!\n";
    }
};

int main() {
    Animal* a = new Dog();
    a->sound();  // Affiche "Woof!" grâce au polymorphisme
    delete a;
    return 0;
}
```

---

### **1.1.4 Pointeurs et polymorphisme**

#### **Appel de fonctions via pointeurs**
Un pointeur de type base peut référencer un objet dérivé. Cela permet un comportement polymorphique.

```cpp
class Animal {
public:
    virtual void makeSound() {
        std::cout << "Un animal fait un son générique.\n";
    }
};

class Cat : public Animal {
public:
    void makeSound() override {
        std::cout << "Meow!\n";
    }
};

int main() {
    Animal* a = new Cat();
    a->makeSound();  // Affiche "Meow!"
    delete a;
    return 0;
}
```

#### **Problèmes potentiels**
Sans `virtual`, le programme utilise la liaison statique :
```cpp
class Animal {
public:
    void makeSound() {  // Pas de virtual
        std::cout << "Un animal fait un son générique.\n";
    }
};

class Dog : public Animal {
public:
    void makeSound() {
        std::cout << "Woof!\n";
    }
};

int main() {
    Animal* a = new Dog();
    a->makeSound();  // Affiche "Un animal fait un son générique."
    delete a;
    return 0;
}
```

---

### **1.1.5 Résumé et bonnes pratiques**

1. **Héritage multiple :**
   - Utilisez `virtual` pour résoudre les ambiguïtés du problème du diamant.
   - Préférez l’héritage simple sauf si l’héritage multiple est absolument nécessaire.

2. **Polymorphisme :**
   - Utilisez `virtual` pour activer le polymorphisme.
   - Ajoutez `override` pour éviter les erreurs dans les classes dérivées.

---

## **1.2 Classes Abstraites et Interfaces**

### **1.2.1 Classes abstraites**

#### **Définition**
Une classe abstraite est une classe qui ne peut pas être instanciée directement et qui peut contenir à la fois des méthodes abstraites (sans implémentation) et des méthodes concrètes (avec implémentation).
Caractéristiques d'une classe abstraite :

   - Méthodes abstraites et concrètes : Une classe abstraite peut contenir des méthodes abstraites (qui n'ont pas d'implémentation et doivent être définies dans les classes dérivées) et des méthodes concrètes (qui ont une implémentation par défaut et peuvent être utilisées telles quelles ou redéfinies).
   - Héritage simple : Contrairement aux interfaces, une classe ne peut hériter que d'une seule classe abstraite (il n'y a pas d'héritage multiple de classes abstraites).
   - Utilisation : Les classes abstraites sont utilisées lorsque vous souhaitez fournir une base commune avec des implémentations de certaines fonctionnalités tout en forçant certaines méthodes à être implémentées par les classes dérivées.

```cpp
class AbstractClass {
public:
    virtual void display() = 0;  // Méthode virtuelle pure
};
```

#### **Exemple**
```cpp
class Shape {
public:
    virtual void draw() = 0;  // Méthode virtuelle pure
};

class Circle : public Shape {
public:
    void draw() override {
        std::cout << "Dessiner un cercle.\n";
    }
};

int main() {
    Shape* s = new Circle();
    s->draw();  // Affiche "Dessiner un cercle."
    delete s;
    return 0;
}
```
Pourquoi utiliser une classe abstraite ?

- Réutilisation du code : Une classe abstraite permet de fournir une implémentation partielle (c'est-à-dire des méthodes concrètes) que les classes dérivées peuvent réutiliser ou étendre.
- Héritage commun : Elle permet de définir un comportement commun pour un ensemble de classes et de forcer des sous-classes à définir certaines méthodes.
- Abstraction partielle : Elle offre un moyen de fournir une abstraction partielle, c'est-à-dire une base commune avec une combinaison de comportements génériques et spécifiques.


---

### **1.2.2 Interfaces**

#### **Définition**
Une interface est une classe abstraite dans laquelle toutes les méthodes sont virtuelles pures.
Une interface est un contrat que les classes peuvent implémenter. Elle définit un ensemble de méthodes que les classes doivent implémenter, mais elle ne fournit aucune implémentation concrète de ces méthodes.
Caractéristiques d'une interface :

- Pas de code implémenté : Une interface ne contient que des signatures de méthodes (c'est-à-dire les déclarations de méthodes sans le code de leur comportement). Les classes qui implémentent l'interface sont responsables de l'implémentation du comportement de ces méthodes.
- Héritage multiple : En général, une classe peut implémenter plusieurs interfaces, ce qui permet un type d'héritage multiple. Cela contraste avec les classes, où une classe peut hériter d'une seule autre classe.
- Utilisation : L'interface est utilisée pour définir des comportements génériques que des classes différentes peuvent implémenter de manière spécifique.

```cpp
class Drawable {
public:
    virtual void draw() = 0;
};
```

#### **Implémentation multiple**
Une classe peut implémenter plusieurs interfaces.

```cpp
class Drawable {
public:
    virtual void draw() = 0;
};

class Serializable {
public:
    virtual void serialize() = 0;
};

class Circle : public Drawable, public Serializable {
public:
    void draw() override {
        std::cout << "Dessiner un cercle.\n";
    }

    void serialize() override {
        std::cout << "Sérialiser un cercle.\n";
    }
};
```

Pourquoi utiliser une interface ?

- Définir des comportements génériques : Les interfaces permettent de définir des comportements que des classes différentes peuvent implémenter de différentes façons.
- Flexibilité : Une classe peut implémenter plusieurs interfaces, ce qui permet d'ajouter des comportements à des classes de manière flexible.
- Découplage : Les interfaces permettent de découpler le code qui utilise les interfaces de l'implémentation concrète des classes, facilitant ainsi le test et la maintenance.


---

# Exercice de Gestion des Ennemis dans un Jeu Vidéo

## Objectif de l'exercice
L'objectif de cet exercice est de créer un système pour gérer différents types d'ennemis dans un jeu vidéo. Chaque ennemi aura des comportements communs, mais aussi des comportements spécifiques (par exemple, attaquer, se déplacer, etc.). L'exercice implique l'utilisation d'interfaces et de classes abstraites pour structurer ces comportements.

### Concepts utilisés
- **Interfaces** : pour définir des comportements communs.
- **Classes Abstraites** : pour fournir des comportements de base et obliger les sous-classes à implémenter des comportements spécifiques.
- **Polymorphisme** : pour permettre à différentes classes d'ennemis de partager des comportements communs tout en ayant des comportements spécifiques.

---

## Instructions

### 1. **Interface `IAttaquer`**
Créez une interface `IAttaquer` qui définit la méthode suivante :
- `void attaquer()`: Cette méthode permet à l'ennemi d'attaquer.

### 2. **Interface `IDeplacement`**
Créez une interface `IDeplacement` qui définit la méthode suivante :
- `void seDeplacer()`: Cette méthode permet à l'ennemi de se déplacer.

### 3. **Interface `IEtat`**
Créez une interface `IEtat` qui définit la méthode suivante :
- `void afficherEtat()`: Cette méthode permet d'afficher l'état de l'ennemi, comme son nom, ses points de vie, et sa vitesse.

### 4. **Interface `IEnnemi`**
Créez une interface `IEnnemi` qui combine les trois interfaces précédentes (`IAttaquer`, `IDeplacement`, `IEtat`). Cette interface servira de contrat pour tous les ennemis du jeu, leur imposant d'implémenter les méthodes des interfaces `IAttaquer`, `IDeplacement`, et `IEtat`.

### 5. **Classe abstraite `Ennemi`**
Créez une classe abstraite `Ennemi` qui implémente l'interface `IEnnemi`. Cette classe doit :
- Contenir des attributs de base pour l'ennemi, comme `nom`, `vie`, et `vitesse`.
- Fournir une implémentation de la méthode `afficherEtat()` qui affiche les informations de base sur l'ennemi (nom, vie, vitesse).
- Ajouter une méthode pure virtuelle `attendre()` que les classes dérivées devront implémenter. Cette méthode représente un comportement spécifique pour chaque type d'ennemi.

### 6. **Classes concrètes d'ennemis**
Créez plusieurs classes concrètes qui héritent de `Ennemi` et implémentent les comportements spécifiques :
- **Classe `Zombie`** : Implémente `attaquer()`, `seDeplacer()`, et `attendre()`. Le zombie se déplace lentement et attaque lentement.
- **Classe `Vampire`** : Implémente `attaquer()`, `seDeplacer()`, et `attendre()`. Le vampire se déplace rapidement et attaque de manière furtive.
- **Classe `Fantome`** : Implémente `attaquer()`, `seDeplacer()`, et `attendre()`. Le fantôme se déplace à travers les murs et attaque furtivement.

### 7. **Classe `Jeu`**
Créez une classe `Jeu` qui :
- Contient une collection d'ennemis.
- Permet d'ajouter des ennemis à cette collection.
- Implémente une méthode `mettreAJour()` qui appelle les méthodes `attaquer()`, `seDeplacer()`, et `afficherEtat()` pour chaque ennemi.

### 8. **Fonction `main`**
Dans la fonction `main` :
- Créez des instances des différentes classes d'ennemis (par exemple, un `Zombie`, un `Vampire`, et un `Fantome`).
- Ajoutez ces ennemis au jeu.
- Mettez à jour l'état du jeu en appelant la méthode `mettreAJour()`.
