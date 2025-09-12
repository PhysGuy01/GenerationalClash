#pragma once
#include <string>
#include <random>

using namespace std;

/*

CRISI ESISTENZIALE
COME PROCEDO???
HO BISOGNO DI UN OGGETTO CASATA CHE RACCHIUDE TUTTI I NOBILI DELLO STESSO COGNOME
OGNI VOLTA CHE UN NOBILE DELLA CASATA SI SPOSA DEVE AGGIORNARE LA FORZA DELLA SUA CASATA
MA COME CAZZO LO TRADUCO IN C++?????

ok quindi una casata e' un vettore di nobili (crea la classe) 

ricordati di mettere un seed fissato in modo da riprodurre i risultati

*/


class Casata {
    
    private:
        string m_cognome;
        int m_forza;
        bool m_genere;      // 0: femmina, 1: maschio
        bool m_sposato;     
        int m_figli;
        // int m_numMembri;
    
    public:

        Casata() {;}
        Casata(string cognome, int forza, bool gen) { m_cognome = cognome; m_forza = forza; m_genere = gen;}

        void setCognome(string cognome) { m_cognome = cognome; }
        string getCognome() const {return m_cognome;}
        
        void setForza(int forza) { m_forza = forza; }
        int getForza() const { return m_forza; }

        void setSposato(bool spos) { m_sposato = spos;}
        bool getSposato() const {return m_sposato;}

        void setFigli(int figli) { m_figli = figli; }
        int getFigli() const { return m_figli; }

        // assegna un numero di figli casuale estratto da una gaussiana centrata in 3 con std dev di 1.5
        // lo assegna sia alla casata corrente che a quella del coniuge
        void calcolaFigli(Casata& coniuge) {
            double figliMedi = 3.;
            double stddev = 1.5;

            random_device rd;
            mt19937 gen(rd()); // questo devi fare in modo da passarlo per referenza dal main direi

            do {
                normal_distribution<double> normal_dist(figliMedi, stddev);
                m_figli = static_cast<int>(normal_dist(gen));
                coniuge.setFigli(m_figli);
                
            } while (m_figli < 0);


        }

        // sposa una casata con un'altra generando un numero di figli e aggiustando cognomi e forza
        void honeymoon(Casata& coniuge) {
            calcolaFigli(coniuge);

            string cognome = (m_genere ? m_cognome : coniuge.getCognome());
            int forza;
            // La forza... 
            // se il coniuge è uyna donna di una casata piu forte

            vector<Casata> vecFigli;
            for (int i = 0; i < getFigli(); i++) {
                Casata figlio(cognome, forza, rand() % 2 + 1);
            }
            
        }

};


// NOTA: RICORDATI CHE C'E' UN BIAS PER LE FIGLIE DI SPOSARSI CON UOMINI DI RANGO PIU ELEVATO PIUTTOSTO CHE IL CONTRARIO

class listaCasate {
    private:
        vector<Casata> m_casate;
        int m_nGenMax; // numero massimo di generazioni
        int m_nGen; // generazione corrente

    public:

        listaCasate(int gen) {
            m_nGenMax = gen; 
        }

        void push_back(Casata cas) {
            m_casate.push_back(cas);
        }

        void generazione() {
            m_nGen++;

        }
};