/* ----------------------------------------------------------------------- */
/*                                  mouse.c                                */
/* ----------------------------------------------------------------------- */
/*             programa para monitorizar la actividad del raton            */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\ll_s_so1.h>    /* biblioteca de llamadas al sistema SO1 */
#include <so1pub.h\escribir.h>

#include <so1pub.h\scanner.h>
#include <so1pub.h\strings.h>
#include <so1pub.h\caracter.h>                                /* mayuscula */
#include <so1pub.h\def_scan.h>                                      /* Esc */

void mostrarFormato ( void )
{
    escribirStr(
        "\n"
        "\n"
        " formato: MOUSE [ -h | -p | -b ] \n"
    ) ;
}

void formato ( void )
{
    mostrarFormato() ;
    exit(-1) ;
}

void help ( void )
{
	mostrarFormato() ;
    escribirStr(
        "\n"
        " monitoriza la actividad del raton         \n"
        "\n"
        " opciones: (por defecto -b)                \n"
        "\n"
        "      -h : muestra este help               \n"
        "      -p : polling mediante leerRatonListo \n"
        "      -b : bloqueo mediante leerRaton      \n"		
    ) ;
    exit(0) ;
}
	
void mostrarEstado ( char opcion, word_t modo )
{

    bool_t nuevoEstado ;
    estadoRaton_t est ;
    char car = (char)0 ;
    byte_t scanCode = (byte_t)0 ;
    int i ;
    int dfRaton ;

    dfRaton = open("RATON", O_RDONLY) ;

    if (dfRaton < 0)
    {
        escribirStr("\n\n recurso RATON no disponible \n") ;
        return ;
    }

    aio_read(dfRaton, (pointer_t)&est, sizeof(estadoRaton_t)) ;

//  leerRatonListo((estadoRaton_t far *)&est) ;

    switch (opcion)
    {
    case 'P' :
        ;
    case 'B' :
        ;
    case 'F' :
        escribirStr(" (clic en C = 0 F = 0 termina) ") ;
        break ;
    case 'C' :
        escribirStr(" (Q o clic en C = 0 F = 0 termina) ") ;
        break ;
    case 'T' :
        escribirStr(" (Esc o clic en C = 0 F = 0 termina) ") ;
        break ;
    default  :
        escribirStr(" opcion incorrecta ") ;
        return ;
    }

    escribirLn() ;
    escribirLn() ;

//  ocultarCursor(TRUE) ;

    est.B0 = 0 ;
    est.B1 = 0 ;
    est.B2 = 0 ;
    est.incX = 0 ;
    est.incY = 0 ;
    /*
        est.X = maxX/2 ;
        est.Y = maxY/2 ;
        est.F = est.Y >> 3 ;
        est.C = est.X >> 3 ;
        est.botonIz = FALSE ;
        est.botonDe = FALSE ;
        est.botonMe = FALSE ;
    */
    i = 0 ;

    nuevoEstado = TRUE ;

    do
    {

        if ((nuevoEstado) || (opcion == 'F'))
        {

            escribirCar('\r') ;
//          escribirCar('\n') ;

            escribirStr(" [") ;
            escribirHex(est.B0, 2) ;
            escribirStr(", ") ;
            escribirHex(est.B1, 2) ;
            escribirStr(", ") ;
            escribirHex(est.B2, 2) ;
            escribirStr("] ") ;

            escribirStr(" (") ;
            if (est.incX < 0)
            {
                escribirStr("-") ;
                escribirDec((~est.incX)+1, 3) ;
            }
            else
                escribirDec(est.incX, 4) ;
            escribirStr(",") ;
            if (est.incY < 0)
            {
                escribirStr("-") ;
                escribirDec((~est.incY)+1, 3) ;
            }
            else
                escribirDec(est.incY, 4) ;
            escribirStr(") ") ;

            escribirStr(" (") ;
            escribirDec(est.X, 3) ;
            escribirStr(", ") ;
            escribirDec(est.Y, 3) ;
            escribirStr(") ") ;

            escribirStr(" C = ") ;
            escribirDec(est.C, 2) ;
            escribirStr(" F = ") ;
            escribirDec(est.F, 2) ;

            escribirStr(" L = ") ;
            escribirDec(est.botonIz, 1) ;
            escribirStr(" R = ") ;
            escribirDec(est.botonDe, 1) ;
            escribirStr(" M = ") ;
            escribirDec(est.botonMe, 1) ;
            escribirDec(i++, 6) ;

        }

        switch (opcion)
        {
        case 'P' :
        case 'F' :
            aio_read(dfRaton, (pointer_t)&est, sizeof(estadoRaton_t)) ;
            nuevoEstado = TRUE ;
            break ;
        case 'B' :
            switch (modo)
            {
            case 2 :
                read(dfRaton, (pointer_t)&est, sizeof(estadoRaton_t)) ;
                break ;
//              leerRaton((estadoRaton_t far *)&est) ;
            }
            break ;
        default  :
            escribirStr(" opcion incorrecta ") ;
            return ;
        }

    }
    while (((est.F != 0) || (est.C != 0) || (est.botonIz == 0)) &&
            (mayuscula(car) != 'Q') && (scanCode != Esc)) ;

    escribirLn() ;

//  ocultarCursor(FALSE) ;

}

void main ( int argc, char * argv [ ] )
{

    char opcion ;
    word_t modo ;

    if (argc == 1) mostrarEstado('B', 2) ;
    else if (argc == 2)
        if (iguales(argv[1], "-h")) help() ;
        else if ((argv[1][0] == '-') &&
                 (((argv[1][2] == (char)0)) || (argv[1][3] == (char)0)))          
        {
            opcion = mayuscula(argv[1][1]) ;
            switch (opcion)                                    /* -px, -bx */
            {
            case 'P' :
                ;
            case 'B' :
                switch (mayuscula(argv[1][2]))
                {
                case 'L' :                                     /* -pl, -bl */
                    modo = 1 ;
                    break ;
                case 'S' :                                     /* -ps, -ps */
                    modo = 0 ;
                    break ;
                default  :
                    modo = 2 ;
                }
                mostrarEstado(opcion, modo) ;
                break ;
            default  :
                formato() ;
            }
        }
        else formato() ;
    else formato() ;
}

