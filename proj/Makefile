
CFLAGS		=-Wall -O3

LM		=../tools/optmath/
INC		=-I$(LM)
OBJS		=proj.o projcalc.o projgrph.o projfile.o
LIBS		=$(LM)matrices.o $(LM)vectors.o -L/usr/X11R6/lib -lX11 -lm

all:		
		make dep
		make proj

proj:           $(OBJS)
		g++   $(CFLAGS) $(INC) -o proj $(OBJS) $(LIBS)

$(OBJS):	projtype.h $(LM)matrices.h $(LM)vectors.h

.C.o:
		g++ -c $(CFLAGS) $(INC) $<

clean:
		rm -f core *~ *.o

distclean:	clean
		rm -f pictures/*.xwd pictures/*.png

dep:
		g++ $(INC) -M *.C > .depend

# include a dependency file if one exists

ifeq (.depend,$(wildcard .depend))
include .depend
endif









