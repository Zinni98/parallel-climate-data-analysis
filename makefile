CC = mpicc

CFLAGS = -std=c99 -Wall -g
 
COMPFLAGS = -I /apps/netCDF4.7.0--gcc-9.1.0/include

LINKFLAGS = -L /apps/netCDF4.7.0--gcc-9.1.0/lib -lnetcdf

all: vnode

vnode: utils.h vnode.h vnode_test.h
	$(CC) $(CFLAGS) -o $@ $^ $(LINKFLAGS) $(COMPFLAGS)

clean:
	rm -r vnode