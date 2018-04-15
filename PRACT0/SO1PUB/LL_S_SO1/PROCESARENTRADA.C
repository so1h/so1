#include <so1pub.h\ll_s_so1.h>

int procesarEntrada ( ffblk_t * ffblk )                /* funcion auxiliar */
{
    entrada_t far * entrada = (entrada_t far *)ffblk->buffer ;
    int i = ffblk->bindx ;
    int j, k ;
    ffblk->ff_attrib = entrada[i].atr ;
    ffblk->ff_ftime = entrada[i].hora ;
    ffblk->ff_fdate = entrada[i].fecha ;
    ffblk->ff_fsize = entrada[i].tam ;
    j = 0 ;
    while ((j < 8) && (entrada[i].nombre[j] != ' '))
    {
        ffblk->ff_name[j] = entrada[i].nombre[j] ;
        j++ ;
    }
    if (entrada[i].ext[0] != ' ') {
        ffblk->ff_name[j] = '.' ;
        j++ ;
    }
    k = 0 ;
    while ((k < 3) && (entrada[i].ext[k] != ' '))
    {
        ffblk->ff_name[j] = entrada[i].ext[k] ;
        k++ ;
        j++ ;
    }
    ffblk->ff_name[j] = '\0' ;
    /* entrada[i].primerCluster ; */
    return(0) ;
}