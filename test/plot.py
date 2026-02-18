import pandas as pd
import matplotlib.pyplot as plt

# var should match variable label in csv
def plot_graph(var, var_label, df):

    plt.figure()
    plt.plot(df.t, df[var])
    plt.title(var_label + " vs Time")
    plt.xlabel("Time")
    plt.ylabel(var_label)
    plt.grid()



if __name__ == "__main__":
    df = pd.read_csv("test\\profile.csv")

    plot_graph("x", "Position", df)
    plot_graph("v", "Velocity", df)
    plot_graph("a", "Accelerataion", df)
    plt.show()
