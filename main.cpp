// -------------------------------------
//      Author: Francesco Specchia
// -------------------------------------

#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <random>
#include <cctype>
#include <locale>


#include "Nobile.h"

using namespace std;


// Trim a string from the end (in place)
inline void rtrim(string &s) {
    s.erase(find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !isspace(ch);
    }).base(), s.end());
}


int main(int argc, char** argv) {

    if (argc < 4) {
        cout << "Utilizzo: " << argv[0] << " <seed(32-bit)> <n_casate(max706)> <n_generazioni>" << endl;
        return -1;
    }
    unsigned long long int seed = atoi(argv[1]);
    mt19937 gen(seed);

    int nCasate = stoi(argv[2]);
    int nGenerazioni = stoi(argv[3]);


    // Carica i nomi delle casate e crea un array
    ifstream filenomi("houses_names.txt");
    listaCasate casate(nGenerazioni, gen);

    string nomex;
    int i = 0;
    while ( getline(filenomi, nomex) ) {
        int forzainiz = 1;
        rtrim(nomex);
        Casata* casa = new Casata(nomex, forzainiz, gen);

        // Genera 2 nobili per ogni casata, uno m l'alro f
        bool maschio = 1, dasposare = true;
        Nobile* adamo = new Nobile(maschio, dasposare);
        Nobile* eva = new Nobile(!maschio, dasposare);

        casa->push_back(adamo);
        casa->push_back(eva);

        casate.push_back(casa);
        
        if (casate.size() >= nCasate) break;

        i++;
    }

    filenomi.close();


    // outputs data of the generation 0 (initial values)
    fstream outgen;
    outgen.open("generations.out", ios::app);
    outgen << seed << endl;
    outgen << 0 << endl; // gen = 0

    for (Casata* casa : casate) 
        outgen << casa->getCognome() << " " << casa->getForzaCasa() << " " << casa->size() << " " << 0 << endl;
    
    outgen.close();


    casate.eseguiNgenerazioni();


    return 0;
}