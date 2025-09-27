import matplotlib.pyplot as plt # type: ignore
import matplotlib.ticker as ticker # type: ignore
import numpy as np # type: ignore
import math

from modules import Casata, loadCasate


listaCasate = []
list_of_int_lines = []

loadCasate("generations.out", listaCasate, list_of_int_lines)

n_gen = listaCasate[0].getNgen()
n_houses = len(listaCasate)

fig, axes = plt.subplots(n_gen, 1, figsize=(10, 4 * n_gen), num="Noble houses\' evolution")

# make axes iterable when n_gen == 1
if n_gen == 1:
    axes = [axes]

for gen_idx, ax in enumerate(axes):
    labels = []
    strengths = []
    nobles = []
    for casa in listaCasate:
        labels.append(casa.getCognome())

        # safe extraction of the gen_idx-th element (use 0 if missing)
        forza_list = casa.getForza()
        nob_list = casa.getNumeroNobili()

        strengths.append(forza_list[gen_idx] if gen_idx < len(forza_list) else 0)
        nobles.append(nob_list[gen_idx] if gen_idx < len(nob_list) else 0)

    x = np.arange(n_houses)
    width = 0.7

    ax.grid(True, linestyle='--', linewidth=0.7, alpha=0.7)


    # if you dont want stacked nobles on top of strengths dont use bottom=np.array(strengths) in p2
    p1 = ax.bar(x, strengths, width, label='Strength', edgecolor='black', alpha=0.7)
    p2 = ax.bar(x, nobles, width, bottom=np.array(strengths), label='Nobles', edgecolor='black', alpha=0.7)

    ax.set_xticks(x)
    ax.set_xticklabels(labels, rotation=45, ha='right')
    ax.set_ylabel('Value')
    ax.yaxis.set_major_locator(ticker.MaxNLocator(integer=True))
    ax.set_title(f'Generation {gen_idx}')
    ax.legend()

    # labeling stacked bars (don't ask how it works it just does)
    for xi, s, n in zip(x, strengths, nobles):
        ax.text(xi, s/2 if s!=0 else 0, f'{s}', ha='center', va='center', fontsize=8, color='white')
        ax.text(xi, s + n/2 if (s+n)!=0 else 0, f'{n}', ha='center', va='center', fontsize=8, color='white')


plt.tight_layout(rect=[0, 0, 1, 0.96])
plt.suptitle("Generations of noble houses with seed: " + str(list_of_int_lines[0]), fontsize=16)
plt.savefig("plot_histo.png", dpi=700)
plt.show()