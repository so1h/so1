#include <so1pub.h\ll_s_so1.h>

void analizarProcesos ( bool_t Ok )
{
    asm {
        mov ax,Ok ;          /* pasamos a SO1 el parametro Ok en el flag Z */
        and ax,ax ;                                 /* activamos los flags */
        mov ax,ANALIZARPROCESOS ;
        int nVIntSO1 ;
    }
}