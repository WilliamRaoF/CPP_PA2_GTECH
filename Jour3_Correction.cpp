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
