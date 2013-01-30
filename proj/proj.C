#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <math.h>
#include "projtype.h"
#include "projgrph.h"
#include "projcalc.h"
#include "projfile.h"

vector INVALIDDOT(3,0.0,0.0,+1.0);

// must be global for tricompare
disttype* mindist;
disttype* maxdist;
disttype* avrdist;

// passed to qsort for sorting triangle index
int tricompare(const void *ti1 ,const void *ti2)
{
  static disttype   TRICOMPd1,TRICOMPd2;

  TRICOMPd1=avrdist[ *((int*)ti1) ];
  TRICOMPd2=avrdist[ *((int*)ti2) ];
  if (TRICOMPd1<TRICOMPd2) return  1;
  if (TRICOMPd1>TRICOMPd2) return -1;

  //TRICOMPd1=maxdist[ *((int*)ti1) ];
  //TRICOMPd2=maxdist[ *((int*)ti2) ];
  //if (TRICOMPd1<TRICOMPd2) return  1;
  //if (TRICOMPd1>TRICOMPd2) return -1;

  return             0;
}

int main(int argc , char **argv)
{
  unsigned long *idx;
  vector* dt;
  vector* vdt;
  tri*    tr;
  vector* nv;
  unsigned long dtcount;
  unsigned long trcount;

  const char *dtfname="data/testbild.dot";
  const char *trfname="data/testbild.tri";
  unsigned long i;
  const char *errmsg;
  vector vx(3),vy(3),vz(3),eye(3),trans(3);
  matrix wtov(3),vtow(3);
  double focus;
  double phi,theta;
  double dist;
  double mindistmin,mindistmax,avrdistmin,avrdistmax,maxdistmin,maxdistmax;
  unsigned long piccount;

  // Xvars
  Display* dpy;
  int      scr;
  GC       gc;
  Window paintwin;
  Colormap cmap;
  int pixcol[64];
  Pixmap pmap;

  // read number of dots and dots themself
  if (0!=(errmsg=ReadDots(dtfname,dtcount,dt)))
    {
      printf("%s",errmsg);
      return 1;
    }
  // dito for triangles
  if (0!=(errmsg=ReadTriangles(trfname,trcount,tr)))
    { 
      printf("%s",errmsg);
      return 1;
    }
  // alloc mem for this and that:
  idx         = (unsigned long*) calloc(trcount,sizeof(*idx));
  vdt         = new              vector[dtcount];
  nv          = new              vector[trcount];
  mindist     = (disttype*)        calloc(trcount,sizeof(disttype));
  maxdist     = (disttype*)        calloc(trcount,sizeof(disttype));
  avrdist     = (disttype*)        calloc(trcount,sizeof(disttype));

  // enough Mem ?
  if ( (idx==0)||(vdt==0)||(mindist==0)||(maxdist==0)||(avrdist==0) )
    {
      printf("Out of Mem\r\n");
      return 1;
    }

  printf("dtcount:%lu trcount:%lu\r\n",dtcount,trcount);

  // Graphics Mode and Palette:
  // SetModeAndPalette();

  // get a Connection to XServer, a Window paintwin with BStore  and select Mouse Clicks
  {
    XSetWindowAttributes a;
    int                  mask;
    unsigned long        color;
    XColor               hard;

    dpy=XOpenDisplay(NULL);
    if (dpy==NULL)
      { printf("No Connection to XServer.\n");
	exit(1);
      }
    scr=DefaultScreen(dpy);
    a.bit_gravity=NorthWestGravity;
    a.win_gravity=NorthWestGravity;
    a.backing_store=Always;
    a.background_pixel=BlackPixel(dpy,scr);
    a.event_mask=0;
    a.override_redirect=False;
    mask=  CWBitGravity | CWWinGravity | CWBackingStore | CWEventMask 
      | CWOverrideRedirect | CWBackPixel ;
    paintwin=XCreateWindow(dpy,RootWindow(dpy,scr),100,400,
			   XWINX,XWINY,
			   0,
			   DefaultDepth(dpy,scr),InputOutput,
			   CopyFromParent,mask,&a);
    XMapWindow(dpy,paintwin);
    XFlush(dpy);
    printf("paintwinID:%lu\n",paintwin);
    cmap=DefaultColormap(dpy,scr);
    gc=XCreateGC(dpy,paintwin,0,NULL);
    for(color=0;color<64;color++)
      {
	hard.flags=0xff;
	hard.red=hard.green=hard.blue=(color << 10);
	//hard.red=(color <<12);
	//hard.blue=(color<<9);
	XAllocColor(dpy,cmap,&hard);
	pixcol[color]=hard.pixel;
      }
    pmap=XCreatePixmap(dpy,paintwin,
		       XWINX,XWINY,
		       DefaultDepth(dpy,scr));

    XSetLineAttributes(dpy,gc,2,LineSolid,CapButt,JoinMiter);
  }

  // Calc nv
  printf("calc %lu normals\n",trcount);
  for(i=0;i<trcount;i++)
    { 
      nv[i]=   (dt[ tr[i].p[1] ] - dt[tr[i].p[0]]).getuni()
	%
          (dt[ tr[i].p[2] ] - dt[tr[i].p[0]]).getuni()
	    ;
      if (0==(i % 100)) { printf("%8lu\r",i);fflush(stdout); }
    }
  // initialize triangle index
  for(i=0;i<trcount;i++) idx[i]=i; // only one time needed

  dist=40;
  if(argc==2)
    {
      sscanf(argv[1],"%lg",&dist);
    }

  piccount=0;
  for(phi=0.0*M_PI;phi<=2.0*M_PI;phi+=0.05*M_PI) // +=0.05 normal
    {

      focus=1.5;

      theta=0.1*M_PI+phi/5.0;

      PolarToKart(vz,1.0,phi,theta);
      BaseFromZ(vx,vy,vz);

      eye=dist*vz;

      vtow.setcolvec(0,vx);
      vtow.setcolvec(1,vy);
      vtow.setcolvec(2,vz);
      
      // wtov=vtow.inverted();
      
      vtow.getDETandINV	(wtov);

      trans=wtov*eye;

      // transform
      printf("%20s\r","transform");fflush(stdout);   
      TransformDots(dtcount,dt,vdt,wtov,trans);
      // Calc Distances of Triangles to EyePoint use only z-Coordinate of already transformed points
      printf("%20s\r","calc dist");fflush(stdout);
      CalcDistTriToEye(trcount,tr,dtcount,vdt,eye,
		       mindist,mindistmin,mindistmax,
		       avrdist,avrdistmin,avrdistmax,
		       maxdist,maxdistmin,maxdistmax  );
      // sort triangles
      printf("%20s\r","sort");fflush(stdout);
      qsort( idx,trcount,sizeof(*idx),tricompare);
      // project
      printf("%20s\r","project");fflush(stdout);
      ProjectDotsToPlane(dtcount,vdt,focus);
      // paint
      printf("%20s\r","paint");fflush(stdout);
      PaintToRegion(dpy,
		    /* paintwin, */ (Window)pmap,  
		    gc,
		    cmap,
		    pixcol,
		    2.0,2.0,                 // height,with
		    32.0,32.0,           // anglecolor, distcolor sum=64
		    vz,                  // light
		    dtcount,vdt,         // dots
		    trcount,tr,          // triangles
		    nv,                  // normvectors
		    idx,                 // dist.-ordered triangle index
		    vz                   // for visibility-check via
		    // surface orientation of triangles (not yet)
		    );
      printf("%20s\r","blit");fflush(stdout);XFlush(dpy);
      XCopyArea(dpy,pmap,paintwin,gc,0,0,XWINX,XWINY,0,0);

      XFlush(dpy);
      // inc piccount
      printf("\npicture:%8lu\n",++piccount);fflush(stdout);

      // enable to get screendumps
      if(0)
	{
	  char cmd[200];
	  // using ImageMagick:
	  // sprintf(cmd, "import -compress RunlengthEncoded -colorspace GRAY -scene %lu -window %lu pictures/picture%04lu.png", piccount,paintwin,piccount);

	  // using the old fashioned xwd (from package x11-apps):
	  // sprintf(cmd, "xwd -id %u -out pictures/somb%04u.xwd", paintwin, piccount);

	  printf("\n%s",cmd);fflush(stdout);

	  system(cmd);
	  printf(" DONE \n");fflush(stdout);
	}

    }// end of for any parameter

  return 0;
}







