import pandas as pd
import matplotlib.pyplot as plt

# Chargement des données
turb = pd.read_csv("data/historique_turbines.csv", sep=";", encoding="utf-8-sig")
cent = pd.read_csv("data/historique_centrale.csv", sep=";", encoding="utf-8-sig")

# Liste des turbines
turbines_ids = sorted(turb["id_turbine"].unique())

# Création grille 3×2
fig, axes = plt.subplots(3, 2, figsize=(14, 10))
axes = axes.flatten()   # pour indexer facilement

# --- Turbines (T1..T5) ---
for i, tid in enumerate(turbines_ids):
    ax = axes[i]
    sub = turb[turb["id_turbine"] == tid].sort_values("ligne")

    ax.plot(sub["ligne"], sub["P_real"], label="Réelle")
    ax.plot(sub["ligne"], sub["P_sim"], linestyle="--", label="Simulée")

    ax.set_title(f"Turbine {tid}")
    ax.set_xlabel("Ligne")
    ax.set_ylabel("Puissance")
    ax.grid(True)
    ax.legend()

# --- Centrale (dernier subplot) ---
ax = axes[-1]
cent_sorted = cent.sort_values("ligne")

ax.plot(cent_sorted["ligne"], cent_sorted["P_real_tot"], label="Réelle", linewidth=2)
ax.plot(cent_sorted["ligne"], cent_sorted["P_sim_tot"], linestyle="--", linewidth=2, label="Simulée")

ax.set_title("Centrale – Production Totale")
ax.set_xlabel("Ligne")
ax.set_ylabel("Puissance totale")
ax.grid(True)
ax.legend()

# Ajustement automatique
fig.suptitle("Production simulée vs réelle – Turbines et Centrale", fontsize=16)
plt.tight_layout(rect=[0, 0, 1, 0.97])

# Affiche tout
plt.show()
