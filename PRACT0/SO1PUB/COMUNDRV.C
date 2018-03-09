/* ----------------------------------------------------------------------- */
/*                               comundrv.c                                */
/* ----------------------------------------------------------------------- */
/*                  fragmento comun a todos los drivers                    */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\ll_s_so1.h>    /* biblioteca de llamadas al sistema SO1 */
#include <so1pub.h\def_proc.h>        /* descProceso_t, e2PFR_t, c2c_t ... */
#include <so1pub.h\stdio.h>                                      /* printf */
#include <so1pub.h\scanner.h>       /* inicScanner, car, obtenCar, comando */
#include <so1pub.h\memory.h>                                   /* memcpy_n */
#include <so1pub.h\comundrv.h>                 

descProceso_t descProceso [ maxProcesos ] ;

e2PFR_t e2PFR ;

c2c_t c2cPFR [ numColasPFR ] ;

int comprobarAmpersand ( void )   /* mira si hay un & al final del comando */
{
    int i ;
    obtenInfoPS(
	    (descProceso_t far *)&descProceso, 
		(e2PFR_t far *)&e2PFR, 
		(c2c_t far *)&c2cPFR
	) ;

//  for ( i = 0 ; i < tamComando ; i++ )
//      comando[0][i] = descProceso[getpindx()].comando[i] ;
	memcpy_n(&comando[0], &descProceso[getpindx()].comando, tamComando) ;	

    inicScanner() ;
    while (car != '\0') obtenCar() ;
    if (comando[0][pos] != '&')   /* truco: crearProceso deja ahi &, <, >, | */
    {
        printf("\n\n este driver debe ejecutarse con & \n") ;
        return(-1) ;
    }
    return(0) ;
}