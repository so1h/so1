/* ----------------------------------------------------------------------- */
/*                                  so1.c                                  */
/* ----------------------------------------------------------------------- */
/*                       Un primer sistema operativo                       */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\ll_s_exc.h>  /* createProcess, waitpid, exit, open, ... */
#include <so1pub.h\strings.h>                           /* strcpy, strlcpy */
#include <so1pub.h\escribir.h>     /* escribirStr, escribirInt, escribirLn */
#include <so1pub.h\biosdata.h>                             /* ptrFechaBios */
#include <so1pub.h\msdos.h>                    /* finProgDOS, hayWindowsNT */
#include <so1pub.h\pic.h>                       /* valorIMR, establecerIMR */
#include <so1pub.h\telon.h>           /* (salvar/restaurar)PantallaInicial */
#include <so1pub.h\debug.h>            /* assert, mostrarFlags, valorFlags */
#include <so1.h\bios.h>                   /* print(Ln/Str/Hex/Dec/Ptr)BIOS */
#include <so1.h\interrup.h>         /* inicTVI, (redirigir/restablecer)Int */
#include <so1.h\llamadas.h>                                     /* isr_SO1 */
#include <so1.h\gm.h>          /* inicGM, k_buscarBloque, k_devolverBloque */
//#include <so1.h\gp.h>                                        /* k_inicGP */
#include <so1.h\db.h>                                            /* inicDB */
#include <so1.h\recursos.h>                   /* inicRecursos, destruirRec */
#include <so1.h\procesos.h>      /* descProceso, descRecurso, inicProcesos */ /* c2cPFR */
//#include <so1.h\main.h>                                          /* main */
#include <so1.h\minifs.h>              /* inicMinisfMSDOS, inicMinisfFAT12 */
#include <so1.h\sf.h>        /* inicSF, inicTablaFichAbiertos, segBuferSO1 */
#include <so1.h\plot.h>                                         /* finPlot */
#include <so1.h\ajustes.h>   /* modoSO1, guardarDS_SO1, IMRInicial, CS_SO1 */
#include <so1.h\so1dbg.h>    /* inicTeclado, leerScancode, esperarScancode */
#include <so1.h\s0.h>                    /* mirarLoQueHay, MostrarLoQueHay */
//       /* hayDOS, hayNT, hayQemu, hayBochs, hayDBox, hayNTVDM, hayFake86 */

/* CON_PROCESO_INICIAL permite configurar el sistema para que lleve a cabo */
/* las inicializaciones por medio de un proceso de usuario (INIT), en vez  */
/* de realizarse dichas inicializaciones mediante codigo del sistema SO1,  */
/* lo que permite reducir el tamanio del codigo del sistema SO1, dado que  */
/* evitamos tener que icluir varias bibliotecas en el.                     */

#define CON_PROCESO_INICIAL TRUE

#if (!CON_PROCESO_INICIAL)
#include <so1pub.h\interpre.h>                      /* interpretarComandos */
#include <so1.h\iniccode.h>                                        /* init */
#include <so1pub.h\saludos.h>                             /* mostrarSaludo */
#endif

#define pSB(x) printStrBIOS(x)                                    /* macro */
//#define E(x) x
#define E(x) { pSB("\n ") ; pSB(#x) ; pSB(" ... ") ; x ; }        /* macro */

void tirarSistema ( word_t loQueHay, int timeout ) ;

//          /* main es llamado desde la funcion startCabecera en ajustes.c */

//  /* en un primer momento solo podemos hacer llamadas al BIOS, ya que no */
//  /* tiene por que haber MSDOS o WINDOWS y SO1 no esta instalado todavia */

