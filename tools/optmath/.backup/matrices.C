#include "matrices.h"
#include <assert.h>
#include <string.h>
#include <stdlib.h>

// #define MATDEBUG

vector matrix::dummyvector=vector(0);

//// De- und Konstruktoren

 matrix::matrix( int d )                    // Quadratische bzw leere Matrix
 {
  int i;
  dim=0;k=0;
  if (d==0) return;
  k=(vector*)calloc( d , sizeof( vector ) );
  for(i=0;i<d;i++) k[i]=vector(d);dim=d;
 }




 matrix::matrix( int nr , int nc , scalar* s) // Init mit Zeiger auf C-Matrix
 {
  int i,j;
  dim=0;k=0;
  if ( (nr==0)||(nc==0) ) return;
  k=(vector*)calloc( nr , sizeof( vector ) );
  if   (s!=0) for(i=0;i<nr;i++) k[i]=vector( nc , s+(i*nc) );
  else        for(i=0;i<nr;i++) k[i]=vector( nc );
  dim=nr;
 }

 matrix::matrix( int nr , vector* v         ) // Init mit Zeilenvektoren
 {
  int i;
  dim=0;k=0;
  if ((nr==0)||(v==0)) return;
  k=(vector*)calloc( nr , sizeof( vector ) );
  for(i=0;i<nr;i++) k[i]=v[i];dim=nr;
 }

 matrix::matrix(matrix& m)
 {
  int i;
  dim=0;k=0;

  if (!m.is_usable()) return;

  k=(vector*)calloc( m.dim , sizeof(vector) );

  for(i=0;i<m.dim;i++) k[i]=m.k[i];
  dim=m.dim;
 }


 matrix::~matrix()                             // Destruktor
 {
  int i;
  for(i=0;i<dim;i++) k[i].vector::~vector();
  free(k);dim=0;k=0;
 }

//// Der Zuweisungsoperator

 matrix& matrix::operator  =(matrix& m)
 {
  int i;
  if (k!=NULL) free(k);
  dim=0;k=0;
  if (m.getrows()==0) return *this;
  k=(vector*)calloc( m.getrows() , sizeof(vector) );
  dim=m.getrows();
  for(i=0;i<dim;i++) k[i]=m.k[i];
  return *this;
 }

//// Bin„re Operatoren Matrix op Matrix

 matrix& matrix::operator +=(matrix& m)
 {
 int i;
#ifdef MATDEBUG
 if (   (!is_usable())||(!m.is_usable())
      ||(dim!=m.getrows())||(getcols()!=m.getcols()) ) return *this;
#endif
 for(i=0;i<dim;i++) k[i]+=m.k[i];
 return *this;
 }

 matrix& matrix::operator -=(matrix& m)
 {
 int i;
#ifdef MATDEBUG
 if (   (!is_usable())||(!m.is_usable())
      ||(dim!=m.getrows())||(getcols()!=m.getcols()) ) return *this;
#endif
 for(i=0;i<dim;i++) k[i]-=m.k[i];
 return *this;
 }

 matrix& matrix::operator *=(matrix& m)
 {
 int i,j;
#ifdef MATDEBUG
 if (   (!is_usable())||(!m.is_usable())
      ||(getcols()!=m.getrows()) ) return *this;
#endif
 matrix local( dim , m.getcols() );
 vector lv(0);
#ifdef MATDEBUG
 if (!local.is_usable()) return *this;
#endif
 for(j=0;j<local.getcols();j++)
  {
  lv=m.getcolvec(j);
  for(i=0;i<local.dim;i++) local[i][j]=k[i]*lv;
  }
 *this=local;
 return *this;
 }

 matrix operator +(matrix& m1 , matrix& m2)
 {
  matrix local(m1);
#ifdef MATDEBUG
  if (!local.is_usable()) return matrix(0);
#endif
  local+=m2;
  return matrix(local);
 }

 matrix operator -(matrix& m1 , matrix& m2)
 {
  matrix local(m1);
#ifdef MATDEBUG
  if (!local.is_usable()) return matrix(0);
#endif
  local-=m2;
  return matrix(local);
 }

 matrix operator *(matrix& m1 , matrix& m2)
 {
  matrix local(m1);
#ifdef MATDEBUG
  if (!local.is_usable()) return matrix(0);
#endif
  local*=m2;
  return matrix(local);
 }

