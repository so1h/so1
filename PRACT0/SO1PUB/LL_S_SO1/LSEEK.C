#include <so1pub.h\ll_s_so1.h>

long lseek ( int df, long pos, word_t whence  )
{
    asm {
        mov bx,df ;
        les dx,pos ;
        mov cx,whence ;
        mov ax,LSEEK ;
        int nVIntSO1 ;
    }
    return((((long)_BX) << 16) | _AX) ;
}