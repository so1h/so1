/* ----------------------------------------------------------------------- */
/*                               leetimer.c                                */
/* ----------------------------------------------------------------------- */
/*                                                                         */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\ll_s_so1.h>    /* biblioteca de llamadas al sistema SO1 */
#include <so1pub.h\escribir.h>
#include <so1pub.h\carsctrl.h>                                       /* FF */
#include <so1pub.h\caracter.h>                           /* dig, mayuscula */
#include <so1pub.h\strings.h>                                   /* iguales */

#define nTicsMax 360

void leeTimer ( word_t nTics )
{

    int i ;
    byte_t buf [nTicsMax] ;
    int dfTimer ;

    if (nTics > nTicsMax)
    {
        escribirStr(" el numero de tics debe ser <= ") ;
        escribirDec(nTicsMax, 1) ;
        exit(0) ;
    }

    dfTimer = open("TIMER", O_RDONLY) ;
    if (dfTimer < 0)
    {
        escribirStr(" no hay driver del timer ") ;
    }
    else
    {
        escribirStr("\n\n ") ;
        escribirDec(nTics, 1) ;
        escribirStr(" x leer(dfTimer) ... ") ;
        for ( i = 0 ; i < nTics ; i++ )
        {
            escribirDec((byte_t)leer(dfTimer), 1) ;
            escribirStr(" ") ;
        }
        escribirStr("\n\n buf = ") ;
        for ( i = 0 ; i < nTics ; i++ )
        {
            buf[i] = (char)0 ;
            escribirDec(buf[i], 1) ;
            escribirStr(" ") ;
        }
        escribirStr("\n\n read(dfTimer, ") ;
        escribirPtr(NULL) ;
        escribirStr(", ") ;
        escribirDec(nTics, 1) ;
        escribirStr(") ... ") ;
        read(dfTimer, NULL, nTics) ;
        escribirStr("\n\n buf = ") ;
        for ( i = 0 ; i < nTics ; i++ )
        {
            escribirDec(buf[i], 1) ;
            escribirStr(" ") ;
        }
        escribirStr("\n\n read(dfTimer, ") ;
        escribirPtr(buf) ;
        escribirStr(", ") ;
        escribirDec(nTics, 1) ;
        escribirStr(") ... ") ;
        read(dfTimer, (pointer_t)buf, nTics) ;
        escribirStr("\n\n buf = ") ;
        for ( i = 0 ; i < nTics ; i++ )
        {
            escribirDec(buf[i], 1) ;
            escribirStr(" ") ;
        }
        escribirStr("\n ") ;
    }

    {
        typedef struct
        {
            dword_t a ;
            dword_t b ;
            word_t c ;
            word_t d;
        } st_t ;
        st_t * st ;

        escribirStr("\n\n aio_read(dfTimer, ") ;
        escribirPtr(buf) ;
        escribirStr(", ") ;
        escribirDec(nTics, 1) ;
        escribirStr(") ... ") ;
        aio_read(dfTimer, (pointer_t)&buf, nTics) ;
        for ( i = 0 ; i < nTics ; i++ )
        {
            escribirDec(buf[i], 1) ;
            escribirStr(" ") ;
        }
        escribirStr("\n ") ;
        st = (st_t *)&buf ;
        escribirStr("\n a = ") ;
        escribirLHex(st->a, 8) ;
        escribirStr("\n b = ") ;
        escribirLHex(st->b, 8) ;
        escribirStr("\n c = ") ;
        escribirHex(st->c, 4) ;
        escribirStr("\n d = ") ;
        escribirHex(st->d, 4) ;
        escribirStr("\n ") ;
    }

    close(dfTimer) ;
    exit(0) ;
}

descProceso_t descProceso [ maxProcesos ] ;

descFichero_t descFichero [ dfsMax ] ;

descRecurso_t descRecurso [ maxRecursos ] ;

e2PFR_t e2PFR ;

c2c_t c2cPFR [ numColasPFR ] ;

void formato ( void )
{
//  escribirStrIntenso(" formato: LEETIMER ( nTics | -h ) ") ;
    escribirStr(" formato: LEETIMER ( nTics | -h ) ") ;
}

void help ( void )
{
    escribirLn() ;
    escribirLn() ;
    escribirStr(" formato : LEETIMER [ nTics | -h ]             \n\n") ;
    escribirStr(" lee bytes del dispositivo TIMER               \n") ;
    escribirStr(" opciones: (por defecto nTics = 1)             \n\n") ;
    escribirStr("   nTics : ejecuta un read(df, dir, nTics)     \n") ;
    escribirStr("      -h : muestra este help                   \n") ;
}

void main ( int argc, char * argv [ ] )
{

    word_t nTics ;

    obtenInfoPFR ((descProceso_t far *)&descProceso,
                  (descFichero_t far *)&descFichero,
                  (descRecurso_t far *)&descRecurso,
                  (e2PFR_t far *)&e2PFR,
                  (c2c_t far *)&c2cPFR
                 ) ;

    if (argc <= 2)
    {
        if ((argc == 2) &&
                (iguales(argv[1], "-h") || iguales(argv[1], "-H")))
        {
            help() ;
            return ;
        }
        if (argc == 1) nTics = 1 ;
        else
        {
            copiarStr(argv[1], comando[0]) ;
            inicScanner() ;
            obtenSimb() ;
            if (simb != s_numero)
            {
                escribirCar(BEL) ;
                formato() ;
                escribirStr("\n error: nTics debe ser un numero y no \"") ;
                escribirStr(argv[1]) ;
                escribirStr("\"\n") ;
                return ;
            }
            else nTics = num ;
        }
        leeTimer(nTics) ;
    }
    else
        formato() ;
}

