#include <iostream>
#include <vector>
#include <ctime>

#include "lista.h"
#include "graf.h"
#include "greedy.h"

using namespace std;

int main() {
    Graf G("test.txt");
    cout << "Najblizi susjed:         " << turaNajbliziSusjed(G.udaljenosti, 0) << endl;
    cout << "Najmanja grana:          " << G.turaNajmanjaGrana() << endl;
    vector<int> tura = G.vratiTuraNajmanjaGrana();
//    cout << "2 Opt Random:            " << duzinaTure(G.udaljenosti, tura) << endl;
//    cout << "Greedy 2 Opt Random:     " << greedy(G.udaljenosti, tura) << endl;
//    tura = _2OptSaRandomMax(G.udaljenosti);
//    cout << "2 Opt Random Max:        " << duzinaTure(G.udaljenosti, tura) << endl;
//    cout << "Greedy 2 Opt Random Max: " << greedy(G.udaljenosti, tura) << endl;
//    vector<int> tura = _3OptSaRandom(G.udaljenosti);
//    cout << "3 Opt Random:            " << duzinaTure(G.udaljenosti, tura) << endl;
    cout << "Greedy 3 Opt Random:     " << greedy(G.udaljenosti, tura) << endl;
    cout << "Greedy random:           " << greedy(G.udaljenosti) << endl;

    return 0;
}