void main ( void )                             /* interrupciones inhibidas */
{

    pid_t pid ;
    pindx_t pindx ;
    int status ;
//  pid_t pidServidorGP ;
    word_t loQueHay ;

    /* --- cuatro formas distintas de esperar la pulsacion de una tecla ---*/
    /* leerTeclaBIOS() ; asm sti ; /* leerTeclaBIOS deja permitidas ints.  */ /* 1 */
    /* asm sti ; while (!(leerTeclaListaBDA() >> 8)) ; asm cli ;           */ /* 2 */
    /* inicTeclado() ; while (!leerScancodeListo()) ; *//* cualquier tecla */ /* 3 */
    /* inicTeclado() ; esperarScancode() ; */           /* cualquier tecla */ /* 4 */

    inicTeclado() ;

    salvarPantallaInicial() ;

    mirarLoQueHay((word_t *)&loQueHay) ;
    mostrarLoQueHay(loQueHay) ;

    switch (modoSO1())
    {
    case modoSO1_Bin : /* SO1.BIN arrancado desde un disco/disquete (BIOS) */
    case modoSO1_Exe :                                          /* SO1.EXE */
    case modoSO1_Com :                                          /* SO1.COM */
    case modoSO1_Bs  :                   /* SO1.BIN arrancado con SYSLINUX */
        mostrarFlags() ;
        printStrBIOS("=> SO1 v1.4.1 (C) Pedro Pablo Lopez Rodriguez") ;
        printLnBIOS() ;
        assert((valorFlags() & 0x0200) == 0x0000,
               "\a\n so1(): ERROR ints. no inhibidas") ;    /* '\a' == BEL */
    }

    /* cs y ds ya tienen los valores ajustados al programa SO1 (ajustes.c) */

    asm
    {
        mov CS_SO1,cs ;                       /* segmento de codigo de SO1 */
        mov DS_SO1,ds ;                        /* segmento de datos de SO1 */
    }

    guardarDS_SO1() ; /* necesario para conocer DS_SO1 desde ints. y excs. */

    E(inicRecursos()) ;
    E(inicProcesos()) ; /* SO1 y proc. 0 mismo espacio de direccionamiento */

    descProceso[indProcesoActual].uid = 0 ;                        /* root */
    descProceso[indProcesoActual].gid = 0 ;                        /* root */

    E(inicGM()) ;         /* asigna memoria para el proceso 0. rec_gm "GM" */

    E(inicDB()) ;                                           /* rec_db "DM" */

    if ((modoSO1() == modoSO1_Bin) || (modoSO1() == modoSO1_Bs))
    {
        printStrBIOS("\n unidadBIOS = 0x") ;
        printHexBIOS(unidadBIOS(), 2) ;
        inicSF(unidadBIOS()) ;  /* asigna memoria a segBuferSO1 y FAT (GM) */
        assert(inicMinisfFAT() == 0, "\a\n so1(): ERROR minisfFAT") ;
        inicTablaFichAbiertos() ;
    }
    else
        assert(inicMinisfMSDOS() == 0, "\a\n so1(): ERROR minisfMSDOS") ;

    E(inicTVI()) ;    /* inicializamos la tabla de vectores de interrucion */

    IMRInicial = valorIMR() ;          /* tomamos nota del IMR (pic 8259A) */

    printStrBIOS("IMRInicial = 0x") ;
    printHexBIOS(IMRInicial, 4) ;

    /* establecemos el vector de interrupcion de llamadas al sistema SO1   */

    printStrBIOS("\n redirigirInt(nVIntSO1, ") ;
    printPtrBIOS(pointer(CS_SO1, (word_t)isr_SO1)) ;
    printStrBIOS(")") ;
    redirigirInt(nVIntSO1, (isr_t)pointer(CS_SO1, (word_t)isr_SO1)) ;
    printStrBIOS(" nVIntSO1 = 0x") ;
    printHexBIOS(nVIntSO1, 2) ;

//  pidServidorGP = inicGP() ;

    /***********************************************************************/
    /* Ahora ya podemos hacer llamadas al sistema SO1 y ejecutar programas */
    /***********************************************************************/

    /* Opciones de diseño:                                                 */
    /*   - mantener este proceso 0 como interprete de comandos (lo que     */
    /*       conlleva que el codigo del sistema ocupe mas memoria)         */
    /*   - mantener este proceso 0 como proceso nulo (por ejemplo con      */
    /*       un bucle infinito) tras lanzar el proceso inicial INIC.       */
    /*   - bloquear este proceso 0 por siempre.                            */
    /*                                                                     */
    /* Por otro lado podemos elegir si los drivers basicos: TIMER, CONSOLA,*/
    /* RETARDO, RELOJ y RATON se lanzan ahora o los lanza un proceso       */
    /* inicial INIC, y lo mismo con los SH que se ejecutan en las consolas.*/

    /* Opcion preferida: lanzar un unico proceso inicial INIT y dejar      */
    /* bloqueado a SO1 a la espera de la terminacion de ese proceso.       */
    /* Dejar que el proceso inicial lleve a cabo las inicializaciones.     */
    /* Con esto se reduce en gran medida el tamanio de SO1.BIN.            */

//  mostrarFlags() ;                                  /* para comprobacion */
    asm sti ;                             /* permitimos las interrupciones */
//  mostrarFlags() ;                                  /* para comprobacion */

#if (CON_PROCESO_INICIAL)          /* se utiliza un proceso inicial de SO1 */

    printStrBIOS("\n creando el proceso inicial INIC: \"inic\"") ;

    if ((pid = createProcess("INIC", "inic")) > 0)
    {
        /* inic */ /* GP */
        pindx = descProceso[indProcesoActual].c2cHijos.primero ;
        descProceso[pindx].uid = 1 ;
        descProceso[pindx].gid = 1 ;
        waitpid(pid, (int far *)&status) ;
    }
    else                                          /* createProcess fallido */
    {
        printStrBIOS("\a\n fallo al crear el proceso inicial: pid = ") ;
        printIntBIOS(pid, 1) ;
        printStrBIOS("\n pulse una tecla para continuar ") ;
        leerTeclaBIOS() ;
    }

#else              /* en este caso no se utiliza un proceso inicial de SO1 */
    if (inic()) != 0)                                  /* inicializaciones */
    {
        printStrBIOS("\a\n terminacion anomala de la funcion inic: err = ") ;
        printIntBIOS(err, 1) ;
        printStrBIOS("\n pulse una tecla para continuar ") ;
        leerTeclaBIOS() ;
    }
