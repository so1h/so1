#include <so1pub.h\ll_s_so1.h>

dfs_t crearFichero ( const char far * nombre,
                     rindx_t          rindx,
                     word_t           menor,
                     tipoFichero_t    tipo )
{
    asm
    {
        les bx,nombre ;
        mov cx,rindx ;
        mov dx,menor ;
        mov si,tipo ;
        mov ax,CREARFICHERO ;
        int nVIntSO1 ;
    }
    return(_AX) ;
}

