;/**************************************************************************/
;/*                              findata.asm                               */
;/**************************************************************************/
;/*    Funcion nula en el segmento de datos para determinar algun punto    */
;/*           del segmento de datos en el momento del enlazado             */
;/**************************************************************************/

_DATA segment word public 'DATA'

_finData proc near
  ; aqui no hay nada de nada */
_finData endp

public _finData

_DATA ends

END



