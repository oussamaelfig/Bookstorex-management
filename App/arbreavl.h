/*
 * Squelette minimal pour classe générique ArbreAVL<T> du TP2.
 * TP2 -- Arbres AVL
 * UQAM / Département d'informatique
 * INF3105 - Structures de données et algorithmes
 *
 */

#ifndef __ARBREAVL_H__
#define __ARBREAVL_H__

#include <assert.h>
#include "pile.h"

template <class T>
class ArbreAVL
{
	/**** Vous n'avez pas le droit de modifier l'interface publique de cette classe ******/
	/**** Vous n'avez donc pas le droit d'ajouter des fonctions publiques ni de **********/
	/**** modifier les signatures des fonctions publiques ********************************/
	/**** Ça s'applique également sur les fonctions publiques de le classe Itérateur *****/
public:
	ArbreAVL();
	ArbreAVL(const ArbreAVL &);
	~ArbreAVL();
	ArbreAVL<T> &operator=(const ArbreAVL<T> &autre);

	/*
	 * Fonctions vues en cours/laboratoire. Le fonctionnement à
	 * implémenter pour le TP2 est identiques.
	 */
	bool vide() const;
	void vider();
	bool contient(const T &) const;
	void inserer(const T &);
	void enlever(const T &);

	/*
	 * Retourne "true" si les arbres AVl ont exactement les mêmes
	 * éléments peu importe l'ordre d'apparition dans les deux
	 * arblres. Si non,  retourne "false".
	 *
	 ************************ ATTENTION **************************
	 * Cet opérateur doit être implémenté en O(min(m,n)).				 *
	 * Où n et m sont les tailles de deux arbres AVL à comparer. *
	 *************************************************************
	 */
	bool operator==(const ArbreAVL<T> &autre) const;

	/*
	 * Cet itérateur est basé sur un parcours inordre de l'arbre
	 * courant.
	 */
	class Iterateur;
	Iterateur debut() const;
	T &operator[](const Iterateur &);
	const T &operator[](const Iterateur &) const;

	/*
	 * Ces fonctions sont implémentées à des fins de test et de diagnostique.
	 * Vous ne devez pas les utiliser dans vos implémentations, ni les modifier!
	 */
	int taille() const;
	int hauteur() const;
	int equilibre(const T &) const;
	int get_equilibre(const T &) const;
	int occurrence(const T &) const;
	void afficher() const;

private:
	struct Noeud
	{
		Noeud(const T &);
		T contenu;
		int equilibre;
		Noeud *gauche;
		Noeud *droite;
	};
	Noeud *racine;
	/**** Vous pouvez ajouter toute fonction privée nécessaire **********************/
	/**** N'oubliez d'expliquer son fonctionnement en commentaire *******************/
	/**** Ça s'applique également sur les attributs privés de le classe Itérateur ***/

	// Fonction recusrsive qui enleve les noeuds desires et les reorganise.
	bool enlever(Noeud *&noeud, const T &element);

	bool inserer(Noeud *&n, const T &e);
	// Une fonction d'utilité à droite
	// Faire tourner le sous-arbre enraciné avec y
	void rotationDroiteGauche(Noeud *&B);

	// Une fonction utilitaire à gauche
	// faire tourner le sous-arbre enraciné avec x
	void rotationGaucheDroite(Noeud *&B);

	// copier un noeud vers un autre noeud
	void copier(const Noeud *, Noeud *&) const;

	// vide l'arbre AVL de tous ses noeuds.
	void vider(Noeud *&noeud);

	/*
	 * Ces fonctions sont implémentées à des fins de test.
	 * Vous ne devez pas les utiliser dans vos implémentations, ni les modifier!
	 */
	int compter(Noeud *) const;
	Noeud *trouver(const T &) const;
	int hauteur(Noeud *n) const;
	int occurrence(Noeud *n, const T &) const;
	void preparerafficher(const Noeud *, int, int &, T *, int *) const;

public:
	class Iterateur
	{
	public:
		Iterateur(const Iterateur &);
		Iterateur(const ArbreAVL &);
		operator bool() const;
		Iterateur operator++(int);
		Iterateur &operator++();

