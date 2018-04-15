#include <so1pub.h\ll_s_so1.h>

int close ( int df )
{
    asm
    {
        mov bx,df ;
        mov ax,CLOSE ;
        int nVIntSO1 ;
    }
    return(_AX) ;
}