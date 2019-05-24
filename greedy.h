//
// Created by Ognjen on 13.05.2019.
//

#ifndef NASP_TSP_GREEDY_GREEDY_H
#define NASP_TSP_GREEDY_GREEDY_H

#include <iostream>
#include <vector>

#include "graf.h"

using namespace std;

/**
 * Pomocna funkcija. Pronalazi kraj komponente koja pocinje na datom indeksu (pocetak)
 * @param tura
 * @param pocetak - indeks pocetka komponente
 * @param pocetni - element koji predstavlja pocetni grad
 * @return indeks na kojem se nalazi zadnji element trazene komponente
 */
int krajKomponente(Lista<int> &tura, int pocetak, int pocetni) {
    for(int i = pocetak; i < tura.size(); i++) {
        if (tura[i] == pocetni)
            return i-1;
        if (i == tura.size() - 1)
            return i;
    }
    return pocetak;
}

/**
 * Pomocna funkcija. Brise pocetne elemente koji se ponavljaju 2 puza za redom. Mijenja proslijedjeni parametar tura
 * (Jer algoritam u nekim slucajevima ostavi duple pocetne elemente u turi nakon spajanja komponenti)
 * @param tura
 * @param pocetni - element koji predstavlja pocetni grad
 */
void obrisiDuple(Lista<int> &tura, int pocetni) {
    for (int i = 1; i < tura.size() -1; i++) {
        if (tura[i-1] == tura[i] && tura[i] == pocetni) {
            tura.izbrisi(i);
            i--;
        }
    }
}

/**
 * Spaja dvije komponente u jednu. Poziva @see krajKomponente i @see obrisiDuple. Mijenja proslijedjeni parametar tura
 * @param tura
 * @param poc_1 - indeks pocetka prve komponente
 * @param poc_2  - indeks pocetka druge komponente
 * @param pocetni - element koji predstavlja pocetni grad
 */
void spojiKomponente(Lista<int> &tura, int poc_1, int poc_2, int pocetni) {
    int kraj_1 = krajKomponente(tura, poc_1, pocetni);
    int kraj_2 = krajKomponente(tura, poc_2, pocetni);

    Lista<int>::Cvor * cvor_kraj_1 = tura.get(kraj_1);
    Lista<int>::Cvor * cvor_kraj_2 = tura.get(kraj_2);
    Lista<int>::Cvor * cvor_prije_poc_2 = tura.get(poc_2-1);
    Lista<int>::Cvor * cvor_poc_2 = tura.get(poc_2);

    cvor_prije_poc_2->sljedeci = cvor_kraj_2->sljedeci;
    cvor_kraj_2->sljedeci = cvor_kraj_1->sljedeci;
    cvor_kraj_1->sljedeci = cvor_poc_2;
    tura.izbrisi(cvor_prije_poc_2);

}

/**
 * Pomocna funkcija. Za dva proslijedjena elementa odredjuje da li vec pripadaju istoj komponenti.
 * (Jer algoritam nekad pokusava spoijiti pocetak i kraj iste komponente)
 * @param tura
 * @param prvi - prvi odabrani element
 * @param drugi - drugi odabrani element
 * @param pocetni - element koji predstavlja pocetni grad
 * @return true ako se elementi vec nalaze u istoj komponenti, false ako su u dvije razlicite komponente
 */
bool uIstojKomponenti(Lista<int> &tura, int prvi, int drugi, int pocetni) {
    if(prvi > drugi) {
        int temp = prvi;
        prvi = drugi;
        drugi = temp;
    }
    for (int i = prvi; i <= drugi; i++) {
        if(tura[i] == pocetni)
            return false;
    }
    return true;
}

/**
 * Pomocna funkcija. Kreira pocetnu turu po specifikaciji projekta. Mijenja proslijedjeni parametar tura
 * @param tura
 * @param n - broj gradova
 */
void pocetnaTura(Lista<int> &tura, int n) {
    for (int i = 0; i < n * 2 - 2; i++)
        if (i % 2 != 0)
            tura.dodajNaKraj(i / 2 + 1);
        else
            tura.dodajNaKraj(0);
}

