;/**************************************************************************/
;/*                              startbss.asm                              */
;/**************************************************************************/
;/*    Funcion nula en el segmento BSS para determinar algun punto de      */
;/*                ese segmento en el momento del enlazado                 */
;/**************************************************************************/

_BSS segment word public 'BSS'

_startBSS proc near
  ; aqui no hay nada de nada
_startBSS endp

public _startBSS

_BSS ends

END

