//Exercice 1

#include <iostream>
#include <memory>
#include <vector>

void manipulationPointeursDynamiques() {
    int taille;
    std::cout << "Entrez la taille du tableau : ";
    std::cin >> taille;

    // Allocation dynamique
    int* tableau = new int[taille];

    // Initialisation des éléments
    for (int i = 0; i < taille; ++i) {
        tableau[i] = i * 2;
    }

    // Affichage des éléments
    std::cout << "Contenu du tableau : ";
    for (int i = 0; i < taille; ++i) {
        std::cout << tableau[i] << " ";
    }
    std::cout << std::endl;

    // Libération de la mémoire
    delete[] tableau;
}

void doubleSuppression() {
    // Allocation dynamique d'un entier
    int* ptr = new int(42);

    // Première suppression
    delete ptr;
    std::cout << "Première suppression effectuée.\n";

    // Tentative de double suppression
    // Ne faites jamais cela dans un programme réel, cela provoque un comportement indéfini.
    //delete ptr; // Ligne commentée pour éviter un comportement indéfini

    std::cout << "Seconde suppression effectuée.\n";
}



//Exercice 2

class MyClass {
public:
    MyClass() {
        std::cout << "Constructeur de MyClass appelé.\n";
    }

    ~MyClass() {
        std::cout << "Destructeur de MyClass appelé.\n";
    }

    void afficherMessage() {
        std::cout << "Ceci est un objet de MyClass.\n";
    }
};

void gestionExclusiveUniquePtr() {
    // Allocation avec std::unique_ptr
    std::unique_ptr<MyClass> obj = std::make_unique<MyClass>();
    obj->afficherMessage();

    // Transfert de propriété
    std::unique_ptr<MyClass> autreObj = std::move(obj);
    if (!obj) {
        std::cout << "L'objet unique_ptr d'origine est maintenant vide.\n";
    }
    autreObj->afficherMessage();
}


class Node {
public:
    int data;
    std::shared_ptr<Node> next;

    Node(int value) : data(value), next(nullptr) {}
};

void gestionPartageeSharedPtr() {
    //Création de nœuds
    std::shared_ptr<Node> head = std::make_shared<Node>(1);
    std::shared_ptr<Node> second = std::make_shared<Node>(2);
    std::shared_ptr<Node> third = std::make_shared<Node>(3);

    // Liaison des nœuds
    head->next = second;
    second->next = third;

    // Affichage des éléments
    std::shared_ptr<Node> current = head;
    while (current) {
        std::cout << current->data << " ";
        current = current->next;
    }
    std::cout << std::endl;
}



/*class Node: public std::enable_shared_from_this<Node> {
public:
    int data;
    std::shared_ptr<Node> parent;
    std::vector<std::weak_ptr<Node>> children;

    Node(int value) : data(value) {}

    void ajouterEnfant(const std::shared_ptr<Node>& enfant) {
        children.push_back(enfant);
        enfant->parent = shared_from_this();
    }

    void afficherRelations() {
        std::cout << "Node: " << data << ", Parent: ";
        if (parent) {
            std::cout << parent->data;
        }
        else {
            std::cout << "Aucun";
        }
        std::cout << ", Enfants: ";
        for (const auto& childWeak : children) {
            if (auto child = childWeak.lock()) {
                std::cout << child->data << " ";
            }
        }
        std::cout << std::endl;
    }
};

void gestionCyclesWeakPtr() {
    auto root = std::make_shared<Node>(1);
    auto child1 = std::make_shared<Node>(2);
    auto child2 = std::make_shared<Node>(3);

    root->ajouterEnfant(child1);
    root->ajouterEnfant(child2);

    root->afficherRelations();
    child1->afficherRelations();
    child2->afficherRelations();
}*/

int main() {
    manipulationPointeursDynamiques();
    doubleSuppression();
    gestionExclusiveUniquePtr();
    gestionPartageeSharedPtr();
    //gestionCyclesWeakPtr();
    return 0;
}
