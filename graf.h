//
// Created by Ognjen on 12.05.2019.
//

#ifndef NASP_TSP_GREEDY_GRAF_H
#define NASP_TSP_GREEDY_GRAF_H

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <climits>
#include <ctime>

#include "netezinski_graf.h"

using namespace std;

struct Grad {
    int indeks;
    double latitude;
    double longitude;

    Grad(int indeks, double latitude, double longitude) : indeks(indeks), latitude(latitude), longitude(longitude) {}
};

class Graf {

    struct Grana {
        int tezina;
        int pocetak;
        int kraj;

        Grana(int tezina, int pocetak, int kraj) : tezina(tezina), pocetak(pocetak), kraj(kraj) {}

        friend bool operator<(const Grana &prva, const Grana &druga) {
            return prva.tezina < druga.tezina;
        }
    };

public:
    int n;
    vector<Grad> gradovi;
    vector<vector<int> > udaljenosti;
    vector<Grana> grane;

    int udaljenost(double x1, double y1, double x2, double y2) {
        return round(sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)));
    }

    Graf(int n);

    Graf(string filename);

    vector<int> vratiTuraNajmanjaGrana();

    int turaNajmanjaGrana();
};

Graf::Graf(int N) {
    n = N;
    const double PI = 4 * atan(1);
    vector<int> indeksi;
    for (int i = 0; i < n; i++)
        indeksi.push_back(i);
    //random_shuffle(indeksi.begin(),indeksi.end());

    for (int i = 0; i < n; i++) {
        int indeks = i;

        double latitude = 1000 * n * sin((indeksi[i] / double(n)) * 2 * PI);
        double longitude = 1000 * n * cos((indeksi[i] / double(n)) * 2 * PI);
        gradovi.push_back(Grad(indeks, latitude, longitude));
    }
    udaljenosti.resize(n);
    for (int i = 0; i < n; i++)
        udaljenosti[i].resize(n);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            udaljenosti[i][j] = udaljenost(gradovi[i].latitude, gradovi[i].longitude, gradovi[j].latitude,
                                           gradovi[j].longitude);
            if (i < j)
                grane.push_back(Grana(udaljenosti[i][j], i, j));
        }
}

Graf::Graf(string filename) {
    ifstream unos(filename);
    while (!unos.eof()) {
        int indeks;
        double latitude;
        double longitude;
        unos >> indeks;
        unos.get();
        unos >> latitude;
        unos.get();
        unos >> longitude;
        unos.get();
        gradovi.push_back(Grad(indeks, latitude, longitude));
    }
    unos.close();
    n = gradovi.size();
    udaljenosti.resize(n);
    for (int i = 0; i < n; i++)
        udaljenosti[i].resize(n);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            udaljenosti[i][j] = udaljenost(gradovi[i].latitude, gradovi[i].longitude, gradovi[j].latitude,
                                           gradovi[j].longitude);
            if (i < j)
                grane.push_back(Grana(udaljenosti[i][j], i, j));
        }
}

int duzinaTure(vector<vector<int>> &udaljenosti, vector<int> &tura) {
    int duzina_ture = 0;
    int n = tura.size();

    for (int i = 0; i < n; i++)
        duzina_ture += udaljenosti[tura[i]][tura[(i + 1) % n]];
    return duzina_ture;
}

void ispisiTuru(vector<int> &tura) {
    for (int i = 0; i < tura.size(); i++)
        cout << tura[i] << " ";
    cout << endl;
}

bool provjeriTuru(vector<int> &tura) {
    int n = tura.size();
    vector<bool> posjecen(n, false);
    if (tura.size() != n)
        return false;
    else {
        for (int i = 0; i < n; i++) {
            if (tura[i] < 0 || tura[i] >= n || posjecen[tura[i]] == true)
                return false;
            else
                posjecen[tura[i]] = true;
        }
    }
    return true;
}

