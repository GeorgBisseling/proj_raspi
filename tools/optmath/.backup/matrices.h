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
 matrix( int nr , int nc , scalar* s=0); // Init mit Zeiger auf C-Matrix
 matrix( int nr , vector* v           ); // Init mit Zeilenvektoren
 matrix( matrix& m );                    // Init mit anderer Matrix
~matrix();                               // Destruktor

//// Der Zuweisungsoperator

 matrix& operator  =(matrix& m);

//// Binaere Operatoren Matrix op Matrix

 matrix& operator +=(matrix& m);
 matrix& operator -=(matrix& m);
 matrix& operator *=(matrix& m);
 friend matrix operator +(matrix& m1 , matrix& m2);
 friend matrix operator -(matrix& m1 , matrix& m2);
 friend matrix operator *(matrix& m1 , matrix& m2);

//// Bin„re Operatoren Matrix op Vektor

 friend vector operator *(matrix& m , vector& v);
 friend vector operator *(vector& v , matrix& m);

//// Bin„re Operatoren Matrix op Skalar

 friend matrix operator *(matrix& m , scalar  s);
 friend matrix operator *(scalar  s , matrix& m);

//// Zugriffsoperatoren

 vector& operator [](int i);

 vector  getcolvec  (int i);
 // int 0==Fehler 1==OK
 int     setcolvec  (int i , vector& v );

 int     swaprows   (int i , int j );
 int     swapcols   (int i , int j );

 int     rowtimes   (int i , scalar s );
 int     coltimes   (int i , scalar s );

 int     rowplus    (int i , vector& v);
 int     colplus    (int i , vector& v);

 int     rowminus   (int i , vector& v);
 int     colminus   (int i , vector& v);

 // Die folgenden geschtzten Methoden entsprechen den obigen
 // nur lassen sie die zeitintensiven Prfungen weg
 // obige Routinen rufen die geschtzten nach Test auf
 protected:
 int     swr   (int i , int j );
 int     swc   (int i , int j );

 int     rt    (int i , scalar s );
 int     ct    (int i , scalar s );

 int     rp    (int i , vector& v);
 int     cp    (int i , vector& v);

 int     rm    (int i , vector& v);
 int     cm    (int i , vector& v);

//// Unaere Operatoren
 public:
 matrix unity(int d=1); // Einheitsmatrix dxd
 scalar det();

 matrix transposed();
 matrix inverted  ();


//// Kleinteile

 vector* getdata() {return k  ;};
 int     getdim () {return dim;};
 int     getrows() {return dim;};
 int     getcols() {if (is_usable()) return k[0].getdim(); else return 0;};
 int     is_usable() { return (dim>0) && (k!=0);};


//// LU Decomposition nach Numerical Recipes in C

int	doLUdecomp	(int *indx,scalar *d);
matrix	getINVfromLU	(int *indx,scalar *d);
scalar	getDETfromLU	(int *indx,scalar *d);
void	backsubst	(int *indx,vector& b);
scalar	getDETandINV	(matrix& newinv);


};


#endif