#endif

    tirarSistema(loQueHay, status) ;         /* ejecutado por el proceso 0 */

}

void tirarSistema ( word_t loQueHay, int timeout )
{

    int i, j ;
    int err ;
    pid_t pid ;
    pindx_t pindx ;
    rindx_t rindx ;
    char far * nombre ;
    char strComando [ 12 ] ;

#if (CON_PROCESO_INICIAL)

    /* La llamada al sistema createProcess utilizada para crear el proceso */
    /* inicial necesita asignar un bloque de memoria para el proceso creado*/
    /* mediante la llamada al sistema ll_buscarBloque que esta implementada*/
    /* a traves del fichero "GM" que se abre con dfGM = open("GM",O_RDONLY)*/
    /* Como GM es el primer fichero que abre SO1 sabemos que el descriptor */
    /* devuelto es el cero: fdGM = 0.                                      */

    close(0) ; /* Cerramos "GM" para no interferir con la entrada estandar */
    /* de SO1 (STDIN) que todavia no se ha asociado a CON0.     */

//  if (descProceso[0].nfa > 0) {
//      for ( i = 0 ; i < 2 ; i++ )
//      if (descProceso[0].tfa[i].dfs >= 0)
//          close(i) ;
//  }

    assert(

        (open("CON0", O_RDONLY) == STDIN) &&
        (open("CON0", O_WRONLY) == STDOUT),

        "\n so1(): ERROR STDIN o STDOUT"

    ) ;

#endif

    /* devolver bloque de memoria ocupado por el bufer seguro en su caso */

    /* devolver bloque de memoria ocupado por la FAT en su caso */

//  k_devolverBloque(seg((pointer_t)&FAT), (2*entradasFAT+15)/16) ; /* peligro */

    finPlot() ;

    escribirStr("\n\n") ;

    if ((pid = createProcess("PS", "ps")) > 0)
        waitpid(pid, (int far *)NULL) ;

    escribirStr("\n destruyendo procesos de usuario (no drivers) ... \n\n") ;

    for ( pindx = 1 ; pindx < maxProcesos ; pindx++ )
        if ((descProceso[pindx].estado != libre) &&
                (descProceso[pindx].esperandoPor != rec_desinstalacion))
        {
            pid = descProceso[pindx].pid ;
            escribirStr(" pindx = ") ;
            escribirInt(pindx, 1) ;
            escribirStr(" pid = ") ;
            escribirInt(pid, 1) ;
            escribirStr(" --> killpid(") ;
            escribirInt(pid, 1) ;
            escribirStr(") ") ;
            if (killpid(pid) == 0)
                escribirStr(" Ok ") ;
            escribirLn() ;
            if (timeout > 0) esperarTicsBIOS(18) ;
        }

    escribirLn() ;

    if ((pid = createProcess("PS", "ps")) > 0)
        waitpid(pid, (int far *)NULL) ;

    escribirStr("\n desinstalando drivers ... \n\n") ;

    if (timeout > 0) esperarTicsBIOS(3*18) ;

    for ( pindx = 1 ; pindx < maxProcesos ; pindx++ )
    {
        if ((descProceso[pindx].estado == bloqueado) &&
                (descProceso[pindx].esperandoPor == rec_desinstalacion))
        {
            strlcpy(strComando, descProceso[pindx].programa, 8) ;
            for ( i = 0 ; i < 8 ; i++ )
                if ((strComando[i] == (char)0) || (strComando[i] == '.'))
                {
                    for ( j = i ; j < 8 ; j++ )
                        strComando[j] = ' ' ;
                    break ;
                }
            strComando[ 8] = ' ' ;
            strComando[ 9] = '-' ;
            strComando[10] = 'u' ;
            strComando[11] = (char)0 ;
            escribirCar(' ') ;
            escribirStr((char *)strComando) ;
            escribirCar(':') ;
            if (timeout > 0) esperarTicsBIOS(18) ;
            if ((pid = createProcess(descProceso[pindx].programa, strComando)) > 0)
            {
                waitpid(pid, (int far *)NULL) ;
                killpid(descProceso[pindx].pid) ;
                escribirLn() ;
            }
        }
    }

    escribirStr("\n destruyendo recursos ... \n") ;

    if (timeout > 0) esperarTicsBIOS(3*18) ;

    rindx = c2cPFR[DROcupados].primero ;
    while (rindx != c2cPFR[DROcupados].cabecera)
    {
        nombre = (char far *)descRecurso[rindx].nombre ;
        pindx = descRecurso[rindx].pindx ;
        pid = descProceso[pindx].pid ;
        if (pindx > 0)
        {
            escribirStr("\n destruyendo recurso ") ;
            escribirInt(rindx, 1) ;
            escribirStr(" ") ;
            escribirStr((char *)nombre) ;
            escribirStr(" ...") ;
            rindx = c2cPFR[DROcupados].e[rindx].sig ;
//          err = destruirRecurso(nombre) ;          /* llamada al sistema */
            err = destruirRec(nombre) ;                 /* funcion interna */
            if (err == 0)
            {
                if (pid > 0) waitpid(pid, (int far *)NULL) ;
                escribirStr(" Ok ") ;
            }
            else
            {
                escribirStr(" error ") ;
                escribirInt(err, 1) ;
            }
            if (timeout > 0) esperarTicsBIOS(18) ;
        }
        else
            rindx = c2cPFR[DROcupados].e[rindx].sig ;
    }

    escribirStr("\n\n") ;

    if ((pid = createProcess("PS", "ps")) > 0)
        waitpid(pid, (int far *)NULL) ;

    asm cli ;

    escribirStr("\n Presione una tecla para terminar ") ;

    esperarScancode() ;

    for ( i = 0 ; i < nVIntMax ; i++ )
        restablecerInt(i) ;

    establecerIMR(IMRInicial) ;        /* mascara de interrupcion del 8259 */

    restaurarPantallaInicial() ;

    tirarS0(loQueHay) ;

}

