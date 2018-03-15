/* ----------------------------------------------------------------------- */
/*                                iniccode.c                               */
/* ----------------------------------------------------------------------- */
/*                    codigo del proceso inicial de SO1                    */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\ll_s_so1.h>         /* exec, waitpid, getpid, open, ... */
#include <so1pub.h\stdio.h>                    /* printf, getchar, putchar */
#include <so1pub.h\escribir.h>            /* escribirStr, escribirDec, ... */
//#include <so1pub.h\put.h>               /* putCar, putLn, putStr, putDec */
#include <so1pub.h\saludos.h>                       /* mostrarSaludoGrande */
#include <so1pub.h\strings.h>                           /* strcpy, strncmp */
#include <so1pub.h\carsctrl.h>                                      /* ESC */
//#include <so1pub.h\pantalla.h>                             /* pantallazo */
#include <so1pub.h\debug.h>            /* assert, valorFlags, mostrarFlags */
#include <so1pub.h\bios_0.h>            /* printStrBIOS ... esperarTicBIOS */
#include <so1pub.h\biosdata.h>                             /* ptrFechaBios */
#include <progusr\consola\consola.h>                      /* descConsola_t */

#include <so1pub.h\interpre.h>                      /* interpretarComandos */

#include "iniccode.h"                                            /* inic_0 */
//                                                         /* valor normal */
#define TIMER   TRUE                                       /*    TRUE      */
#define RETARDO TRUE                                       /*    TRUE      */ 
#define RELOJ   TRUE                                       /*    TRUE      */ 
#define RATON   TRUE                                       /*    TRUE      */ 
#define CONRAT  FALSE                                      /*    FALSE     */

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

/* inic_0 lanza los principales drivers de SO1 */

