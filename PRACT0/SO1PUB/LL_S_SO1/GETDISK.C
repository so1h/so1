#include <so1pub.h\ll_s_so1.h>

int getdisk ( void )
{
    asm {
        mov ax,GETDISK ;
        int nVIntSO1 ;
    }
    return(_AX) ;                                          /* unidadLogica */
}