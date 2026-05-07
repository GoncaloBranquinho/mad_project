/*--------------------------------------------------------*\
| Author: Ana Paula Tomas                                  |
| File: rectParts.c                                        |
| MAD 2025/2026                                            |
| Creates s instances of rectangular partitions of size nr |
| (s and nr defined by the user)                           |
\---------------------------------------------------------*/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//-------------   Options for compilation -----------------------
// --- output image
#define PICTSTEPBYSTEP
//---  output LaTeX pictures

#define FACELABELS
//-- label faces from 1 to nr
// #define SHOWCOORDS
//-- show coordinates of the vertices

// uniform or geometric distribution for random choices
// #define UNIFORM
//   (maybe not very interesting)

#define GEOMETRIC
// #define Tau 0.7
#define Tau 0.75
//----------------------------------------------------

#include "types.h"

VERT *Vert;
HEDGE *HEdge;
FACE *Face;
IDV SWCorner;
int NVerts, NHEdges, NFaces;
ZONE Zone[2];
GLINE *Grid[2];

#include "macros.h"

static GLINE *initlines();
static void allocDCEL(int rect);
static void defineInitialPart();
static void initZones();
// static GLINE *searchGridline(DIRECTION dir,int coord);

void initPartGrid(int rect);
LIST *newElemZone(int tdim, int odim, LIST *prev, LIST *nxt, IDHE e, IDV swc);
GLINE *createGridLine(CUT *cut, GLINE *first);
GLINE *newGridline(int coord, GLINE *prev, GLINE *nxt, IDV idv);
void initV(IDV v, GLINE *lx, GLINE *ly, IDV nx, IDV ny, IDHE h);
void initF(IDF f, IDHE hedge);
void initHE(IDHE e, IDHE twin, IDHE nxt, IDHE prev, IDF f, IDV ov);
void defineVertOnEdge(IDV v, IDHE s, DIRECTION dir, GLINE *line);
void split_edge(IDHE s, IDV vNew1, int dir, GLINE *line);
IDHE split_edge_shot(LIST *z, GLINE *line, DIRECTION dir);
void zf_update_dims(LIST *z, int dimt, int dimo);
void updateZones(CUT *cut, IDHE edir, IDHE eorth, IDV swc);
void insertCut(CUT *cut);
void insertHcut(IDHE eG1, IDHE eG2, IDHE e0, IDHE ekf, IDHE s, IDV vNew0);
void insertVcut(IDHE eG1, IDHE eG2, IDHE e0, IDHE ekf, IDHE s, IDV vNew0);
void updateHEfaces(IDHE eG1);
void selectCut(CUT *cut, DIRECTION dir, int (*fsel)(int));
void createRpart(int nrect, FILE *fr, FILE *fpict, int okPict);
int nverts_face(IDF f);
void outputRpart(FILE *fresult, FILE *fpict, int okPict);
void drawRpart(FILE *fresult);
void freeMem();
void free_glist(GLINE *line);
void free_zlist(LIST *z);
int dimGrid(GLINE *line);
int uniformDistr(int nz);
int geometricDistr(int nz);

/* ------------------ \
   Initial partition
\ ------------------ */

void initPartGrid(int rect) {
  Grid[HORIZONTAL] = initlines();
  Grid[VERTICAL] = initlines();
  allocDCEL(rect);
  defineInitialPart();
}

static GLINE *initlines() { // to create the two initial lines
  GLINE *init;
  init = newGridline(0, NULL, NULL, UNDEFV);
  GNXT(init) = newGridline(1, init, NULL, UNDEFV);
  return init;
}

static void allocDCEL(int rect) {
  //  printf("\n EMAX = %d\n VMAX = %d\n FMAX =
  //  %d\n\n",MAXEDGES(rect),MAXVERTS(rect),MAXFACES(rect));

  HEdge = (HEDGE *)(malloc(sizeof(HEDGE) * MAXEDGES(rect)));
  Face = (FACE *)(malloc(sizeof(FACE) * MAXFACES(rect)));
  Vert = (VERT *)(malloc(sizeof(VERT) * MAXVERTS(rect)));
}

