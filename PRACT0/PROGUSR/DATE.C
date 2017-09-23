/* ----------------------------------------------------------------------- */
/*                                  date.c                                 */
/* ----------------------------------------------------------------------- */
/*                   comando que muestra la fecha actual                   */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\ll_s_so1.h>    /* biblioteca de llamadas al sistema SO1 */
#include <so1pub.h\escribir.h>
#include <so1pub.h\strings.h>                                   /* iguales */

#include <so1pub.h\puertos.h>                   /* CMOS_RTC_DAY_MONTH, ... */
#include <so1pub.h\cmos.h>                                     /* leerCmos */

struct
{
    char nombre [10] ;
    word_t longitud ;
} descDiaSemana [8] =
{
    { "error",     5 },
    { "domingo",   7 },
    { "lunes",     5 },
    { "martes",    6 },
    { "miercoles", 9 },
    { "jueves",    6 },
    { "viernes",   7 },
    { "sabado",    6 }
} ;

struct
{
    char nombre [11] ;
    word_t longitud ;
} descMes [13] =
{
    { "error",       5 },
    { "enero",       5 },
    { "febrero",     7 },
    { "marzo",       5 },
    { "abril",       5 },
    { "mayo",        4 },
    { "junio",       5 },
    { "julio",       5 },
    { "agosto",      6 },
    { "septiembre", 10 },
    { "octubre",     7 },
    { "noviembre",   9 },
    { "diciembre",   9 }
} ;

void formato ( void )
{
//  escribirStrIntenso(" formato: DATE [ -n | -f | -h ] ") ; */
    escribirStr(" formato: DATE [ -n | -f | -h ] ") ;
}

void help ( void )
{
    escribirLn() ;
    escribirLn() ;
    escribirStr(" formato : DATE [ -n | -f | -h ]          \n\n") ;
    escribirStr(" muestra la fecha actual                  \n\n") ;
    escribirStr(" opciones: (por defecto -n)               \n\n") ;
    escribirStr("      -n : formato de la fecha DD/MM/AAAA \n") ;
    escribirStr("      -f : formato en castellano          \n") ;
    escribirStr("      -h : muestra este help              \n") ;
}

#pragma warn -aus

void main ( int argc, char * argv [ ] )
{

    word_t dia ;
    word_t mes ;
    word_t anio ;
    word_t diaSemana ;

    if ((argc == 2) && (iguales(argv[1], "-h") || iguales(argv[1], "-H")))
    {
        help() ;
        return ;
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
//          escribirStr("\n\n fecha: ") ; */
            escribirStr(" ") ;
            escribirStr(descDiaSemana[bcdToInt(diaSemana)].nombre) ;
//          escribirStr(" dia ") ; */
            escribirStr(" ") ;
            escribirDec(bcdToInt(dia), 1) ;
            escribirStr(" de ") ;
            escribirStr(descMes[bcdToInt(mes)].nombre) ;
            escribirStr(" de ") ;
            escribirStr("20") ;
            escribirHex(anio, 2) ;
        }
        else
        {
            escribirStr(" ") ;
            escribirHex(dia, 2) ;
            escribirCar('/') ;
            escribirHex(mes, 2) ;
            escribirCar('/') ;
            escribirStr("20") ;
            escribirHex(anio, 2) ;
        }
    }
    else formato() ;
}

#pragma warn +aus
