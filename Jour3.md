# **Multithreading en C++**
---

## **1. Concepts de Base**

### **1.1. Qu’est-ce qu’un thread ?**
Un **thread** est la plus petite unité d’exécution d’un programme. Contrairement à un programme classique qui s’exécute sur un seul fil d’exécution, un programme multithread peut exécuter plusieurs threads en parallèle.

Exemple simple :
- Un programme mono-thread fait une tâche à la fois, comme lire un fichier ou calculer une somme.
- Un programme multi-thread peut lire un fichier tout en calculant une somme sur une autre partie des données.

---

### **1.2. Différence entre process et thread**
| **Processus**             | **Thread**                                    |
|---------------------------|-----------------------------------------------|
| Un processus est une instance d’un programme en exécution. | Un thread est une sous-partie d’un processus. |
| Chaque processus a son propre espace mémoire. | Les threads partagent la mémoire du processus parent. |
| La communication entre processus est complexe (IPC). | La communication entre threads est plus rapide. |

---

### **1.3. Pourquoi utiliser le multithreading ?**
1. **Performances** : Exploite les processeurs multi-cœurs.
2. **Réactivité** : Améliore les temps de réponse des programmes interactifs.
3. **Parallélisme** : Exécution simultanée de plusieurs tâches indépendantes.

---

## **2. Création d’un Thread**

### **2.1. Syntaxe de Base**
Voici un exemple pour créer et exécuter un thread simple en C++ :
```cpp
#include <iostream>
#include <thread>

using namespace std;

void afficher() {
    cout << "Bonjour depuis un thread\n";
}

int main() {
    thread t(afficher); // Lancer un thread
    t.join();           // Attendre que le thread se termine
    return 0;
}
```

#### **Explications :**
- **`thread t(afficher)`** : Crée un nouveau thread qui exécute la fonction `afficher`.
- **`t.join()`** : Bloque le thread principal jusqu’à ce que `t` ait fini.

---

### **2.2. Passer des Arguments à un Thread**
Pour passer des arguments, utilisez simplement les paramètres de la fonction :
```cpp
#include <iostream>
#include <thread>

using namespace std;

void afficherMessage(const string& message, int nombre) {
    for (int i = 0; i < nombre; ++i) {
        cout << message << "\n";
    }
}

int main() {
    string msg = "Bonjour, thread!";
    thread t(afficherMessage, msg, 3); // Passer des arguments
    t.join();
    return 0;
}
```

#### **Passer une référence :**
Par défaut, les arguments sont copiés. Pour passer une référence :
```cpp
thread t(afficherMessage, ref(msg), 3);
```

---

### **2.4. Exercices Pratiques :**
1. Modifier l’exemple ci-dessus pour passer un tableau d’entiers à la fonction du thread.
2. Créer un thread qui calcule la somme des éléments d’un tableau.

---

## **3. Gestion des Threads**

### **3.1. Join et Detach**
- **`join()`** : Attendre que le thread se termine avant de continuer.
- **`detach()`** : Le thread devient indépendant, mais il faut s’assurer que le programme principal dure assez longtemps pour qu’il s’exécute.

Exemple avec `detach()` :
```cpp
#include <iostream>
#include <thread>
#include <chrono>

using namespace std;

void fonction() {
    cout << "Thread détaché en cours d'exécution\n";
}

int main() {
    thread t(fonction);
    t.detach(); // Le thread continue en arrière-plan
    this_thread::sleep_for(chrono::seconds(1)); // Permet de voir le thread
    return 0;
}
```

#### **Problème potentiel :**
- Si le thread principal se termine avant que le thread détaché ait fini, le comportement est indéfini.

---

### **3.2. Exercices Pratiques :**
1. Modifier l’exemple ci-dessus pour lancer deux threads détachés qui affichent des messages différents.
2. Expérimenter en supprimant `this_thread::sleep_for` et observer le comportement.

---

## **4. Synchronisation des Threads**

Les threads partagent souvent des ressources (comme des variables globales). Cela peut entraîner des **conditions de course** (race conditions), où plusieurs threads accèdent et modifient des données simultanément.

---

### **4.1. Mutex**
Un **mutex** (abréviation de mutual exclusion) est un outil de synchronisation qui permet de contrôler l'accès concurrent à une ressource partagée. L'objectif principal d'un **mutex** est d'empêcher plusieurs threads d'accéder **simultanément** à une ressource critique, ce qui pourrait entraîner des conditions de course (race conditions).

Exemple :
```cpp
#include <iostream>
#include <thread>

using namespace std;

int compteur = 0;
mutex mtx; // Mutex global

void incrementer() {
    for (int i = 0; i < 100000; ++i) {
        mtx.lock();       // Verrouiller le mutex
        ++compteur;       // Accès à la ressource partagée
        mtx.unlock();     // Déverrouiller le mutex
    }
}

int main() {
    thread t1(incrementer);
    thread t2(incrementer);

    t1.join();
    t2.join();

    cout << "Compteur : " << compteur << endl;
    return 0;
}
```

### **4.1.1 Problèmes avec `lock()` et `unlock()`**

