#include <so1pub.h\ll_s_so1.h>

int aio_read ( int df, pointer_t dir, word_t nbytes )
{
    asm {
        mov bx,df ;
        les dx,dir ;
        mov cx,nbytes ;
        mov ax,AIO_READ ;
        int nVIntSO1 ;
    }
    return(_AX) ;
}