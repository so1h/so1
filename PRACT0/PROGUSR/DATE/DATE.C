/* ----------------------------------------------------------------------- */
/*                                  date.c                                 */
/* ----------------------------------------------------------------------- */
/*                   comando que muestra la fecha actual                   */
/*                   (accediendo directamente a la CMOS)                   */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\ll_s_so1.h>    /* biblioteca de llamadas al sistema SO1 */
#include <so1pub.h\stdio.h>                             /* printf, getchar */
#include <so1pub.h\strings.h>                                   /* iguales */

#include <so1pub.h\puertos.h>                   /* CMOS_RTC_DAY_MONTH, ... */
#include <so1pub.h\cmos.h>                                     /* leerCmos */

char nombreDiaSemana [8] [10] =
{
    "error",    
    "domingo",  
    "lunes",    
    "martes",  
    "miercoles",
    "jueves",    
    "viernes",  
    "sabado"
} ;

char nombreMes [13] [11] =
{
    "error",      
    "enero",     
    "febrero",    
    "marzo",      
    "abril",     
    "mayo",     
    "junio",    
    "julio",    
    "agosto",    
    "septiembre",
    "octubre",   
    "noviembre",  
    "diciembre"
} ;

void formato ( void )
{
//  escribirStrIntenso(" formato: DATE [ -n | -f | -h ] ") ; */
    printf(" formato: DATE [ -n | -f | -h ] ") ;
}

void help ( void )
{
	printf(
	    ""                                                               "\n"
		""                                                               "\n"
		" formato : DATE [ -n | -f | -h ]"                               "\n"
		""                                                               "\n"
        " muestra la fecha actual"                                       "\n"
		""                                                               "\n"
        " opciones: (por defecto -n)"                                    "\n"
		""                                                               "\n"
        "      -n : formato de la fecha DD/MM/AAAA"                      "\n"
        "      -f : formato en castellano"                               "\n"
        "      -h : muestra este help"                                   "\n"
	) ;
}

int main ( int argc, char * argv [ ] )
{
    word_t dia ;
    word_t mes ;
    word_t anio ;
    word_t diaSemana ;

    if ((argc == 2) && (iguales(argv[1], "-h") || iguales(argv[1], "-H")))
    {
        help() ;
        return(0) ;
    }
    if ((argc == 1) ||
            ((argc == 2) &&
             (iguales(argv[1], "-n") || iguales(argv[1], "-N") ||
              iguales(argv[1], "-f") || iguales(argv[1], "-F"))))
    {
        dia = leerCmos(CMOS_RTC_DAY_MONTH) ;
        mes = leerCmos(CMOS_RTC_MONTH) ;
        anio = leerCmos(CMOS_RTC_YEAR) ;
        if (iguales(argv[1], "-f") || iguales(argv[1], "-F"))
        {
            diaSemana = leerCmos(CMOS_RTC_DAY_WEEK) ;
			printf(
//              "\n"
//              "\n"
//              " fecha:"
                " %s"
//              " dia"
                " %u de %s de 20%02x", 
				nombreDiaSemana[bcdToInt(diaSemana)],
                bcdToInt(dia),
				nombreMes[bcdToInt(mes)],
                anio
            ) ;
        }
        else
            printf(" %02x/%02x/20%02x", dia, mes, anio) ;           /* BCD */
		                                  /* decimal codificado en binario */
//		getchar() ;
		return(0) ;
    }
    formato() ;
	return(-1) ;
}