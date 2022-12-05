CC = mpicc

CFLAGS = -std=c99 -Wall -g
 
COMPFLAGS = -I /apps/netCDF4.7.0--gcc-9.1.0/include

LINKFLAGS = -L /apps/netCDF4.7.0--gcc-9.1.0/lib -lnetcdf

all: vnode_test

vnode_test: utils.c vnode.c vnode_test.c
	$(CC) $(CFLAGS) -o $@ $^ $(LINKFLAGS) $(COMPFLAGS)

clean:
	rm -r vnode_test