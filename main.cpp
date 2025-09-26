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
        cout << "Utilizzo: " << argv[0] << " <seed> <n_casate(max706)> <n_generazioni>" << endl;
        return -1;
    }
    int seed = atoi(argv[1]);
    mt19937 gen(seed);

    int nCasate = stoi(argv[2]);
    int nGenerazioni = stoi(argv[3]);


    // Carica i nomi delle casate e crea un array
    ifstream filenomi("nomi_casate.txt");
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


    casate.eseguiNgenerazioni();



    // -----------------------------------------------------
    //   Scrive su file i dati delle casate per plottarli
    // -----------------------------------------------------

    ofstream output("dati_casate.out");
    for ( Casata* casa : casate ) {
        output << casa->getCognome() << " " << casa->getForzaCasa() << " " << casa->size() << endl;
    }

    output.close();


    return 0;
}