/**
 * Pomocna funkcija. Kreira pocetnu turu od bazne ture insertovanjem pocetnog elementa. Mijenja proslijedjeni parametar tura
 * @param tura
 * @param baznaTura - vec kreirana validna tura
 * @param n - broj gradova
 * @param pocetni - element koji predstavlja pocetni grad
 */
void pocetnaTura(Lista<int> &tura, vector<int> &baznaTura, int n) {
    for (int i = 0; i < n * 2 - 2; i++)
        if (i % 2 != 0)
            tura.dodajNaKraj(baznaTura[i / 2 + 1]);
        else
            tura.dodajNaKraj(baznaTura[0]);
}

/**
 * Nalazi elemente i i j za koje se ostvaruje max usteda. Mijenja proslijedjenje parametre: max_usteda, i_max, j_max
 * Poziva @see uIstojKomponenti
 * @param tura
 * @param udaljenosti
 * @param max_usteda - trenutna maksimalna usteda u duzini ture
 * @param i_max - pocetak grane cijim se uvodjenjem dobija maksimalna usteda
 * @param j_max - kraj grane cijim se uvodjenjem dobija maksimalna usteda
 * @param pocetni - element koji predstavlja pocetni grad
 */
void maxUsteda(Lista<int> &tura, vector<vector<int>> &udaljenosti, int& max_usteda,  int &i_max, int &j_max, int pocetni) {
    for (int i = 0; i < tura.size(); i++) {
        for (int j = 0; j < tura.size(); j++) {
            if (j > 0 && tura[j-1] == pocetni && j != i - 1) {
                if (tura[i] != pocetni && tura[j] != pocetni && tura[i] != tura[j] && !uIstojKomponenti(tura, i, j, pocetni)) {

                    int usteda = udaljenosti[tura[i]][pocetni] + udaljenosti[pocetni][tura[j]] -
                                 udaljenosti[tura[i]][tura[j]];

                    if (usteda > max_usteda) {
                        max_usteda = usteda;
                        i_max = i;
                        j_max = j;

                    }

                }
            }
        }
    }
}

/**
 *
 * @param udaljenosti - udaljenosti izmedju gradova
 * @param baznaTura
 * @param pocetni - element koji predstavlja pocetni grad
 * @return duzina ture
 */
int greedy(vector<vector<int>> &udaljenosti, vector<int> &baznaTura) {

    int n = udaljenosti.size();
    auto tura = Lista<int>();
    if (baznaTura.empty())
        pocetnaTura(tura, n);
    else
        pocetnaTura(tura, baznaTura, n);
    int pocetni = tura[0];

    for (int brKomponenti = n; brKomponenti > 0; brKomponenti--) {
        int max_usteda = 0, i_max, j_max;

        maxUsteda(tura, udaljenosti, max_usteda, i_max, j_max, pocetni);
        if (max_usteda > 0) {
            spojiKomponente(tura, i_max, j_max, pocetni);
        }
    }

    // obrisi eventualne "pocetne" koji nisu na pocetku ture
    for(int i = 0; i < tura.size(); i++){
        if (i != 0 && tura[i] == pocetni) {
            tura.izbrisi(i);
            i--;
        }
    }
    cout << tura.size() << endl;
    auto turaVektor = tura.uVektor();
    return duzinaTure(udaljenosti, turaVektor);
}

/**
 * Interface funkcija. Pocinje bez bazne ture.
 * @param udaljenosti - udaljenosti izmedju gradova
 * @param pocetni - element koji predstavlja pocetni grad
 * @return duzina ture
 */
int greedy(vector<vector<int>> &udaljenosti) {
    vector<int> baznaTura;
    return greedy(udaljenosti, baznaTura);
}

int greedyRandom(vector<vector<int>> &udaljenosti) {
    int broj_cvorova = udaljenosti.size();
    vector<int> tura(broj_cvorova);
    for (int i = 0; i < broj_cvorova; i++)
        tura[i] = i;

    return greedy(udaljenosti, tura);
}


#endif //NASP_TSP_GREEDY_GREEDY_H
