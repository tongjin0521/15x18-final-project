import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

csvs = ["psc", "ghc"]

for source in csvs:
    # Read the data from the CSV file
    data = pd.read_csv(source + "-5000.csv")

    # Calculate the mean and standard deviation for each combination of METHOD, AGENTNUM, ITERNUM, and SOURCE
    grouped_data = (
        data.groupby(["METHOD", "AGENTNUM", "ITERNUM", "SOURCE"])
        .agg({"TIME": [np.mean, np.std, np.min, np.max]})
        .reset_index()
    )

    # Define a function to get the data for a specific combination of METHOD, ITERNUM, and SOURCE
    def get_data_a(method, iternum, source):
        return grouped_data[
            (grouped_data["METHOD"] == method)
            & (grouped_data["ITERNUM"] == iternum)
            & (grouped_data["SOURCE"] == source)
        ]

    def get_data_i(method, agentnum, source):
        return grouped_data[
            (grouped_data["METHOD"] == method)
            & (grouped_data["AGENTNUM"] == agentnum)
            & (grouped_data["SOURCE"] == source)
        ]

    def get_data_s(method, agentnum, iternum):
        return grouped_data[
            (grouped_data["METHOD"] == method)
            & (grouped_data["AGENTNUM"] == agentnum)
            & (grouped_data["ITERNUM"] == iternum)
        ]

    # Define the data sizes for each source file
    data_sizes = {
        "cleavland-9.csv": 9,
        "cleavland.csv": 303,
        "cleavland-100.csv": 100,
        "cleavland-600.csv": 600,
        "cleavland-1000.csv": 1000,
    }

    # Plot the first graph
    plt.figure(figsize=(8, 6))
    for method in ["Sequential", "MPI", "OpenMP"]:
        data = get_data_a(method, 10, "cleavland.csv")
        x = data["AGENTNUM"]
        y = data["TIME"]["mean"]
        yerr = data["TIME"]["std"]
        plt.errorbar(x, y, yerr=yerr, label=method, marker="o", capsize=3)
    plt.xlabel("AGENTNUM")
    plt.ylabel("TIME")
    plt.title("ITERNUM=10 and SOURCE=cleavland.csv")
    plt.legend()
    plt.savefig(source + "plot-1.png")
    plt.close()

    # Plot the second graph
    plt.figure(figsize=(8, 6))
    for method in ["Sequential", "MPI", "OpenMP"]:
        data = get_data_i(method, 5, "cleavland.csv")
        x = data["ITERNUM"]
        y = data["TIME"]["mean"]
        yerr = data["TIME"]["std"]
        plt.errorbar(x, y, yerr=yerr, label=method, marker="o", capsize=3)
    plt.xlabel("ITERNUM")
    plt.ylabel("TIME")
    plt.title("AGENTNUM=5 and SOURCE=cleavland.csv")
    plt.legend()
    plt.savefig(source + "plot-2.png")
    plt.close()

    # Plot the third graph
    plt.figure(figsize=(8, 6))
    for method in ["Sequential", "MPI", "OpenMP"]:
        data = get_data_s(method, 5, 10)
        x = [data_sizes[source] for source in data["SOURCE"]]
        y = data["TIME"]["mean"]
        yerr = data["TIME"]["std"]

        pairs = list(zip(x, y, yerr))
        sorted_pairs = sorted(pairs, key=lambda pair: pair[0])
        x = [pair[0] for pair in sorted_pairs]
        y = [pair[1] for pair in sorted_pairs]
        yerr = [pair[2] for pair in sorted_pairs]

        plt.errorbar(x, y, yerr=yerr, label=method, marker="o", capsize=3)
    plt.xlabel("Data Size")
    plt.ylabel("TIME")
    plt.title("AGENTNUM=5 and ITERNUM=10")
    plt.legend()
    plt.savefig(source + "plot-3.png")
    plt.close()
