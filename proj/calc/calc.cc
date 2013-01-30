#include <stdio.h>
#include <math.h>
#include "vectors.h"
#include "matrices.h"

#define XSTART ((double)-15)
#define XEND   ((double)+15)
#define YSTART ((double)-15)
#define YEND   ((double)+15)

/*
  #define XSTART ((double)-50)
  #define XEND   ((double)+50)
  #define YSTART ((double)-50)
  #define YEND   ((double)+50)
*/

/*
  #define XSTART ((double) 0)
  #define XEND   ((double)+1)
  #define YSTART ((double)-1)
  #define YEND   ((double)+1)
*/

/*
  #define XSTART ((double)-20)
  #define XEND   ((double)+20)
  #define YSTART ((double)-5)
  #define YEND   ((double)+5)
*/

#define CALCX  ((int)101)
#define CALCY  ((int)101)

struct point { double x,y,z; };
struct tri   { unsigned long p[3]; };

point dt[CALCX][CALCY];
tri   tr[(CALCX-1)*(CALCY-1)*2];

double function( double x , double y )
{
  const double height = 10.0;

  /* Sombrero */
  double sq;
  sq=sqrt(x*x+y*y);
  if (sq==0.0) return height;
  return height*sin(sq)/sq;

  /* Pyramide 
     double blockdist;
     double retval;
     if ( fabs(x)>fabs(y) ) blockdist=fabs(x); else blockdist=fabs(y);
     retval=10.0-blockdist;
     if (retval<=0.0) retval=0.0;
     return retval;
  */
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
  fwrite(&count, 1, sizeof(count),trfp);

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
