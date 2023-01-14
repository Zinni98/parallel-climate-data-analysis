"""Algorithm to compute speedup and efficiency of a parallel algorithm from 
execution times file"""

import pandas as pd
from makeplots import plotter
from tabulate import tabulate


class Statistics:
    """Support class to wrap methods for computing both
    speedup and efficiency"""

    def __init__(
        self, filename: pd.DataFrame, serial_time, metric: str
    ) -> None:
        self.data = filename
        self.serial_time = serial_time
        self.metric = metric

    def compute_speedups(self) -> pd.DataFrame:
        """Function to compute speedup of a parallel program
        Returns:
        ------------
            speedup_df : pandas DataFrame
                Dataframe containing speedup statistics
        """
        # speedup formula : S = Tserial/Tparallel
        speedup_df = self.data.copy()
        speedup_df.rename(columns={"Time": "Speedup"}, inplace=True)
        speedup_df["Speedup"] = (
            self.serial_time / self.data["Time"]
        )  # / self.serial_time
        return speedup_df

    def compute_efficiency(self) -> pd.DataFrame:
        """Function to compute efficiency of a parallel program
        Returns:
        ------------
            efficiency_df: pandas DataFrame
                DataFrame containing efficiency statistics
        """
        # efficiency formula : E = S/(# cores)
        speedup_df = self.compute_speedups()
        efficiency_df = speedup_df.copy()
        efficiency_df.rename(columns={"Speedup": "Efficiency"}, inplace=True)
        efficiency_df["Efficiency"] = (
            speedup_df["Speedup"] / self.data["# Processes"]
        )
        return efficiency_df

    def selection(self):
        """Select which plot to perform based on metric"""
        if self.metric == "Time":
            print(tabulate(self.data, headers="keys", tablefmt="github"))
            plotter(df=self.data, metric=self.metric)
        elif self.metric == "Speedup":
            print(
                tabulate(
                    self.compute_speedups(), headers="keys", tablefmt="github"
                )
            )
            plotter(df=self.compute_speedups(), metric=self.metric)
        else:
            print(
                tabulate(
                    self.compute_efficiency(), headers="keys", tablefmt="github"
                )
            )
            plotter(df=self.compute_efficiency(), metric=self.metric)
