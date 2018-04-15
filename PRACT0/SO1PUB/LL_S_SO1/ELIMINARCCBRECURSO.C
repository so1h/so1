#include <so1pub.h\ll_s_so1.h>

int eliminarCcbRecurso ( callBack_t cb, const char far * nombre )
{
    asm {
        les bx,nombre ;                                   /* ES:BX         */
        mov cx,es ;                                       /* CX:BX nombre  */
        les dx,cb ;                                       /* ES:DX cb      */
        mov ax,ELIMINARCCBRECURSO ;
        int nVIntSO1 ;
    }
    return(_AX) ;
}