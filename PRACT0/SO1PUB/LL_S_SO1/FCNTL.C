#include <so1pub.h\ll_s_so1.h>

int fcntl ( int df, word_t cmd, word_t arg  )
{
    asm
    {
        mov bx,df ;
        mov cx,cmd ;
        mov dx,arg ;
        mov ax,FCNTL ;
        int nVIntSO1 ;
    }
    return(_AX) ;
}