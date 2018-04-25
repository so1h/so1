#include <so1pub.h\ll_s_so1.h>

bool_t ll_devolverBloque ( word_t segmento, word_t tam )
{
	bool_t res ;
    int df = open("GM", O_WRONLY) ;
    if (df < 0) return(FALSE) ;
	res = ioctl(df, 1, tam) ;                      /* 1 ==> devolverBloque */
	close(df) ;
    return(res) ; 	
}