;/**************************************************************************/
;/*                                start.asm                               */
;/**************************************************************************/

_TEXT segment byte public 'CODE'

_startCode proc near
;/* aqui no hay nada de nada */
_startCode endp

public _startCode

_TEXT ends

;/**************************************************************************/

_DATA segment word public 'DATA'

_startData proc near
;/* aqui no hay nada de nada */
_startData endp

public _startData

_DATA ends

;/**************************************************************************/

_BSS segment word public 'BSS'

_startBSS proc near
;/* aqui no hay nada de nada */
_startBSS endp

public _startBSS

_BSS ends

END