int inic_0 ( word_t numConsolas, word_t ticsPorRodaja )  
{

    int i, j ;
    int df ;
    pid_t pid ;
    int status ;
//  int timeout ;
    int dfCon ;
    char strArg [16] ;
    char nombre [9] ;
    int anioBIOS ;

    /* Vamos a lanzar procesos drivers, con la llamada createProcess       */

    /* Las interrupciones podrían estar inhibidas si se ejecuta desde SO1  */
    /* esta funcion, por lo que podria no multiplexarse la CPU mediante    */
    /* las interrupciones del timer. Por ese motivo se supone que          */
	/* createProcess pasa a ejecucion primero el proceso creado (TIMER)    */
	/* habilitando sus interrupciones. Si la funcion inic se ejecuta       */
	/* dentro del proceso INIC, en ese caso las interrupciones estarían    */
	/* permitidas.                                                         */

    /* RETARDO debe ejecutarse el primero para intentar asegurar que el    */
    /* calculo del numero de vueltas del bucle no se vea interferido por   */
    /* la ejecucion de otros procesos.                                     */

#if (RETARDO)
    if ((pid = createProcess("RETARDO", "RETARDO -q &")) < 0)        /* GP */
    {
        printStrBIOS(
            "\a\n fallo al arrancar RETARDO"
            "\n pid = "
        ) ;
        printIntBIOS(pid, 1) ;
    }
    esperarFichero("RETARDO") ;
    printStrBIOS(", RETARDO&") ;

    /* qemu se retarda de mas, por lo que lo tratamos de manera especial   */

    anioBIOS = 10*(ptrFechaBios[6] - '0') + (ptrFechaBios[7] - '0') ;
    if (anioBIOS == 99)              /* fecha del BIOS de qemu: "06/23/99" */
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
#if (FALSE) 
	sprintf(strArg, "TIMER %d &", ticsPorRodaja) ;
#else	
    strcpy(strArg, "TIMER X &") ;
//#if (ticsPorRodaja < 10)
#if (FALSE)
    strArg[6] = '0' + ticsPorRodaja ;
#else
    strArg[6] = '0' + (ticsPorRodaja / 10) ;
    strArg[7] = '0' + (ticsPorRodaja % 10) ;
#endif
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

    /* aqui ya se multiplexa la CPU entre los procesos no bloqueados       */

#endif

    /* mostrarFlags() ; */

    printStrBIOS("\n inicConsola(") ;
    printDecBIOS(numConsolas, 1) ;
    printStrBIOS(") ") ;

#if (CONRAT)
//  strcpy(strArg, "CONRAT X &") ;
//  strArg[7] = '0' + numConsolas ;
    sprintf(strArg, "CONRAT %d &", numConsolas) ;
    if ((pid = createProcess("CONRAT", "CONRAT&")) < 0)              /* GP */
    {
#else

    ll_buscarBloque(                                                 /* GM */
	    (((numConsolas+1)*sizeof(descConsola_t))+15)/16
	) ; 
    
  strcpy(strArg, "CONSOLA -q X &") ;
  strArg[11] = '0' + numConsolas ;
//    sprintf(strArg, "CONSOLA -q %d &", numConsolas) ;
    if ((pid = createProcess("CONSOLA", strArg)) < 0)                /* GP */
    {
#endif
        printStrBIOS(
            "\a\n fallo al arrancar la consola"
            "\n pid = "
        ) ;
        printIntBIOS(pid, 1) ;
    }
	
#if (TRUE)	
    strcpy(nombre, "CONX") ;
    for ( i = 0 ; i < numConsolas ; i++ )     /* esperamos a que terminen de */
    {                                              /* crearse las 6 consolas */
        nombre[3] = '0' + i ;
        esperarFichero(nombre) ;  
    }
#else	
    for ( i = 0 ; i < numConsolas ; i++ )     /* esperamos a que terminen de */
    {                                              /* crearse las 6 consolas */
	    sprintf(nombre, "CON%i", i) ;
        esperarFichero(nombre) ;
	}
#endif
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
//               /* de INIT (STDIN) que todavia no se ha asociado a CON0.    */
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
    strcpy(nombre, "CONX") ;
    for ( i = 1 ; i <= numConsolas/2 ; i++ )
    {
        nombre[3] = '0' + i ;
        close(STDIN) ;
        close(STDOUT) ;
        close(STDERR) ;
        open(nombre, O_RDONLY) ;
        open(nombre, O_WRONLY) ;
        open(nombre, O_WRONLY) ;
        if ((pid = createProcess("LOGIN", "LOGIN")) < 0)               /* GP */
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
    if ((pid = createProcess("RELOJ", "RELOJ -q &")) < 0)            /* GP */
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
    if (strncmp(ptrFechaBios, "01/01/99", 8))                  /* !hayVDos */
    {
        if ((pid = createProcess("RATON", "RATON -q &")) < 0)        /* GP */
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

#define ptrPant ((pantalla_t far *)pointer(0xB800, 0x0000))

    if ((df = open("TIMER", O_RDONLY)) >= 0)
    {
        for ( i = 20 ; i < 60 ; i++ )
        {
     		lseek(STDOUT, 21*80+i, SEEK_SET) ;              	
			putchar(0xFE) ;                           /* cuadrado pequenio */
//          pantallazo(ptrPant, 50, (char)254, atrNormal, 21, 20, 21, i) ;
            leer(df) ;   /* permite interrupciones mientras esta bloqueado */
            car = leerListo(STDIN);
            if (car != '\0')
            {
                if (car == ESC) break ;
                car = getchar() ;
                if (car == ESC) break ;
            }
        }
        close(df) ;
    }
    else
        printf("\a\n fallo al abrir el fichero TIMER") ;
    putchar('\f') ;

    mostrarSaludoGrande() ;

    if ((pid = createProcess("INFO", "info")) > 0)                   /* GP */
        waitpid(pid, (int far *)&status) ;                           /* GP */
    else
        printf("\n") ;

    printf("\n flags = %04X", valorFlags()) ;
    if (getpid() != 0)
    {
        putchar('\n') ;
        asm sti ;                         /* permitimos las interrupsiones */
    }
    else
    {
        printf(" se van a permitir las interrupciones ... \n") ;
        assert((valorFlags() & 0x0200) == 0x0000,
               "\n so1(): ERROR ints. no inhibidas") ;
        asm sti ;                         /* permitimos las interrupsiones */
        printf("\n flags = %04X interrupciones permitidas \n", valorFlags()) ;
        assert((valorFlags() & 0x0200) == 0x0200,
               "\a\n so1(): ERROR ints. no permitidas") ;
    }

    if ((pid = createProcess("CONSOLA", "consola -c 1")) > 0)        /* GP */
        waitpid(pid, (int far *)&status) ;                           /* GP */
    else
        putchar('\n') ;

#if (TRUE)

	
    strcpy(strArg, "INIC X") ;
	strArg[5] = '0' + numConsolas ;
//  sprintf(strArg, "INIC %i", numConsolas) ;

	exec("INIC", strArg) ;          /* resto del proceso de inicializacion */
    
	return(0) ;
	
#else 
	
    status = interpretarComandos() ;                              /* shell */

//    /* enviamos a todas las consolas el mensaje de terminacion inminente */

    timeout = status ;
    close(STDOUT) ;            /* no va a usarse mas por parte del proceso */

    if (timeout >= 0)
    {
        strcpy(nombre, "CONX") ;
        for ( i = 0 ; i < numConsolas ; i++ )
        {
            nombre[3] = '0' + i ;
            if ((dfCon = open(nombre, O_WRONLY)) > 0)
            {
                if (i > 0) putLn(dfCon) ;
                putStr(dfCon, "\n El sistema se cerrara en ") ;
                putDec(dfCon, timeout, 1) ;
                putStr(dfCon, " segundos ") ;
                close(dfCon) ;                  /* luego se vuelve a abrir */
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

    if (getppid() == 0) exit(timeout) ;                /* fin proceso INIT */

    return(timeout) ;                                  /* fin funcion inic */

#endif	
	
}