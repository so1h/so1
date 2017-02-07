/***************************************************************************/
/*                               fincode.h                                 */
/***************************************************************************/
/*    Funcion nula en el segmento de codigo para determinar algun punto    */
/*                 del codigo en el momento del enlazado                   */
/***************************************************************************/

#include <so1pub.h\fincode.h>

asm {
_TEXT segment byte public 'CODE'
_finCode proc near
  /* aqui no hay nada de nada */
_finCode endp
public _finCode
_TEXT ends
}

