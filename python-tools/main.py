"""Test file"""

from argparser import Parser
from benchmark_stats import Statistics
from cumulative_stats import cumulative_stats
from makeplots import read_input_file

SERIAL_TIME = 640.034928  # time in seconds


if __name__ == "__main__":
    # parser initialization
    parser = Parser()
    parser.add_arg(name="input_file", choices=["results.txt"])
    parser.add_arg(name="metric", choices=["Time", "Speedup", "Efficiency"])

    args = parser.parse_args()

    # sanity print
    print(f"Received benchmark file : {args.input_file}")
    print(f"Selected metric : {args.metric}")

    bench_table = read_input_file(file=args.input_file, metric="Time")
    print(bench_table)
    bench_table = cumulative_stats(data=bench_table)
    print(bench_table)
    stats = Statistics(
        filename=bench_table, metric=args.metric, serial_time=SERIAL_TIME
    )

    stats.selection()
