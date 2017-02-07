/***************************************************************************/
/*                               startbss.c                                */
/***************************************************************************/
/*    Funcion nula en el segmento BSS para determinar algun punto de       */
/*                ese segmento en el momento del enlazado                  */
/***************************************************************************/

#include <so1pub.h\startbss.h>

asm {
_BSS segment word public 'BSS'
 assume NOTHING
_startBSS proc near
  /* aqui no hay nada de nada */
_startBSS endp

public _startBSS

_BSS ends
}

