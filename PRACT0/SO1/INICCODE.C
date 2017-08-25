/* ----------------------------------------------------------------------- */
/*                                iniccode.c                               */
/* ----------------------------------------------------------------------- */
/*                    codigo del proceso inicial de SO1                    */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\ll_s_so1.h>         /* exec, waitpid, getpid, open, ... */
#include <so1pub.h\escribir.h>            /* escribirStr, escribirDec, ... */
#include <so1pub.h\put.h>                 /* putCar, putLn, putStr, putDec */
#include <so1pub.h\saludos.h>                       /* mostrarSaludoGrande */
#include <so1pub.h\strings.h>                                 /* copiarStr */
#include <so1pub.h\carsctrl.h>                                      /* ESC */
#include <so1pub.h\pantalla.h>                               /* pantallazo */
#include <so1pub.h\debug.h>            /* assert, valorFlags, mostrarFlags */
#include <so1pub.h\bios_0.h>            /* printStrBIOS, printDecBIOS, ... */
#include <so1pub.h\biosdata.h>                             /* ptrFechaBios */

#include <so1pub.h\interpre.h>                      /* interpretarComandos */

#include <so1.h\iniccode.h>                                        /* inic */
/* valor normal */
#define TIMER   TRUE       /*    TRUE      */
//#define TIMER   FALSE    /*    FALSE     */
#define CONRAT  FALSE      /*    FALSE     */
#define RETARDO TRUE       /*    TRUE      */ 
#define RELOJ   TRUE       /*    TRUE      */ 
#define RATON   TRUE       /*    TRUE      */ 
//#define RATON   FALSE    /*    FALSE     */ 

#define numConsolas 6

#define ticsPorRodaja 18

void esperarFichero ( char * nombre )    /* esperar poder abrir el fichero */
{
    int df ;
    while ((df = open(nombre, O_RDONLY)) < 0)
    {
//      printCarBIOS('.') ;                      /* para indicar la espera */
        printStrBIOS(nombre) ;  /* para indicar la espera y el dispositivo */
//      printStrHastaBIOS(nombre, 3, FALSE) ;                      /* idem */
//      printCarBIOS(nombre[0]) ;                                  /* idem */
        printCarBIOS(' ') ;
    }
    close(df) ;
}

