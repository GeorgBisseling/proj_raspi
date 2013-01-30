LINEAR		=../../tools/optmath/
INC		=-I$(LINEAR)
CFLAGS		=-Wall -O3
PROGS		=calc calcappl calcpola force calctime

all:		$(PROGS)

clean:
		rm -f $(PROGS) *.o core 

distclean:	clean
		rm -f *~

force:          force.o $(LINEAR)matrices.o $(LINEAR)vectors.o
		g++ $(CFLAGS) $(INC) -o force force.o \
		$(LINEAR)matrices.o $(LINEAR)vectors.o -lm

calc:           calc.o $(LINEAR)matrices.o $(LINEAR)vectors.o
		g++ $(CFLAGS) $(INC) -o calc calc.o \
		$(LINEAR)matrices.o $(LINEAR)vectors.o -lm

calcrast:	calcrast.o $(LINEAR)matrices.o $(LINEAR)vectors.o
		g++ $(CFLAGS) $(INC) -o calcrast calcrast.o \
		$(LINEAR)matrices.o $(LINEAR)vectors.o -lm

calcappl:       calcappl.o $(LINEAR)matrices.o $(LINEAR)vectors.o
		g++ $(CFLAGS) $(INC) -o calcappl calcappl.o $(LINEAR)matrices.o $(LINEAR)vectors.o -lm

calcpola:       calcpola.o $(LINEAR)matrices.o $(LINEAR)vectors.o
		g++ $(CFLAGS) $(INC) -o calcpola calcpola.o $(LINEAR)matrices.o $(LINEAR)vectors.o -lm

calctime:	calctime.cc $(LINEAR)matrices.o $(LINEAR)vectors.o
		g++ $(CFLAGS) $(INC) -o calctime calctime.cc $(LINEAR)matrices.o $(LINEAR)vectors.o -lm

calc.o:         calc.cc $(LINEAR)matrices.h $(LINEAR)vectors.h
		g++ -c $(CFLAGS) $(INC) -o calc.o calc.cc

calcrast.o:	calcrast.cc $(LINEAR)matrices.h $(LINEAR)vectors.h
		g++ -c $(CFLAGS) $(INC) -o calcrast.o calcrast.cc

force.o:        force.cc $(LINEAR)matrices.h $(LINEAR)vectors.h
		g++ -c $(CFLAGS) $(INC) -o force.o force.cc

calcappl.o:     calcappl.cc $(LINEAR)matrices.h $(LINEAR)vectors.h
		g++ -c $(CFLAGS) $(INC) -o calcappl.o calcappl.cc

calcpola.o:     calcpola.cc $(LINEAR)matrices.h $(LINEAR)vectors.h
		g++ -c $(CFLAGS) $(INC) -o calcpola.o calcpola.cc

matrices.o:     $(LINEAR)matrices.C $(LINEAR)matrices.h $(LINEAR)vectors.h
		g++ -c $(CFLAGS) $(INC) -o $(LINEAR)matrices.o $(LINEAR)matrices.C

vectors.o:      $(LINEAR)vectors.C $(LINEAR)vectors.h
		g++ -c $(CFLAGS) $(INC) -o $(LINEAR)vectors.o $(LINEAR)vectors.C
