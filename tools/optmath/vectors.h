#ifndef _VECTORS_H
#define _VECTORS_H

#include <stdio.h>



typedef double scalar;

class matrix;

class vector
{
  friend class matrix;

protected:

  static scalar dummyscalar;

  int dim;
  scalar* k;

public:

  vector( int d=0               );
  vector( int d      , scalar* s);
  vector( int d      , scalar  s , ... );
  vector( const vector& v        );
  ~vector(                       );

  friend vector operator *( scalar  s  , const vector& v );
  friend vector operator *( const vector& v  , scalar  s );
  friend vector operator /( const vector& v  , scalar  s );

  friend vector operator +( const vector& v1 , const vector& v2);
  friend vector operator -( const vector& v1 , const vector& v2);
  friend scalar operator *( const vector& v1 , const vector& v2);

  friend vector operator %( const vector& v1 , const vector& v2);

  inline  scalar& operator [](int i) const {return k[i];};

  vector& operator +=(const vector& v);
  vector& operator -=(const vector& v);
  vector& operator  =(const vector& v);

  int     operator ==(const vector& v) const;
  int     operator !=(const vector& v) const;


  scalar* getdata() const {return k  ;};
  int     getdim () const {return dim;};
  int     getrows() const {return 1  ;};
  int     getcols() const {return dim;};
  int     is_usable() const { return (dim>0) && (k!=0);};
  scalar  getsqr() const ;
  scalar  getabs() const ;
  vector getuni() const ;

  ////////////////////////////////////////////////////////
  // new in optmath:
    
    vector(FILE* fp, int d=0);       // d==0 : read dimension from file
					  // d!=0 : this is the dimension
 

					  int     read (FILE* fp, int d=0); // d==0 : read dimension from file
										 // d!=0 : this is the dimension

										 int     write(FILE* fp, int dump=0) const ; // dump==0 : don't dump dimension
// dump!=0 : do it

};
#endif
