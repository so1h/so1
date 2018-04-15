#include <so1pub.h\ll_s_so1.h>

void yield ( void )
{
    asm
    {
        mov ax,YIELD ;
        int nVIntSO1 ;
    }
}