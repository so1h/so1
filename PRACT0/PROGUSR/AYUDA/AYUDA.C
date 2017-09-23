/* ----------------------------------------------------------------------- */
/*                                 ayuda.c                                 */
/* ----------------------------------------------------------------------- */
/*                     programa ayuda para el sistema SO1                  */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\ll_s_so1.h>                                     /* exit */
#include <so1pub.h\stdio.h>                               /* printf, getch */
#include <so1pub.h\carsctrl.h>                                      /* ESC */

void pause ( void )
{
    int i ;
	
    printf(
        "\n"
        " Pulse una tecla para continuar (ESC para salir) ... "
    ) ;

    if (getch() == ESC)
    {
        printf("\n") ;
        exit(0) ;
    }

    for ( i = 0 ; i < 53 ; i++ ) printf("\b") ; 
}

void ayuda ( void )
{
    printf(
        "\n"
        "\n"
        "           comandos internos del shell (SH) de SO1:                \n"
        "\n"
        "   !         (i) repite el comando anterior                        \n"
        "   ID        (i) muestra el usuario y el pid de su sesion SH       \n"
        "   SU        (i) se pasa a superusuario (root) uid = 0 gid = 0     \n"
        "   DIR/LS    (i) muestra el directorio de la unidad de arranque    \n"
        "   CLS/CLEAR (i) borra la pantalla                                 \n"
        "   RET       (i) muestra/establece el retardo activo de SO1        \n"
        "   VER       (i) muestra la version del sistema SO1 (VER [-c])     \n"
        "   EXIT      (i) fin del interprete de comandos (shell)            \n"
        "   PTOS      (i) escribe puntos hasta presionar una tecla          \n"
        "   CTTY      (i) permite cambiar la consola del interprete         \n"
        "   TYPE      (i) escribe por la salida estandar un fichero         \n"
        "   HALT      (i) detencion y salida del sistema SO1                \n"
        "   STATUS    (i) deja ver el estado de terminacion de los comandos \n"
        "   REBOOT    (i) detencion y salida del sistema SO1                \n"
        "   SHUTDOWN  (i) detencion y salida del sistema SO1                \n"
    ) ;

    pause() ;

    printf(
        "         otros comandos de SO1 (externos):                        \n"
        "\n"
        "   PS     (e) muestra la tabla de procesos de SO1                 \n"
        "   DP     (e) muestra en detalle los descriptores de proceso      \n"
        "   DR     (e) muestra los descritores de los recursos             \n"
        "   SH     (e) interprete de comandos (dispositivos CONx ...)      \n"
        "   DFS    (e) muestra los ficheros especiales del sistema         \n"
        "   MEM    (e) muestra las particiones de memoria de SO1           \n"
        "   DMP    (e) volcado de memoria (indicar direccion SSSS:DDDD)    \n"
        "   C2C    (e) muestra las colas de procesos del sistema           \n"
        "   HELP   (e) lista de comandos con sus argumentos                \n"
        "   INFO   (e) muestra informacion sobre el sistema                \n"
        "   KILL   (e) mata procesos (indicar pid > 0 o -a)                \n"
        "   TIME   (e) muestra la hora actual en formato HH:MM:SS          \n"
        "   DATE   (e) muestra la fecha actual en formato DD/MM/AAAA       \n"
        "   AYUDA  (e) esta lista de comandos (ver tambien HELP)           \n"
        "   CLOCK  (e) muestra la hora actual de forma continua            \n"
    ) ;

    pause() ;

    printf(
        "         manejo de la interfaz multiscreen de SO1: \n"
        "\n"
        "   Alt + F1   conmuta a la consola CON1 (SH)                     \n"
        "   Alt + F2   conmuta a la consola CON2 (SH)                     \n"
        "   Alt + F3   conmuta a la consola CON3 (SH)                     \n"
        "   Alt + F4   conmuta a la consola CON4 (no usada inicialmente)  \n"
        "   Alt + F5   conmuta a la consola CON5 (no usada inicialmente)  \n"
        "   Alt + F6   conmuta a la consola CON6 (no usada inicialmente)  \n"
        "   Alt + \'\\'  conmuta a la consola CON0 (consola inicial)      \n"
        "\n"
        "   Alt + \'+\'  modificacion ciclica del numero de lineas        \n"
        "   Alt + \'-\'  modificacion ciclica del numero de lineas        \n"
    ) ;

    pause() ;

    printf(
        "         autocompletado de comandos de SO1: \n"
        "\n"
        "   Tab       autocompleta nombres de comandos internos (CLS, .. )\n"
        "   May + Tab autocompleta nombres de comandos externos (DFS, .. )\n"
		"\n"
        "         otros: \n"
        "\n"
        "   Ctrl + L  borra la pantalla de la consola actual              \n"
        "   Esc       borra la linea de comando actual                    \n"
    ) ;
}

void main ( void )
{
    ayuda() ;
}

