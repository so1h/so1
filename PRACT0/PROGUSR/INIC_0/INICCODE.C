/* ----------------------------------------------------------------------- */
/*                                iniccode.c                               */
/* ----------------------------------------------------------------------- */
/*                    codigo del proceso inicial de SO1                    */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\ll_s_so1.h>  /* exec, waitpid, getpid, open, yield, ... */
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
#define RETARDO TRUE                                       /*    TRUE      */ 
//#define RETARDO FALSE                                    /*    TRUE      */ 
#define TIMER   TRUE                                       /*    TRUE      */
//#define TIMER   FALSE                                    /*    TRUE      */
#define CONSOLA TRUE                                       /*    TRUE      */ 
#define RELOJ   TRUE                                       /*    TRUE      */ 
#define RATON   TRUE                                       /*    TRUE      */ 
//#define RATON   FALSE                                    /*    TRUE      */ 

void esperarFichero ( char far * nombre ) /* espera hasta abrir el fichero */
{
    int df ;

    while ((df = open(nombre, O_RDONLY)) < 0) 
    {
		printCarBIOS('.') ;
		yield() ;
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
    char strCmd [16] ;
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
	strcpy(strCmd, "RETARDO -q &") ; 
    if ((pid = createProcess("RETARDO", strCmd)) < 0)                /* GP */
        printStrBIOS(
		    strprintf(
                "\a\n fallo al arrancar RETARDO"
                "\n pid = %i", pid
		    )	
        ) ;
    printStrBIOS(strprintf("\n \"%s\"", strCmd)) ;
    esperarFichero("RETARDO") ;

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
#endif /* RETARDO */

#if (TIMER)
	sprintf(strCmd, "TIMER %i &", ticsPorRodaja) ;
    if ((pid = createProcess("TIMER", strCmd)) < 0) /* timer 18 & */ /* GP */
        printStrBIOS(
		    strprintf(
                "\a\n fallo al arrancar el timer"
                "\n pid = %i", pid
            ) 
		) ;
    printStrBIOS(strprintf(", \"%s\"", strCmd)) ;
    esperarFichero("TIMER") ;

    /* aqui ya se multiplexa la CPU entre los procesos no bloqueados       */

#endif /* TIMER */

    /* mostrarFlags() ; */

#if (CONSOLA) 

    buscarBloque(                                                    /* GM */
	    (((numConsolas+1)*sizeof(descConsola_t))+15)/16
	) ; 
    
    sprintf(strCmd, "CONSOLA -q %i &", numConsolas) ;
    printStrBIOS(strprintf(", \"%s\"\n ", strCmd)) ;

    if ((pid = createProcess("CONSOLA", strCmd)) < 0)                /* GP */
        printStrBIOS(
            strprintf(		    
                "\a\n fallo al arrancar la consola"
                "\n pid = ", pid
			) 
        ) ;	
//	printStrBIOS("\n creando consolas: ") ;
	printStrBIOS("\r creando consolas: ") ;    /* \r para ocultar la linea */
    for ( i = 0 ; i <= numConsolas ; i++ )  /* esperamos a que terminen de */
	{                                        /* crearse todas las consolas */
	    printStrBIOS(strprintf("CON%i ", i)) ;
    	esperarFichero(strprintf("CON%i", i)) ;                         
	}
#endif /* CONSOLA */

    /* aqui tenemos ya (6) consolas CON0, ... , CON5 disponibles para E/S  */
	/* no obstante seguimos utilizando printStrBIOS para que no se nos     */
	/* descoloque la pantalla inicial (nueva posicion del cursor).         */
	
    /* Tanto la llamada createProcess como la ejecucion del driver CONSOLA */
    /* necesitan asignar un bloque de memoria para el proceso creado o     */
    /* para las consolas mediante la llamada al sistema ll_buscarBloque    */
    /* que esta implementada mediante el fichero "GM" que se abre en ese   */
	/* momento con dfGM = open("GM", O_RDONLY) ; Como ese es el primer     */
	/* fichero que abre el proceso inicial (SO1) sabemos que el descriptor */
	/* devuelto es cero: fdGM = 0.                                         */

//  close(0) ; /* Cerramos "GM" para no interferir con la entrada estandar */
//             /* de INIT (STDIN) que todavia no se ha asociado a CON0.    */
//             /* !!!!!! YA NO ES NECESARO close(0), ya que ahora en esas  */
//             /* llamadas al sistema se halla allí ese close(df).         */ 

    assert(

        (open("CON0", O_RDONLY) == STDIN ) &&
        (open("CON0", O_WRONLY) == STDOUT) &&
        (open("CON0", O_WRONLY) == STDERR),

        "\n so1(): ERROR STDIN, STDOUT o STDERR"

    ) ;

    printStrBIOS("(CON0 E/S estandar)") ;
		
    /* aqui el proceso INIC ya utiliza CON0 como entrada/salida estandar   */

    /* while (TRUE) { car = leer(STDIN) ; putCar(STDOUT, car) ; }          */

    /* leer(STDIN) ; */  /* permite interrupciones mientras esta bloqueado */

//  printStrBIOS("\n arrancando procesos de login: ") ;
    printStrBIOS("\n") ;
	
    for ( i = 1 ; i <= numConsolas/2 ; i++ )   /* creacion procesos de login */
    {
        close(STDIN ) ;                /* cerramos los descriptores 0, 1 y 2 */
        close(STDOUT) ;                /* que estan asociados a CON0         */
        close(STDERR) ;

        sprintf(nombre, "CON%i", i) ; 

        open(nombre, O_RDONLY) ;       /* asociamos los descritores 0, 1 y 2 */
        open(nombre, O_WRONLY) ;       /* a CONi para que los procesos hijos */
        open(nombre, O_WRONLY) ;       /* trabajen con la consola CONi       */   
		
        sprintf(strCmd, "LOGIN &") ;
        if ((pid = createProcess("LOGIN", strCmd)) < 0)                /* GP */
            printStrBIOS(
			    strprintf(
				    "\a\n fallo al arrancar LOGIN en %s (pid = %i)", 
					nombre, pid
				) 
			) ;
        printStrBIOS(strprintf(" \"%s\" (%s)", strCmd, nombre)) ;		
        if (i < numConsolas/2) printStrBIOS(",") ;
    }

    /* aqui, ya tenemos logines (LOGIN.BIN) en CON1, CON2, CON3            */

    close(STDIN ) ;          /* recuperamos CON0 como e/s estandar de INIC */
    close(STDOUT) ;
    close(STDERR) ;
    open("CON0", O_RDONLY) ;
    open("CON0", O_WRONLY) ;
    open("CON0", O_WRONLY) ;

    printStrBIOS("\n arrancando drivers: ") ;	
	
#if (RELOJ)
    sprintf(strCmd, "RELOJ -q &") ;
    if ((pid = createProcess("RELOJ", strCmd)) < 0)                  /* GP */
        printStrBIOS(
		    strprintf(
                "\a\n fallo al arrancar el reloj"
                "\n pid = %i", pid
			)
        ) ;
    printStrBIOS(strprintf("\"%s\"", strCmd)) ;		
    esperarFichero("RELOJ") ;
#endif

#if (RATON)
    if (strncmp(ptrFechaBios, "01/01/99", 8))                /* no hayVDos */
    {
		sprintf(strCmd, "RATON -q &") ;
        if ((pid = createProcess("RATON", strCmd)) < 0)              /* GP */
            printStrBIOS(
			    strprintf(
                    "\a\n fallo al arrancar el raton"
                    "\n pid = %i", pid
				)
            ) ;
        printStrBIOS(strprintf(", \"%s\" ", strCmd)) ;		
        esperarFichero("RATON") ;
    }
#endif

    /* mostramos una barra de progreso del proceso INIC                    */           

//#define ptrPant ((pantalla_t far *)pointer(0xB800, 0x0000))

    if ((df = open("TIMER", O_RDONLY)) >= 0)
    {
        for ( i = 20 ; i < 60 ; i++ )
        {
     		lseek(STDOUT, 21*80+i, SEEK_SET) ;        /* fila 21 columna i */         	
			putchar(0xFE) ;                           /* cuadrado pequenio */
//          pantallazo(ptrPant, 50, (char)254, atrNormal, 21, 20, 21, i) ;
            leer(df) ;   /* permite interrupciones mientras esta bloqueado */
            car = leerListo(STDIN);        /* para poder parar el progreso */
            if (car != '\0')
            {
                if (car == ESC) break ;  /* tecla Esc ==> salida inmediata */
                car = getchar() ;
                if (car == ESC) break ;  /* tecla Esc ==> salida inmediata */
            }
        }
        close(df) ;
    }
    else {
        printf("\a\n fallo al abrir el fichero TIMER") ;
		getchar() ;
	}
	
    putchar('\f') ;                                /* borramos la pantalla */

    mostrarSaludoGrande() ;                                         /* SO1 */

    if ((pid = createProcess("INFO", "info")) > 0)                   /* GP */
        waitpid(pid, (int far *)&status) ;                           /* GP */
    else
        putchar('\n') ;

    printf("\n flags = %04X\n", valorFlags()) ;
	
	/* conmutamos a la consola CON1 con el comando "consola -c 1"          */
	
    if ((pid = createProcess("CONSOLA", "consola -c 1")) > 0)        /* GP */
        waitpid(pid, (int far *)&status) ;                           /* GP */
    else
        putchar('\n') ;

    /* damos paso a la siguiente fase del proceso inicial (INIC)           */

	exec("INIC", strprintf("INIC %i", numConsolas)) ;        
    
	return(_AX) ;           /* por poner algo ya que nunca va a ejecutarse */	
}