	private:
		Noeud *courant;
		const ArbreAVL &arbre_associe;
		Pile<Noeud *> chemin;
		friend class ArbreAVL;
	};
};

/************ Fonctions publiques ***************/

template <class T>
ArbreAVL<T>::Noeud::Noeud(const T &c) : contenu(c), equilibre(0), gauche(nullptr), droite(nullptr)
{
}

template <class T>
ArbreAVL<T>::ArbreAVL() : racine(nullptr)
{
}

template <class T>
ArbreAVL<T>::ArbreAVL(const ArbreAVL<T> &autre) : racine(nullptr)
{
	copier(autre.racine, racine);
}

template <class T>
ArbreAVL<T>::~ArbreAVL()
{
	vider();
}

template <class T>
bool ArbreAVL<T>::vide() const
{
	return racine == nullptr;
}

template <class T>
void ArbreAVL<T>::vider()
{
	vider(racine);
}

template <class T>
void ArbreAVL<T>::vider(Noeud *&noeud)
{
    if (noeud == nullptr)
    {
        return;
    }
    if (noeud->gauche != nullptr)
    {
        vider(noeud->gauche);
    }
    if (noeud->droite != nullptr)
    {
        vider(noeud->droite);
    }
    delete noeud;
    noeud = nullptr;
}

template <class T>
bool ArbreAVL<T>::contient(const T &element) const
{
	bool est_dans_larbre = false;
	if (racine != nullptr) {
        Noeud* temp = racine;
        while (temp != nullptr && !est_dans_larbre) {
            if (temp->contenu == element) {
                est_dans_larbre = true;
            } else if (temp->contenu < element) {
                temp = temp->droite;
            } else {
                temp = temp->gauche;
            }
        }
    }
    return est_dans_larbre;
}

template <class T>
void ArbreAVL<T>::rotationDroiteGauche(Noeud *&B)
{
	Noeud *temp = B->droite;
    int eb = temp->equilibre;
    int ea = B->equilibre;
    int nea = -(eb < 0 ? eb : 0) + 1 + ea;
    int neb = (nea > 0 ? nea : 0) + 1 + eb;

    temp->equilibre = neb;
    B->equilibre = nea;
    B->droite = temp->gauche;
    temp->gauche = B;
    B = temp;
}

template <class T>
void ArbreAVL<T>::rotationGaucheDroite(Noeud *&B)
{
 	Noeud *a = B->gauche;
    Noeud *b = B;
    int ea = a->equilibre;
    int eb = b->equilibre;
    int neb = -(ea > 0 ? ea : 0) - 1 + eb;
    int nea = ea + (neb < 0 ? neb : 0) - 1;

    a->equilibre = nea;
    b->equilibre = neb;
    b->gauche = a->droite;
    a->droite = b;
    B = a;
}

template <class T>
bool ArbreAVL<T>::inserer(Noeud *&n, const T &e)
{
	if (n == nullptr)
	{
		n = new Noeud(e);
		return true;
	}
	if (e < n->contenu)
	{
		if (inserer(n->gauche, e))
		{ // cas gauche
			n->equilibre++;
			if (n->equilibre == 0)
				return false;
			if (n->equilibre == 1)
				return true;
			if (n->gauche->equilibre == -1)
				rotationDroiteGauche(n->gauche);
			rotationGaucheDroite(n);
		}
		return false;
	}
	else if (n->contenu < e)
	{
		if (inserer(n->droite, e))
		{ // cas droite
			n->equilibre--;
			if (n->equilibre == 0)
				return false;
			if (n->equilibre == -1)
				return true;
			if (n->droite->equilibre == 1)
				rotationGaucheDroite(n->droite);
			rotationDroiteGauche(n);
		}
		return false;
	}
	else
	{ // e == n->contenu
		n->contenu = e;
		return false;
	}
}

template <class T>
void ArbreAVL<T>::inserer(const T &e)
{
	inserer(racine, e);
}

