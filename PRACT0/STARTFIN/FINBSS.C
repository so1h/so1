/***************************************************************************/
/*                                finbss.c                                 */
/***************************************************************************/
/*    Funcion nula en el segmento BSS para determinar algun punto de       */
/*                ese segmento en el momento del enlazado                  */
/***************************************************************************/

#include <so1pub.h\finbss.h>

asm {
_BSS segment word public 'BSS'
 assume NOTHING
//_finBSS: ;
_finBSS proc near
  /* aqui no hay nada de nada */
_finBSS endp
public _finBSS
_BSS ends
}
