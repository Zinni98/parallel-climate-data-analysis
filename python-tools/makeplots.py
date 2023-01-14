"Algorthm to perform benchmarks"

from typing import List, Tuple

import matplotlib
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
from matplotlib.lines import Line2D
from pandas._libs.lib import NoDefault


def read_input_file(file: str, metric: str) -> pd.DataFrame:
    """Reads a file as a pandas DataFrame
    Parameters:
    ------------
        file : str
            path to a file
        cols : List
            list of string containing column names
    Returns:
    ------------
        df : pd.DataFrame
            read file in form of pandas DataFrame
    """
    cols = ["# Processes", metric]
    dataframe = pd.read_csv(file, names=cols, skiprows=1)
    return dataframe


def onpick1(event):
    """Function defined to catch event on matplotlib plot"""
    if isinstance(event.artist, Line2D):
        thisline = event.artist
        xdata = thisline.get_xdata()
        ydata = thisline.get_ydata()
        ind = event.ind
        print("Process=" + str(np.take(xdata, ind)[0]))  # Print X point
        print("Stats=" + str(np.take(ydata, ind)[0]))  # Print Y point


def plotter(df: pd.DataFrame, metric: str, filename: str = "./trends/") -> None:
    """Function to plots graphs
    Parameters:
    ------------
        df : pandas DataFrame
            DataFrame containing data to be plot
        metric : str
            selected metric to title y axis
    Returns:
    ------------
        None
    """
    matplotlib.style.use("ggplot")

    plt.rcParams["figure.figsize"] = [7.00, 3.50]
    plt.rcParams["figure.autolayout"] = True

    fig, axes = plt.subplots()

    # df.plot(x="# Processes", y=metric, kind="line", ax=axes, )
    axes.set_xlabel("Number of Processes", picker=True)
    axes.set_ylabel(metric, picker=True)
    (_,) = axes.plot(df["# Processes"], df[metric], "-r", picker=5, marker=".")

    # following line show static coordinates
    # for xy in zip(df["# Processes"], df[metric]):
    #    plt.annotate("(%.2f, %.2f)" % xy, xy=xy)

    fig.canvas.mpl_connect("pick_event", onpick1)
    plt.savefig(filename)
    plt.show()
