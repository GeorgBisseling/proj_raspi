#include <stdio.h>
#include <math.h>
#include "vectors.h"
#include "matrices.h"

#define XSTART ((double)-1)
#define XEND   ((double)+1)
#define YSTART ((double)-1)
#define YEND   ((double)+1)

#define CALCX  ((int)51)
#define CALCY  ((int)51)

struct point { double x,y,z; };
struct tri   { unsigned long p[3]; };

point dt[CALCX][CALCY];
tri   tr[(CALCX-1)*(CALCY-1)*2];


double max(double a , double b) 
{ 
  if(a>b) return a; else return b;
}

double function( double x , double y )
{
  double rsq,xsq,ysq,f,px,py;

  f=0;
  // Anteile der Raumgrenzen
  f+=exp(-16*(x+1.0)*(x+1.0));
  f+=exp(-16*(1.0-x)*(1.0-x));
  f+=exp(-16*(y+1.0)*(y+1.0));
  f+=exp(-16*(1.0-y)*(1.0-y));
  f/=3.0;

  //ein kleines Rechteck bei -0.3;-0.3 x -0.2x-0.1
  if(x<-0.3) px=-0.3;
  else if(x>-0.2) px=-0.2;
  else px=x;
  if(y<-0.3) py=-0.3;
  else if(y>-0.1) py=-0.1;
  else py=y;
  xsq=(x-px)*(x-px);
  ysq=(y-py)*(y-py);
  rsq=xsq+ysq;
  f+=exp(-16*rsq)/3.0;

  // und ein kleiner Kreis bei 0.4;0.4 mit Radius 0.1
  xsq=(x-0.4)*(x-0.4);
  ysq=(y-0.4)*(y-0.4);
  rsq=xsq+ysq;
  if (rsq<0.01) rsq=0.01;
  f+=exp(-16*rsq)/3.0;

  return f;

}

int main()
{
  int xc,yc;
  double x,y;
  unsigned long count;

  FILE *dtfp,*trfp;
  const char *dtfname="../data/fresh.dot";
  const char *trfname="../data/fresh.tri";

  dtfp=fopen(dtfname,"wb");

  if (dtfp==0) { printf("couldn't open %s for Write.\r\n", dtfname);
    return 1;
  }

  for(xc=0;xc<CALCX;xc++)
    {
      x=XSTART+xc*((XEND-XSTART)/(CALCX-1));
      for(yc=0;yc<CALCY;yc++)
	{
	  y=YSTART+yc*((YEND-YSTART)/(CALCY-1));
	  dt[xc][yc].x=x;
	  dt[xc][yc].y=y;
	  dt[xc][yc].z=function(x,y);
	}
    }

  count=CALCX*CALCY;
  fwrite(&count,1          ,sizeof(count),dtfp);
  fwrite(dt    ,CALCX*CALCY,sizeof(point),dtfp);
  fclose(dtfp);

  printf("%s written.\r\n",dtfname);

  trfp=fopen(trfname,"wb");

  if (trfp==0) { printf("couldn't open %s for Write.\r\n", dtfname);
    return 1;
  }

  count=(CALCX-1)*(CALCY-1)*2;
  fwrite(&count,1          ,sizeof(count),trfp);

  // first half
  count=0;
  for(xc=0;xc<CALCX-1;xc++)
    for(yc=0;yc<CALCY-1;yc++)
      {
	tr[count].p[0]= yc   +( xc   *CALCY);
	tr[count].p[2]=(yc+1)+( xc   *CALCY);
	tr[count].p[1]= yc   +((xc+1)*CALCY);
	count++;
      }
  // second half
  for(xc=1;xc<CALCX;xc++)
    for(yc=1;yc<CALCY;yc++)
      {
	tr[count].p[0]= yc   +( xc   *CALCY);
	tr[count].p[2]=(yc-1)+( xc   *CALCY);
	tr[count].p[1]= yc   +((xc-1)*CALCY);
	count++;
      }

  fwrite(tr    ,(CALCX-1)*(CALCY-1)*2,sizeof(tri),trfp);
  fclose(trfp);

  printf("%s written.\r\n",trfname);


  return 0;
}
