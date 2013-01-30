// #pragma inline

#include <stdio.h>
//#include <std.h>
#include <stdlib.h>
#include <unistd.h>
// #include <pc.h>

#include "vectors.h"
#include "matrices.h"




int main()
{
vector a,b,c;

int i;

a=vector(100);
b=vector(100);
c=vector(100);


for(i=0;i<50000;i++) 
{
a=b-c;
}


return 0;
}











