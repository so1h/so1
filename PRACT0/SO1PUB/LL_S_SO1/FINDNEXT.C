#include <so1pub.h\ll_s_so1.h>

int findNext ( ffblk_t * ffblk )
{
    entrada_t far * entrada = (entrada_t far *)ffblk->buffer ;
    int res ;
    int i ;
    for ( i = ++ffblk->bindx ; i < 16 ; i++ )
        if ((entrada[i].nombre[0] != 0x00) && (entrada[i].nombre[0] != 0xE5))
            break ;
    ffblk->bindx = i ;
    if (ffblk->bindx < 16)
    {
        procesarEntrada(ffblk) ;
        return(0) ;
    }
    else
    {
        ffblk->eindx += 16 ;
        asm
        {
            mov dx,ffblk ;
            mov ax,FINDNEXT ;
            int nVIntSO1 ;                             /* ffblk->bindx = 0 */
            mov res,ax ;
        }
        if (res == 0)
            procesarEntrada(ffblk) ;
        return(res) ;
    }
}