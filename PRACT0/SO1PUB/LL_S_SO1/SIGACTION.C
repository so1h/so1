#include <so1pub.h\ll_s_so1.h>

int sigaction ( int sig, const struct sigaction * act, struct sigaction * oact )
{
    asm
    {
        mov si,sig ;
        mov bx,act ;
        mov dx,oact ;
        mov ax,SIGACTION ;
        int nVIntSO1 ;
    }
    return(_AX) ;
}