CFLAGS		=-Wall -O3

lintest:        lintest.o matrices.o vectors.o
		g++ $(CFLAGS) -o lintest lintest.o matrices.o vectors.o -lm 
lintest.o:      lintest.C matrices.h vectors.h Makefile
		g++ $(CFLAGS) -c -o lintest.o lintest.C
matrices.o:     matrices.C matrices.h vectors.h Makefile
		g++ $(CFLAGS) -c -o matrices.o matrices.C
vectors.o:      vectors.C vectors.h Makefile
		g++ $(CFLAGS) -c -o vectors.o vectors.C

clean:		
		rm -f lintest dump core *.o 

distclean:	clean
		rm -f *~

#
