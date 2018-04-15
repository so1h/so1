#include <so1pub.h\ll_s_so1.h>

pid_t getpid ( void )
{
    asm
    {
        mov ax,GETPID ;
        int nVIntSO1 ;
    }
    return(_AX) ;
}