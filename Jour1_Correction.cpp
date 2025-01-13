#include <iostream>
#include <string>
#include <vector>
#include <memory>

class IAttaquer {
public:
    virtual void attaquer() = 0; // Methode pure virtuelle
};

class IDeplacement {
public:
    virtual void seDeplacer() = 0; // Methode pure virtuelle
};
class IEtat {
public:
    virtual void afficherEtat() = 0; // Methode pure virtuelle
};

class IEnnemi : public IAttaquer, public IDeplacement, public IEtat {
public:
};



class Ennemi : public IEnnemi {
protected:
    std::string nom;
    int vie;
    double vitesse;

public:
    Ennemi(const std::string& nom, int vie, double vitesse)
        : nom(nom), vie(vie), vitesse(vitesse) {}


    void afficherEtat() override {
        std::cout << "Nom: " << nom << ", Vie: " << vie << ", Vitesse: " << vitesse << std::endl << std::endl;
    }

    virtual void attendre() = 0; // Methode pure virtuelle
};

class Zombie : public Ennemi {
public:
    Zombie(const std::string& nom, int vie, double vitesse)
        : Ennemi(nom, vie, vitesse) {}

    void attaquer() override {
        std::cout << nom << " attaque lentement avec ses griffes.\n";
    }

    void seDeplacer() override {
        std::cout << nom << " se deplace lentement vers sa cible.\n";
    }

    void attendre() override {
        std::cout << nom << " reste immobile, grogne lentement...\n";
    }
};

class Vampire : public Ennemi {
public:
    Vampire(const std::string& nom, int vie, double vitesse)
        : Ennemi(nom, vie, vitesse) {}

    void attaquer() override {
        std::cout << nom << " attaque furtivement en mordant sa cible.\n";
    }

    void seDeplacer() override {
        std::cout << nom << " se deplace rapidement et discretement.\n";
    }

    void attendre() override {
        std::cout << nom << " reste cache dans l'ombre, attendant le bon moment.\n";
    }
};

class Fantome : public Ennemi {
public:
    Fantome(const std::string& nom, int vie, double vitesse)
        : Ennemi(nom, vie, vitesse) {}

    void attaquer() override {
        std::cout << nom << " attaque en effrayant sa cible de maniere furtive.\n";
    }

    void seDeplacer() override {
        std::cout << nom << " traverse les murs et flotte silencieusement.\n";
    }

    void attendre() override {
        std::cout << nom << " reste invisible en attendant sa proie.\n";
    }
};


class Jeu {
private:
    std::vector<std::shared_ptr<Ennemi>> ennemis;

public:
    void ajouterEnnemi(const std::shared_ptr<Ennemi>& ennemi) {
        ennemis.push_back(ennemi);
    }

    void mettreAJour() {
        for (const auto& ennemi : ennemis) {
            ennemi->attaquer();
            ennemi->seDeplacer();
            ennemi->attendre();
            ennemi->afficherEtat();
        }
    }
};

int main() {
    // Cree des instances des differents ennemis
    auto zombie = std::make_shared<Zombie>("Zombie", 50, 1.0);
    auto vampire = std::make_shared<Vampire>("Vampire", 80, 3.5);
    auto fantome = std::make_shared<Fantome>("Fantome", 60, 2.0);

    // Cree une instance du jeu
    Jeu jeu;
    jeu.ajouterEnnemi(zombie);
    jeu.ajouterEnnemi(vampire);
    jeu.ajouterEnnemi(fantome);

    // Met à jour l'etat des ennemis
    jeu.mettreAJour();

    return 0;
}
