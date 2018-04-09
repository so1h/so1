/* ------------------------------------------------------------------------*/
/*                                  menum.c                                */
/* ----------------------------------------------------------------------- */
/*         programa de usuario con un menu para probar los mensajes        */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\ll_s_so1.h>    /* biblioteca de llamadas al sistema SO1 */
#include <so1pub.h\ll_s_msj.h>     /* send, receive, sendrec, notify, echo */
#include <so1pub.h\stdio.h>                    /* printf, getchar, putchar */
#include <so1pub.h\ctype.h>                                     /* tolower */
#include <so1pub.h\strings.h>                                    /* strcpy */

/* leerPidx: lee un pindx (ANY) de la entrada estandar */

leerPindx ( pindx_t * pindx, bool_t valeANY, word_t max ) 
{
	int acum = 0 ;
    char car ;
	
    for ( ; ; ) 
	{
        car = getchar() ; putchar(car) ;                    /* lee con eco */
		if (('0' <= car) && (car <= '9')) 
			acum = 10*acum + (car - '0') ;
		else if (valeANY && (tolower(car) == 'a') && (acum == 0)) 
		{   *pindx = ANY ; 
	        return ;
		}
		else if ((car != '\n') || (acum >= max)) 
		{ 
             putchar('\a') ;
             *pindx = (pindx_t)-1 ;			                      /* error */
		     return ;                                       
		} 
		else                                   
		{   *pindx = acum ;                                /* pindx normal */
	        return ;
		}
    } 
}	

#define leerNum(n) leerPindx((pindx_t *)n, FALSE, 1000) 

#define goToFC(F, C) lseek(STDOUT, F*80+C, SEEK_SET) 

void leerStr ( char * str )       /* leer un string de la entrada estandar */
{
    int i = 0 ;
    char car = ' ' ;
    while (car != '\n') 
	{
        car = getchar() ;
        if ((' ' <= car) && (car <= '~') && (i < (tamMensaje-5))) {
            str[i] = car ;
            putchar(car) ;                                          /* eco */
            i++ ;
        }
        else putchar('\a') ;
    }
    str[i] = '\0' ;
}

mensaje_0_t mensaje = {       /* estructura para enviar/recibir un mensaje */

    0x0000,                                            /* mensaje.pindxOrg */ 
	0x0000,                                            /* mensaje.tipo     */
	"_________________________"                        /* mensaje.info     */
	
} ;

int main ( void ) 
{
	char car ;
	char opcion ;
    pindx_t pindx = 0 ;
    char strPindx [ 5 ] ;
    sprintf(strPindx, "%i", pindx) ; 
  
    for ( ; ; ) 
	{
        putchar('\f') ;                /* borra el contenido de la ventana */
        printf(
//          ""                                                           "\n"
            ""                                                           "\n"
            "  MENUM (c) Pedro Pablo Lopez Rodriguez (05-04-2018) [bn1002]\n"
            ""                                                           "\n"
            "  SO1: elegir operacion:                  (pindx MENUM = %i) \n"
            ""                                                           "\n"
            "  ('p') - pindx             = %s"                           "\n"
	        ""                                                           "\n"
            "  ('o') - mensaje.pindxOrg  = %i"                           "\n"
	        ""                                                           "\n"
            "  ('t') - mensaje.tipo      = %i"                           "\n"
            ""                                                           "\n"
            "  ('i') - mensaje.info      = \"%s\""                       "\n"
            ""                                                           "\n",
	        getpindx(), strPindx, 
			mensaje.pindxOrg, mensaje.tipo, mensaje.info
		) ;
		if (pindx != ANY) printf(			
            "  ('s') - send    (%s, &mensaje)"                           "\n"
            ""                                                           "\n",
			strPindx
		) ;
		printf(	
            "  ('r') - receive (%s, &mensaje)"                           "\n"
            ""                                                           "\n",
			strPindx
		) ;
		if (pindx != ANY) printf(					
            "  ('c') - sendrec (%s, &mensaje)"                           "\n"
            ""                                                           "\n"
            "  ('n') - notify  (%s)"                                     "\n"
            ""                                                           "\n",
			strPindx, strPindx
		) ; 
        printf(		
            "  ('x') - salir del programa"                               "\n"
            ""                                                           "\n"
        ) ;

        goToFC(3, 25) ;
		
        opcion = tolower(getchar()) ; putchar(opcion) ;
		
		if ((pindx == ANY) && 
		    ((opcion == 's') || (opcion == 'c') || (opcion == 'n'))) 
		{	
		    putchar('\a') ;
			continue ;
		}
		
        switch (opcion) 
		{		
        case 'p' :                          /* leer indice del proceso con */
			goToFC(5, 30) ; printf("   \b\b\b") ;    /* el que comunicarse */ 
            leerPindx(&pindx, TRUE, maxProcesos) ;              /* (pindx) */
            if (pindx == ANY) 
			{   
		        sprintf(strPindx, "ANY") ;
				break ;
			}
	        else if (pindx < 0) pindx = 0 ; 
			sprintf(strPindx, "%i", pindx) ;
            break ;
        case 'o' :                                /* leer mensaje.pindxOrg */
			goToFC(7, 30) ; printf("   \b\b\b") ; 
            leerPindx(&mensaje.pindxOrg, TRUE, maxProcesos) ;
            break ;
        case 't' :                                    /* leer mensaje.tipo */
			goToFC(9, 30) ; printf("   \b\b\b") ; 
            leerNum(&mensaje.tipo) ;
            break ;
        case 'i' :                                    /* leer mensaje.info */
			goToFC(11, 31) ; printf("%30s", " ") ; goToFC(11, 31) ;  
			leerStr(mensaje.info) ;
            break ;
        case 's' :                                                 /* send */
			goToFC(13, 33) ; printf("  esperando ") ; 
            send(pindx, &mensaje) ;
            break ;
        case 'r' :                                              /* receive */
			if (pindx != ANY) goToFC(15, 33) ; else goToFC(13, 33) ; 
			printf("  esperando ") ;
            receive(pindx, &mensaje) ;
            break ;
        case 'c' :                                              /* sendrec */
			goToFC(17, 33) ; printf("  esperando ") ;
            sendrec(pindx, &mensaje) ;
            break ;
        case 'n' :                                               /* notify */
			goToFC(19, 33) ; printf("  esperando ") ;
            notify(pindx) ;
            break ;
        case 'x' :                                     /* fin del programa */
			goToFC(22, 0) ;		
			return(0) ;                          
        default :                                        /* opcion erronea */
            putchar('\a') ;
        }
    }
}