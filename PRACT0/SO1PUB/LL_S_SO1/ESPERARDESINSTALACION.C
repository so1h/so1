#include <so1pub.h\ll_s_so1.h>

int esperarDesinstalacion ( word_t tamDATA, ... )
{
    if (tamDATA != 0x0000)
    {
        word_t finCodeDriver ;
        word_t finishDriver ;
        word_t tamPila ;
        finCodeDriver = *((word_t far *)((pointer_t)&tamDATA + 1*sizeof(word_t))) ;
        finishDriver  = *((word_t far *)((pointer_t)&tamDATA + 2*sizeof(word_t))) ;
        tamPila       = *((word_t far *)((pointer_t)&tamDATA + 3*sizeof(word_t))) ;
        asm
        {
            mov cx,finCodeDriver ;
            mov dx,finishDriver ;
            mov si,tamPila ;
        }
    }
    asm
    {
        mov bx,tamDATA ;
        mov ax,ESPERARDESINSTALACION ;
        int nVIntSO1 ;
    }
    return(_AX) ;
}