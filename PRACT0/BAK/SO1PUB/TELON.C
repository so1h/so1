/* ----------------------------------------------------------------------- */
/*                                  telon.c                                */
/* ----------------------------------------------------------------------- */
/*                                                                         */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\tipos.h>                                   /* byte_t */
#include <so1pub.h\def_pant.h>                              /* maxFilas */
#include <so1pub.h\pantalla.h> /* pantalla_t, copiarPantalla, atrNormal */
#include <so1pub.h\bios_0.h>   /* (read/goTo)XYBIOS, print(Str/Ptr)BIOS */ /* leerTeclaBIOS */
#include <so1pub.h\biosdata.h>                           /* ptrBiosArea */

static pantalla_t far * ptrPant ;

static byte_t cursorOriginalF ;
static byte_t cursorOriginalC ;
static byte_t lineaOriginal1 ;
static byte_t lineaOriginal2 ;

static byte_t VIDEO_mode ;                /* si != 0x07 => enColor == TRUE */

static pantalla_t far * copiaDeLaPantallaOriginal ;

static word_t segMemVideo ;           /* 0xB000 (mono) o 0xB800 (en color) */

void salvarPantallaInicial ( void ) {

  byte_t F, C ;

  readXYBIOS(&cursorOriginalF, &cursorOriginalC,
             &lineaOriginal1, &lineaOriginal2) ;
  VIDEO_mode = ptrBiosArea->VIDEO_mode ;
  if (VIDEO_mode != 0x07)
    segMemVideo = 0xB800 ;
  else
    segMemVideo = 0xB000 ;
  ptrPant = (pantalla_t far *)MK_FP(segMemVideo, 0x0000) ;

  /* se ha elegido guardar la copia de la pantalla original a continuacion */
  /* de la pantalla original (no es seguro que no de puede dar problemas). */

  copiaDeLaPantallaOriginal = (pantalla_t far *)(&ptrPant->t[maxFilas][0]) ;
  copiarPantalla(ptrPant, copiaDeLaPantallaOriginal, maxFilas) ;

  for ( F = 0 ; F < maxFilas ; F++ )                  /* ponemos atrNormal */
    for ( C = 0 ; C < maxColumnas ; C++ )
      ptrPant->t[F][C].atr = atrNormal ;

//printStrBIOS("\n ptrPant = ") ;
//printPtrBIOS(ptrPant) ;
//printStrBIOS("\n copiaDeLaPantallaOriginal = ") ;
//printPtrBIOS(copiaDeLaPantallaOriginal) ;
//leerTeclaBIOS() ;

}

void restaurarPantallaInicial ( void ) {
  copiarPantalla(copiaDeLaPantallaOriginal, ptrPant, maxFilas) ;
  goToXYBIOS(cursorOriginalF, cursorOriginalC) ;
}

