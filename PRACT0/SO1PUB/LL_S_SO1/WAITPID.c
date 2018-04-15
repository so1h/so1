#include <so1pub.h\ll_s_so1.h>

pid_t waitpid ( pid_t pid, int far * statloc )
{
    asm
    {
        mov bx,pid ;
        les dx,statloc ;
        mov ax,WAITPID ;
        int nVIntSO1 ;
    }
    return(_AX) ;
}