template <class T>
void ArbreAVL<T>::enlever(const T &e)
{
	enlever(racine, e);
}

template <class T>
bool ArbreAVL<T>::enlever(Noeud *&noeud, const T &element)
{
    if (noeud == nullptr)
    {
        return false;
    }
    if (element < noeud->contenu)
    {
         bool retour = false;
        if (enlever(noeud->gauche, element))
        {
            noeud->equilibre--;
            if (noeud->equilibre == 0)
                return true;
            if (noeud->equilibre == -1)
                return false;
            retour = noeud->droite->equilibre != 0;
            assert(noeud->equilibre == -2);
            if (noeud->droite->equilibre == 1)
            {
                rotationGaucheDroite(noeud->droite);
            }
            rotationDroiteGauche(noeud);
        }
        return retour;
    }
    else if (element > noeud->contenu)
    {
         bool retour = false;
        if (enlever(noeud->droite, element))
        {
            noeud->equilibre++;
            if (noeud->equilibre == 0)
                return true;
            if (noeud->equilibre == 1)
                return false;
            retour = noeud->gauche->equilibre != 0;
            assert(noeud->equilibre == 2);
            if (noeud->gauche->equilibre == -1)
                rotationDroiteGauche(noeud->gauche);
            rotationGaucheDroite(noeud);
        }
        return retour;
    }
    else // if(element == noeud->contenu)
    {
        Noeud *temp = noeud;
        if (noeud->gauche == nullptr)
        {
            noeud = noeud->droite;
            delete temp;
            return true;
        }
        if (noeud->droite == nullptr)
        {
            noeud = noeud->gauche;
            delete temp;
            return true;
        }
        temp = noeud->gauche;
        while (true)
        {
            if (temp->droite == nullptr)
            {
                break;
            }
            temp = temp->droite;
        }
        noeud->contenu = temp->contenu;
        noeud = temp;
        return enlever(noeud->gauche, noeud->contenu);
    }
}


template <class T>
ArbreAVL<T> &ArbreAVL<T>::operator=(const ArbreAVL &autre)
{
	if (this == &autre)
	{
		return *this;
	}
	vider();
	copier(autre.racine, racine);
	return *this;
}

template <class T>
bool ArbreAVL<T>::operator==(const ArbreAVL<T> &autre) const
{
	Iterateur iter1(*this);
	iter1 = debut();
	Iterateur iter2(autre);
	iter2 = debut();
	while (iter1 && iter2)
	{
		if(iter1.courant != iter2.courant)
		{
			return false;
		}
		iter1++;
		iter2++;
	}
	if(iter1.courant == nullptr && iter2.courant ==nullptr)
	{
		return true;
	}
	return false;
}

template <class T>
typename ArbreAVL<T>::Iterateur ArbreAVL<T>::debut() const
{
	Iterateur iter(*this);
	iter.courant = racine;
	if (iter.courant != nullptr)
	{
		while (iter.courant->gauche != nullptr)
		{
			iter.chemin.empiler(iter.courant);
			iter.courant = iter.courant->gauche;
		}
	}
	return iter;
}

template <class T>
T &ArbreAVL<T>::operator[](const Iterateur &i)
{
	assert(&i.arbre_associe == this);
	assert(i.courant);
	return i.courant->contenu;
}

template <class T>
const T &ArbreAVL<T>::operator[](const Iterateur &i) const
{
	assert(&i.arbre_associe == this);
	assert(i.courant);
	return i.courant->contenu;
}

/************ Fonctions privées ***************/

template <class T>
void ArbreAVL<T>::copier(const Noeud *source, Noeud *&noeud) const
{
	if (source != nullptr)
	{
		noeud = new Noeud(source->contenu);
		copier(source->gauche, noeud->gauche);
		copier(source->droite, noeud->droite);
	}
}

/************ Iterateur ***************/

template <class T>
ArbreAVL<T>::Iterateur::Iterateur(const ArbreAVL &a) : arbre_associe(a), courant(nullptr)
{
}

