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
