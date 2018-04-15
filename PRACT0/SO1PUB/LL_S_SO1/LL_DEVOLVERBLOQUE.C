#include <so1pub.h\ll_s_so1.h>

bool_t ll_devolverBloque ( word_t segmento, word_t tam )
{
    word_t w [2] ;
    int dfGM = open("GM", O_RDONLY) ;
    if (dfGM < 0) return(FALSE) ;
    w[0] = segmento ;
    w[1] = tam ;
    return(read(dfGM, (pointer_t)&w, 4) == 4) ;
}