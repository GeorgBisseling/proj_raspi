#ifndef _PROJCALC_H
#define _PROJCALC_H

#include <math.h>
#include "projtype.h"

#define min(a,b) ( (a>b) ? b : a )
#define max(a,b) ( (a>b) ? a : b )
#define SQR(A) ((A)*(A))
#define TOL    ((double)1.0E-3)


///////////////////////////////////////////////////////////////////////////// 
// Baut eine Basis nur aus dem ViewZ Vektor.
// 0==Error 1==ok
int BaseFromZ( vector& vx , vector& vy  , vector& vz  );

/////////////////////////////////////////////////////////////////////////////
void PolarToKart( vector& v , scalar r , scalar phi , scalar theta );


/////////////////////////////////////////////////////////////////////////////
void 
CalcDistTriToEye
  (unsigned long trcount,
   tri*          tr,
   unsigned long dtcount,
   vector*       vdt,
   vector        eye,
   disttype*       mindist,double& mindistmin,double& mindistmax,
   disttype*       avrdist,double& avrdistmin,double& avrdistmax,
   disttype*       maxdist,double& maxdistmin,double& maxdistmax
  );


/////////////////////////////////////////////////////////////////////////////
void TransformDots(unsigned long dtcount,
                   vector*       dt,
                   vector*       vdt,
                   matrix&       T,
                   vector&       trans
                  );


/////////////////////////////////////////////////////////////////////////////
void
ProjectDotsToPlane(unsigned long dtcount,
                   vector*       vdt,
                   double        focus
                  );


#endif