//// Bin„re Operatoren Matrix op Vektor

 vector operator *(matrix& m , vector& v)
 {
#ifdef MATDEBUG
 if (  (!m.is_usable())||(!v.is_usable())
     ||(m.getcols()!=v.getcols())            ) return vector(0);
#endif
 vector local(m.dim);
#ifdef MATDEBUG
 if (!local.is_usable()) return vector(0);
#endif
 int i;
 for(i=0;i<m.dim;i++) local[i]=m.k[i]*v;
 return vector(local);
 }

 vector operator *(vector& v , matrix& m)
 {
#ifdef MATDEBUG
 if (  (!m.is_usable())||(!v.is_usable())
     ||(v.getcols()!=m.dim)                ) return vector(0);
#endif
 vector local(m.getcols());
#ifdef MATDEBUG
 if ( !local.is_usable()) return vector(0);
#endif
 int i;
 for(i=0;i<m.getcols();i++) local[i]=v*m.getcolvec(i);
 return vector(local);
 }

//// Binaere Operatoren Matrix op Skalar

 matrix operator *(matrix& m , scalar  s)
 {
  matrix local(m);
#ifdef MATDEBUG
  if (!local.is_usable()) return matrix(0);
#endif
  int i;
  for(i=0;i<m.dim;i++) local.k[i]=local.k[i]*s;
  return matrix(local);
 }



 matrix operator *(scalar  s , matrix& m)
 {
  matrix local(m);
#ifdef MATDEBUG
  if (!local.is_usable()) return matrix(0);
#endif
  int i;
  for(i=0;i<m.dim;i++) local.k[i]=local.k[i]*s;
  return matrix(local);
 }

//// Zugriffsoperatoren

 vector& matrix::operator [](int i)
 {
#ifdef MATDEBUG
  if ((!is_usable()) || (i<0) || (i>=dim) ) return dummyvector;
#endif
  return k[i];
 }

 vector  matrix::getcolvec  (int i)
 {
#ifdef MATDEBUG
  if ((!is_usable()) || (i<0) || (i>=getcols()) ) return vector(0);
#endif
  vector local(dim);
#ifdef MATDEBUG
  if (!local.is_usable()) return vector(0);
#endif
  int j;
  for(j=0;j<dim;j++) local[j]=k[j][i];
  return vector( local );
 }

 int     matrix::setcolvec  (int i , vector& v )
 {
#ifdef MATDEBUG
  if (   (!is_usable()) || (i<0) || (i>=getcols())
      || (v.getcols()!=dim) ) return 0;
#endif
  int j;
  for (j=0;j<dim;j++) k[j][i]=v[j];return 1;
 }

 int     matrix::swaprows   (int i , int j )
 {
#ifdef MATDEBUG
  if (   (!is_usable()) || (i<0) || (i>=dim)
      ||                   (j<0) || (j>=dim) ) return 0;
#endif
  return swr(i,j);
 }

 int     matrix::swapcols   (int i , int j )
 {
#ifdef MATDEBUG
  if (   (!is_usable()) || (i<0) || (i>=getcols())
      ||                   (j<0) || (j>=getcols()) ) return 0;
#endif
  return swc(i,j);
 }

 int     matrix::rowtimes   (int i , scalar s )
 {
#ifdef MATDEBUG
  if ( (!is_usable()) || (i<0) || (i>=dim) ) return 0;
#endif
  return rt(i,s);
 }

 int     matrix::coltimes   (int i , scalar s )
 {
#ifdef MATDEBUG
  if ( (!is_usable()) || (i<0) || (i>=getcols()) ) return 0;
#endif
  return ct(i,s);
 }

 int     matrix::rowplus    (int i , vector& v)
 {
#ifdef MATDEBUG
  if (   (!is_usable()) || (!v.is_usable()) || (getcols()!=v.dim)
      || (i<0) || (i>=dim) ) return 0;
#endif
  return rp(i,v);
 }

 int     matrix::colplus    (int i , vector& v)
 {
#ifdef MATDEBUG
  if (   (!is_usable()) || (!v.is_usable()) || (dim!=v.dim)
      || (i<0) || (i>=dim) ) return 0;
#endif
  return cp(i,v);
 }

 int     matrix::rowminus   (int i , vector& v)
 {
#ifdef MATDEBUG
  if (   (!is_usable()) || (!v.is_usable()) || (getcols()!=v.dim)
      || (i<0) || (i>=dim) ) return 0;
#endif
  return rm(i,v);
 }

 int     matrix::colminus   (int i , vector& v)
 {
#ifdef MATDEBUG
  if (   (!is_usable()) || (!v.is_usable()) || (dim!=v.dim)
      || (i<0) || (i>=dim) ) return 0;
#endif
  return cm(i,v);
 }

 // Die folgenden geschuetzten Methoden entsprechen den obigen
 // nur lassen sie die zeitintensiven Pruefungen weg
 // obige Routinen rufen die geschuetzten nach Test auf
 int     matrix::swr   (int i , int j )
 {
  char swaparray[ sizeof( vector )];
  // harte Kopie verhindert Kon/Destruktor-Orgien

  memcpy( swaparray , k+i       , sizeof(vector) );
  memcpy( k+i       , k+j       , sizeof(vector) );
  memcpy( k+j       , swaparray , sizeof(vector) );

  return 1;
 }

 int     matrix::swc   (int i , int j )
 {
  vector swapvec=vector( getcolvec(i) );
  setcolvec( i , getcolvec(j) );
  setcolvec( j , swapvec      );
  return 1;
 }

 int     matrix::rt    (int i , scalar s ) {k[i]=k[i]*s;return 1;}
 int     matrix::ct    (int i , scalar s )
 {
  int j;for(j=0;j<dim;j++) k[j][i]*=s;return 1;
 }

 int     matrix::rp    (int i , vector& v) {k[i]+=v;return 1;}
 int     matrix::cp    (int i , vector& v)
 {
 int j;for(j=0;j<dim;j++) k[j][i]+=v.k[j];return 1;
 }

 int     matrix::rm    (int i , vector& v) {k[i]-=v;return 1;}
 int     matrix::cm    (int i , vector& v)
 {
 int j;for(j=0;j<dim;j++) k[j][i]-=v.k[j];return 1;
 }



