#pragma once
#include <random>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>

using namespace std;

class Nobile {
    private:
        int m_forzaNob;
        int m_nfigli;
        bool m_dasposare;

        bool m_gender; // 0: femmina, 1: maschio

    public:
        Nobile(int gender, bool dasposare) : m_gender(gender), m_dasposare(dasposare), m_nfigli(0) {}

        void setFigli(int figli) { m_nfigli = figli; }
        int getFigli() const { return m_nfigli; }

        void setDaSposare(bool dasposare) { m_dasposare = dasposare; }
        bool getDaSposare() const { return m_dasposare; }

        bool getGender() const { return m_gender; }

        bool operator==(const Nobile& other) const {
            return this == &other;
        }
        bool operator!=(const Nobile& other) const {
            return !(this == &other);
        }

        // calcola i figli che hanno il nobile e il suo coniuge
        void calcolaFigli(Nobile* coniuge, mt19937& m_gen) {
            double figliMedi = 3.;
            double stddev = 1.5;

            do {
                normal_distribution<double> normal_dist(figliMedi, stddev);
                m_nfigli = static_cast<int>(normal_dist(m_gen));
                coniuge->setFigli(m_nfigli);
                
            } while (m_nfigli < 0);
        }
};



class Casata : public vector<Nobile*> {
    private:
        mt19937& m_gen;
        
        string m_cognome;
        int m_forzaCasa;
        int m_forzaParziale; // forza accumulata da ogni matrimonio in una gen che viene poi sommata alla forza totale alla fine della gen

    public: 
        Casata(string cognome, int forza, mt19937& gen) : m_cognome(cognome), m_forzaCasa(forza), m_gen(gen), m_forzaParziale(0) {}

        void setCognome(string cognome) { m_cognome = cognome; }
        string getCognome() const {return m_cognome;}
        
        int getForzaCasa() const { return m_forzaCasa; }
        void addForza(int forza) { m_forzaCasa += forza; } // use of this module is discouraged. Please use addForzaParz()
        void addForzaParz(int forza) { m_forzaParziale += forza;}
        void updateForza() { m_forzaCasa += m_forzaParziale; m_forzaParziale = 0;}

        // trova il nobile e lo elimina
        void deleteMember(Nobile* oldNobile) {
            auto it = find(begin(), end(), oldNobile);
            if (it != end()) {
                delete *it;
                erase(it);  
            }
        }       

        // sposa un nobile con un'altro generando un numero di figli e assegnandoli alla casata del padre
        // infine elimina i nobili genitori
        void honeymoon(Nobile* nob, Nobile* coniuge, Casata* casaConiuge) {
            nob->calcolaFigli(coniuge, m_gen);

            Casata* casaPadre = (nob->getGender() ? this : casaConiuge);


            // Casa piu' alta riceve un guadagno dato dalla funzione y1 = 1 - Delta/4 
            // Casa più bassa riceve un guadagno dato dalla funzione y2 = 1 + Delta/2
            // in questo modo l'ipergamia e' premiata e l'ipogamia e' leggermente penalizzata
            int Delta = this->getForzaCasa() - casaConiuge->getForzaCasa();
            int forzaIpogamo = 1 - Delta / 4;
            int forzaIpergamo = 1 + Delta / 2;
            if (Delta > 0) { // this > coniuge
                this->addForzaParz(forzaIpogamo);
                casaConiuge->addForzaParz(forzaIpergamo);
            } else {
                this->addForzaParz(forzaIpergamo);
                casaConiuge->addForzaParz(forzaIpogamo);
            }


            uniform_int_distribution<> testacroce(0, 1);
            for (int i = 0; i < nob->getFigli(); i++) {
                bool rand01 = testacroce(m_gen);
                bool daNONsposare = false; // e' ancora un bambino...

                casaPadre->push_back(new Nobile(rand01, daNONsposare));
            }

            coniuge->setDaSposare(false);
            nob->setDaSposare(false);
            
        }

