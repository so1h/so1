#include <so1pub.h\ll_s_so1.h>

word_t ll_buscarBloque ( word_t tam )
{
	word_t segmento ;
    int df = open("GM", O_WRONLY) ;
    if (df < 0) return(0x0000) ;
	segmento = ioctl(df, 0, tam) ;                   /* 0 ==> buscarBloque */
	close(df) ;
    return(segmento) ; 	
}