//// Unaere Operatoren

 matrix matrix::unity(int d) // Einheitsmatrix dxd
 {
  matrix local(d);
#ifdef MATDEBUG
  if (!local.is_usable()) return matrix(0);
#endif
  int i;
  for(i=0;i<d;i++) local.k[i][i]=1.0;
  return matrix(local);
 }

 scalar matrix::det()
 {
//// ???
#ifdef MATDEBUG
  if ( (!is_usable()) || (getcols()!=dim) ) return 0;

  if (1==dim) {if (k[0][0]!=0.0) return 1.0/k[0][0]; else return 0; }
  if (2==dim) {return k[0][0]*k[1][1]-k[1][0]*k[1][0];}

  assert( dim==3 ); // spaeter, wenn Submatrix implementiert ist
#endif
  return
  +  k[0][0]*k[1][1]*k[2][2]
                                 -  k[2][0]*k[1][1]*k[0][2]
  +  k[0][1]*k[1][2]*k[2][0]
                                 -  k[2][1]*k[1][2]*k[0][0]
  +  k[0][2]*k[1][0]*k[2][1]
                                 -  k[2][2]*k[1][0]*k[0][1]
  ;
 }


 matrix matrix::transposed()
 {
#ifdef MATDEBUG
  if (!is_usable()) return matrix(0);
#endif
  matrix local( getcols() , dim );
#ifdef MATDEBUG
  if (!local.is_usable()) return matrix(0);
#endif
  int i;
  for(i=0;i<local.dim;i++) local[i]=getcolvec(i);
  return matrix(local);
 }



 matrix matrix::inverted  ()
 {
#ifdef MATDEBUG
  if ( (!is_usable()) || (dim!=getcols()) ) return matrix(0);
#endif
  // if (det()==0.0) return matrix(0);
  // ???

  matrix u(0); u=u.unity(dim);
  matrix me(*this);
#ifdef MATDEBUG
  if ( (!u.is_usable()) || (!me.is_usable()) ) return matrix(0);
#endif
  int col,row,addrow,dia;
  scalar fac;


  // Hauptdiagonale != 0
  for(dia=0;dia<dim;dia++)
    if (me[dia][dia]==0.0)
      {
      for(addrow=0;(me[addrow][dia]==0.0)&&(addrow<dim);addrow++)
      assert( addrow<dim );
      me.rp(dia,me[addrow]);u.rp(dia,u[addrow]);
      }

  //Unteres Dreieck auf Null
  // Schleife ber Spalten
  //   Schleife ber Zeilen des unteren Dreiecks
  //     Komponenten zu Null
  for(col=0;col<dim-1;col++)
    for(row=col+1;row<dim;row++)
      if (me[row][col]!=0.0)
        {
         fac=me[row][col] / me[col][col];
         me.rm(row,fac*me[col]); u.rm(row,fac*u[col]);
         me[row][col]=0.0; // hilfts ?
        }

  //Oberes Dreieck auf Null
  // Schleife ber Spalten
  //   Schleife ber zeilen des oberen Dreiecks
  //     Komponenten zu Null
  for(col=dim-1;col>0;col--)
    for(row=col-1;row>=0;row--)
      if (me[row][col]!=0.0)
        {
         fac=me[row][col] / me[col][col];
         me.rm(row,fac*me[col]);u.rm(row,fac*u[col]);
         me[row][col]=0.0; // hilfts ?
        }

  // Hauptdiagonale normieren
  for(dia=0;dia<dim;dia++)
    {
     fac=me[dia][dia];
     // me[dia]=me[dia] / fac; // diese Operation ist berflssig,
                               // da die matrix me nicht mehr verwendet wird
     u [dia]=u [dia] / fac;
     // me[dia][dia]=1.0;      // dito
    }

  return matrix( u );
 }