static void
defineInitialPart() { // (0,0)  SW corner, square in CCW , standard coordinates
  GLINE *l1x, *l2x, *l1y, *l2y;
  int i;

  // lines
  // x=0  and x=1
  l1x = Grid[VERTICAL];
  l2x = GNXT(Grid[VERTICAL]);
  // y=0  and y=1
  l1y = Grid[HORIZONTAL];
  l2y = GNXT(Grid[HORIZONTAL]);
  GNODE(l2x) = 1;
  GNODE(l2y) = 3;
  GNODE(l1x) = 0;
  GNODE(l1y) = 0;

  // vertices
  initV(3, l1x, l2y, UNDEFV, 2, 6);      // NW
  initV(2, l2x, l2y, UNDEFV, UNDEFV, 4); // NE
  initV(1, l2x, l1y, 2, UNDEFV, 2);      // SE
  initV(0, l1x, l1y, 3, 1, 0);           // SW

  // faces and edges
  initF(0, 1); // unbounded face
  initF(1, 0); // unit square
  for (i = 0; i < 4; i++) {
    initHE(2 * i, 2 * i + 1, (2 * (i + 1)) % 8, (8 + 2 * (i - 1)) % 8, 1, i);
    initHE(2 * i + 1, 2 * i, (8 + 2 * (i - 1) + 1) % 8, (2 * (i + 1) + 1) % 8,
           0, (i + 1) % 4);
  }

  NFaces = 2;
  NVerts = 4;
  NHEdges = 8;

  // zones
  initZones();

  SWCorner = 0; // SW corner
}

/* -------------------- Zones --------------------------------- */

static void initZones() {
  ZHEAD(VERTICAL) =
      newElemZone(1, 1, NULL, NULL, 6, 0); // to add vertical lines
  ZNUMBFS(VERTICAL) = ZNUMBFS(HORIZONTAL) = 1;
  ZHEAD(HORIZONTAL) =
      newElemZone(1, 1, NULL, NULL, 0, 0); // to add horiz. lines
}

LIST *newElemZone(int tdim, int odim, LIST *prev, LIST *nxt, IDHE e, IDV swc) {
  LIST *z = (LIST *)malloc(sizeof(LIST));
  ZFDIMT(z) = tdim;
  ZFDIMO(z) = odim;
  ZFPREV(z) = prev;
  ZFNXT(z) = nxt;
  ZFHEDGE(z) = e;
  ZFSWCORNER(z) = swc;
  return z;
}

/* -------------------- Grid --------------------------------- */

GLINE *newGridline(int coord, GLINE *prev, GLINE *nxt,
                   IDV idv) { // to define a new grid line
  GLINE *newline = (GLINE *)malloc(sizeof(GLINE));
  GCORD(newline) = coord;
  GNODE(newline) = idv;
  GPREV(newline) = prev;
  GNXT(newline) = nxt;
  return newline;
}

/*
static GLINE *searchGridline(DIRECTION dir,int coord)
{ // to find a grid line
  GLINE *p =  Grid[dir];
  while (p != NULL && GCORD(p) != coord)
    p = GNXT(p);
  return p;
}
*/

GLINE *createGridLine(CUT *cut, GLINE *first) {
  GLINE *prev = NULL, *newL;
  int coord = CDIMT(cut);
  while (coord != GCORD(first)) {
    prev = first;
    first = GNXT(first);
  }
  newL = newGridline(coord, prev, first, SWCorner);
  GNXT(prev) = newL;
  GPREV(first) = newL;

  // update coordinates (how to do it lazily???)
  do {
    GCORD(first)++;
  } while ((first = GNXT(first)) != NULL);
  return newL;
}

/* -------------------- DCEL --------------------------------- */

void initV(IDV v, GLINE *lx, GLINE *ly, IDV nx, IDV ny, IDHE h) {
  VNXT(v, VERTICAL) = nx;
  VNXT(v, HORIZONTAL) = ny;
  VLINE(v, VERTICAL) = lx;
  VLINE(v, HORIZONTAL) = ly;
  VHEDGE(v) = h;
}

void initF(IDF f, IDHE hedge) { FACEHEDGE(f) = hedge; }

void initHE(IDHE e, IDHE twin, IDHE nxt, IDHE prev, IDF f, IDV ov) {
  ENXT(e) = nxt;
  EPREV(e) = prev;
  EFACE(e) = f;
  EORIG(e) = ov;
  ETWIN(e) = twin;
}