#### **Risque d'oubli**
Si un thread oublie de déverrouiller le mutex (par exemple, en cas d'exception), d'autres threads pourraient rester bloqués. Cela peut entraîner un **deadlock** (blocage mutuel).

#### **Solution : `lock_guard`**
C++ propose la classe `lock_guard` pour gérer automatiquement le verrouillage et le déverrouillage du mutex. Quand un `lock_guard` est détruit (par exemple, en quittant un bloc de code), il déverrouille automatiquement le mutex.

##### Exemple avec `lock_guard` :
```cpp
#include <iostream>
#include <thread>
#include <mutex>

using namespace std;

int compteur = 0;
mutex mtx;

void incrementer() {
    for (int i = 0; i < 100000; ++i) {
        lock_guard<mutex> lock(mtx); // Verrouillage automatique
        ++compteur;
    }
}

int main() {
    thread t1(incrementer);
    thread t2(incrementer);

    t1.join();
    t2.join();

    cout << "Compteur : " << compteur << endl;
    return 0;
}
```

#### Avantages :
1. Plus sûr : Évite les oublis de `unlock()`.
2. Lisibilité : Le code est plus propre.

---

### **4.1.2 Autres outils de gestion de mutex**

#### **`unique_lock`**
`unique_lock` est une autre classe pour gérer les mutex. Elle est plus flexible que `lock_guard` car elle permet :
- De verrouiller et déverrouiller manuellement.
- De différer le verrouillage.

##### Exemple avec `unique_lock` :
```cpp
#include <iostream>
#include <thread>
#include <mutex>

using namespace std;

int compteur = 0;
mutex mtx;

void incrementer() {
    unique_lock<mutex> lock(mtx); // Verrouillage différé
    for (int i = 0; i < 100000; ++i) {
        ++compteur;
    }
    // Le mutex est automatiquement déverrouillé ici
}

int main() {
    thread t1(incrementer);
    thread t2(incrementer);

    t1.join();
    t2.join();

    cout << "Compteur : " << compteur << endl;
    return 0;
}
```

---

#### **`scoped_lock` (C++17)**
`scoped_lock` permet de verrouiller plusieurs mutex en même temps pour éviter les deadlocks. Il est plus sûr que `unique_lock` pour cette tâche.

##### Exemple avec plusieurs mutex :
```cpp
#include <iostream>
#include <thread>
#include <mutex>

using namespace std;

mutex mtx1, mtx2;

void fonction1() {
    scoped_lock lock(mtx1, mtx2); // Verrouille les deux mutex
    cout << "Fonction 1 en cours\n";
}

void fonction2() {
    scoped_lock lock(mtx1, mtx2);
    cout << "Fonction 2 en cours\n";
}

int main() {
    thread t1(fonction1);
    thread t2(fonction2);

    t1.join();
    t2.join();

    return 0;
}
```




---

### **4.2. Variables de condition**
Utilisées pour suspendre un thread jusqu’à ce qu’une certaine condition soit remplie.

Exemple :
```cpp
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

using namespace std;

mutex mtx;
condition_variable cv;
bool prêt = false;

void travailleur() {
    unique_lock<mutex> lock(mtx);
    cv.wait(lock, [] { return prêt; }); // Attendre que "prêt" soit vrai
    cout << "Thread réveillé\n";
}

void signaler() {
    lock_guard<mutex> lock(mtx);
    prêt = true;
    cv.notify_one(); // Réveiller un thread
}

int main() {
    thread t1(travailleur);
    this_thread::sleep_for(chrono::seconds(1));
    thread t2(signaler);
    t1.join();
    t2.join();
    return 0;
}
```

---

### **4.3. Exercices Pratiques :**
1. Créer un programme où un thread produit des données (par exemple, un entier aléatoire), et un autre thread les consomme.
2. Utiliser `condition_variable` pour synchroniser deux threads qui alternent leur travail.

---

## **5. Tâches Asynchrones avec `async` et `future`**

Pour des tâches plus simples, utilisez `async` :
```cpp
#include <iostream>
#include <future>

using namespace std;

int calcul() {
    return 42;
}

int main() {
    future<int> resultat = async(launch::async, calcul);
    cout << "Résultat : " << resultat.get() << "\n"; // Récupérer le résultat
    return 0;
}
```

---

### **5.1. Exercices Pratiques :**
1. Créer un programme qui lance plusieurs tâches avec `async` pour calculer des sommes partielles d’un tableau.
2. Expérimenter avec les politiques de lancement (`launch::async` et `launch::deferred`).

---

## **6. Résumé**
- Le multithreading permet d’exécuter plusieurs tâches simultanément.
- Utilisez `thread` pour créer des threads, `mutex` pour la synchronisation, et `async` pour des tâches asynchrones.
- Faites attention aux conditions de course et à la gestion des threads détachés.

---

### **Exercice Final**
Créer un programme qui :
1. Lance trois threads pour exécuter des calculs différents (par exemple, somme, produit, et différence de deux nombres).
2. Synchronise les threads avec des mutex.
3. Affiche les résultats une fois que tous les threads ont fini.
