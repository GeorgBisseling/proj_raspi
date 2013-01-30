#ifndef _PROJTYPE_H
#define _PROJTYPE_H

#include <vectors.h>
#include <matrices.h>

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#define XWINX	384	
#define XWINY	384


// no dot should have a positive z-coordinate in v :

extern vector INVALIDDOT;


struct tri   { unsigned long p[3]; };

typedef float disttype;

#endif
