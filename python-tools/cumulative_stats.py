import pandas as pd


def cumulative_stats(data: pd.DataFrame) -> pd.DataFrame:
    """Function to group different runs with the same number of Processes
    Parameters:
    ------------
        df : pandas DataFrame
            DataFrame containing data to be plot
    Returns:
    ------------
        df : pandas DataFrame
            DataFrame containing cumulative stats
    """
    df: pd.DataFrame = data.copy()
    df = data.groupby(["# Processes"], as_index=False).mean()
    return df


# df = pd.DataFrame(
#    {
#        "# Processes": [1, 1, 1, 2, 2, 2, 3, 4, 5, 6, 6, 6, 7, 8, 9, 10],
#        "Time": [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16],
#    }
# )
# print(df)
# df = cumulative_stats(df)
# print(df)
