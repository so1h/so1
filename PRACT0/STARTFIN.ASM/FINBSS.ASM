;/**************************************************************************/
;/*                               finbss.asm                               */
;/**************************************************************************/
;/*    Funcion nula en el segmento BSS para determinar algun punto de      */
;/*                ese segmento en el momento del enlazado                 */
;/**************************************************************************/

_BSS segment word public 'BSS'

_finBSS proc near
  ; aqui no hay nada de nada
_finBSS endp

public _finBSS

_BSS ends

END

