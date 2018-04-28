/* ----------------------------------------------------------------------- */
/*                                  mouse.c                                */
/* ----------------------------------------------------------------------- */
/*             programa para monitorizar la actividad del raton            */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\ll_s_so1.h>    /* biblioteca de llamadas al sistema SO1 */
#include <so1pub.h\stdio.h>                    /* printf, getchar, putchar */
#include <so1pub.h\scanner.h>
#include <so1pub.h\strings.h>                                   /* strcmpu */
#include <so1pub.h\ctype.h>                                     /* toupper */
#include <so1pub.h\def_scan.h>                                      /* Esc */

void mostrarFormato ( void )
{
    printf(
        ""                                                               "\n"
        ""                                                               "\n"
        " formato: MOUSE [ -h | -p | -b ]"                               "\n"
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
    printf(
        ""                                                               "\n"
        " monitoriza la actividad del raton"                             "\n"
        ""                                                               "\n"
        " opciones: (por defecto -b)"                                    "\n"
        ""                                                               "\n"
        "      -h : muestra este help"                                   "\n"
        "      -p : polling mediante leerRatonListo"                     "\n"
        "      -b : bloqueo mediante leerRaton"                          "\n"		
    ) ;
    exit(0) ;
}
	
void mostrarEstado ( char opcion, word_t modo )
{
    bool_t nuevoEstado ;
    estadoRaton_t est ;
    char car = '\0' ;
    byte_t scanCode = (byte_t)0 ;
    int i ;
    int dfRaton ;

    dfRaton = open("RATON", O_RDONLY) ;

    if (dfRaton < 0)
    {
        printf(" recurso RATON no disponible ") ;
        return ;
    }

    aio_read(dfRaton, (pointer_t)&est, sizeof(estadoRaton_t)) ;

//  leerRatonListo((estadoRaton_t far *)&est) ;

    switch (opcion)
    {
    case 'P' : ;
    case 'B' : ;
    case 'F' : printf(" (clic en C = 0 F = 0 termina) ") ; break ;
    case 'C' : printf(" (Q o clic en C = 0 F = 0 termina) ") ; break ;
    case 'T' : printf(" (Esc o clic en C = 0 F = 0 termina) ") ; break ;
    default  : printf(" opcion incorrecta ") ; return ;
    }

    printf("\n\n") ;

//  ocultarCursor(TRUE) ;

    est.B0 = 0 ;
    est.B1 = 0 ;
    est.B2 = 0 ;
    est.incX = 0 ;
    est.incY = 0 ;
//  est.X = maxX/2 ;
//  est.Y = maxY/2 ;
//  est.F = est.Y >> 3 ;
//  est.C = est.X >> 3 ;
//  est.botonIz = FALSE ;
//  est.botonDe = FALSE ;
//  est.botonMe = FALSE ;
    i = 0 ;

    nuevoEstado = TRUE ;

    do
    {
        if ((nuevoEstado) || (opcion == 'F'))
        {
            printf(
    			"\r [%02X, %02X, %02X] (%4i,%4i)  (%3i, %3i) C = %2i F = %2i L = %i R = %i M = %i%6i", 
				est.B0, est.B1, est.B2, 
				est.incX, est.incY, 
			    est.X, est.Y, 
				est.C, est.F, 
				est.botonIz, est.botonDe, est.botonMe, 
                i++
			) ;
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
//              leerRaton((estadoRaton_t far *)&est) ;
                break ;
            }
            break ;
        default  :
            printf(" opcion incorrecta ") ;
            return ;
        }

    }
    while (((est.F != 0) || (est.C != 0) || (est.botonIz == 0)) &&
            (toupper(car) != 'Q') && (scanCode != Esc)) ;

    printf("\n") ;

//  ocultarCursor(FALSE) ;

}

int main ( int argc, char * argv [ ] )
{

    char opcion ;
    word_t modo ;

    if (argc == 1) mostrarEstado('B', 2) ;
    else if (argc == 2)
        if (!strcmpu(argv[1], "-h")) help() ;
        else if ((argv[1][0] == '-') &&
                 (((argv[1][2] == '\0')) || (argv[1][3] == '\0')))          
        {
            opcion = toupper(argv[1][1]) ;
            switch (opcion)                                    /* -px, -bx */
            {
            case 'P' :
                ;
            case 'B' :
                switch (toupper(argv[1][2]))
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
	return(0) ;
}