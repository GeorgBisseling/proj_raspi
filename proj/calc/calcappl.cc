#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "vectors.h"
#include "matrices.h"

// #define CALCX  ((int)301)
// #define CALCY  ((int)226)
#define CALCX  ((int)31)
#define CALCY  ((int)23)


struct point { double x,y,z; };
struct tri   { unsigned long p[3]; };

point dt[CALCX][CALCY];

long idx[CALCX][CALCY];

#define INDEXOFPOINT(a,b)   ((a*CALCX)+b)

#define MAXDEPTH   256

#define XSTART   (double)-2.2
#define XEND     (double)+1.0
#define YSTART   (double)-1.2
#define YEND     (double)+1.2

/*
  #define XSTART   (double)-0.7
  #define XEND     (double)-0.45
  #define YSTART   (double)-0.75
  #define YEND     (double)-0.5
*/

double iterate(double re, double im)
{
  double x,y,x2,y2;
  int    k;

  x=0;y=0;x2=0;y2=0;k=0;

  while ( (x2+y2<=4) && (k <MAXDEPTH) )
    {
      y =(double)2.0*x*y+im;
      x =x2-y2+re;
      x2=x*x;
      y2=y*y;
      k++;
    }
  return	log10(1.0+((double)k))/log10(MAXDEPTH) - 0.5;
}

unsigned long triindex;
unsigned long dotindex;

FILE *dtfp,*trfp;
const char *dtfname="../data/fresh.dot";
const char *trfname="../data/fresh.tri";

void CalcSquare(int xs,int xe,int ys,int ye,int depth)
{
  tri T;
  int recterm=0;
  int xt,yt;
  double test; 

  if (depth>20) 
    {
      printf("\n\nMAXDEPTH REACHED !\n");
      printf("xs%10d\txe%10d\n",xs,xe);
      printf("ys%10d\tye%10d\n\n",ys,ye);
      exit(1);
    }

  if ((xs>=xe)||(ys>=ye)) return; // one point or line

  if ((xs+1>=xe)&&(ys+1>=ye)) recterm=1; // minimal size, no further optimization possible

  if ( (!recterm) && ((xs+80>=xe)&&(ys+60>=ye)) )
    {
      recterm=1;
      test=dt[xs][ys].z;
      for(xt=xs;(xt<=xe)&&(recterm==1);xt++)
	for(yt=ys;(yt<=ye)&&(recterm==1);yt++)
	  if( test!=dt[xt][yt].z ) {recterm=0;break; } 
    }

  if (recterm)
    {
      if (idx[xs][ys]==-1) {idx[xs][ys]=dotindex;dotindex++;fwrite(&(dt[xs][ys]),sizeof(point),1,dtfp);}
      if (idx[xe][ys]==-1) {idx[xe][ys]=dotindex;dotindex++;fwrite(&(dt[xe][ys]),sizeof(point),1,dtfp);}
      if (idx[xs][ye]==-1) {idx[xs][ye]=dotindex;dotindex++;fwrite(&(dt[xs][ye]),sizeof(point),1,dtfp);}
      if (idx[xe][ye]==-1) {idx[xe][ye]=dotindex;dotindex++;fwrite(&(dt[xe][ye]),sizeof(point),1,dtfp);}
    
      T.p[0]=idx[xs][ys];
      T.p[1]=idx[xe][ys];
      T.p[2]=idx[xs][ye];
      fwrite(&T,sizeof(T),1,trfp);
      triindex++;

      T.p[0]=idx[xe][ye];
      T.p[1]=idx[xs][ye];
      T.p[2]=idx[xe][ys];
      fwrite(&T,sizeof(T),1,trfp);
      triindex++;

      if(0==(dotindex % 100)) 
	{
	  printf("rdepth: %8d :: %8lu dots & %8lu triangles\r",
		 depth,dotindex,triindex);
	  fflush(stdout);
	}
      return;
    }

  unsigned long pivotx=xs+ (xe-xs)/2;
  unsigned long pivoty=ys+ (ye-ys)/2;

  CalcSquare(xs,pivotx,ys,pivoty,depth+1);
  CalcSquare(pivotx,xe,ys,pivoty,depth+1);
  CalcSquare(xs,pivotx,pivoty,ye,depth+1);
  CalcSquare(pivotx,xe,pivoty,ye,depth+1);
}

int main()
{
  int xs,ys;
  double XCENTER,YCENTER;

  XCENTER=(XSTART+XEND)/2.0;
  YCENTER=(YSTART+YEND)/2.0;

  dtfp=fopen(dtfname,"wb");
  trfp=fopen(trfname,"wb");
  if ( (dtfp==0) || (trfp==0) )
    {
      printf("Error opening Files for Write.\r\n");
      return 255;
    }

  fwrite(&dotindex,sizeof(dotindex),1,dtfp);
  fwrite(&triindex,sizeof(triindex),1,trfp);

  dotindex=0;
  triindex=0;

  printf("Initializing %dx%d Points\r\n",CALCX,CALCY);

  double x,y;

  for(xs=0;xs<CALCX;xs++)
    {
      x=XSTART+xs*(XEND-XSTART)/CALCX;
      printf("current x %8d\r",xs);
      for(ys=0;ys<CALCY;ys++)
	{
	  y=YSTART+ys*(YEND-YSTART)/CALCY;
	  dt[xs][ys].x=x-XCENTER;
	  dt[xs][ys].y=y-YCENTER;
	  dt[xs][ys].z=iterate( x , y);
	  idx[xs][ys]=-1;
	}
    }

  printf("\nx,y-values initialized\r\nstarting optimization\n");

  CalcSquare(0,CALCX-1,0,CALCY-1,1);

  dotindex++;
  triindex++;
  fseek(dtfp,0,0);
  fseek(trfp,0,0);
  fwrite(&dotindex,sizeof(dotindex),1,dtfp);
  fwrite(&triindex,sizeof(triindex),1,trfp);
  fclose(dtfp);
  fclose(trfp);

  printf("\n\nGenerated %8lu dots & %8lu triangles\n",dotindex,triindex);
  printf("Done\n\n\n");fflush(stdout);

  return 0;
}


