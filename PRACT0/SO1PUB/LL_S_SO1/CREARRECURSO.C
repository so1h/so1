#include <so1pub.h\ll_s_so1.h>

rindx_t crearRecurso ( descRecurso_t far * dR )
{
    asm
    {
        les bx,dR ;
        mov ax,CREARRECURSO ;
        int nVIntSO1 ;
    }
    return(_AX) ;
}
