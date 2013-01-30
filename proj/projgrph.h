#ifndef _PROJGRPH_H
#define _PROJGRPH_H

#include <stdio.h>
// #include <std.h>
// #include <graphics.h>
#include <math.h>
#include "projtype.h"

#define min(a,b) ( (a>b) ? b : a )
#define max(a,b) ( (a>b) ? a : b )
#define SQR(A) ((A)*(A))
#define TOL    ((double)1.0E-3)

////////////////////////////////////////////////////////////////////////////
void
PaintToRegion(Display*       dpy,
	      Window         R,
	      GC             gc,
	      Colormap       cmap,
	      int*           pixcol,
              double         height,
              double         width,
              double         anglecolor,
              double         distcolor,
              vector         light,
              unsigned long  dtcount,
              vector*        vdt,
              unsigned long  trcount,
              tri*           tri, 
              vector*        nv,
              unsigned long *idx,
              vector         vz
             );


////////////////////////////////////////////////////////////////////////////
void SetModeAndPalette();


#endif














