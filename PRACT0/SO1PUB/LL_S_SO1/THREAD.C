#include <so1pub.h\ll_s_so1.h>

pid_t thread ( void * (* funcion) (void * arg), word_t SP0, void * arg )
{
    asm
    {
        mov bx,funcion ;
        mov cx,SP0 ;
        mov dx,arg ;
        mov ax,THREAD ;
        int nVIntSO1 ;
    }
    return(_AX) ;
}