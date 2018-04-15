#include <so1pub.h\ll_s_so1.h>

int destruirRecurso ( char far * nombre, bool_t matando )
{
    asm {
        les bx,nombre ;
        mov cx,matando ;
        mov ax,DESTRUIRRECURSO ;
        int nVIntSO1 ;
    }
    return(_AX) ;
}