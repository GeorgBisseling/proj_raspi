#include <stdio.h>
#include <math.h>

#include "vectors.h"
#include "matrices.h"

#define DT              ((double)     0.001)
#define STEPS                      10000
#define SIGMA           ((double)    10.000)
#define R               ((double)    28.000)

#define A               ((double)     0.398)
#define B               ((double)     2.000)
#define C               ((double)     4.000)

int main(int argc,char** arg)
{
  double t,dt,z1,z2,z3,z3fake,dz1,dz2,dz3;
  signed long count,i,c;

  FILE *dtfp,*trfp;
  const char *dtfname="../data/fresh.dot";
  const char *trfname="../data/fresh.tri";

  dtfp=fopen(dtfname,"wb");

  if (dtfp==0) {
    printf("couldn't open %s for Write.\r\n", dtfname);
    return 1;
  }

  trfp=fopen(trfname,"wb");

  if (trfp==0) { 
    printf("couldn't open %s for Write.\r\n", trfname);
    return 1;
  }

  count=STEPS+150;
  fwrite(&count,1          ,sizeof(count),dtfp);
  fwrite(&count,1          ,sizeof(count),trfp);

  for(i=0;i<50;i++)
    {
      z1=((double)i) / 2.5 - 10.0;
      z2=0;
      z3=-2.5;
      fwrite(&z1,1,sizeof(z1),dtfp);
      fwrite(&z2,1,sizeof(z1),dtfp);
      fwrite(&z3,1,sizeof(z1),dtfp);
    
      c=i;fwrite(&c,1,sizeof(c),trfp);
      c=i;fwrite(&c,1,sizeof(c),trfp);
      c=i;fwrite(&c,1,sizeof(c),trfp);
    }

  for(i=0;i<50;i++)
    {
      z1=0;
      z2=((double)i) / 2.5 - 10.0;
      z3=-2.5;
      fwrite(&z1,1,sizeof(z1),dtfp);
      fwrite(&z2,1,sizeof(z1),dtfp);
      fwrite(&z3,1,sizeof(z1),dtfp);
    
      c=50+i;fwrite(&c,1,sizeof(c),trfp);
      c=50+i;fwrite(&c,1,sizeof(c),trfp);
      c=50+i;fwrite(&c,1,sizeof(c),trfp);
    }

  for(i=0;i<50;i++)
    {
      z1=0;
      z2=0;
      z3=((double)i) / 20 -2.5;
      fwrite(&z1,1,sizeof(z1),dtfp);
      fwrite(&z2,1,sizeof(z1),dtfp);
      fwrite(&z3,1,sizeof(z1),dtfp);
    
      c=100+i;fwrite(&c,1,sizeof(c),trfp);
      c=100+i;fwrite(&c,1,sizeof(c),trfp);
      c=100+i;fwrite(&c,1,sizeof(c),trfp);
    }

  z1  = 20.0;
  z2  = -2.0 ;
  z3  = 0.5510;

  sscanf(arg[1],"%lg",&z1);
  sscanf(arg[2],"%lg",&z2);
  sscanf(arg[3],"%lg",&z3);

  t   =  0.00;
  dt  =    DT;

  for(count=150;count<STEPS+150;count++)
    {
      if (0==((count-150) % 10)) 
        {
	  printf("%10ld %10.5lg %10.5lg %10.5lg\n",STEPS-count+150,z1,z2,z3);
	  fflush(stdout);
        }
    
      fwrite(&z1,1,sizeof(z1),dtfp);
      fwrite(&z2,1,sizeof(z1),dtfp);
      z3fake=z3-2.5;
      fwrite(&z3fake,1,sizeof(z1),dtfp);

      if (count<1+150)
        {
	  fwrite(&count,1,sizeof(count),trfp);
	  fwrite(&count,1,sizeof(count),trfp);
	  fwrite(&count,1,sizeof(count),trfp);
        }
      else
        {
	  c=count  ;fwrite(&c,1,sizeof(c),trfp);
	  c=count  ;fwrite(&c,1,sizeof(c),trfp);
	  c=count-1;fwrite(&c,1,sizeof(c),trfp);
        }

      for(i=0;i<10;i++)
        {
	  dz1 =   -z2-z3;
	  dz2 =   z1+A*z2;
	  dz3 =   B+z3*(z1-C);
	  t+=dt;
	}
    }

  fclose(dtfp);
  fclose(trfp);

  return 0;
}
