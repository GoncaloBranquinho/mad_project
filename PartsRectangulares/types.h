/*--------------------------------------------------------*\
| Author: Ana Paula Tomas                                  |
| File: types.h                                            |
| MAD 2025/2026                                            |
\---------------------------------------------------------*/

#ifndef RTYPES_H
#define RTYPES_H

#include <stdio.h>
#include <stdlib.h>


typedef int IDV;
typedef int IDHE;
typedef int IDF;

typedef struct halfedge {
  IDV origin;
  IDHE twin, nxt, prev;
  IDF face;
} HEDGE;

typedef struct face {
  IDHE swhedge;  // aresta com início no SW-vertex
} FACE;

typedef struct vertex {
  struct gridline *lines[2];
  IDV nxt[2];  // in the same XX and YY grid lines 
  IDHE hedge;  // origin of
} VERT;

typedef struct gridline {
  int coord;
  IDV v;   // first vertex in the grid line 
  struct gridline *nxt, *prev;
} GLINE;

typedef struct list {
  IDHE startHE; // first hedge (in face) along cut direction 
  IDV swcorner;
  int truncDim, otherDim;
  struct list *prev, *nxt;
} LIST;

typedef struct zone {
  int nfaces;
  LIST *firstelem;
} ZONE;

typedef enum {HORIZONTAL,VERTICAL} DIRECTION;

typedef struct cut {
  int range, dimt, dimo;
  DIRECTION dir;
  LIST *zk;
} CUT;

#endif