int turaNajbliziSusjed(vector<vector<int>> &udaljenosti, int pocetni) {

    vector<int> tura;
    int n = udaljenosti.size();
    vector<bool> posjecen(n, false);
    posjecen[pocetni] = true;
    tura.push_back(pocetni);
    for (int i = 0; i < n - 1; i++) {
        int trenutni = tura[tura.size() - 1];
        int udaljenostNajblizeg = INT_MAX;
        int najblizi;
        for (int j = 0; j < n; j++) {
            if (!posjecen[j] && udaljenosti[trenutni][j] < udaljenostNajblizeg) {
                udaljenostNajblizeg = udaljenosti[trenutni][j];
                najblizi = j;
            }
        }
        posjecen[najblizi] = true;
        tura.push_back(najblizi);
    }
    if (provjeriTuru(tura))
        return duzinaTure(udaljenosti, tura);
    else
        throw "Greska";
}

vector<int> turaNS(vector<vector<int>> &udaljenosti, int pocetni) {
    vector<int> tura;
    int n = udaljenosti.size();
    vector<bool> posjecen(n, false);
    posjecen[pocetni] = true;
    tura.push_back(pocetni);
    for (int i = 0; i < n - 1; i++) {
        int trenutni = tura[tura.size() - 1];
        int udaljenostNajblizeg = INT_MAX;
        int najblizi;
        for (int j = 0; j < n; j++) {
            if (!posjecen[j] && udaljenosti[trenutni][j] < udaljenostNajblizeg) {
                udaljenostNajblizeg = udaljenosti[trenutni][j];
                najblizi = j;
            }
        }
        posjecen[najblizi] = true;
        tura.push_back(najblizi);
    }
    if (provjeriTuru(tura))
        return tura;
    else
        throw "Greska";
}

int Graf::turaNajmanjaGrana() {
    Ruta ruta(n);
    sort(grane.begin(), grane.end());
    vector<int> stepeni(n, 0);
    int trenutna = 0;
    for (int i = 0; i < n - 1; i++) {
        while (true) {
            Grana trenutnaGrana = grane[trenutna];
            int pocetak = trenutnaGrana.pocetak;
            int kraj = trenutnaGrana.kraj;
            if (stepeni[pocetak] == 2 || stepeni[kraj] == 2) {
                trenutna++;
                continue;
            }
            ruta.dodajGranu(pocetak, kraj);
            if (ruta.imaLiCiklusa(pocetak)) {
                ruta.obrisiGranu(pocetak, kraj);
                trenutna++;
                continue;
            } else {
                stepeni[pocetak]++;
                stepeni[kraj]++;
                trenutna++;
                break;
            }

        }
    }

    vector<int> tura = ruta.vratiTuru();
    if (provjeriTuru(tura))
        return duzinaTure(udaljenosti, tura);
    else
        throw "Greska";
}

vector<int> Graf::vratiTuraNajmanjaGrana() {
    Ruta ruta(n);
    sort(grane.begin(), grane.end());
    vector<int> stepeni(n, 0);
    int trenutna = 0;
    for (int i = 0; i < n - 1; i++) {
        while (true) {
            Grana trenutnaGrana = grane[trenutna];
            int pocetak = trenutnaGrana.pocetak;
            int kraj = trenutnaGrana.kraj;
            if (stepeni[pocetak] == 2 || stepeni[kraj] == 2) {
                trenutna++;
                continue;
            }
            ruta.dodajGranu(pocetak, kraj);
            if (ruta.imaLiCiklusa(pocetak)) {
                ruta.obrisiGranu(pocetak, kraj);
                trenutna++;
                continue;
            } else {
                stepeni[pocetak]++;
                stepeni[kraj]++;
                trenutna++;
                break;
            }

        }
    }

    vector<int> tura = ruta.vratiTuru();
    if (provjeriTuru(tura))
        return tura;
    else
        throw "Greska";
}

