#include <so1pub.h\ll_s_so1.h>

int open ( const char far * nombre, modoAp_t modoAp )
{
    asm
    {
        les bx,nombre ;
        mov dx,modoAp ;
        mov ax,OPEN ;
        int nVIntSO1 ;
    }
    return(_AX) ;                                                    /* df */
}