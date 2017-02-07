;/**************************************************************************/
;/*                              startcode.asm                             */
;/**************************************************************************/
;/*    Funcion nula en el segmento de codigo para determinar algun punto   */
;/*                 del codigo en el momento del enlazado                  */
;/**************************************************************************/

_TEXT segment byte public 'CODE'

_startCode proc near
  ; aqui no hay nada de nada
_startCode endp

public _startCode

_TEXT ends

END

