include ../Makefile.inc

# Pthread library targets 

all: lib

dot.o: dot.c
	$(CC) $(CFLAGS)  -c dot.c

vvm.o: vvm.c
	$(CC) $(CFLAGS)  -c vvm.c

mvv.o: mvv.c
	$(CC) $(CFLAGS)  -c mvv.c

mmm.o : mmm.c
	$(CC) $(CFLAGS)  -c mmm.c

lib: dot.o mvv.o vvm.o mmm.o 
	ar -rc libpthreads_bblas.a  dot.o vvm.o mvv.o mmm.o 
	ranlib libpthreads_bblas.a

# Default Targets for Cleaning up the Environment

clean :
	rm *.o
	touch *.c

pristine:
	make clean
	rm *.a