void Zamijeni_Grane(vector<int> &tura, int i, int j) {
    for (int k = 0; k < (j - i) / 2; k++) {
        int pomocna = tura[i + 1 + k];
        tura[i + 1 + k] = tura[j - k];
        tura[j - k] = pomocna;
    }
}

void Zamijeni_Grane(vector<int> &tura, int i, int j, int k, int tip) {
    vector<int> nova_tura = tura;

    if (tip == 3) {
        for (int t = i + 1; t <= i + k - j; t++)
            nova_tura[t] = tura[k - t + i + 1];
        for (int t = i + k - j + 1; t <= k; t++)
            nova_tura[t] = tura[t - k + j];
    } else if (tip == 2) {
        for (int t = i + 1; t <= i + k - j; t++)
            nova_tura[t] = tura[j + t - i];
        for (int t = i + k - j + 1; t <= k; t++)
            nova_tura[t] = tura[t - k + j];
    } else if (tip == 1) {
        for (int t = i + 1; t <= j; t++)
            nova_tura[t] = tura[j + i + 1 - t];
        for (int t = j + 1; t <= k; t++)
            nova_tura[t] = tura[k + j + 1 - t];
    } else if (tip == 0) {
        for (int t = i + 1; t <= i + k - j; t++)
            nova_tura[t] = tura[j + t - i];
        for (int t = i + k - j + 1; t <= k; t++)
            nova_tura[t] = tura[i + k + 1 - t];
    }
    tura = nova_tura;
}

vector<int> _2OptSaRandom(vector<vector<int>> &udaljenosti) {
    int broj_cvorova = udaljenosti.size();
    vector<int> tura(broj_cvorova);
    for (int i = 0; i < broj_cvorova; i++)
        tura[i] = i;
    random_shuffle(tura.begin(), tura.end());

    //cout<<"Duzina pocetne ture: "<<duzinaTure(udaljenosti,tura)<<endl;
    while (true) {
        bool nadjena_bolja_tura = false;

        for (int i = 0; i < broj_cvorova - 1; i++)
            for (int j = i + 2; j < broj_cvorova; j++) {
                if (udaljenosti[tura[i]][tura[i + 1]] + udaljenosti[tura[j]][tura[(j + 1) % broj_cvorova]]
                    > (udaljenosti[tura[i]][tura[j]] + udaljenosti[tura[i + 1]][tura[(j + 1) % broj_cvorova]])) {
                    Zamijeni_Grane(tura, i, j);
                    nadjena_bolja_tura = true;
                }
            }
        if (!nadjena_bolja_tura)
            break;
    }
    return tura;
}

vector<int> _2OptSaRandomMax(vector<vector<int>> &udaljenosti) {
    int broj_cvorova = udaljenosti.size();
    vector<int> tura(broj_cvorova);
    for (int i = 0; i < broj_cvorova; i++)
        tura[i] = i;
    random_shuffle(tura.begin(), tura.end());

    //cout<<"Duzina pocetne ture: "<<duzinaTure(udaljenosti,tura)<<endl;
    while (true) {
        int najveca_usteda = 0;
        int indeks_i, indeks_j;

        for (int i = 0; i < broj_cvorova - 1; i++)
            for (int j = i + 2; j < broj_cvorova; j++) {
                int usteda = udaljenosti[tura[i]][tura[i + 1]] + udaljenosti[tura[j]][tura[(j + 1) % broj_cvorova]]
                             - (udaljenosti[tura[i]][tura[j]] + udaljenosti[tura[i + 1]][tura[(j + 1) % broj_cvorova]]);

                if (usteda > najveca_usteda) {
                    najveca_usteda = usteda;
                    indeks_i = i;
                    indeks_j = j;
                }
            }
        if (najveca_usteda == 0)
            break;
        Zamijeni_Grane(tura, indeks_i, indeks_j);
        cout << duzinaTure(udaljenosti, tura) << endl;
    }
    return tura;
}

