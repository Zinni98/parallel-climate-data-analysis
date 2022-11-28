CC = mpic++

CFLAGS = -Wall -g

COMPFLAGS = -std=c99 -I /apps/netCDF4.7.0--gcc-9.1.0/include

LINKFLAGS = -L /apps/netCDF4.7.0--gcc-9.1.0/lib -lnetcdf

all: main.exe

main.o: main.cpp
	$(CC) $(COMPFLAGS) $(CFLAGS) -c $^

main.exe: main.o
	$(CC) $(LINKFLAGS) $(CFLAGS) -o $@ $^

clean:
	rm -r main.o main.exe