int inic ( void )                  /* lanza los principales drivers de SO1 */
{

    int i, j ;
    int df ;
    pid_t pid ;
    int status ;
    int timeout ;
    int dfCon ;
    char strArg [15] ;
    char nombre [9] ;
    int anioBIOS ;

    /* Vamos a lanzar procesos que son drivers, con la llamada createProcess */

    /* Las interrupciones podrían estar inhibidas si se ejecuta desde SO1    */
    /* esta funcion, por lo que podria no multiplexarse la CPU mediante las  */
    /* interrupciones del timer. Por ese motivo se supone que createProcess  */
    /* pasa a ejecucion primero el proceso creado (TIMER) habilitando sus    */
    /* interrupciones. Si la funcion inic se ejecuta dentro del proceso      */
    /* INIC, en ese caso las interrupciones estarían permitidas.             */

    /* RETARDO debe ejecutarse el primero para intentar asegurar que el      */
    /* calculo del numero de vueltas del bucle no se vea interferido por la  */
    /* ejecucion de otros procesos.                                          */

#if (RETARDO)
    if ((pid = createProcess("RETARDO", "RETARDO -q &")) < 0)          /* GP */
    {
        printStrBIOS(
            "\a\n fallo al arrancar RETARDO"
            "\n pid = "
        ) ;
        printIntBIOS(pid, 1) ;
    }
    esperarFichero("RETARDO") ;
    printStrBIOS(", RETARDO&") ;

    /* qemu se retarda de mas, por lo que lo tratamos de una manera especial */

    anioBIOS = 10*(ptrFechaBios[6] - '0') + (ptrFechaBios[7] - '0') ;
    if (anioBIOS == 99)                /* fecha del BIOS de qemu: "06/23/99" */
    {
//      printStrBIOS("\n retardoActivo original = ") ;
//      printLDecBIOS(retardoActivo(1), 1) ;
        retardoActivo(retardoActivo(1)/100L) ;
//      printStrBIOS("\n retardoActivo nuevo = ") ;
//      printLDecBIOS(retardoActivo(1), 1) ;
    }
#endif

#if (TIMER)
    printStrBIOS("\n inicTimer(") ;
    printDecBIOS(ticsPorRodaja, 1) ;
    printStrBIOS(") ... ticsPorRodaja = ") ;
    printDecBIOS(ticsPorRodaja, 1) ;
    printCarBIOS(' ') ;
    copiarStr("TIMER X &", strArg) ;
#if (ticsPorRodaja < 10)
    strArg[6] = '0' + ticsPorRodaja ;
#else
    strArg[6] = '0' + (ticsPorRodaja / 10) ;
    strArg[7] = '0' + (ticsPorRodaja % 10) ;
#endif

    /* Aqui se da a veces una condicion de carrera quedandose el sistema   */
    /* bloqueado. Es posible que el problema este en el driver (TIMER).    */

//  printStrBIOS("\n inic: antes de createProcess TIMER") ;
    if ((pid = createProcess("TIMER", strArg)) < 0)
    {
        /* timer 18 & */ /* GP */
        printStrBIOS(
            "\a\n fallo al arrancar el timer"
            "\n pid = "
        ) ;
        printIntBIOS(pid, 1) ;
    }
//  printStrBIOS("\n inic: tras createProcess TIMER") ;
    esperarFichero("TIMER") ;

    /* aqui hay ya multiplexacion de la CPU entre los procesos no bloqueados */

#endif

    /* mostrarFlags() ; */

    printStrBIOS("\n inicConsola(") ;
    printDecBIOS(numConsolas, 1) ;
    printStrBIOS(") ") ;

#if (CONRAT)
    copiarStr("CONRAT X &", strArg) ;
    strArg[7] = '0' + numConsolas ;
    if ((pid = createProcess("CONRAT", "CONRAT&")) < 0)                /* GP */
    {
#else
    copiarStr("CONSOLA -q X &", strArg) ;
    strArg[11] = '0' + numConsolas ;
    if ((pid = createProcess("CONSOLA", strArg)) < 0)                  /* GP */
    {
#endif
        printStrBIOS(
            "\a\n fallo al arrancar la consola"
            "\n pid = "
        ) ;
        printIntBIOS(pid, 1) ;
    }
    copiarStr("CONX", nombre) ;
    for ( i = 0 ; i < numConsolas ; i++ )
    {
        nombre[3] = '0' + i ;
        esperarFichero(nombre) ;
    }
    printLnBIOS() ;

    /* aqui tenemos ya (6) consolas CON0, ... , CON5 disponibles para E/S    */

    mostrarFlags() ;

    /* Tanto la llamada createProcess como la ejecucion del driver CONSOLA   */
    /* necesitan asignar un bloque de memoria para el proceso creado o para  */
    /* las consolas mediante la llamada al sistema ll_buscarBloque que esta  */
    /* implementada mediante el fichero "GM" que se abre en ese momento con  */
    /* dfGM = open("GM", O_RDONLY) ; Como ese es el primer fichero que abre  */
    /* el proceso inicial (SO1) sabemos que el descriptor devuelto es cero:  */
    /* fdGM = 0.                                                             */

    close(0) ;   /* Cerramos "GM" para no interferir con la entrada estandar */
    /* de INIT (STDIN) que todavia no se ha asociado a CON0.   */
    assert(

        (open("CON0", O_RDONLY) == STDIN) &&
        (open("CON0", O_WRONLY) == STDOUT) &&
        (open("CON0", O_WRONLY) == STDERR),

        "\n so1(): ERROR STDIN, STDOUT o STDERR"

    ) ;

    /* aqui, el proceso inicial ya utiliza CON0 como entrada/salida estandar */

    /* while (TRUE) { car = leer(STDIN) ; putCar(STDOUT, car) ; } */

    /* leer(STDIN) ; *//* permite las interrupciones mientras esta bloqueado */

    printStrBIOS("\n\n comandos: ") ;
    copiarStr("CONX", nombre) ;
    for ( i = 1 ; i <= numConsolas/2 ; i++ )
    {
        nombre[3] = '0' + i ;
        close(STDIN) ;
        close(STDOUT) ;
        close(STDERR) ;
        open(nombre, O_RDONLY) ;
        open(nombre, O_WRONLY) ;
        open(nombre, O_WRONLY) ;
        if ((pid = createProcess("LOGIN", "LOGIN")) < 0)                 /* GP */
        {
            printStrBIOS("\a\n fallo al arrancar LOGIN en ") ;
            printStrBIOS(nombre) ;
            printStrBIOS("\n (pid = ") ;
            printIntBIOS(pid, 1) ;
            printCarBIOS(')') ;
        }
        printStrBIOS("LOGIN") ;
        if (i < numConsolas/2) printStrBIOS("&, ") ;
        else printStrBIOS("&") ;
    }

    /* aqui, ya tenemos interpretes de comandos (SH.BIN) en CON1, CON2, CON3 */

    close(STDIN) ;
    close(STDOUT) ;
    close(STDERR) ;
    open("CON0", O_RDONLY) ;
    open("CON0", O_WRONLY) ;
    open("CON0", O_WRONLY) ;

#if (RELOJ)
    if ((pid = createProcess("RELOJ", "RELOJ -q &")) < 0)              /* GP */
    {
        printStrBIOS(
            "\a\n fallo al arrancar el reloj"
            "\n pid = "
        ) ;
        printIntBIOS(pid, 1) ;
    }
    esperarFichero("RELOJ") ;
    printStrBIOS(", RELOJ&") ;
#endif

#if (RATON)
    if (!igualesHasta(
                (char far *)ptrFechaBios,
                (char far *)"01/01/99", 8))                           /* !hayVDos */
    {
        if ((pid = createProcess("RATON", "RATON -q &")) < 0)            /* GP */
        {
            printStrBIOS(
                "\a\n fallo al arrancar el raton"
                "\n pid = "
            ) ;
            printIntBIOS(pid, 1) ;
        }
        esperarFichero("RATON") ;
        printStrBIOS(", RATON&") ;
    }
#endif

#if (FALSE)                                     /* depuracion msdos Takeda */
    {
        char car ;
        printStrBIOS("\n probando Takeda ") ;
        while ((car = leerCarBIOS()) != 'F')                               /* Ok */
        {
//while ((car = (char)teclaListaBIOS()) != 'F') {/* (no saca del bufer) Ok */
//while ((car = (char)leerTeclaListaBDA()) != 'F') {        /* no lee nada */ /* problema */
//while ((car = leerListo(STDIN)) != 'F') {                 /* no lee nada */
//while ((car = leer(STDIN)) != 'F') {              /* el programa termina */
            if (car != (char)0)
                printCarBIOS(car) ;
        }
    }
#endif

#define ptrPant ((pantalla_t far *)pointer(0xB800, 0x0000))

    if ((df = open("TIMER", O_RDONLY)) >= 0)
    {
        for ( i = 20 ; i < 60 ; i++ )
        {
            pantallazo(ptrPant, 50, (char)254, atrNormal, 21, 20, 21, i) ;
#if (TRUE)                 /* FALSE para que funcione no se cuelgue Takeda */
            leer(df) ;     /* permite las interrupciones mientras esta bloqueado */
            car = leerListo(STDIN);
            if (car != (char)0)
            {
                if (car == ESC) break ;
                car = leer(STDIN) ;
                if (car == ESC) break ;
            }
#endif
        }
        close(df) ;
    }
    else
        printStrBIOS("\a\n fallo al abrir el fichero TIMER") ;
    escribirCar('\f') ;
    /*
      cursorF = 0 ;
      cursorC = 0 ;
    */
    mostrarSaludoGrande() ;

    if ((pid = createProcess("INFO", "info")) > 0)                     /* GP */
        waitpid(pid, (int far *)&status) ;                               /* GP */
    else
        escribirLn() ;

    escribirStr("\n flags = ") ;
    escribirHex(valorFlags(), 4) ;
    if (getpid() != 0)
    {
        escribirLn() ;
        asm sti ;                             /* permitimos las interrupsiones */
    }
    else
    {
        escribirStr(" se van a permitir las interrupciones ... \n") ;
        assert((valorFlags() & 0x0200) == 0x0000,
               "\n so1(): ERROR ints. no inhibidas") ;
        asm sti ;                             /* permitimos las interrupsiones */
        escribirStr("\n flags = ") ;
        escribirHex(valorFlags(), 4) ;
        escribirStr(" interrupciones permitidas \n") ;
        assert((valorFlags() & 0x0200) == 0x0200,
               "\a\n so1(): ERROR ints. no permitidas") ;
    }

    if ((pid = createProcess("CONSOLA", "consola -c 1")) > 0)                     /* GP */
        waitpid(pid, (int far *)&status) ;                               /* GP */
    else
        escribirLn() ;

    status = interpretarComandos() ;                                /* shell */

    /* enviamos a todas las consolas el mensaje de terminacion inminente */

    timeout = status ;
    close(STDOUT) ;             /* no se va a usar mas por parte del proceso */

    if (timeout >= 0)
    {
        copiarStr("CONX", nombre) ;
        for ( i = 0 ; i < numConsolas ; i++ )
        {
            nombre[3] = '0' + i ;
            if ((dfCon = open(nombre, O_WRONLY)) > 0)
            {
                if (i > 0) putLn(dfCon) ;
                putStr(dfCon, "\n El sistema se cerrara en ") ;
                putDec(dfCon, timeout, 1) ;
                putStr(dfCon, " segundos ") ;
                close(dfCon) ;                          /* luego se vuelve a abrir */
            }
        }
        for ( j = 0 ; j < timeout ; j++ )
        {
            esperarTicsBIOS(18) ;
            for ( i = 0 ; i < numConsolas ; i++ )
            {
                nombre[3] = '0' + i ;
                if ((dfCon = open(nombre, O_WRONLY)) > 0)
                {
                    putCar(dfCon, '.') ;
                    close(dfCon) ;
                }
            }
        }
    }

    if (getppid() == 0) exit(timeout) ;                  /* fin proceso INIT */

    return(timeout) ;                                    /* fin funcion inic */

}
