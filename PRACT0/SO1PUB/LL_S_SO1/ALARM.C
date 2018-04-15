#include <so1pub.h\ll_s_so1.h>

unsigned int alarm ( unsigned int seconds )
{
    asm
    {
        mov cx,seconds ;
        mov ax,ALARM ;
        int nVIntSO1 ;
    }
    return(_AX) ;
}