void defineVertOnEdge(IDV v, IDHE s, DIRECTION dir, GLINE *line) {
  int orthdir = ORTHOGONAL(dir);
  IDV vprev;

  VLINE(v, orthdir) = VLINE(EORIG(s), orthdir);
  VLINE(v, dir) = line;
  VHEDGE(v) = ENXT(s);
  VNXT(v, dir) = UNDEFV;
  vprev = (dir == HORIZONTAL ? EORIG(s) : EEND(ENXT(s)));
  VNXT(v, orthdir) = VNXT(vprev, orthdir);
  VNXT(vprev, orthdir) = v;
}

void split_edge(IDHE s, IDV vNew1, int dir,
                GLINE *line) { // splits s and twin(s) to insert vNew1

  IDHE eNew1 = NHEdges, eNew2 = NHEdges + 1;
  NHEdges += 2;

  // --- split edges s and twin(s) to insert vNew1
  initHE(eNew1, ETWIN(s), ENXT(s), s, EFACE(s), vNew1);
  initHE(eNew2, s, ENXT(ETWIN(s)), ETWIN(s), EFACE(ETWIN(s)), vNew1);
  ENXT(ETWIN(s)) = eNew2;
  EPREV(ENXT(eNew2)) = eNew2;
  ENXT(s) = eNew1;
  EPREV(ENXT(eNew1)) = eNew1;
  ETWIN(ETWIN(s)) = eNew1;
  ETWIN(s) = eNew2;

  // --- update vNew1 data
  defineVertOnEdge(vNew1, s, dir, line);
}

IDHE split_edge_shot(LIST *z, GLINE *line, DIRECTION dir) {
  IDHE s = ZFHEDGE(z); // first edge along cut direction
  int c0, c1, coord = GCORD(line);
  IDV vNew1 = NVerts++;

  // encontrar s de saida que intersecta line
  while (ISDIRECTION(s, dir))
    s = (dir == VERTICAL ? EPREV(s) : ENXT(s));
  // continua procura na normal
  c0 = VCORD(EORIG(s), dir);
  c1 = VCORD(EEND(s), dir);
  while (!INTERSECTS(c0, c1, coord)) {
    s = (dir == VERTICAL ? EPREV(s) : ENXT(s));
    c0 = VCORD(EORIG(s), dir);
    c1 = VCORD(EEND(s), dir);
  }

  // partir aresta de saida no ponto de interseccao
  split_edge(s, vNew1, dir, line);

  return s; // starts crossed segement   s -- vNew1-- ENXT(s)
}

void zf_update_dims(LIST *z, int dimt, int dimo) {
  ZFDIMO(z) = dimo;
  ZFDIMT(z) = dimt;
  while ((z = ZFNXT(z)) != NULL)
    if (ZFDIMT(z) > dimt)
      ZFDIMT(z) = dimt;
}

void updateZones(CUT *cut, IDHE edir, IDHE eorth, IDV swc) {
  int orthd;
  DIRECTION dir = CDIR(cut);
  LIST *z = ZHEAD(dir), *aux;

  // update Zone[dir]  //--- free extra zones
  while (z != CZK(cut)) {
    aux = z;
    z = ZFNXT(z);
    free(aux);
  }
  ZHEAD(dir) = z;
  ZNUMBFS(dir) -= (CRANGE(cut) - 1);
  ZFPREV(z) = NULL;
  zf_update_dims(z, CDIMT(cut), CDIMO(cut));
  ZFHEDGE(z) = edir;
  ZFSWCORNER(z) = swc;

  // update zone along orthogonal direction Zone[orthd]
  orthd = ORTHOGONAL(dir);
  z = newElemZone(CDIMO(cut), CDIMT(cut), NULL, ZHEAD(orthd), eorth, swc);
  ZFDIMO(ZHEAD(orthd)) -= (CDIMT(cut) - 1);
  ZHEAD(orthd) = z;
  ZNUMBFS(orthd) += 1;
  ZFPREV(ZFNXT(z)) = z;
}

/*
 ------------------
   Insert cut
 ------------------
*/

