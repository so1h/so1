#include <so1pub.h\ll_s_so1.h>

pindx_t getpindx ( void )
{
    asm
    {
        mov ax,GETPINDX ;
        int nVIntSO1 ;
    }
    return(_AX) ;
}