/***************************************************************************/
/*                               findata.c                                 */
/***************************************************************************/
/*    Funcion nula en el segmento de datos para determinar algun punto     */
/*           del segmento de datos en el momento del enlazado              */
/***************************************************************************/

#include <so1pub.h\findata.h>

asm {
 assume NOTHING
_finData proc near
  /* aqui no hay nada de nada */
_finData endp

public _finData
}


