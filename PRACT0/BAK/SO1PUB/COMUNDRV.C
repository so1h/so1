/* ----------------------------------------------------------------------- */
/*                               comundrv.c                                */
/* ----------------------------------------------------------------------- */
/*                  fragmento comun a todos los drivers                    */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\ll_s_so1.h>    /* biblioteca de llamadas al sistema SO1 */
#include <so1pub.h\def_proc.h>        /* descProceso_t, e2PFR_t, c2c_t ... */
#include <so1pub.h\stdio.h>                                      /* printf */

descProceso_t descProceso [ maxProcesos ] ;

e2PFR_t e2PFR ;

c2c_t c2cPFR [ numColasPFR ] ;

int comprobarAmpersand ( void )   /* mira si hay un & al final del comando */
{
    int i ;
    pindx_t pindx ;
    char * comando ;

    obtenInfoPS(
	    (descProceso_t far *)&descProceso, 
		(e2PFR_t far *)&e2PFR, 
		(c2c_t far *)&c2cPFR
	) ;

	pindx = getpindx() ;
	comando = (char *)&descProceso[pindx].comando ;
	i = 0 ;
	while (comando[i] != '\0') i++ ;
    if (comando[i+1] != '&')    /* truco: crearProceso deja ahi &, <, >, | */
    {
        printf("\n\n este driver debe ejecutarse con & \n") ;
        return(-1) ;
    }
    return(0) ;
}