void insertCut(CUT *cut) {
  IDHE eG1, eG1t, eG2, eG2t, s, ekf, e0;
  IDV vNew0;
  int i;
  DIRECTION dir = CDIR(cut);
  GLINE *line = createGridLine(cut, Grid[dir]);
  LIST *z;

  s = split_edge_shot(CZK(cut), line, dir);

  // change dir-coordinate of chain vertices
  for (i = 0, z = ZHEAD(dir); i < CRANGE(cut); i++) {
    VLINE(ZFSWCORNER(z), dir) = line;
    z = ZFNXT(z);
  }

  ekf = ZFHEDGE(CZK(cut));
  e0 = ZFHEDGE(ZHEAD(dir));

  vNew0 = NVerts++;
  eG1 = NHEdges;
  eG1t = NHEdges + 1; // cut direction
  eG2 = NHEdges + 2;
  eG2t = NHEdges + 3; // orthogonal to the cut direction
  NHEdges += 4;
  ETWIN(eG1) = eG1t;
  ETWIN(eG1t) = eG1;
  ETWIN(eG2) = eG2t;
  ETWIN(eG2t) = eG2;

  updateZones(cut, eG1, eG2, vNew0);

  if (dir == HORIZONTAL)
    insertHcut(eG1, eG2, e0, ekf, s, vNew0);
  else
    insertVcut(eG1, eG2, e0, ekf, s, vNew0);

  SWCorner = vNew0;
}

void insertVcut(IDHE eG1, IDHE eG2, IDHE e0, IDHE ekf, IDHE s,
                IDV vNew0) { // caso vertical
  IDHE eG1t = ETWIN(eG1), eG2t = ETWIN(eG2);

  EORIG(eG2) = vNew0;
  EORIG(eG1) = EORIG(ekf);
  EORIG(eG2t) = SWCorner;
  EORIG(eG1t) = vNew0;
  EFACE(eG2) = NFaces++;
  FACEHEDGE(EFACE(eG2)) = eG2;
  EFACE(eG2t) = 0;
  EFACE(eG1t) = 0;
  FACEHEDGE(0) = eG2t;

  // ligar eG1 e eG1t a vk
  EPREV(eG1) = EPREV(ekf);
  ENXT(EPREV(eG1)) = eG1;
  ENXT(eG1t) = ENXT(ETWIN(ekf));
  EPREV(ENXT(eG1t)) = eG1t;
  // ligar G2 e G2t a v0
  ENXT(eG2) = ETWIN(e0);
  EPREV(ENXT(eG2)) = eG2;
  EPREV(eG2t) = ETWIN(ENXT(e0));
  ENXT(EPREV(eG2t)) = eG2t;
  // liga ekf e twin(ekf) a s
  ENXT(ETWIN(ekf)) = ENXT(s);
  EPREV(ekf) = s;
  ENXT(s) = ekf;
  EPREV(ENXT(ETWIN(ekf))) = ETWIN(ekf);
  EORIG(ekf) = EEND(s);

  // ligacoes em vNew0
  ENXT(eG1) = eG2;
  EPREV(eG2) = eG1;
  ENXT(eG2t) = eG1t;
  EPREV(eG1t) = eG2t;

  updateHEfaces(eG2);

  // verts
  initV(vNew0, Grid[VERTICAL], Grid[HORIZONTAL], EORIG(eG1), SWCorner, eG2);
  VNXT(EEND(ekf), VERTICAL) = EEND(s);
}

void insertHcut(IDHE eG1, IDHE eG2, IDHE e0, IDHE ekf, IDHE s,
                IDV vNew0) { // caso horizontal
  IDHE eG1t = ETWIN(eG1), eG2t = ETWIN(eG2);

  EORIG(eG2) = SWCorner;
  EORIG(eG1) = vNew0;
  EORIG(eG2t) = vNew0;
  EORIG(eG1t) = EORIG(ETWIN(ekf));
  EFACE(eG1) = NFaces++;
  FACEHEDGE(EFACE(eG1)) = eG1;
  EFACE(eG2t) = 0;
  EFACE(eG1t) = 0;
  FACEHEDGE(0) = eG1t;

  // ligar eG1 e eG1t a vk
  ENXT(eG1) = ENXT(ekf);
  EPREV(eG1t) = EPREV(ETWIN(ekf));
  EPREV(ENXT(eG1)) = eG1;
  ENXT(EPREV(eG1t)) = eG1t;
  // ligar G2 e G2t a v0
  EPREV(eG2) = ETWIN(e0);
  ENXT(eG2t) = ENXT(ETWIN(e0));
  ENXT(EPREV(eG2)) = eG2;
  EPREV(ENXT(eG2t)) = eG2t;
  // liga ekf e twin(ekf) a s
  ENXT(ekf) = ENXT(s);
  EPREV(ENXT(ekf)) = ekf;
  EPREV(ETWIN(ekf)) = s;
  ENXT(s) = ETWIN(ekf);

  // ligacoes em vNew0
  ENXT(eG2) = eG1;
  EPREV(eG1) = eG2;
  ENXT(eG1t) = eG2t;
  EPREV(eG2t) = eG1t;
  EORIG(ETWIN(ekf)) = EEND(s);

  updateHEfaces(eG1);

  // verts
  initV(vNew0, Grid[VERTICAL], Grid[HORIZONTAL], SWCorner, EEND(eG1), eG1);
  VNXT(EORIG(ekf), HORIZONTAL) = EEND(s);
}

