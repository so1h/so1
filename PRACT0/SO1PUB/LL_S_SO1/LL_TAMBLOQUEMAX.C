#include <so1pub.h\ll_s_so1.h>

word_t ll_tamBloqueMax ( void )
{
	word_t tamBloqueMax ;
    int df = open("GM", O_WRONLY) ;
    if (df < 0) return(0x0000) ;
	tamBloqueMax = ioctl(df, 2, 0) ;
    close(df) ;	
    return(tamBloqueMax) ;                           /* 2 ==> tamBloqueMax */
}