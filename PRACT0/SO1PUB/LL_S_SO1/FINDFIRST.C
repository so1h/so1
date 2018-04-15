#include <so1pub.h\ll_s_so1.h>

int findFirst ( byte_t unidadLogica, ffblk_t * ffblk )
{
    int res ;
    asm
    {
        mov bl,unidadLogica ;
        mov dx,ffblk ;
        mov ax,FINDFIRST ;
        int nVIntSO1 ;                                 /* ffblk->bindx = 0 */
        mov res,ax ;
    }
    if (res == 0)
        procesarEntrada(ffblk) ;
    return(res) ;
}