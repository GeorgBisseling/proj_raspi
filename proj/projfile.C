#include "projfile.h"

const char* ReadDots(const char* dtfname,unsigned long &dtcount,vector* &dt )
{
  static char errmsg[300];
  unsigned long i;
  double x,y,z;
  FILE* dtfp;

  if (0==(dtfp=fopen(dtfname,"rb")))
    {
      sprintf(errmsg,"Error opening %s for read.\r\n",dtfname);
      return errmsg;
    }

  fread(&dtcount,1,sizeof(dtcount),dtfp);

  if (0==(dt=new vector[dtcount]))
    {
      fclose(dtfp);
      return "Out of Mem\r\n";
    }

  for(i=0;i<dtcount;i++)
    {
      fread(&x,1,sizeof(double),dtfp);
      fread(&y,1,sizeof(double),dtfp);
      fread(&z,1,sizeof(double),dtfp);
      dt[i]=vector(3,x,y,z);
      if (!dt[i].is_usable())
	{
	  fclose(dtfp);
	  return "Out of Mem\r\n";
	}
    }
  fclose(dtfp);
  return ((char*)0);
}

const char* ReadTriangles(const char* trfname, unsigned long &trcount,tri* &tr )
{
  static char errmsg[300];
  FILE* trfp;

  if (0==(trfp=fopen(trfname,"rb")))
    {
      sprintf(errmsg,"Error opening %s for read.\r\n",trfname);
      return errmsg;
    }

  fread(&trcount,1,sizeof(trcount),trfp);

  if (0==(tr=(tri*)calloc(trcount,sizeof(tri))))
    {
      fclose(trfp);
      return "Out of Mem\r\n";
    }

  fread(tr,trcount,sizeof(tri),trfp);
  fclose(trfp);
  return ((char*)0);
}




