// #pragma inline

#include <stdio.h>
#include <std.h>
#include <stdlib.h>
#include <unistd.h>
// #include <pc.h>

#include "vectors.h"
#include "matrices.h"


#define DIM 4
#define NUM 100

int main()
{
vector *a,*b;
int i,j;
FILE* fp;

a= new vector[NUM] ;
b= new vector[NUM] ;

for(i=0;i<NUM;i++) 
  {
   a[i]=vector(DIM);
   for(j=0;j<DIM;j++) a[i][j]=((double)(rand() % 1000)) / 1000.0;
  }

fp=fopen("dump","w");
for(i=0;i<NUM;i++) a[i].write(fp,0);
fclose(fp);

fp=fopen("dump","r");
for(i=0;i<NUM;i++) b[i].read(fp,DIM);
fclose(fp);

for(i=0;i<NUM;i++)
  if ( a[i] != b[i] )
    {
     printf("err at %d \n",i);
     exit(1);
    }
printf("ok\n");


return 0;
}











