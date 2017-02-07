/* ----------------------------------------------------------------------- */
/*                                  clock.c                                */
/* ----------------------------------------------------------------------- */
/*                programa que muestra continuamente la hora               */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\ll_s_so1.h>    /* biblioteca de llamadas al sistema SO1 */
#include <so1pub.h\escribir.h>
#include <so1pub.h\carsctrl.h>                                       /* FF */
#include <so1pub.h\caracter.h>                           /* dig, mayuscula */
#include <so1pub.h\strings.h>                                   /* iguales */

int dfReloj ;

void formato ( void )
{
    escribirStr(
        " formato: CLOCK [ -n | -b | -h ] "
    ) ;
}

void help ( void )
{
    escribirStr(
        "\n"
        "\n"
        " formato : CLOCK [ -n | -b | -h ]         \n"
        "\n"
        " muestra continuamente la hora actual     \n"
        "\n"
        " opciones: (por defecto -n)               \n"
        "\n"
        "      -n : escribe HH:MM:SS               \n"
        "      -b : borra y centra HH:MM:SS        \n"
        "      -h : muestra este help              \n"
    ) ;
}

#pragma option -w-par            /* omitir warning parameter is never used */

void borrarCentrar ( word_t tamWin )
{
//  word_t lineas, columnas ;
    escribirCar(FF) ;
//  lineas = tamWin & 0x00FF ;
//  columnas = tamWin >> 8 ;
//  gotoXYWindow((columnas - 8)/2, lineas/2) ;
}

#pragma option -wpar           /* permitir warning parameter is never used */

void escribirHora ( word_t horas, word_t minutos, word_t segundos )
{
    if (horas <= 9) escribirCar('0') ;
    escribirDec(horas, 1) ;
    escribirCar(':') ;
    if (minutos <= 9) escribirCar('0') ;
    escribirDec(minutos, 1) ;
    escribirCar(':') ;
    if (segundos <= 9) escribirCar('0') ;
    escribirDec(segundos, 1) ;
}

void leerHora ( word_t * horas, word_t * minutos, word_t * segundos )
{
    char strHora [ 8 ] ;
    lseek(dfReloj, 0, SEEK_SET) ;
    read(dfReloj, (pointer_t)&strHora, sizeof(strHora)) ;
    *horas    = (word_t)(10*(strHora[0] - '0') + (strHora[1] - '0')) ;
    *minutos  = (word_t)(10*(strHora[3] - '0') + (strHora[4] - '0')) ;
    *segundos = (word_t)(10*(strHora[6] - '0') + (strHora[7] - '0')) ;
}

void main ( int argc, char * argv [ ] )
{

    word_t horas, h_ant ;
    word_t minutos, m_ant ;
    word_t segundos, s_ant ;

    word_t tamWin, tamWinAnt ;

    bool_t mantenerCentrado = FALSE ;

    int dfTimer = open("TIMER", O_RDONLY) ;

    if (dfTimer < 0)
    {
        escribirStr(" recurso TIMER no disponible ") ;
        exit(-1) ;
    }

    dfReloj = open("RELOJ", O_RDONLY) ;

    if (dfReloj < 0)
    {
        escribirStr(" recurso RELOJ no disponible ") ;
        exit(-1) ;
    }

    if ((argc == 2) && (iguales(argv[1], "-h") || iguales(argv[1], "-H")))
    {
        help() ;
        return ;
    }
    if ((argc == 2) &&
            (iguales(argv[1], "-b") || iguales(argv[1], "-B")))
    {
        mantenerCentrado = TRUE ;
//      tamWin = tamWindow() ;
        tamWin = 80 ;
        tamWinAnt = tamWin ;
        borrarCentrar(tamWin) ;
    }
    else if ((argc != 1) &&
             ((argc != 2) ||
              (!iguales(argv[1], "-n") && !iguales(argv[1], "-N"))))
    {
        formato() ;
        return ;
    }

//  ocultarCursor(TRUE) ;

    escribirStr(" ") ;

    leerHora((word_t *)&horas, (word_t *)&minutos, (word_t *)&segundos) ;
    escribirHora(horas, minutos, segundos) ;

    do
    {
        h_ant = horas ;
        m_ant = minutos ;
        s_ant = segundos ;

        leer(dfTimer) ;                      /* esperamos al siguiente tic */

//      traerAlFrenteWindow() ;
        leerHora((word_t *)&horas, (word_t *)&minutos, (word_t *)&segundos) ;

        if (mantenerCentrado)
        {
            /*    if ((tamWin = tamWindow()) != tamWinAnt) { */
            if ((tamWin = 80) != tamWinAnt)
            {
                tamWinAnt = tamWin ;
                borrarCentrar(tamWin) ;
                escribirHora(horas, minutos, segundos) ;
                continue ;
            }
        }
        if ((segundos != s_ant) || (minutos != m_ant) || (horas != h_ant))
        {
            escribirStr("\b\b\b\b\b\b\b\b") ;
            escribirHora(horas, minutos, segundos) ;
        }
    }
    while (leerAsciiListo(STDIN) == (char)0) ;

//  ocultarCursor(FALSE) ;

    close(dfTimer) ;

}


