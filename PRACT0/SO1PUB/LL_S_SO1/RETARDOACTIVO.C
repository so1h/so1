#include <so1pub.h\ll_s_so1.h>

dword_t retardoActivo ( dword_t nVueltas ) 
{
    int df ;
	dword_t res = 0L ;                            /* resultado por defecto */
    retardarProceso_t retardarProceso ;
    switch (nVueltas) {
    case 0L :                                                        /* 0L */
        if ((df = open("RETARDO", O_RDONLY)) < 0) return(-1L) ;
        retardarProceso = (retardarProceso_t)1L ;              /* envio 1L */
        read(df, (pointer_t)&retardarProceso, 4) ;
        retardarProceso() ;                /* bucle de retardo establecido */
        break ;
    case 1L :                                                        /* 1L */
        if ((df = open("RETARDO", O_RDONLY)) < 0) return(-1L) ;
        res = 0L ;                                             /* envio 0L */
        read(df, (pointer_t)&res, 4) ;            /* res = nVueltasRetardo */ 
        break ;
    default :                                                     /* 2L .. */
        if ((df = open("RETARDO", O_WRONLY)) < 0) return(-1L) ;
        write(df, (pointer_t)&nVueltas, 4) ;       /* establecemos retardo */
        break ;
    }
    close(df) ;
    return(res) ;
}