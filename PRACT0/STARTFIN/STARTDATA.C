/***************************************************************************/
/*                              startdata.c                                */
/***************************************************************************/
/*    Funcion nula en el segmento de datos para determinar algun punto     */
/*           del segmento de datos en el momento del enlazado              */
/***************************************************************************/

#include <so1pub.h\startdata.h>

asm {
 assume NOTHING
_startData proc near
  /* aqui no hay nada de nada */
_startData endp

public _startData
}


