#include <so1pub.h\ll_s_so1.h>

pid_t getgid ( void )
{
    asm
    {
        mov ax,GETGID ;
        int nVIntSO1 ;
    }
    return(_AX) ;
}