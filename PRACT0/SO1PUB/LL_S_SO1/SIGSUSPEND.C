#include <so1pub.h\ll_s_so1.h>

#pragma option -w-par

void sigsuspend ( int sig )
{
    asm
    {
        mov ax,SIGSUSPEND ;
        int nVIntSO1 ;
    }
}