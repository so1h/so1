#include <so1pub.h\ll_s_so1.h>

dword_t retardoActivo ( dword_t nVueltas )
{
    int df ;
    dword_t res = (dword_t)0L ;
    retardarProceso_t retardarProceso ;
    dword_t nVueltasRetardo ;
    if ((df = open("RETARDO", O_RDONLY)) < 0) return((dword_t)-1L) ;
    switch (nVueltas) {
    case (dword_t)0L :
        *((pointer_t)&retardarProceso) = 0x01 ;
        read(df, (pointer_t)&retardarProceso, 4) ;
        retardarProceso() ;
        break ;
    case (dword_t)1L :
        *((pointer_t)&nVueltasRetardo) = 0x00 ;
        read(df, (pointer_t)&nVueltasRetardo, 4) ;
        res = nVueltasRetardo ;
        break ;
    default :
        close(df) ;
        df = open("RETARDO", O_WRONLY) ;
        nVueltasRetardo = nVueltas ;
        write(df, (pointer_t)&nVueltasRetardo, 4) ;
        break ;
    }
    close(df) ;
    return(res) ;
}