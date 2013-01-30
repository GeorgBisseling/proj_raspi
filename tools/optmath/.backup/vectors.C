#include <math.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h> 
#include "vectors.h"

#define VECDEBUG

scalar  vector::dummyscalar=0.0;


// kon- und destruktoren

vector::vector( int d                )
{
dim=0;k=0;
if (d==0) return;
k=(scalar *) calloc( d , sizeof( scalar ) );
dim=d;memset( k , 0 , d*sizeof(scalar) );
}


vector::vector( int d      , scalar* s)
{
dim=0;k=0;
k=(scalar *) calloc( d , sizeof( scalar ) );
dim=d;memcpy(k,s, d*sizeof(scalar) );
}

vector::vector( int d      , scalar  s , ... )
{
dim=0;k=0;
if (d==0) return;
k=(scalar *)calloc( d , sizeof(scalar) );
scalar *p;p=&s;
memcpy(k,p,d*sizeof(scalar) );dim=d;
}



vector::vector( vector& v            )
{
dim=0;k=0;
if ((v.dim==0) || (v.k==0)) return;
k=(scalar *) calloc( v.dim , sizeof( scalar ) );
dim=v.dim;memcpy(k,v.k, dim*sizeof(scalar) );
}

vector::~vector()
{
if (k!=NULL) free(k);
dim=0;k=0;
}

// friends

vector operator *( scalar  s  , vector& v )
{
vector local;
int i;

local = v;

if (local.k==0) return vector(local);
for(i=0;i<local.dim;i++) local.k[i]*=s;
return vector(local);
}

vector operator *( vector& v  , scalar  s )
{
return s*v;
}

vector operator /( vector& v  , scalar  s )
{
vector local;
int i;

local=v;
if (local.k==0) return vector(local);
for(i=0;i<local.dim;i++) local.k[i]/=s;
return vector(local);
}


vector operator +( vector& v1 , vector& v2)
{
vector local;
int i;

local=v1;

#ifdef VECDEBUG
if (    (v1.dim==0) || (v2.dim==0)
     || (v1.k  ==0) || (v2.k  ==0)
     || (v1.dim!=v2.dim)
   ) return vector(local);
if (local.k==0) return vector(local);
#endif

for(i=0;i<local.dim;i++) local.k[i]+=v2.k[i];
return vector(local);
}

vector operator -( vector& v1 , vector& v2)
{
vector local;
int i;

local=v1;

#ifdef VECDEBUG
if (    (v1.dim==0) || (v2.dim==0)
     || (v1.k  ==0) || (v2.k  ==0)
     || (v1.dim!=v2.dim)
   ) return vector(local);
if (local.k==0) return vector(local);
#endif

for(i=0;i<local.dim;i++) local.k[i]-=v2.k[i];
return vector(local);
}

scalar operator *( vector& v1 , vector& v2)
{
scalar local;
int i;

#ifdef VECDEBUG
if (    (!v1.is_usable())||(!v2.is_usable())
     || (v1.dim!=v2.dim)
   ) return 0;
#endif

local=0.0;
for(i=0;i<v1.dim;i++) local+= v1.k[i]*v2.k[i];
return local;
}


vector operator %( vector& v1 , vector& v2) // cross product
{
vector local;
int i;

local=v1;

#ifdef VECDEBUG
if (    (v1.dim!=3) || (v2.dim!=3)
     || (v1.k  ==0) || (v2.k  ==0)
   ) return vector(local);
if (local.k==0) return vector(local);
#endif

local.k[0]= v1.k[1]*v2.k[2] - v1.k[2]*v2.k[1];
local.k[1]= v1.k[2]*v2.k[0] - v1.k[0]*v2.k[2];
local.k[2]= v1.k[0]*v2.k[1] - v1.k[1]*v2.k[0];
return vector(local);
}

// methoden

scalar& vector::operator [](int i)
{
#ifdef VECDEBUG
if ( (!is_usable()) || (i<0) || (i>=dim) ) return dummyscalar;
#endif
return k[i];
}




vector& vector::operator +=(vector& v)
{
int i;

for(i=0;i<dim;i++) k[i]+=v.k[i];
return *this;
}

vector& vector::operator -=(vector& v)
{
int i;

for(i=0;i<dim;i++) k[i]-=v.k[i];
return *this;
}

vector& vector::operator  =(vector& v)
{
if (0!= (this->k) ) free( (this->k) );
dim=0;k=0;

#ifdef VECDEBUG
if ((v.dim==0) || (v.k==0)) return *this;
#endif
k=(scalar *) calloc( v.dim , sizeof( scalar ) );
#ifdef VECDEBUG
if (k==0) return *this;
#endif
dim=v.dim;memcpy(k,v.k, dim*sizeof(scalar) );
return *this;
}

int vector::operator ==(vector& v)
{
if ( (!is_usable()) && (!v.is_usable()) ) return 1;
#ifdef VECDEBUG
if ( (!is_usable()) || (!v.is_usable()) || (dim!=v.dim) ) return 0;
#endif
int i;
int eq=1;
for(i=0;i<dim;i++) eq&=(k[i]==v.k[i]);
return eq;
}
int vector::operator !=(vector& v)
{
return !(*this==v);
}



scalar  vector::getsqr()
{
#ifdef VECDEBUG
 if (!is_usable()) return 0.0;
#endif
 int i;
 scalar sum=0.0;
 for(i=0;i<dim;i++) sum+=k[i]*k[i];
 return sum;
}

scalar  vector::getabs()
{
 return sqrt( getsqr() );
}

vector  vector::getuni()
{
 vector local;


 local=*this;
 int i;
 for(i=0;i<local.dim;i++) local.k[i]=0.0;
  
 scalar abs;
 abs=getabs();
 if (abs==0.0) return vector(local);

 for(i=0;i<dim;i++) local.k[i]=k[i]/abs;

 return vector(local);
}

////////////////////////////////////////////////////////
// new in optmath:


 vector::vector
       (FILE* fp, int d)	// d==0 : read dimension from file
				// d!=0 : this is the dimension
{
if ( read(fp,d) ) *this=vector(0);
}
 

int vector::read
       (FILE* fp, int d)	// d==0 : read dimension from file
				// d!=0 : this is the dimension
{
int i,fields=0;

if (d==0)
  { 
   fields=fscanf(fp," %u",&d);
   if (fields!=1) return 1;
   fields=0;
  }

*this=vector(d);
if ((d==0) || (k==0) || (dim==0)) return 1;


for(i=0;i<dim;i++) fields+=fscanf(fp," %lg", k+i);
fscanf(fp," \n");
return ((fields!=dim) || !is_usable() );
}

int vector::write
       (FILE* fp, int dump)	// dump==0 : don't dump dimension
				// dump!=0 : do it
{
int i;

if( dump ) fprintf(fp," %u",dim);
for(i=0;i<dim;i++) fprintf(fp," %22.20lg", k[i]);
fprintf(fp," \n");
return 0;
}