        // Setta dasposare = true in ogni nobile nel vettore Casata per la nuova generazione
        // E uccide tutti i nobili che non si sono sposati nella scorsa generazione (lol)
        void resetDaSposare() {
            auto it = begin();
            while (it != end()) {
                if ((*it)->getFigli() > 0) {
                    delete *it;
                    it = erase(it); // erase returns the next valid iterator
                } else {
                    (*it)->setDaSposare(true);
                    ++it;
                }
            }
        }
};


class listaCasate : public vector<Casata*> {
    private:
        int m_nGenMax; // numero massimo di generazioni
        int m_currentGen; // generazione corrente

        mt19937& m_gen;

    public:
        listaCasate(int genMax, mt19937& gen) : m_nGenMax(genMax), m_currentGen(1), m_gen(gen) {}

        // Crea un ordinamento tra le classi per sortare i vettori
        static bool comp(const Casata& casa1, const Casata& casa2) {
            return (casa1.getForzaCasa() > casa2.getForzaCasa());
        }


        Nobile* cercaConiuge(Nobile* nob, int iterCasata, exponential_distribution<>& probabCasata, int& posizCasataConiuge) {
            vector<int> alreadyCheckedpositions;
            int maxTries = size() - iterCasata; // Limita i tentativi

            for (int tries = 0; tries < maxTries; ++tries) {
                int prob = probabCasata(m_gen);
                posizCasataConiuge = 1 + iterCasata + prob;

                // Gestione bordo: se si supera la fine, si torna all'ultima casata disponibile
                if (posizCasataConiuge >= size()) {
                    posizCasataConiuge = size() - 1;
                }

                // Evita di controllare due volte la stessa posizione
                if (std::find(alreadyCheckedpositions.begin(), alreadyCheckedpositions.end(), posizCasataConiuge) != alreadyCheckedpositions.end()) {
                    continue;
                }
                alreadyCheckedpositions.push_back(posizCasataConiuge);

                Casata* casaConiuge = (*this)[posizCasataConiuge];
                for (Nobile* coniuge : *casaConiuge) {
                    if (coniuge->getDaSposare() && (coniuge->getGender() != nob->getGender())) {
                        return coniuge;
                    }
                }
            }
            // Nessun coniuge trovato, ritorna il nobile stesso
            // i know its bad dont judge me~ >.<
            return nob;
        }

        Nobile* gayMarriage() {;} // yet to implement
        Nobile* sweetHomeAlabama() {;} // Searches for a partner within the house of a given noble. Yet to implement

        // Crea un ordinamento tra le classi per sortare i vettori
        static bool compPtr(const Casata* casa1, const Casata* casa2) {
            return (casa1->getForzaCasa() > casa2->getForzaCasa());
        }

        void eseguiGenerazione() {
            sort(begin(), end(), compPtr);

            double lambda = .5;
            exponential_distribution<> probabCasata(lambda); 
            int iterCasata = 0;
            int countrMatrimoni = 0;
            for (Casata* casa : *this) {
                for (size_t i = 0; i < casa->size(); ++i) { 
                    Nobile* nob = (*casa)[i];
                    if (nob->getDaSposare()) {
                        int posizCasataConiuge;
                        Nobile* coniuge = cercaConiuge(nob, iterCasata, probabCasata, posizCasataConiuge);

                        if (nob != coniuge) {
                            casa->honeymoon(nob, coniuge, (*this)[posizCasataConiuge]);
                            countrMatrimoni++;
                        }
                    }
                }

                iterCasata++;
            }

            // outputs data of each house after this generation
            fstream outgen;
            outgen.open("generations.out", ios::app);
            outgen << m_currentGen << endl;
            for (Casata* casa : *this) {
                casa->updateForza();
                casa->resetDaSposare();
                outgen << casa->getCognome() << " " << casa->getForzaCasa() << " " << casa->size() << " " << countrMatrimoni << endl;
            }
            outgen.close();

            m_currentGen++;

        }


        void eseguiNgenerazioni(int nGen = -1) {
            if ((nGen <= 0) || (nGen > m_nGenMax)) nGen = m_nGenMax;
            for (int i = 0; i < m_nGenMax; i++) {
                eseguiGenerazione();
            }
        }


};