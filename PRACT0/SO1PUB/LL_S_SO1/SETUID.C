#include <so1pub.h\ll_s_so1.h>

int setuid ( uid_t uid )
{
    asm
    {
        mov bx,uid ;
        mov ax,SETUID ;
        int nVIntSO1 ;
    }
    return(0) ;
}