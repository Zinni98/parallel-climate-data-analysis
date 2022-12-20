CC = mpicc

CFLAGS = -std=c99 -Wall -g
 
COMPFLAGS = -I /apps/netCDF4.7.0--gcc-9.1.0/include

LINKFLAGS = -L /apps/netCDF4.7.0--gcc-9.1.0/lib -lnetcdf

all: parallel serial

parallel: utils.c parallel_max.c
	$(CC) $(CFLAGS) -o $@ $^ $(LINKFLAGS) $(COMPFLAGS) -lm

serial: utils.c serial.c
	$(CC) $(CFLAGS) -o $@ $^ $(LINKFLAGS) $(COMPFLAGS) -lm

clean:
	rm -r parallel serial