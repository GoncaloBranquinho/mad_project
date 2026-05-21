rectParts.c

   * cria partições retangulares com nr retângulos

   * permite gerar várias instâncias com nr retângulos
    (sendo o número de instâncias definido no input)

   * depende de types.h e macros.h

   * compilar
       gcc rectParts.c

   * executar

    ./a.out res resFigs.tex

    produz um ficheiro res com os exemplos e resFigs.tex com figuras para LaTeX
    (resFigs.tex opcional)
    
    Para ver as figuras, executar
    
    pdflatex resFigs.tex
    
    que produzirá um ficheiro resFigs.pdf com as figuras

  * diferentes opções de compilação
  
    definidas por constantes PICTSTEPBYSTEP, FACELABELS, e SHOWCOORDS
    e ainda UNIFORM e GEOMETRIC  (preferencialmente, deve estar GEOMETRIC ativa)


//-------------   Options for compilation -----------------------
// --- output image
//#define PICTSTEPBYSTEP
//---  output LaTeX pictures

#define FACELABELS
//-- label faces from 1 to nr
//#define SHOWCOORDS
//-- show coordinates of the vertices

//uniform or geometric distribution for random choices
//#define UNIFORM
//  (maybe not very interesting)

#define GEOMETRIC
//#define Tau 0.7
#define Tau 0.75
//----------------------------------------------------
