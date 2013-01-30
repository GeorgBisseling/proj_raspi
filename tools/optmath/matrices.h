#ifndef _MATRICES_H
#define _MATRICES_H

#include "vectors.h"
#include <math.h>

class matrix
{
protected:
  
  static vector dummyvector;
  
  int dim; // Zahl der Zeilen
  vector* k;
  
public:

  //// De- und Konstruktoren

  matrix( int d=0 );                      // Quadratische bzw leere Matrix
  matrix( int nr , int nc , scalar* s );  // Init mit Zeiger auf C-Matrix
  matrix( int nr , int nc              ); // Init mit Nullen
  matrix( int nr ,  const vector* v           ); // Init mit Zeilenvektoren
  matrix( const matrix& m );                    // Init mit anderer Matrix
  ~matrix();                               // Destruktor

  //// Der Zuweisungsoperator

  matrix& operator  =(const matrix& m);

  //// Binaere Operatoren Matrix op Matrix

  matrix& operator +=( const matrix& m);
  matrix& operator -=( const matrix& m);
  matrix& operator *=( const matrix& m);
  friend matrix operator +( const matrix& m1 ,  const matrix& m2);
  friend matrix operator -( const matrix& m1 ,  const matrix& m2);
  friend matrix operator *( const matrix& m1 ,  const matrix& m2);

  //// Binäre Operatoren Matrix op Vektor

  friend vector operator *( const matrix& m ,  const vector& v);
  friend vector operator *( const vector& v ,  const matrix& m);

  //// Binäre Operatoren Matrix op Skalar

  friend matrix operator *( const matrix& m , scalar  s);
  friend matrix operator *(scalar  s ,  const matrix& m);

  //// Zugriffsoperatoren

  vector& operator [](int i) const {return k[i];};

  vector  getcolvec  (int i) const;
  // int 0==Fehler 1==OK
  int     setcolvec  (int i ,  const vector& v );

  int     swaprows   (int i , int j );
  int     swapcols   (int i , int j );

  int     rowtimes   (int i , scalar s );
  int     coltimes   (int i , scalar s );

  int     rowplus    (int i ,  const vector& v);
  int     colplus    (int i ,  const vector& v);

  int     rowminus   (int i ,  const vector& v);
  int     colminus   (int i ,  const vector& v);

  // Die folgenden geschtzten Methoden entsprechen den obigen
  // nur lassen sie die zeitintensiven Prfungen weg
  // obige Routinen rufen die geschtzten nach Test auf
protected:
  int     swr   (int i , int j );
  int     swc   (int i , int j );

  int     rt    (int i , scalar s );
  int     ct    (int i , scalar s );

  int     rp    (int i ,  const vector& v);
  int     cp    (int i ,  const vector& v);

  int     rm    (int i ,  const vector& v);
  int     cm    (int i ,  const vector& v);

  //// Unaere Operatoren
public:
  matrix unity(int d=1) const ; // Einheitsmatrix dxd
  scalar det() const ;			// nicht empfohlen s. LU Dekomposition

  matrix transposed() const ;
  matrix inverted  () const ;	// nicht empfohlen s. LU Dekomposition


  //// Kleinteile

  int     is_usable() const { return (dim>0) && (k!=0);};
  vector* getdata()   const {return k  ;};
  int     getdim ()   const {return dim;};
  int     getrows()   const {return dim;};
  int     getcols()   const {if (is_usable()) return k[0].getdim(); 
  else return 0;};



  //// LU Decomposition nach Numerical Recipes in C

  int	doLUdecomp	(int *indx,scalar *d);
  matrix	getINVfromLU	(int *indx,scalar *d);
  scalar	getDETfromLU	(int *indx,scalar *d) const ;
  void	backsubst	(int *indx, const vector& b);
  scalar	getDETandINV	(matrix& newinv) const ;


};


#endif






