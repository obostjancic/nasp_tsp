#include <iostream>
#include <vector>
#include <ctime>

#include "graf.h"

using namespace std;



int main() {
    Graf G("test.txt");
    /*vector<int> tura = turaNS(G.udaljenosti,0);
    _2OptSaPocetnom(G.udaljenosti,tura);
    cout<<duzinaTure(G.udaljenosti,tura);*/

//  long int zbir = 0;
//  for(int i=0;i<50;i++) {
//    vector<int> tura = _2OptSaRandom(G.udaljenosti);
//    //cout<<"Duzina konacne ture: "<<duzinaTure(G.udaljenosti,tura)<<endl;
//    zbir += duzinaTure(G.udaljenosti,tura);
//  }
//  cout<<zbir/50;

    /*long int zbir = 0;
    for(int i=0;i<1;i++) {
      vector<int> tura = _3OptSaRandom(G.udaljenosti);
      //cout<<"Duzina konacne ture: "<<duzinaTure(G.udaljenosti,tura)<<endl;
      zbir += duzinaTure(G.udaljenosti,tura);
    }
    cout<<zbir;*/
    /*int n = G.udaljenosti.size();
    vector<vector<int>> state(n);
      for(auto& neighbors : state)
          neighbors = vector<int>((1 << n) - 1, INT_MAX);
    cout<<tsp(G.udaljenosti,0,1,state);*/
    //Graf G(5);
//    int final_res = INT_MAX;
    //TSP(G.udaljenosti,final_res);
//    cout<<final_res;
    clock_t start = clock();
    cout << greedy(G) << endl;
    clock_t afterGreedy = clock();
    cout << "Greedy: " << double(afterGreedy - start) / CLOCKS_PER_SEC << endl;
    cout << iscrpnaPretraga(G.udaljenosti) << endl;
    clock_t afterFullSearch = clock();
    cout << "Full search: " << double(afterFullSearch - afterGreedy) / CLOCKS_PER_SEC << endl;


//    static const int arr[] = {0, 1, 0, 2, 4, 7, 8, 6, 5, 0, 3, 0, 9};
//    vector<int> tura (arr, arr + sizeof(arr) / sizeof(arr[0]) );
//    ispisiTuru(tura);
//    spojiKomponente(tura, 10, 3, 0);
//    ispisiTuru(tura);
    return 0;
}