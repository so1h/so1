#include <so1pub.h\ll_s_so1.h>

int kill ( pid_t pid, int sig )
{
    asm
    {
        mov si,sig ;
        mov bx,pid ;
        mov ax,KILL ;
        int nVIntSO1 ;
    }
    return(_AX) ;
}