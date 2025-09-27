
# each house has as datamembers lists of strenght and number of nobles 
# to represent their evolution through generations
class Casata:
    def __init__(self, m_cognome, m_evoluzioneForza, m_arrNumNobili, nGen):
        self.m_cognome = m_cognome
        self.m_evoluzioneForza = m_evoluzioneForza if m_evoluzioneForza is not None else []
        self.m_arrNumNobili = m_arrNumNobili if m_arrNumNobili is not None else []
        self.m_nGen = nGen

    def getCognome(self):
        return self.m_cognome
    
    def getForza(self):
        return self.m_evoluzioneForza
        
    def appendForza(self, forza):
        self.m_evoluzioneForza.append(forza)
    
    def getNumeroNobili(self):
        return self.m_arrNumNobili
    
    def appendNumNobili(self, n):
        self.m_arrNumNobili.append(n)

    def updateNgen(self):
        self.m_nGen += 1

    def getNgen(self):
        return self.m_nGen

def loadCasate(filename, listaCasate, list_of_integer_lines):

    try:
        file = open(filename, "r")
    except:
        raise RuntimeError('Couldn\'t find ' + filename + 'in this directory. Did you run the simulation?') 

    current_gen = 0
    for line in file.readlines():
        try:
            current_gen = int(line)
            list_of_integer_lines.append(current_gen)
        except:
            datiLinea = line.split()
            cognome = datiLinea[0]
            forza = int(datiLinea[1])
            numeroMembri = int(datiLinea[2])


            casaAggiunta = False
            for casaTest in listaCasate:
                if cognome == casaTest.getCognome():
                    casaTest.appendForza(forza)
                    casaTest.appendNumNobili(numeroMembri)
                    casaTest.updateNgen()
                    casaAggiunta = True
            
            if not casaAggiunta:
                listaCasate.append(Casata(cognome, [forza], [numeroMembri], 1))
