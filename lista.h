//
// Created by Ognjen on 20.05.2019.
//

#ifndef NASP_TSP_GREEDY_LISTA_H
#define NASP_TSP_GREEDY_LISTA_H

#include <vector>

using namespace std;

template <typename T>
class Lista {
    public:
        struct Cvor {
            T element;
            Cvor *sljedeci;
            Cvor(T element, Cvor *sljedeci):element{element},sljedeci{sljedeci}{}
        };

        Lista(): pocetak{nullptr}, kraj{nullptr}, velicina{0}{}
        Lista(const Lista<T>& drugi);
        void izVektora(const vector<T>& vektor) {
            for (int i = 0; i < vektor.size(); i++) {
                dodajNaKraj(vektor[i]);
            }
        }
        vector<T> uVektor() {
            auto rez = vector<T>();
            for(int i = 0; i < velicina; i++)
                rez.push_back(get(i)->element);
            return rez;
        }
        Lista<T> &operator=(Lista<T> &drugi);
        bool prazna() { 
            return velicina == 0;
        }
        Cvor * getPocetak() {
            return pocetak; 
        }
        Cvor * get(int indeks) {
            int i = 0;
            for(Cvor *trenutni = pocetak; trenutni != nullptr; trenutni =trenutni->sljedeci) {
                if (i == indeks)
                    return trenutni;
                i++;
            }
            return nullptr;
        }
        T operator[] (int indeks) {
            return get(indeks)->element;
        }
        Cvor * getKraj() {
            return kraj; 
        }
        int size() {
            return velicina;
        }
        void dodajNaPocetak(T element);
        void dodajNaKraj(T element);
        void insert(int indeks, T element);
        void izbrisi(Cvor *pok);
        void izbrisi(int indeks);
        void swap(Cvor* pok1, Cvor* pok2);
        void smanjiVelicinu() {
            velicina--;
        }
        void print() {
            for(Cvor *trenutni = pocetak; trenutni != nullptr; trenutni =trenutni->sljedeci)
                cout << trenutni->element << " ";
            cout << endl;
        }
        ~Lista();

    private:
        int velicina;
        Cvor *pocetak, *kraj;
};

template <typename T>
void Lista<T>::dodajNaPocetak(T element) {
    pocetak = new Cvor(element,pocetak);
    if(prazna())
        kraj = pocetak;
    velicina++;
}

template <typename T>
void Lista<T>::dodajNaKraj(T element) {
    if(prazna())
        dodajNaPocetak(element);
    else {
        kraj = kraj->sljedeci = new Cvor(element,nullptr);
        velicina++;
    }
}

template <typename T>
void Lista<T>::insert(int indeks, T element) {

    if(indeks == 0)
        dodajNaPocetak(element);
    else if(indeks == velicina)
        dodajNaKraj(element);
    else {
        Cvor * pok = get(indeks - 1);
        pok->sljedeci = new Cvor(element, pok->sljedeci);
        velicina++;
    }
}

template <typename T>
void Lista<T>::izbrisi(Cvor *pok) {
    if(pok == nullptr) {
        velicina--;
        return;
    }

    if(velicina == 1) {
        delete pocetak;
        pocetak = kraj = nullptr;
    }
    else if(pok == kraj) {
        Cvor *trenutni = pocetak;
        while(trenutni->sljedeci != kraj)
            trenutni = trenutni->sljedeci;
        delete trenutni->sljedeci;
        trenutni->sljedeci = nullptr;
        kraj = trenutni;
    }
    else if(pok->sljedeci == nullptr) {
        Cvor * pret;
        for(Cvor *trenutni = pocetak; trenutni != nullptr; trenutni =trenutni->sljedeci)
            if(trenutni->sljedeci == pok)
                pret = trenutni;

        pret->sljedeci = nullptr;
        kraj = pret;
        delete pok;
    }
    else {
        if(pok->sljedeci == kraj)
            kraj = pok;
        Cvor *za_brisanje = pok->sljedeci;
        pok->element = pok->sljedeci->element;
        pok->sljedeci = za_brisanje->sljedeci;
        delete za_brisanje;
    }
    velicina--;
}

template <typename T>
void Lista<T>::izbrisi(int indeks) {
    izbrisi(get(indeks));
}
template <typename T>
Lista<T>::~Lista() {
    while(!prazna())
        izbrisi(pocetak);
}

template <typename T>
Lista<T>::Lista(const Lista<T>& drugi):pocetak(nullptr), kraj(nullptr), velicina(0) {
    for(Cvor *trenutni = drugi.pocetak; trenutni != nullptr; trenutni = trenutni->sljedeci)
        dodajNaKraj(trenutni->element);
}

template <typename T>
Lista<T>& Lista<T>:: operator=(Lista<T> &drugi) {
    if(this == &drugi)
        return *this;
    this->~Lista();
    for(Cvor *trenutni = drugi.pocetak; trenutni! = nullptr; trenutni = trenutni->sljedeci)
        dodajNaKraj(trenutni->element);
    return *this;
}

template <typename T>
void Lista<T>::swap(Cvor* pok1, Cvor* pok2)
{
    T temp = pok1->element;
    pok1->element = pok2->element;
    pok2->element = temp;
}


#endif //NASP_TSP_GREEDY_LISTA_H
