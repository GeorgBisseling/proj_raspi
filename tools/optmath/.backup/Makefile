
CFLAGS		=-O6

lintest:        lintest.o matrices.o vectors.o
		gcc $(CFLAGS) -o lintest lintest.o matrices.o vectors.o -lm 
lintest.o:      lintest.C matrices.h vectors.h
		gcc $(CFLAGS) -c -o lintest.o lintest.C
matrices.o:     matrices.C matrices.h vectors.h
		gcc $(CFLAGS) -c -o matrices.o matrices.C
vectors.o:      vectors.C vectors.h
		gcc $(CFLAGS) -c -o vectors.o vectors.C

