import matplotlib.pyplot as plt # type: ignore
import matplotlib.ticker as ticker # type: ignore
import numpy as np # type: ignore
import math

from modules import Casata, loadCasate

listaCasate = []
list_of_integer_lines = []

loadCasate("generations.out", listaCasate, list_of_integer_lines)

num_casate = len(listaCasate)

# Calculate grid size (as square as possible)
ncols = math.ceil(math.sqrt(num_casate))
nrows = math.ceil(num_casate / ncols)

figure, axes = plt.subplots(nrows, ncols, figsize=(5*ncols, 4*nrows), num="Noble houses\' evolution")

# Flatten axes for easy iteration, in case of 2D array
axes = np.array(axes).flatten()

for idx, casa in enumerate(listaCasate):
    ax1 = axes[idx]
    ax2 = ax1.twinx()  # Create a secondary y-axis for noble count

    l1, = ax1.plot(casa.m_evoluzioneForza, color='tab:blue', label=f"{casa.getCognome()}'s Strength",marker='o')
    l2, = ax2.plot(casa.m_arrNumNobili, color='tab:orange', label=f"{casa.getCognome()}'s nobles",marker='s')

    ax1.set_title(casa.getCognome())
    ax1.set_ylabel("Strength", color='tab:blue')
    ax2.set_ylabel("Nobles", color='tab:orange')
    ax1.set_xlabel("Generations")

    ax1.grid(True, linestyle='--', linewidth=0.7, alpha=0.7)

    # Force integer ticks on all axes
    ax1.xaxis.set_major_locator(ticker.MaxNLocator(integer=True))
    ax1.yaxis.set_major_locator(ticker.MaxNLocator(integer=True))
    ax2.yaxis.set_major_locator(ticker.MaxNLocator(integer=True))

    # Combine legends
    lines = [l1, l2]
    labels = [line.get_label() for line in lines]
    ax1.legend(lines, labels)

# Hide any unused subplots
for ax in axes[len(listaCasate):]:
    ax.set_visible(False)

plt.tight_layout(rect=[0, 0, 1, 0.96])
plt.suptitle("Evolution of noble houses with seed: " + str(list_of_integer_lines[0]), fontsize=16)
plt.savefig("line_plot.png", dpi=700)
plt.show()
