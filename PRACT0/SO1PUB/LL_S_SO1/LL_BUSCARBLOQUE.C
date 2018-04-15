#include <so1pub.h\ll_s_so1.h>

word_t ll_buscarBloque ( word_t tam )
{
    word_t w = tam ;
    int dfGM = open("GM", O_RDONLY) ;
    if (dfGM < 0) return(0x0000) ;
    read(dfGM, (pointer_t)&w, 2) ;
    return(w) ;
}