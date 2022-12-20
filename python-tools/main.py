"""Test file"""

from random import random

from argparser import Parser
from benchmark_stats import Statistics
from makeplots import read_input_file
from tabulate import tabulate

SERIAL_TIME = random()


if __name__ == "__main__":
    # parser initialization
    parser = Parser()
    parser.add_arg(name="input_file", choices=["input.txt"])
    parser.add_arg(name="metric", choices=["Time", "Speedup", "Efficiency"])

    args = parser.parse_args()

    # sanity printd
    print(f"Received benchmark file : {args.input_file}")
    print(f"Selected metric : {args.metric}")

    bench_table = read_input_file(file=args.input_file, metric="Time")

    stats = Statistics(
        filename=bench_table, metric=args.metric, serial_time=SERIAL_TIME
    )

    stats.selection()