void updateHEfaces(IDHE eG1) {
  IDHE e = ENXT(eG1);
  IDF f = EFACE(eG1);

  do {
    EFACE(e) = f;
  } while ((e = ENXT(e)) != eG1);
}

/*
 ------------------
   Select cut
 ------------------
*/

// uniform
int uniformDistr(int nz) { return 1 + rand() % nz; }
#include <math.h>
double log(double num);

// geometric
int geometricDistr(int nz) {
  double p = rand() * 1.0 / RAND_MAX;
  return 1 + ((int)(log(1.0 - p + pow(p, nz * 1.0)) / log(Tau)));
}

void selectCut(CUT *cut, DIRECTION dir, int (*fsel)(int)) { // select cut
  int i;
  LIST *z;
  CDIR(cut) = dir;
  CRANGE(cut) = fsel(ZNUMBFS(dir)); // number of rects involved
  z = ZHEAD(dir);
  CDIMO(cut) = ZFDIMO(z);
  for (i = 1; i < CRANGE(cut); i++) {
    z = ZFNXT(z);
    CDIMO(cut) += ZFDIMO(z);
  }
  CZK(cut) = z;
  CDIMT(cut) = 1 + rand() % ZFDIMT(z);
}

void createRpart(int nrects, FILE *fresult, FILE *fpicts, int okPict) {
  CUT cut;
  DIRECTION *dirs = (DIRECTION *)malloc(sizeof(DIRECTION) * nrects);
  int i;

  for (i = 1; i < nrects; i++)
    dirs[i - 1] = rand() % 2;

  initPartGrid(nrects);
  for (i = 1; i < nrects; i++) {
#ifdef UNIFORM
    selectCut(&cut, dirs[i - 1], uniformDistr);
#endif
#ifdef GEOMETRIC
    selectCut(&cut, dirs[i - 1], geometricDistr);
#endif
    insertCut(&cut);
#ifdef PICTSTEPBYSTEP
    if (okPict)
      drawRpart(fpicts);
#endif
  }
  if (okPict)
    drawRpart(fpicts);
  outputRpart(fresult, fpicts, okPict);
  free(dirs);
  freeMem();
}

void freeMem() {
  free_zlist(ZHEAD(HORIZONTAL));
  free_zlist(ZHEAD(VERTICAL));
  free_glist(Grid[HORIZONTAL]);
  free_glist(Grid[VERTICAL]);

  free(HEdge);
  free(Face);
  free(Vert);
}

void free_glist(GLINE *line) {
  if (line == NULL)
    return;
  free_glist(GNXT(line));
  free(line);
}

void free_zlist(LIST *z) {
  if (z == NULL)
    return;
  free_zlist(ZFNXT(z));
  free(z);
}

int nverts_face(IDF f) {
  IDHE e0, e;
  int n = 0;
  e0 = e = FACEHEDGE(f);
  do {
    n++;
  } while ((e = ENXT(e)) != e0);
  return n;
}

#define ABS(X) ((X) >= 0 ? (X) : (-(X)))

