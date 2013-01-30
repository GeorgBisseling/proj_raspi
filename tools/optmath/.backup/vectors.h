#ifndef _VECTORS_H
#define _VECTORS_H

#include <stdio.h>



typedef double scalar;

class matrix;

class vector
{
friend matrix;

protected:

static scalar dummyscalar;

int dim;
scalar* k;

public:

 vector( int d=0               );
 vector( int d      , scalar* s);
 vector( int d      , scalar  s , ... );
 vector( vector& v             );
~vector(                       );

friend vector operator *( scalar  s  , vector& v );
friend vector operator *( vector& v  , scalar  s );
friend vector operator /( vector& v  , scalar  s );

friend vector operator +( vector& v1 , vector& v2);
friend vector operator -( vector& v1 , vector& v2);
friend scalar  operator *( vector& v1 , vector& v2);

friend vector operator %( vector& v1 , vector& v2);

inline  scalar& operator [](int i);
vector& operator +=(vector& v);
vector& operator -=(vector& v);
vector& operator  =(vector& v);

int     operator ==(vector& v);
int     operator !=(vector& v);


scalar* getdata() {return k  ;};
int     getdim () {return dim;};
int     getrows() {return 1  ;};
int     getcols() {return dim;};
int     is_usable() { return (dim>0) && (k!=0);};
scalar  getsqr();
scalar  getabs();
vector getuni();

////////////////////////////////////////////////////////
// new in optmath:

 vector(FILE* fp, int d=0);       // d==0 : read dimension from file
				  // d!=0 : this is the dimension
 

int     read (FILE* fp, int d=0); // d==0 : read dimension from file
				  // d!=0 : this is the dimension

int     write(FILE* fp, int dump=0); // dump==0 : don't dump dimension
				     // dump!=0 : do it

};
#endif

//

