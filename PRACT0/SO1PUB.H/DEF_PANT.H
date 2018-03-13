/* ----------------------------------------------------------------------- */
/*                               def_pant.h                                */
/* ----------------------------------------------------------------------- */
/*                                                                         */
/* ----------------------------------------------------------------------- */

#ifndef DEF_PANT_H
#define DEF_PANT_H

#include <so1pub.h\tipos.h>                                      /* byte_t */
#include <so1pub.h\biosdata.h>               /* ptrBiosArea->VIDEO_lastrow */

#define maxFilasAct (ptrBiosArea->VIDEO_lastrow + 1)         /* 25, 28, 50 */

#define maxFilas 50             

#define maxColumnasAct (ptrBiosArea->VIDEO_width)                    /* 80 */

#define maxColumnas 80                           

#define atrNormal 0x07                      /* Atributo normal de pantalla */
                                            /*   ((negro << 4) + blanco)   */
typedef struct {
    byte_t car ;                                         /* caracter ascii */
    byte_t atr ;                                               /* atributo */
} pos_t ;                     /* una posicion de la pantalla en modo texto */

typedef struct {
  pos_t t [ maxFilas ] [ maxColumnas ] ;                          /* 50x80 */
} pantalla_t ;                                   /* pantalla en modo texto */

typedef struct {
  pantalla_t far * pantalla ;
  byte_t F ;
  byte_t C ;  
} display_t ;  

#endif /* DEF_PANT_H */