#include <so1pub.h\ll_s_so1.h>

word_t ll_tamBloqueMax ( void )
{
    word_t tam ;
    int dfGM = open("GM", O_RDONLY) ;
    if (dfGM < 0) return(0x0000) ;
    aio_read(dfGM, (pointer_t)&tam, 2) ;
    return(tam) ;
}