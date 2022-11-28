CC = mpic++

CFLAGS = -Wall -g
 
COMPFLAGS = -I /apps/netCDF4.7.0--gcc-9.1.0/include

LINKFLAGS = -L /apps/netCDF4.7.0--gcc-9.1.0/lib -lnetcdf

all: main

#main.o: main.cpp
#	$(CC) $(COMPFLAGS) $(CFLAGS) -c $^

main: main.cpp
	$(CC) $(CFLAGS) -o $@ $^ $(LINKFLAGS) $(COMPFLAGS)

clean:
	rm -r main.o main