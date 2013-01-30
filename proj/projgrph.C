#include "projgrph.h"

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
              tri*           tr, 
              vector*        nv,
              unsigned long *idx,
              vector         vz
	      )
{
  unsigned long i,j;
  double        color;
  double        GRSIZEX,GRSIZEY,GRSIZEX2,GRSIZEY2;
  double        scalex,scaley;
  short         *vdtx,*vdty;
  vector        mlight;
  XPoint	      points[4];

  // Clear Drawable
  XSetFunction  (dpy,gc,GXcopy);
  XSetForeground(dpy,gc,pixcol[0]);
  XFillRectangle(dpy,R,gc,0,0,XWINX,XWINY);

  // get pixelsize of Drawable GR.....
  GRSIZEX=XWINX;GRSIZEY=XWINY;GRSIZEX2=XWINX/2;GRSIZEY2=XWINY/2;

  scalex= (GRSIZEX-1) / (width); scaley= (GRSIZEY-1) / (height);

  // calc point x-, y-Coo into temp array:
  vdtx=(short*)calloc(dtcount,sizeof(*vdtx));
  vdty=(short*)calloc(dtcount,sizeof(*vdty));

  if ( (vdtx==0)||(vdty==0) )
    {
      printf("Out of Mem\r\n");
      exit(1);
    }


  for(i=0;i<dtcount;i++)
    {
      //    double tmp; 
      //    if ( (*vdtp)!=INVALIDDOT )
      //      {
      //       tmp=(short)(GRSIZEX2+scalex* (double)vdt[i][0]) ;
      //       if (fabs(tmp) < ((double)32000) ) vdtx[i]=(short)tmp;
      //       else                              vdtx[i]=32000;
      //       
      //       tmp=(short)(GRSIZEY2-scaley* (double)vdt[i][1]) ;
      //       if (fabs(tmp) < ((double)32000) ) vdty[i]=(short)tmp;
      //       else                              vdty[i]=32000;
      //      }
      //    else
      //      {
      //       vdtx[i]=32000;
      //       vdty[i]=32000;
      //      }
      vdtx[i]=(short)(GRSIZEX2+scalex* (double)vdt[i][0]) ;
      vdty[i]=(short)(GRSIZEY2-scaley* (double)vdt[i][1]) ;


    }// for i ... calc into array


  // paint:

  mlight=-1.0*light;


  for(j=0;j<trcount;j++) 
    {
      i=idx[j];
      // if ( (vz*nv[i])<0 ) continue; // paint front only    

      points[0].x=vdtx[ tr[i].p[0] ];	points[0].y=vdty[ tr[i].p[0] ];
      points[1].x=vdtx[ tr[i].p[1] ];	points[1].y=vdty[ tr[i].p[1] ];
      points[2].x=vdtx[ tr[i].p[2] ];	points[2].y=vdty[ tr[i].p[2] ];
      // for XDrawLines(...
      points[3].x=points[0].x;		points[3].y=points[0].y;

      // calculate color
      color = distcolor * ((double)j) / ((double)trcount);
      color+= anglecolor* max( nv[i]*light , nv[i]*mlight );
      if (color< 1.0) color= 1.0;
      else {if (color>63.0) color=63.0;}
      XSetForeground(dpy,gc,pixcol[(int)color]);


      XFillPolygon(dpy,R,gc,points,3,Convex,CoordModeOrigin);
      // XDrawLines  (dpy,R,gc,points,4       ,CoordModeOrigin);
      // XDrawPoint  (dpy,R,gc,points[0].x,points[0].y);
    }// for i
  free(vdtx);free(vdty);
}// PaintToRegion

////////////////////////////////////////////////////////////////////////////
void SetModeAndPalette()
{

  // allocate some colors

}

