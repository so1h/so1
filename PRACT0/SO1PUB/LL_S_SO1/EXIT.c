#include <so1pub.h\ll_s_so1.h>

void exit ( int status )
{
    asm
    {
        mov bx,status ;
        mov ax,EXIT ;
        int nVIntSO1 ;
    }
}