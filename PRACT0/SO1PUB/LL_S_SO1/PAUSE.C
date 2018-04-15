#include <so1pub.h\ll_s_so1.h>

void pause ( void )
{
    asm
    {
        mov ax,PAUSE ;
        int nVIntSO1 ;
    }
}