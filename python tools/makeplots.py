import matplotlib.pyplot as plt
import pandas as pd

plt.rcParams["figure.figsize"] = [7.00, 3.50]
plt.rcParams["figure.autolayout"] = True

cols = ["bytes", "time", "rate"]
df = pd.read_csv("input.txt", header=None, names=cols, skiprows=1)

print("Contents in csv file:\n", df)
print("Indexes : ", df.index)
print("Indexes : ", df.columns)
print("Indexes : ", df.head())

_, ax = plt.subplots()
ax.plot(df["bytes"], df["time"], "-r", label="Exec Times")
ax.plot(df["bytes"], df["rate"], "-b", label="Rate")
ax.legend(frameon=False, loc="upper center", ncol=2)
ax.grid()
plt.xlabel("Bytes sent")
plt.ylabel("Times / Rates")
plt.title("Benchmark")
plt.show()
