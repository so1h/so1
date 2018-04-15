/* ------------------------------------------------------------------------*/
/*                                 progdb.c                                */
/* ----------------------------------------------------------------------- */
/*         programa de usuario con un menu para probar los mensajes        */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\ll_s_so1.h>    /* biblioteca de llamadas al sistema SO1 */
#include <so1pub.h\ll_s_msj.h>     /* send, receive, sendrec, notify, echo */
#include <so1pub.h\stdio.h>                    /* printf, getchar, putchar */

#include <so1pub.h\ctype.h>                                     /* tolower */
#include <so1pub.h\strings.h>                           /* strcpy, strcmpu */
#include <so1pub.h\scanner.h>                    /* inicScanner, obtenSimb */ 

mensaje_0_t respuesta ;

#define tamBufer 5120

byte_t bufer [ tamBufer ] ;

int main ( int argc, char * argv [ ] ) 
{
	int df ;
	word_t n ;
    int nbytes ;
	int i, j, k ;
	dword_t tam ;
	dword_t pos ;
	dword_t posDespues ;
	
	if (argc < 2) return(0) ;
	
	df = open(argv[1], O_RDONLY) ; 
	printf("\n\n open(\"%s\", O_RDONLY) = %i\n", argv[1], df) ;
	
	if (df > 0) 
	{
        k = 0 ;                              /* comando es una variable global */
        for ( i = 2 ; i < argc ; i ++ )
        {
            for ( j = 0 ; argv[i][j] != '\0' ; j++ )
                comando[0][k++] = toupper(argv[i][j]) ;
            comando[0][k++] = ' ' ;
        }
        comando[0][k--] = '\0' ;		
        inicScanner() ;
        saltarBlancos() ;
        numDec() ;
		if ((simb == s_numero) || (simb == s_numeroLargo)) 
		{
    		if (simb == s_numero) pos = num ;
    		else if (simb == s_numeroLargo) pos = numLargo ;
	    	else return(0) ;
            saltarBlancos() ;
            numDec() ;
    		if (simb != s_numero) return(0) ; 
            n = num ;			
			if (n > tamBufer) return(0) ;
			printf("\n pos = %li\n", pos) ;
   	        posDespues = lseek(df, pos, SEEK_SET) ;
         	printf("\n lseek(%i, %li, SEEK_SET) = %li\n", df, pos, posDespues) ;
    	    nbytes = read(df, (pointer_t)&bufer, n) ;
			posDespues = lseek(df, 0, SEEK_CUR) ;
			printf("\n posDespues = %li\n", posDespues) ;
   	        tam = lseek(df, 0, SEEK_END) ;
	        printf(
			    "\n read(%i, &bufer, %i) = %i   pos = %li tam = %li\n", 
			    df, n, nbytes, posDespues, tam
			) ;
	
            for ( i = 0 ; i < nbytes ; i++ ) 
	        {
                if ( i % 16 == 0) printf("\n %06lX: ", pos + i ) ;	   
                printf("%02X ", bufer[i]) ;	
	        }
	     	printf("\n") ;
		}
    }	
#if (FALSE)	
	aio_read(df, bufer, 512) ;
	close(df) ;

	printf("\n leer tecla ... ") ;
	getchar() ;
	receive(1, &respuesta) ;
	printf("\n\n respuesta = %s\n", respuesta.info) ;
#endif 

	return(0) ;
}