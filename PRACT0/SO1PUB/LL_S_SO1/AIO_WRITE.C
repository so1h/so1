#include <so1pub.h\ll_s_so1.h>

int aio_write ( int df, pointer_t dir, word_t nbytes )
{
    asm
    {
        mov bx,df ;
        les dx,dir ;
        mov cx,nbytes ;
        mov ax,AIO_WRITE ;
        int nVIntSO1 ;
    }
    return(_AX) ;
}