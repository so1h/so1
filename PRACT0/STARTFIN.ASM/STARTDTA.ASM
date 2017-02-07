;/**************************************************************************/
;/*                              startdata.asm                             */
;/**************************************************************************/
;/*    Funcion nula en el segmento de datos para determinar algun punto    */
;/*           del segmento de datos en el momento del enlazado             */
;/**************************************************************************/

_DATA segment word public 'DATA'

_startData proc near
  ; aqui no hay nada de nada */
_startData endp

public _startData

_DATA ends

END