void _2OptSaPocetnom(vector<vector<int>> &udaljenosti, vector<int> &tura) {

    int broj_cvorova = tura.size();
    while (true) {
        bool nadjena_bolja_tura = false;

        for (int i = 0; i < broj_cvorova - 1; i++)
            for (int j = i + 2; j < broj_cvorova; j++) {
                if (udaljenosti[tura[i]][tura[i + 1]] + udaljenosti[tura[j]][tura[(j + 1) % broj_cvorova]]
                    > (udaljenosti[tura[i]][tura[j]] + udaljenosti[tura[i + 1]][tura[(j + 1) % broj_cvorova]])) {
                    Zamijeni_Grane(tura, i, j);
                    nadjena_bolja_tura = true;
                }
            }
        if (!nadjena_bolja_tura)
            break;
    }

}

int _3OptSaPocetnom(vector<vector<int>> &udaljenosti, vector<int> &tura) {
    int broj_cvorova = tura.size();
    while (true) {
        bool nadjena_bolja_tura = false;

        for (int i = 0; i < broj_cvorova - 2; i++)
            for (int j = i + 1; j < broj_cvorova - 1; j++)
                for (int k = j + 1; k < broj_cvorova; k++) {
                    if (udaljenosti[tura[i]][tura[i + 1]]
                        + udaljenosti[tura[j]][tura[(j + 1)]]
                        + udaljenosti[tura[k]][tura[(k + 1) % broj_cvorova]]
                        > udaljenosti[tura[i]][tura[k]]
                          + udaljenosti[tura[i + 1]][tura[(j + 1)]]
                          + udaljenosti[tura[j]][tura[(k + 1) % broj_cvorova]]) {
                        Zamijeni_Grane(tura, i, j, k, 3);
                        cout << duzinaTure(udaljenosti, tura) << endl;
                        nadjena_bolja_tura = true;
                    }
                    if (udaljenosti[tura[i]][tura[i + 1]]
                        + udaljenosti[tura[j]][tura[(j + 1)]]
                        + udaljenosti[tura[k]][tura[(k + 1) % broj_cvorova]]
                        > udaljenosti[tura[i]][tura[j + 1]]
                          + udaljenosti[tura[i + 1]][tura[k]]
                          + udaljenosti[tura[j]][tura[(k + 1) % broj_cvorova]]) {
                        Zamijeni_Grane(tura, i, j, k, 2);
                        cout << duzinaTure(udaljenosti, tura) << endl;
                        nadjena_bolja_tura = true;
                    }
                    if (udaljenosti[tura[i]][tura[i + 1]]
                        + udaljenosti[tura[j]][tura[(j + 1)]]
                        + udaljenosti[tura[k]][tura[(k + 1) % broj_cvorova]]
                        > udaljenosti[tura[i]][tura[j]]
                          + udaljenosti[tura[i + 1]][tura[k]]
                          + udaljenosti[tura[j + 1]][tura[(k + 1) % broj_cvorova]]) {
                        Zamijeni_Grane(tura, i, j, k, 1);
                        cout << duzinaTure(udaljenosti, tura) << endl;
                        nadjena_bolja_tura = true;
                    }
                    if (udaljenosti[tura[i]][tura[i + 1]]
                        + udaljenosti[tura[j]][tura[(j + 1)]]
                        + udaljenosti[tura[k]][tura[(k + 1) % broj_cvorova]]
                        > udaljenosti[tura[i]][tura[j + 1]]
                          + udaljenosti[tura[k]][tura[(j)]]
                          + udaljenosti[tura[i + 1]][tura[(k + 1) % broj_cvorova]]) {
                        Zamijeni_Grane(tura, i, j, k, 0);
                        cout << duzinaTure(udaljenosti, tura) << endl;
                        nadjena_bolja_tura = true;
                    }

                }
        if (!nadjena_bolja_tura)
            break;
    }
    return duzinaTure(udaljenosti, tura);
}

