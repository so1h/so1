#include <so1pub.h\ll_s_so1.h>

pid_t getppid ( void )
{
    asm
    {
        mov ax,GETPPID ;
        int nVIntSO1 ;
    }
    return(_AX) ;
}