template <class T>
ArbreAVL<T>::Iterateur::Iterateur(const Iterateur &i) : arbre_associe(i.arbre_associe), courant(i.courant)
{
}

template <class T>
typename ArbreAVL<T>::Iterateur ArbreAVL<T>::Iterateur::operator++(int)
{
	Iterateur copie(*this);
	operator++();
	return copie;
}

template <class T>
typename ArbreAVL<T>::Iterateur &ArbreAVL<T>::Iterateur::operator++()
{
	assert(courant);
	Noeud *suivant = courant->droite;
	while (suivant)
	{
		chemin.empiler(suivant);
		suivant = suivant->gauche;
	}
	if (!chemin.vide())
		courant = chemin.depiler();
	else
		courant = nullptr;
	return *this;
}

template <class T>
ArbreAVL<T>::Iterateur::operator bool() const
{
	return courant != nullptr;
}

/************ Fonctions de tests ***************/

#include <climits>

template <class T>
int ArbreAVL<T>::taille() const
{
	return compter(racine);
}

template <class T>
int ArbreAVL<T>::hauteur() const
{
	return hauteur(racine);
}

template <class T>
int ArbreAVL<T>::equilibre(const T &e) const
{
	int equi = INT_MIN;
	if (contient(e))
	{
		Noeud *n = trouver(e);
		equi = hauteur(n->gauche) - hauteur(n->droite);
	}
	return equi;
}

template <class T>
int ArbreAVL<T>::get_equilibre(const T &e) const
{
	int equi = INT_MIN;
	if (contient(e))
	{
		Noeud *n = trouver(e);
		equi = n->equilibre;
	}
	return equi;
}

template <class T>
int ArbreAVL<T>::occurrence(const T &e) const
{
	return occurrence(racine, e);
}

template <class T>
int ArbreAVL<T>::compter(Noeud *n) const
{
	if (n == nullptr)
		return 0;
	return 1 + compter(n->gauche) + compter(n->droite);
}

template <class T>
typename ArbreAVL<T>::Noeud *ArbreAVL<T>::trouver(const T &e) const
{
	Noeud *n = racine;
	while (n != nullptr && n->contenu != e)
	{
		if (n->contenu > e)
			n = n->gauche;
		else
			n = n->droite;
	}
	return n;
}

template <class T>
int ArbreAVL<T>::hauteur(Noeud *n) const
{
	if (n == nullptr)
		return 0;
	int g = hauteur(n->gauche);
	int d = hauteur(n->droite);
	return 1 + (g < d ? d : g);
}

template <class T>
int ArbreAVL<T>::occurrence(Noeud *n, const T &e) const
{
	int o = 0;
	if (n != nullptr)
	{
		if (n->contenu == e)
			o++;
		o += occurrence(n->gauche, e) + occurrence(n->droite, e);
	}
	return o;
}

#include <iostream>

template <class T>
void ArbreAVL<T>::afficher() const
{
	std::cout << "Contenu de l'arbre (";
	int n = taille();
	std::cout << n << " noeuds)\n";
	T *elements = new T[n];
	int *profondeurs = new int[n];
	n = 0;
	preparerafficher(racine, 0, n, elements, profondeurs);
	for (int p = 0;; p++)
	{
		bool derniereprofondeur = true;
		for (int i = 0; i < n; i++)
		{
			if (profondeurs[i] == p)
			{
				std::cout << elements[i];
				derniereprofondeur = false;
			}
			std::cout << '\t';
		}
		std::cout << '\n';
		if (derniereprofondeur)
			break;
	}
	delete[] profondeurs;
	delete[] elements;
	std::cout << "-------------" << std::endl;
}

template <class T>
void ArbreAVL<T>::preparerafficher(const Noeud *n, int profondeur, int &rang, T *elements, int *profondeurs) const
{
	if (n == nullptr)
		return;
	preparerafficher(n->gauche, profondeur + 1, rang, elements, profondeurs);
	elements[rang] = n->contenu;
	profondeurs[rang] = profondeur;
	rang++;
	preparerafficher(n->droite, profondeur + 1, rang, elements, profondeurs);
}

#endif
