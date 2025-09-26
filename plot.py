import matplotlib.pyplot as plt # type: ignore
import numpy as np # type: ignore
import math


# each house has as datamembers lists of strenght and number of nobles 
# to represent their evolution through generations
class Casata:
    def __init__(self, m_cognome, m_evoluzioneForza, m_arrNumNobili):
        self.m_cognome = m_cognome
        self.m_evoluzioneForza = m_evoluzioneForza if m_evoluzioneForza is not None else []
        self.m_arrNumNobili = m_arrNumNobili if m_arrNumNobili is not None else []

    def getCognome(self):
        return self.m_cognome
    
    def getForza(self, i):
        return self.m_evoluzioneForza[i]
    
    def appendForza(self, forza):
        self.m_evoluzioneForza.append(forza)
    
    def getNumeroNobili(self, i):
        return self.m_arrNumNobili[i]
    
    def appendNumNobili(self, n):
        self.m_arrNumNobili.append(n)



file = open("generations.out", "r")

listaCasate = []

current_gen = 0
for line in file.readlines():
    try:
        current_gen = int(line)
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
                casaAggiunta = True
        
        if not casaAggiunta:
            listaCasate.append(Casata(cognome, [forza], [numeroMembri]))


num_casate = len(listaCasate)

# Calculate grid size (as square as possible)
ncols = math.ceil(math.sqrt(num_casate))
nrows = math.ceil(num_casate / ncols)

figure, axes = plt.subplots(nrows, ncols, figsize=(5*ncols, 4*nrows))

# Flatten axes for easy iteration, in case of 2D array
axes = np.array(axes).flatten()

for idx, casa in enumerate(listaCasate):
    ax1 = axes[idx]
    ax2 = ax1.twinx()  # Create a secondary y-axis for noble count

    l1, = ax1.plot(casa.m_evoluzioneForza, color='tab:blue', label=f"Forza {casa.getCognome()}")
    l2, = ax2.plot(casa.m_arrNumNobili, color='tab:orange', label=f"Nobili {casa.getCognome()}")

    ax1.set_title(casa.getCognome())
    ax1.set_ylabel("Strenght", color='tab:blue')
    ax2.set_ylabel("Nobles", color='tab:orange')
    ax1.set_xlabel("Generations")

    ax1.grid(True, linestyle='--', linewidth=0.7, alpha=0.7)


    # Combine legends
    lines = [l1, l2]
    labels = [line.get_label() for line in lines]
    ax1.legend(lines, labels)

# Hide any unused subplots
for ax in axes[len(listaCasate):]:
    ax.set_visible(False)

plt.tight_layout()
plt.savefig("plot.png", dpi=700)
plt.show()
