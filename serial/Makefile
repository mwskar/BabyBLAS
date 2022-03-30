include ../Makefile.inc

# Timing Library targets 

all: lib

dot.o: dot.c
	$(CC) $(CFLAGS) -c dot.c

mvv.o: mvv.c
	$(CC) $(CFLAGS) -c mvv.c

vvm.o : vvm.c
	$(CC) $(CFLAGS) -c vvm.c

mmm.o : mmm.c
	$(CC) $(CFLAGS) -c mmm.c

lib: vvm.o mmm.o dot.o mvv.o
	ar -rc libserial_bblas.a vvm.o mmm.o  dot.o mvv.o
	ranlib libserial_bblas.a

# Default Targets for Cleaning up the Environment

clean :
	rm *.o
	touch *.c

pristine:
	make clean
	rm *.a
