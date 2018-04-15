#include <so1pub.h\ll_s_so1.h>

int ioctl ( int df, word_t cmd, word_t arg  )
{
    asm {
        mov bx,df ;
        mov cx,cmd ;
        mov dx,arg ;
        mov ax,IOCTL ;
        int nVIntSO1 ;
    }
    return(_AX) ;
}