int dimGrid(GLINE *line) {
  if (line == NULL)
    return 0;
  return 1 + dimGrid(GNXT(line));
}
void drawRpart(FILE *fr) {
  int f, nv, dx, dy, delta, xi, yi;
  IDHE e;

  // fprintf(fr,"\n%d\n", NFaces-1);

  dy = dimGrid(Grid[HORIZONTAL]);
  dx = dimGrid(Grid[VERTICAL]);

  fprintf(fr, "\n\n\\begin{picture}(%d,%d)\n", dx + 1, dy + 1);

  for (f = 1; f < NFaces; f++) {
    nv = nverts_face(f);

    e = FACEHEDGE(f);
#ifdef FACELABELS
    xi = VCORD(EORIG(e), VERTICAL);
    yi = VCORD(EORIG(e), HORIZONTAL);
    fprintf(fr, "\\put(%.1f,%.1f){\\makebox(0,0){%d}}\n", xi + 1.3, yi + 1.3,
            f);
#endif
    while (nv--) {
      xi = VCORD(EORIG(e), VERTICAL);
      yi = VCORD(EORIG(e), HORIZONTAL);
      dx = VCORD(EEND(e), VERTICAL) - xi;
      dy = VCORD(EEND(e), HORIZONTAL) - yi;
      if (dx) {
        delta = ABS(dx);
        dx = dx / delta;
      } else {
        delta = ABS(dy);
        dy = dy / delta;
      }
      fprintf(fr, "\\put(%d,%d){\\line(%d,%d){%d}}\n", xi + 1, yi + 1, dx, dy,
              delta);
      e = ENXT(e);
    }
  }
  fprintf(fr, "\\end{picture}\n\n");
}

void outputRpart(FILE *fresult, FILE *fpicts, int okPicts) {
  int f, nv;
  IDHE e;
  IDV v0;

  fprintf(fresult, "%d\n", NFaces - 1);

  for (f = 1; f < NFaces; f++) {
    nv = nverts_face(f);
#ifdef SHOWCOORDS
    if (okPicts) {
      fprintf(fpicts, "{\\footnotesize \n\n");
      fprintf(fpicts, "Face: %d\n\n\\   \\    NVerts: %d\n\n \\   \\  ", f, nv);
    }
#endif
    fprintf(fresult, "%d %d", f, nv);
    e = FACEHEDGE(f);
    while (nv--) {
      v0 = EORIG(e);
#ifdef SHOWCOORDS
      if (okPicts)
        fprintf(fpicts, " (%d %d)", VCORD(v0, VERTICAL), VCORD(v0, HORIZONTAL));
#endif
      fprintf(fresult, " %d %d", VCORD(v0, VERTICAL), VCORD(v0, HORIZONTAL));
      e = ENXT(e);
    }
    fprintf(fresult, "\n");
    if (okPicts) {
#ifdef SHOWCOORDS
      fprintf(fpicts, "}");
#endif
      fprintf(fpicts, "\n\n");
    }
  }
}

int main(int argc, char *argv[]) {
  FILE *fresult, *fpictures;
  int nr, s;
  if (argc < 2) {
    printf("USO:  prog resultados (imagens)\n");
    exit(EXIT_FAILURE);
  }

  if ((fresult = fopen(argv[1], "w")) == NULL) {
    printf("ERRO NA ABERTURA DO FICHEIRO:  %s\n", argv[1]);
    exit(EXIT_FAILURE);
  }

  if (argc == 3) {
    if ((fpictures = fopen(argv[2], "w")) == NULL) {
      printf("ERRO NA ABERTURA DO FICHEIRO:  %s\n", argv[2]);
      exit(EXIT_FAILURE);
    }
    fprintf(fpictures, "\\documentclass{article}\n\\usepackage{a4wide}\n");
    fprintf(fpictures, "\\setlength{\\unitlength}{0,5cm}\n");
    fprintf(fpictures, "\\begin{document}\n\n\n");
  }

  srand((unsigned int)time(NULL));

  printf("Number of rectangles? Number of instances?\n");
  // performs  (nr-1)  cuts
  scanf("%d %d", &nr, &s);
  fprintf(fresult, "%d\n", s);
  argc = (argc == 3 ? 1 : 0);
  while (s) {
    s--;
    createRpart(nr, fresult, fpictures, argc);
    // fprintf(fresult,"\n \\newpage\n\n");
    if (argc)
      fprintf(fpictures, "\n \\newpage\n\n");
  }
  if (argc) {
    fprintf(fpictures, "\n\n\\end{document}\n");
    fclose(fpictures);
  }
  fclose(fresult);

  return 0;
}
