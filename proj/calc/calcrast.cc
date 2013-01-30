#include <stdio.h>
#include <math.h>
#include "vectors.h"
#include "matrices.h"

#define XSTART ((double)-1)
#define XEND   ((double)+1)
#define YSTART ((double)-1)
#define YEND   ((double)+1)

#define CALCX  ((int)101)
#define CALCY  ((int)101)

struct point { double x,y,z; };
struct tri   { unsigned long p[3]; };

point dt[CALCX][CALCY];
tri   tr[(CALCX-1)*(CALCY-1)*2];

double function( double x , double y )
{
  double r, a;
  /*  Berthold K.P. Horn */
  /*
    return  (  cos( PI/2 * (y+1) ) * 1.08
    + cos( PI/2 * (x+2) ) * 0.92 ) /2;
  */
  r = sqrt( x*x + y*y );
  if(0==r) return 0;
  a = atan2( y, x );
  return  (a + PI) / (2*PI) + r;
}

int main()
{
  int xc,yc;
  double x,y;
  unsigned long count;

  FILE *dtfp,*trfp;
  char *dtfname="../data/fresh.dot";
  char *trfname="../data/fresh.tri";


  dtfp=fopen(dtfname,"wb");

  if (dtfp==0) { printf("couldn't open %s for Write.\r\n", dtfp);
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

  if (trfp==0) { printf("couldn't open %s for Write.\r\n", dtfp);
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