//// LU Decomposition nach Numerical Recipes in C


// this one does the decomposition in place
// so "*this" is destroyed !!!
int	matrix::doLUdecomp	(int *indx,scalar *d)
{
int i,imax,j,l;
scalar big,dum,sum,temp;
vector vv(dim);

*d=1.0;
for(i=0;i<dim;i++)
	{
	big=0.0;
	for(j=0;j<dim;j++)
		if ((temp=fabs(k[i][j])) >big) big=temp;
	if (big==0.0) return 1;// singular
	vv[i]=1.0/big;
	}  
		
for(j=0;j<dim;j++)
	{
	for(i=0;i<j;i++)
		{
		sum=k[i][j];
		for(l=0;l<i;l++) sum -= k[i][l]*k[l][j];
		k[i][j]=sum;
		}
	big=0.0;
	for(i=j;i<dim;i++)
		{
		sum=k[i][j];
		for(l=0;l<j;l++) sum -= k[i][l]*k[l][j];
		k[i][j]=sum;
		if ((dum=vv[i]*fabs(sum)) >= big) {big=dum;imax=i;}
		}
	if(j!=imax) {swr(imax,j);*d=-(*d);vv[imax]=j;}
	indx[j]=imax;
	if (k[j][j]==0.0) k[j][j]=1.0E-20;
	if (j!=dim-1)
		{
		dum=1.0/(k[j][j]);
		for(i=j+1;i<dim;i++) k[i][j] *= dum;
		}
	}// end of for j -- next column
return 0; // OK
} // end of doLUdecomp


matrix	matrix::getINVfromLU	(int *indx,scalar *d)
{
int i,j;
matrix y(dim);
vector col(dim);

for(j=0;j<dim;j++)
	{
	for(i=0;i<dim;i++) col[i]=0.0;
	col[j]=1.0;
	backsubst(indx,col);
	y.setcolvec(j,col);
	}
return y;
}

scalar	matrix::getDETfromLU	(int *indx,scalar *d)
{
int	j;
scalar	det;

det=*d;
for(j=0;j<dim;j++) det*=k[j][j];
return det;
} // end of getDETfromLU


void	matrix::backsubst	(int *indx,vector& b)
{
int	i,ii=-1,ip,j;
scalar	sum;

for(i=0;i<dim;i++)
	{
	ip=indx[i];
	sum=b[ip];
	b[ip]=b[i];
	if(ii>-1)
		for(j=ii;j<=i-1;j++) sum-=k[i][j]*b[j];
	else if(sum!=0.0) ii=i;
	b[i]=sum;
	}
for(i=dim-1;i>=0;i--)
	{
	sum=b[i];
	for(j=i+1;j<dim;j++) sum-=k[i][j]*b[j];
	b[i]=sum/(k[i][i]);
	}
}// end of backsubst

scalar	matrix::getDETandINV	(matrix& newinv)
{
int	indx[dim];
scalar	d;

matrix LU(*this);
if (LU.doLUdecomp(indx,&d)) return 0.0;
newinv=LU.getINVfromLU(indx,&d);
return LU.getDETfromLU(indx,&d);
} // end of getDETandINV