vector<int> _3OptSaRandom(vector<vector<int>> &udaljenosti) {
    int broj_cvorova = udaljenosti.size();
    vector<int> tura(broj_cvorova);
    for (int i = 0; i < broj_cvorova; i++)
        tura[i] = i;
    random_shuffle(tura.begin(), tura.end());
    while (true) {
        bool nadjena_bolja_tura = false;

        for (int i = 0; i < broj_cvorova - 2; i++)
            for (int j = i + 1; j < broj_cvorova - 1; j++)
                for (int k = j + 1; k < broj_cvorova; k++) {
                    if (udaljenosti[tura[i]][tura[i + 1]]
                        + udaljenosti[tura[j]][tura[(j + 1)]]
                        + udaljenosti[tura[k]][tura[(k + 1) % broj_cvorova]]
                        > udaljenosti[tura[i]][tura[k]]
                          + udaljenosti[tura[i + 1]][tura[(j + 1)]]
                          + udaljenosti[tura[j]][tura[(k + 1) % broj_cvorova]]) {
                        Zamijeni_Grane(tura, i, j, k, 3);
                        cout << duzinaTure(udaljenosti, tura) << endl;
                        nadjena_bolja_tura = true;
                    }
                    if (udaljenosti[tura[i]][tura[i + 1]]
                        + udaljenosti[tura[j]][tura[(j + 1)]]
                        + udaljenosti[tura[k]][tura[(k + 1) % broj_cvorova]]
                        > udaljenosti[tura[i]][tura[j + 1]]
                          + udaljenosti[tura[i + 1]][tura[k]]
                          + udaljenosti[tura[j]][tura[(k + 1) % broj_cvorova]]) {
                        Zamijeni_Grane(tura, i, j, k, 2);
                        cout << duzinaTure(udaljenosti, tura) << endl;
                        nadjena_bolja_tura = true;
                    }
                    if (udaljenosti[tura[i]][tura[i + 1]]
                        + udaljenosti[tura[j]][tura[(j + 1)]]
                        + udaljenosti[tura[k]][tura[(k + 1) % broj_cvorova]]
                        > udaljenosti[tura[i]][tura[j]]
                          + udaljenosti[tura[i + 1]][tura[k]]
                          + udaljenosti[tura[j + 1]][tura[(k + 1) % broj_cvorova]]) {
                        Zamijeni_Grane(tura, i, j, k, 1);
                        cout << duzinaTure(udaljenosti, tura) << endl;
                        nadjena_bolja_tura = true;
                    }
                    if (udaljenosti[tura[i]][tura[i + 1]]
                        + udaljenosti[tura[j]][tura[(j + 1)]]
                        + udaljenosti[tura[k]][tura[(k + 1) % broj_cvorova]]
                        > udaljenosti[tura[i]][tura[j + 1]]
                          + udaljenosti[tura[k]][tura[(j)]]
                          + udaljenosti[tura[i + 1]][tura[(k + 1) % broj_cvorova]]) {
                        Zamijeni_Grane(tura, i, j, k, 0);
                        cout << duzinaTure(udaljenosti, tura) << endl;
                        nadjena_bolja_tura = true;
                    }

                }
        if (!nadjena_bolja_tura)
            break;
    }
    return tura;
}

int iscrpnaPretraga(vector<vector<int>> &udaljenosti) {

    int n = udaljenosti.size();
    vector<int> tura(n);
    for (int i = 0; i < n; i++)
        tura[i] = i;

    int najkraca_tura = INT_MAX;
    do {
        if (duzinaTure(udaljenosti, tura) < najkraca_tura)
            najkraca_tura = duzinaTure(udaljenosti, tura);
    } while (next_permutation(tura.begin(), tura.end()));

    return najkraca_tura;

}


#endif //NASP_TSP_GREEDY_GRAF_H
