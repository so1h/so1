/* ----------------------------------------------------------------------- */
/*                                printgen.c                               */
/* ----------------------------------------------------------------------- */
/*                                                                         */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\printgen.h>                               /* printCar_t */

static char dig [17] = "0123456789ABCDEF" ;

int printGenLn ( printCar_t p ) 
{
    p('\r') ;                                 /* retorno de carro (char)13 */
    p('\n') ;                                 /* salto de linea   (char)10 */
    return(2) ;
}

int printGenStr ( const char far * str, printCar_t p ) 
{
    char car ;
    int cont = 0 ;
    while ((car = *str++) != '\0') 
	{
        if (car == '\n') 
		{ 
	        p('\r') ; 
			cont++ ; 
		}
        p(car) ; 
		cont++ ;
    }
    return(cont) ;
}

int printGenStrHasta ( const char far * str, word_t n, bool_t lleno, printCar_t p ) {
    char car ;
    word_t i = 0 ;
    while ((i < n) && ((car = *str++) != '\0')) 
	{
        if (car == '\n') 
			p('\r') ;
        p(car) ;
        i++ ;
    }
    if (lleno)
        while (i < n) 
		{
            p(' ') ;
            i++ ;
        }
    return(i) ;
}

#define tamStrMax 11

static char str[tamStrMax] = "" ;

#define codigoPrintGenDec(tamStr, tamStrM1, tamStrM2)                       \
{                                                                           \
/*  char str[tamStr] ; */                                                   \
    word_t i, j ;                                                           \
    int cont = 0 ;                                                          \
    char * ptr ;                                                            \
    if (l == 0) l = 1 ;                                                     \
    if (num == 0)                                                           \
	{                                                                       \
        for ( i = 0 ; i < l-1 ; i++ ) p(' ') ;                              \
        p('0') ;                                                            \
        return(l) ;                                                         \
    }                                                                       \
    ptr = (char *)&str[tamStrM1] ;                                          \
    *ptr = '\0' ;                                                           \
    i = 0 ;                                                                 \
    while (num > 0)                                                         \
	{                                                                       \
        str[tamStrM2-i] = dig[(word_t)(num%10)] ;                           \
        num = num/10 ;                                                      \
        i++ ;                                                               \
    }                                                                       \
    for ( j = i ; j < l ; j++ ) p(' ') ;                                    \
    ptr = (char *)&str[tamStrM2-i+1] ;                                      \
    while (*ptr != '\0')                                                    \
	{                                                                       \
        p(*ptr) ;                                                           \
        ptr++ ;                                                             \
        cont++ ;                                                            \
    }                                                                       \
    return(l + cont) ;                                                      \
}

int printGenDec ( word_t num, word_t l, printCar_t p ) 
{
    codigoPrintGenDec(6, 5, 4)
}

int printGenLDec ( dword_t num, word_t l, printCar_t p ) 
{
    codigoPrintGenDec(11, 10, 9)
}

#define codigoPrintGenInt(tamStr, tamStrM1, tamStrM2)                       \
{                                                                           \
/*  char str[tamStr] ; */                                                   \
    word_t i, j ;                                                           \
    char * ptr ;                                                            \
    bool_t negativo = (bool_t)(num < 0) ;                               /**/\
    if (l == 0) l = 1 ;                                                     \
    if (num == 0)                                                           \
	{                                                                       \
        for ( i = 0 ; i < l-1 ; i++ ) p(' ') ;                              \
        p('0') ;                                                            \
        return(0) ;                                                         \
    }                                                                       \
    if (num < 0) num = -num ;                                           /**/\
    ptr = (char *)&str[tamStrM1] ;                                          \
    *ptr = '\0' ;                                                           \
    i = 0 ;                                                                 \
    while (num > 0)                                                         \
	{                                                                       \
        str[tamStrM2-i] = dig[(word_t)(num%10)] ;                           \
        num = num/10 ;                                                      \
        i++ ;                                                               \
    }                                                                       \
    if (negativo)                                                       /**/\
	{                                                                   /**/\
        for ( j = i ; j < l-1 ; j++ ) p(' ') ;                          /**/\
        p('-') ;                                                        /**/\
    }                                                                   /**/\
    else                                                                /**/\
        for ( j = i ; j < l ; j++ ) p(' ') ;                                \
    ptr = (char *)&str[tamStrM2-i+1] ;                                      \
    while (*ptr != '\0')                                                    \
	{                                                                       \
        p(*ptr) ;                                                           \
        ptr++ ;                                                             \
    }                                                                       \
    return(0) ;                                                             \
}

