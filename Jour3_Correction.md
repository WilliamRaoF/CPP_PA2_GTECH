##5.1
```cpp
#include <iostream>
#include <vector>
#include <future>
#include <thread>

// Fonction pour calculer la somme d'une portion du tableau
long long partial_sum(const std::vector<int>& data, size_t start, size_t end) {
    std::cout << "Calcul de la somme de l'intervalle [" << start << ", " << end << ") sur le thread "
        << std::this_thread::get_id() << std::endl;

    long long sum = 0;
    for (size_t i = start; i < end; ++i) {
        sum += data[i];
    }
    return sum;
}

int main() {
    const size_t data_size = 1'000'000;
    const size_t num_tasks = 4;
    std::vector<int> data(data_size, 1); 

    std::vector<std::future<long long>> futures;
    size_t block_size = data_size / num_tasks;

    std::cout << "---- Lancement avec std::launch::async ----\n";
    for (size_t i = 0; i < num_tasks; ++i) {
        size_t start = i * block_size;
        size_t end = (i == num_tasks - 1) ? data_size : (i + 1) * block_size;
        futures.push_back(std::async(std::launch::async, partial_sum, std::cref(data), start, end));
    }

    long long total_sum = 0;
    for (auto& fut : futures) {
        total_sum += fut.get();
    }
    std::cout << "Somme totale (async): " << total_sum << "\n";

    futures.clear();

    std::cout << "\n---- Lancement avec std::launch::deferred ----\n";
    for (size_t i = 0; i < num_tasks; ++i) {
        size_t start = i * block_size;
        size_t end = (i == num_tasks - 1) ? data_size : (i + 1) * block_size;
        futures.push_back(std::async(std::launch::deferred, partial_sum, std::cref(data), start, end));
    }

    total_sum = 0;
    for (auto& fut : futures) {
        total_sum += fut.get();
    }
    std::cout << "Somme totale (deferred): " << total_sum << "\n";

    return 0;
}

```
## Exercice final
```cpp
#include <iostream>
#include <thread>
#include <mutex>

using namespace std;

int somme = 0;
int produit = 0;
int difference = 0;

mutex mtx;

void calculerSomme(int a, int b) {
    lock_guard<mutex> lock(mtx);
    somme = a + b;
    cout << "Somme : " << somme << endl;
}

void calculerProduit(int a, int b) {
    lock_guard<mutex> lock(mtx);
    produit = a * b;
    cout << "Produit : " << produit << endl;
}

void calculerDifference(int a, int b) {
    lock_guard<mutex> lock(mtx);
    difference = a - b;
    cout << "Différence : " << difference << endl;
}

int main() {
    int a = 10, b = 5;

    // Lancer les 3 threads
    thread t1(calculerSomme, a, b);
    thread t2(calculerProduit, a, b);
    thread t3(calculerDifference, a, b);

    // Attendre la fin des threads
    t1.join();
    t2.join();
    t3.join();

    // Affichage récapitulatif final
    cout << "\n--- Résumé des résultats ---" << endl;
    cout << "Somme : " << somme << endl;
    cout << "Produit : " << produit << endl;
    cout << "Différence : " << difference << endl;

    return 0;
}

```
