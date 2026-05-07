/*--------------------------------------------------------*\
| Author: Ana Paula Tomas                                  |
| File: types.h                                            |
| MAD 2025/2026                                            |
\---------------------------------------------------------*/

#ifndef MACROS_H
#define MACROS_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAXEDGES(R) (6*(R-1)+8)
#define MAXVERTS(R) (2*(R-1)+4)
#define MAXFACES(R) ((R)+1)

#define XX 0
#define YY 1
#define ORTHOGONAL(D) (1-(D))
#define UNDEFV -1

#define KEY_V(N) (N)
#define KEY_HE(N) (N)
#define KEY_F(N) (N)

// D is a direction
#define ZHEAD(D) ((Zone[(D)]).firstelem)
#define ZNUMBFS(D) ((Zone[(D)]).nfaces)

// P is a pointer to a zone element
#define ZFHEDGE(P) ((P) -> startHE)  // in cut direction
#define ZFSWCORNER(P) ((P)->swcorner)  
#define ZFDIMT(P) ((P)->truncDim)
#define ZFDIMO(P) ((P)->otherDim)
#define ZFPREV(P) ((P)->prev)
#define ZFNXT(P) ((P)->nxt)

// P is a pointer to the gridline
#define GCORD(P) ((P)-> coord)
#define GNODE(P) ((P) -> v) 
#define GPREV(P) ((P)->prev)
#define GNXT(P) ((P)->nxt)
 
// V is IDV 
#define VNXT(V,C)  (Vert[(V)].nxt[(C)])
#define VCORD(V,C) (Vert[(V)].lines[(C)] -> coord)
#define VLINE(V,C) (Vert[(V)].lines[(C)])
#define VHEDGE(V) (Vert[(V)].hedge)

// E is IDHE
#define ENXT(E)  (HEdge[(E)].nxt)
#define EPREV(E) (HEdge[(E)].prev)
#define ETWIN(E) (HEdge[(E)].twin)
#define EFACE(E) (HEdge[(E)].face)
#define EORIG(E) (HEdge[(E)].origin)
#define EEND(E) (EORIG(ETWIN((E))))

// F is IDF
#define FACEHEDGE(F) (Face[(F)].swhedge)

#define ISDIRECTION(E,D) (VCORD(EORIG((E)),(D))== VCORD(EORIG(ETWIN((E))),(D)))
#define INTERSECTS(A,B,C) (((C) > (A) && (C) < (B)) || ((C) > (B) && (C) < (A)))

// Cuts:   (C) pointer to cut
#define CRANGE(C) ((C) -> range)
#define CDIMT(C) ((C) -> dimt)
#define CDIMO(C) ((C) -> dimo)
#define CDIR(C) ((C) -> dir)
#define CZK(C) ((C) -> zk)

#endif