int printGenInt ( int num, word_t l, printCar_t p ) 
{
  codigoPrintGenInt(6, 5, 4)
}

int printGenLInt ( long int num, word_t l, printCar_t p ) 
{
  codigoPrintGenInt(11, 10, 9)
}

int printGenHex ( word_t num, word_t l, printCar_t p ) 
{
/*  char str[5] ; */
    word_t i, j ;
    int cont = 0 ;
    char * ptr ;
    if (l == 0) l = 1 ;
    if (num == 0) 
	{
        for ( i = 0 ; i < l-1 ; i++ ) p('0') ;
        p('0') ;
        return(l) ;
    }
    ptr = &str[4] ;
    *ptr = '\0' ;
    i = 0 ;
    while (num > 0) 
	{
        str[3-i] = dig[num%16] ;
        num = num/16 ;
        i++ ;
    }
    for ( j = i ; j < l ; j++ ) p('0') ;
    ptr = &str[3-i+1] ;
    while (*ptr != '\0') 
	{
        p(*ptr) ;
        ptr++ ;
        cont++ ;
    }
    return(l + cont) ;
}

int printGenLHex ( dword_t num, word_t l, printCar_t p ) 
{
/*  char str[9] ; */
    char * ptr ;
    word_t far * ptrWord ;
    word_t i, j ;
    if (l == 0) l = 1 ;
    if (num == 0) 
	{
        for ( i = 0 ; i < l-1 ; i++ ) p('0') ;
        p('0') ;
        return(l) ;
    }
    str[8] = '\0' ;
    i = 0 ;
    ptrWord = (word_t far *)&num ;
    while (num > 0) 
	{
        str[7-i] = dig[ptrWord[0]%16] ;
        ptrWord[0] = ptrWord[0]/16 + 0x1000*(ptrWord[1]%16) ;
        ptrWord[1] = ptrWord[1]/16 ;
        i++ ;
    }
    for ( j = i ; j < l ; j++ ) p('0') ;
    ptr = &str[7-i+1] ;

    while (i > 0) 
	{	
        p(*ptr) ;
        ptr++ ;
        i-- ;
    }
    return(0) ;
}

#define codigoPrintGenBin(es16o32)                                           \
{                                                                            \
    word_t i, j ;                                                            \
    word_t pot ;                                                             \
    if (l == 0) l = 1 ;                                                      \
    if (num == 0)                                                            \
	{                                                                        \
        for ( i = 0 ; i < l-1 ; i++ ) p('0') ;                               \
        p('0') ;                                                             \
        return(0) ;                                                          \
    }                                                                        \
    i = es16o32 ;                                                            \
    pot = 1 << (i-1) ;                                                       \
    while ((pot & num) == 0)                                                 \
	{                                                                        \
        pot = (pot >> 1) ;                                                   \
        i-- ;                                                                \
    }                                                                        \
    if (i > l) l = i ;                                                       \
    else pot = (1 << (l-1)) ;                                                \
    for ( j = 0 ; j < l ; j++ )                                              \
	{                                                                        \
	    p('0' + ((pot & num) != 0)) ;                                        \
        pot = (pot >> 1) ;                                                   \
    }                                                                        \
    return(0) ;                                                              \
}

int printGenBin ( word_t num, word_t l, printCar_t p ) 
{
  codigoPrintGenBin(16) 
}

int printGenLBin ( dword_t num, word_t l, printCar_t p ) 
{
  codigoPrintGenBin(32) 
}

int printGenPtr ( pointer_t ptr, printCar_t p ) 
{
    printGenHex(FP_SEG(ptr), 4, p) ; p(':') ;
    printGenHex(FP_OFF(ptr), 4, p) ;
    return(9) ;
}