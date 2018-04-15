#include <so1pub.h\ll_s_so1.h>

int killpid ( int pid )    /* mata directamente sin enviar señal. ah = 0ch */
{
    asm
    {
        mov dx,pid ;
        mov ax,KILLPID ;
        int nVIntSO1 ;
    }
    return(_AX) ;
}