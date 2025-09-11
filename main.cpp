#include <vector>
#include <random>
#include <iostream>
#include <cmath>
#include <fstream>
#include <time.h>

#include "Casata.h"

using namespace std;

int main(int argc, char* argv[]) {

    if (argc < 3) {
        cout << "Utilizzo: " << argv[0] << " <n_casate> <n_generazioni>" <<endl;
        return -1;
    }

    srand(time(0));

    // numero di casate e generazioni
    int nCasate = stoi(argv[1]);
    int nGenerazioni = stoi(argv[2]);


    // Carica i nomi delle casate e crea un array
    ifstream filenomi("nomi_casate.txt");
    vector<string> nomiCasate;
    string nomex;

    while ( getline(filenomi, nomex) ) {
        vector<Casata> casate;
        Casata casa(nomex, 1, rand() % 2 + 1);
        casate.push_back(casa);
    
        if (casate.size() >= nCasate) break;
    }






    filenomi.close();


    return 0;
}