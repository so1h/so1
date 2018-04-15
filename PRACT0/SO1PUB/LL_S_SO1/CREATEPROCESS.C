#include <so1pub.h\ll_s_so1.h>

pid_t createProcess ( const char far * nombre,
                      const char far * comando )
{
    asm
    {
        les bx,nombre ;                                   /* ES:BX         */
        mov cx,es ;                                       /* CX:BX nombre  */
        les dx,comando ;                                  /* ES:DX comando */
        mov ax,CREATEPROCESS ;
        int nVIntSO1 ;
    }
    return(_AX) ;                               /* DX pid del proceso hijo */
}