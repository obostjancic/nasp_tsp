#include <iostream>
#include <vector>
#include <ctime>

#include "lista.h"
#include "graf.h"
#include "greedy.h"

using namespace std;

int main() {
    Graf G("test.txt");
    vector<int> tura = turaNS(G.udaljenosti,0);
    cout << turaNajbliziSusjed(G.udaljenosti, 0) << endl;
    cout << G.turaNajmanjaGrana() << endl;

    clock_t start = clock();
    tura = G.vratiTuraNajmanjaGrana();
    cout << greedy(G.udaljenosti, tura, 0) << endl;
    clock_t afterGreedy = clock();
    cout << "Greedy: " << double(afterGreedy - start) / CLOCKS_PER_SEC << endl;
    return 0;
}