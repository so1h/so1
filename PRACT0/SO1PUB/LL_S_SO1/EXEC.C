#include <so1pub.h\ll_s_so1.h>

int exec ( const char far * nombre,
           const char far * comando )
{
    asm {
        les bx,nombre ;                                   /* ES:BX         */
        mov cx,es ;                                       /* CX:BX nombre  */
        les dx,comando ;                                  /* ES:DX comando */
        mov ax,EXEC ;
        int nVIntSO1 ;
    }
    return(_AX) ;
}