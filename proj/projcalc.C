#include "projcalc.h"


// Baut eine Basis nur aus dem ViewZ Vektor.
//
// 0==Error 1==ok

int BaseFromZ( vector& vx , vector& vy , vector& vz )
{
 vz=vz.getuni();

 if (fabs(vz[0])<TOL) vz[0]=0.0;
 if (fabs(vz[1])<TOL) vz[1]=0.0;


 if ( (vz[0]==0.0) && (vz[1]==0.0) ) 
    {
     if (vz[2]>0.0) 
       {
        vz[2]=1.0; 
        vx=vector(3, 1.0, 0.0, 0.0);
        vy=vector(3, 0.0, 1.0, 0.0);
       } 
     else 
       {
        vz[2]=-1.0;
        vx=vector(3,-1.0, 0.0, 0.0);
        vy=vector(3, 0.0,-1.0, 0.0);
       }
     return 1;
    }

 if ( vz[0]==0.0 )
    {
     vx=vector(3,1.0,0.0,0.0);
     vy=vz % vx;
     if (vy[2]<0) {vx=vx*-1.0;vy=vz % vx;}
     return 1;
    }

 if ( vz[1]==0.0 )
    {
     vx=vector(3,0.0,1.0,0.0);
     vy=vz % vx;
     if (vy[2]<0) {vx=vx*-1.0;vy=vz % vx;}
     return 1;
    }



 vx[2]=0.0;

 if ( vz[1]!=vz[0] )
   {
    vx[0]=sqrt( fabs( 0.5*(1.0-SQR(vz[1]/vz[0])) ) );
    vx[1]=-vx[0]*vz[0]/vz[1];
   }
 else
   {
    vx[1]=0.7071;
    vx[0]=-vx[1];
   }

 vx=vx.getuni();
 vy=vz%vx;
 // DIE RICHTUNG wird durch vy[2]>0!!! bestimmt
 if (vy[2]<=0) { vx=-1.0*vx; vy=vz % vx ; }
 vy=vy.getuni();

 if ( vx.is_usable() && vy.is_usable() && vz.is_usable() ) return 1;

 return 0 ;
}

/////////////////////////////////////////////////////////////////////////////

void PolarToKart( vector& v , scalar r , scalar phi , scalar theta )
{
////printf("Entering Polar\r\n");
v[0]=r*cos(phi)*sin(theta);
v[1]=r*sin(phi)*sin(theta);
v[2]=r*cos(theta);
}

/////////////////////////////////////////////////////////////////////////////

// this one uses only the z-Part of the dots,
// assuming that they are already transformed

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
  )
{
double d0,d1,d2;
unsigned long i;




mindistmin=avrdistmin=maxdistmin=1.0E100;
mindistmax=avrdistmax=maxdistmax=0.0;

for(i=0;i<trcount;i++)
  {
   d0=-vdt[ (tr+i)->p[0] ][2] ;
   d1=-vdt[ (tr+i)->p[1] ][2] ;
   d2=-vdt[ (tr+i)->p[2] ][2] ;

	*(mindist+i)	=min( d0    , min(d1,d2) );
	mindistmin	=min( *(mindist+i) , mindistmin );
	mindistmax	=max( *(mindist+i) , mindistmax );

	*(avrdist+i)	=(d0+d1+d2)/3.0;
	avrdistmin	=min( *(avrdist+i) , avrdistmin );
	avrdistmax	=max( *(avrdist+i) , avrdistmax );

	*(maxdist+i)	=max( d0 , max(d1,d2) );
	maxdistmin	=min( *(maxdist+i) , maxdistmin );
	maxdistmax	=max( *(maxdist+i) , maxdistmax );
  }// end of for
}// end of CalcDistTriToEye


////////////////////////////////////////////////////////////////////////////

void TransformDots(unsigned long dtcount,
                   vector*       dt,
                   vector*       vdt,
                   matrix&       T,
                   vector&       trans
                  )
{
unsigned long i;

for(i=0;i<dtcount;i++) 
  {
   *(vdt+i)=(T*( *(dt+i) ) )-trans;
  }
}

/////////////////////////////////////////////////////////////////////////////

void 
ProjectDotsToPlane(unsigned long dtcount,
                   vector*       vdt,
                   double        focus
                  )
{
unsigned long i;
double        L;

for(i=0;i<dtcount;i++)
  {
//   if (vdt[i][2]>-1.0E-10) vdt[i]=INVALIDDOT;
//   else
       {
        L=-focus/vdt[i][2];
        vdt[i]=L*vdt[i];
       }
  }
}















