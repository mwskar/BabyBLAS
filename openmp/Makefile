include ../Makefile.inc

# OpenMP library targets 

all: lib

dot.o: dot.c
	$(CC) $(CFLAGS) $(OPM) -c dot.c

mvv.o: mvv.c
	$(CC) $(CFLAGS) $(OPM) -c mvv.c

vvm.o: vvm.c
	$(CC) $(CFLAGS) $(OPM) -c vvm.c

mmm.o : mmm.c
	$(CC) $(CFLAGS) $(OPM) -c mmm.c

lib: dot.o vvm.o mvv.o mmm.o 
	ar -rc libopenmp_bblas.a dot.o vvm.o mvv.o mmm.o 
	ranlib libopenmp_bblas.a

# Default Targets for Cleaning up the Environment

clean :
	rm *.o
	touch *.c

pristine:
	make clean
	rm *.a
