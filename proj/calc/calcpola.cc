#include <stdio.h>
#include <math.h>
#include "vectors.h"
#include "matrices.h"

#define XSTART ((double) 0)
#define XEND   ((double)+1)
#define YSTART ((double) 0)
#define YEND   ((double)+1)

#define CALCX  ((int)81)
#define CALCY  ((int)11)

struct point { double x,y,z; };
struct tri   { unsigned long p[3]; };

point dt[CALCX][CALCY];
tri   tr[(CALCX-1)*(CALCY-1)*2];

void function(double par1, double par2,
              double &fx,double &fy,double &fz
	      )
{
  fz=(5*par2+5)*cos(par1*8.0*3.1415);
  fx=(5*par2+5)*sin(par1*8.0*3.1415);
  fy=40*fabs(par1-0.5);
}

int main()
{
  int xc,yc;
  double x,y,fx,fy,fz;
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
        
	  function(x,y,fx,fy,fz);

	  dt[xc][yc].x=fx;
	  dt[xc][yc].y=fy;
	  dt[xc][yc].z=fz;
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

  fwrite(tr, (CALCX-1)*(CALCY-1)*2, sizeof(tri),trfp);
  fclose(trfp);

  printf("%s written.\r\n", trfname);

  return 0;
}
