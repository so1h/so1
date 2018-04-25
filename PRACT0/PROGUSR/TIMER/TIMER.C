/* ----------------------------------------------------------------------- */
/*                                 timer.c                                 */
/* ----------------------------------------------------------------------- */
/*             implementacion del recurso TIMER (temporizador)             */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\ajustusr.h>          /* save_DS0, setraw_DS, restore_DS */
#include <so1pub.h\comundrv.h>  /* ptrIndProcesoActual, comprobarAmpersand */
#include <so1pub.h\ll_s_so1.h>    /* biblioteca de llamadas al sistema SO1 */
#include <so1pub.h\stdio.h>                                      /* printf */
#include <so1pub.h\ctype.h>                                     /* toupper */
#include <so1pub.h\strings.h>                           /* strcpy, strcmpu */
#include <so1pub.h\scanner.h>               /* inicScanner, obtenSimb, ... */
#include <so1pub.h\pic.h>                                     /* IRQ_TIMER */
#include <so1pub.h\def_timer.h>                            /* argCbTimer_t */

#define REDUCIR_DRIVER TRUE     /* TRUE ==> reducimos la memoria utilizada */
//#define REDUCIR_DRIVER FALSE  

#pragma option -w-use /* (comundrv.h) warning: 'x' declared but never used */

rindx_t rec_timer = 0 ;             /* inicializacion provisional --> DATA */ /* se necesita en readTimer */

/* ----------------------------------------------------------------------- */
/*                   seccion de implementacion del driver                  */
/* ----------------------------------------------------------------------- */

#define nVIntTimer   0x08       /* numero de vector utilizado por el timer */

void far isr_timer ( void ) ;                                   /* forward */

dobleEnlace_t eTimer [ maxProcesos + 1 ] = { { 0, 0 } } ;          /* DATA */

c2c_t colaTimer = { 0, 0, 0, NULL } ;                              /* DATA */

int ticsPorLeer [ maxProcesos ] = { 0 } ;                 /* tics por leer */

pointer_t dirDestino [ maxProcesos ] =         /* dir. donde dejar sig tic */
{
    NULL                                                           /* DATA */
} ;

#define ticsPorRodajaMax 180              /* maximo valor de ticsPorRodaja */

#define ticsPorRodajaPorDefecto 18                    /* valor por defecto */

word_t ticsPorRodaja = 0 ;                                         /* DATA */

word_t ticsVerReloj  = 4 ;                                         /* DATA */

word_t contTicsReloj = 0 ;                    /* 0, 1, 2, ... ticsVerReloj */

//char ocioso [4] = { '|', '/', '-', '\\' } ;                      /* DATA */

dword_t contTics = 0 ;                                             /* DATA */

/* ----------------------------------------------------------------------- */

#pragma warn -par

int far openTimer ( int dfs, modoAp_t modo )
{
    return(dfs) ;
}

int far releaseTimer ( int dfs )
{
    return(0) ;
}

#pragma warn-rvl

int far readTimer ( int dfs, pointer_t dir, word_t nbytes )
{
    pindx_t indProcesoActual ;

    save_DS0() ;                            /* guarda el DS anterior (SO1) */
    setraw_DS() ;           /* establece el DS correspondiente al programa */

    indProcesoActual = *ptrIndProcesoActual ;

    /* El proceso se bloquea a la espera de que transcurran nbytes tics    */
    /* Si (dir != NULL) para cada siguiente tic i                          */
    /*   se guarda en dir[i] el bytes (contTics % 256)                     */

    ticsPorLeer[indProcesoActual] = nbytes ;
    if (FP_SEG(dir) == ptrDescProceso[indProcesoActual].CSProc)
        dirDestino[indProcesoActual] = dir ;
    else if (dir != NULL)
        dirDestino[indProcesoActual] = dir ;
    else
        dirDestino[indProcesoActual] = dir ;      /* solo espera sin copia */
    encolarPC2c(indProcesoActual, (ptrC2c_t)&colaTimer) ;
    bloquearProcesoActual(rec_timer) ;                       /* no retorna */
}

#define tamBuf 12

char buf [ tamBuf ] = { '\0' } ; /* no poner como variable local *//* DATA */

int far aio_readTimer ( int dfs, pointer_t dir, word_t nbytes )
{
    /* El proceso lee los bytes que solicita del "fichero" virtual:        */
    /*                                                                     */
    /*   TTTTTTTTRRRRRRRRCCCCXXXX (digitos hexadecimales)                  */
    /*                                                                     */
    /* donde TTTTTTTT son los 4 bytes de contTics                          */
    /* donde RRRRRRRR son los 4 bytes de contRodajas                       */
    /* donde CCCC     son los 2 bytes de contTicsRodaja                    */
    /* donde XXXX     son los 2 bytes de ticsPorRodaja                     */

    int i, j ;
    int df ;
    word_t pos ;
    word_t posNueva ;

    save_DS0() ;                            /* guarda el DS anterior (SO1) */
    setraw_DS() ;           /* establece el DS correspondiente al programa */

    df = (*ptrTramaProceso)->BX ;
    pos = (word_t)ptrDescProceso[*ptrIndProcesoActual].tfa[df].pos ;

    if (pos >= tamBuf) nbytes = 0 ;
    else
    {
        *((dword_t far *)&(buf[ 0])) = contTics ;              /* contTics */
        *((dword_t far *)&(buf[ 4])) = *ptrContRodajas ;    /* contRodajas */
        *((int     far *)&(buf[ 8])) = *ptrContTicsRodaja ;/*contTicsRodaja*/
        *((word_t  far *)&(buf[10])) = ticsPorRodaja ;    /* ticsPorRodaja */
        posNueva = pos + nbytes ;
        if (posNueva > tamBuf)
        {
            posNueva = tamBuf ;
            nbytes = posNueva - pos ;
        }
        j = 0 ;
        for ( i = pos ; i < posNueva ; i++ )             /* primeros bytes */
            dir[j++] = buf[i] ;
    }

    restore_DS0() ;                               /* restaura el DS de SO1 */
    return(nbytes) ;
}

int far writeTimer ( int dfs, pointer_t dir, word_t nbytes )
{
    return(0) ;
}

int far aio_writeTimer ( int dfs, pointer_t dir, word_t nbytes )
{
    /* El proceso escribe los bytes en el "fichero" virtual:               */
    /*                                                                     */
    /*   TTTTTTTTRRRRRRRRCCCCXXXX (digitos hexadecimales)                  */
    /*                                                                     */
    /* donde TTTTTTTT son los 4 bytes de contTics                          */
    /* donde RRRRRRRR son los 4 bytes de contRodajas                       */
    /* donde CCCC     son los 2 bytes de contTicsRodaja                    */
    /* donde XXXX     son los 2 bytes de ticsPorRodaja                     */

    int i, j ;
    int df ;
    word_t pos ;
    word_t posNueva ;

    dword_t contTicsAux ;
    dword_t contRodajasAux ;
    int contTicsRodajaAux ;
    word_t ticsPorRodajaAux ;

    save_DS0() ;                            /* guarda el DS anterior (SO1) */
    setraw_DS() ;           /* establece el DS correspondiente al programa */

    df = (*ptrTramaProceso)->BX ;
    pos = (word_t)ptrDescProceso[*ptrIndProcesoActual].tfa[df].pos ;

    if (pos >= tamBuf) nbytes = 0 ;
    else
    {
        *((dword_t far *)&(buf[ 0])) = contTics ;              /* contTics */
        *((dword_t far *)&(buf[ 4])) = *ptrContRodajas ;    /* contRodajas */
        *((int     far *)&(buf[ 8])) = *ptrContTicsRodaja ;/*contTicsRodaja*/
        *((word_t  far *)&(buf[10])) = ticsPorRodaja ;    /* ticsPorRodaja */
        posNueva = pos + nbytes ;
        if (posNueva > tamBuf)
        {
            posNueva = tamBuf ;
            nbytes = posNueva - pos ;
        }
        j = 0 ;
        for ( i = pos ; i < posNueva ; i++ )
            buf[i] = dir[j++];

        contTicsAux       = *((dword_t far *)&(buf[ 0])) ;     /* contTics */
        contRodajasAux    = *((dword_t far *)&(buf[ 4])) ;  /* contRodajas */
        contTicsRodajaAux = *((int     far *)&(buf[ 8])) ;/*contTicsRodaja */
        ticsPorRodajaAux  = *((word_t  far *)&(buf[10])) ;/* ticsPorRodaja */

        if ((-1 <= contTicsRodajaAux) &&           /* comprobar correccion */
                (0 < ticsPorRodajaAux) &&
                (ticsPorRodajaAux <= ticsPorRodajaMax))
        {
            contTics           = contTicsAux ;     /* poner nuevos valores */
            *ptrContRodajas    = contRodajasAux ;
            if (contTicsRodajaAux < ticsPorRodajaAux)
                *ptrContTicsRodaja = contTicsRodajaAux ;
            else
                *ptrContTicsRodaja = -1 ;
            ticsPorRodaja      = ticsPorRodajaAux ;
        }
        else nbytes = -1 ;
    }
    restore_DS0() ;                               /* restaura el DS de SO1 */
    return(nbytes) ;
}

long far lseekTimer ( int dfs, long pos, word_t whence )
{
    int df ;
    long res = -1L ;
    long posNueva ;

    save_DS0() ;                            /* guarda el DS anterior (SO1) */
    setraw_DS() ;           /* establece el DS correspondiente al programa */

    switch (whence)
    {
    case SEEK_SET :
        if ((0 <= pos) && (pos <= (long)tamBuf))
            res = pos ;
        break ;
    case SEEK_CUR :
        df = (*ptrTramaProceso)->BX ;
        posNueva = ptrDescProceso[*ptrIndProcesoActual].tfa[df].pos + pos ;
    case SEEK_END :
        if (whence == SEEK_END) posNueva = (long)tamBuf + pos ;
        if (posNueva < 0L)
            res = 0 ;
        else if (posNueva > (long)tamBuf)
            res = (long)tamBuf ;
        else
            res = posNueva ;
        break ;
    default :
        ;
    }

    restore_DS0() ;                               /* restaura el DS de SO1 */
    return(res) ;
}

int far fcntlTimer ( int dfs, word_t cmd, word_t arg )
{
    return(0) ;
}

int far ioctlTimer ( int dfs, word_t request, word_t arg )
{
    return(0) ;
}

int far eliminarTimer ( pindx_t pindx )
{
    save_DS0() ;                            /* guarda el DS anterior (SO1) */
    setraw_DS() ;           /* establece el DS correspondiente al programa */

    if (estaPC2c(pindx, (ptrC2c_t)&colaTimer))
        eliminarPC2c(pindx, (ptrC2c_t)&colaTimer) ;

    restore_DS0() ;                               /* restaura el DS de SO1 */
    return(0) ;
}

#pragma warn +par

/* ----------------------------------------------------------------------- */
/*                  rutina de servicio de la interrupcion                  */
/* ----------------------------------------------------------------------- */

void procesarTicColaTimer ( void )
{
    int i ;
    pindx_t pindx ;
    for ( i = 0 ; i < colaTimer.numElem ; i++ )
    {
        pindx = desencolarPC2c(&colaTimer) ;
        if (dirDestino[pindx] != NULL)
            *(dirDestino[pindx])++ = (char)(contTics % 256) ;
        if (--(ticsPorLeer[pindx]) <= 0)
        {
            ptrDescProceso[pindx].estado = preparado ;
            encolarPC2c(pindx, (ptrC2c_t)&ptrC2cPFR[PPreparados]) ;
        }
        else
            encolarPC2c(pindx, (ptrC2c_t)&colaTimer) ;
    }
}

#define maxCbTM 6

argCbTimer_t argCbTimer = { 0L, 0L, (int)0, (unsigned)0 } ;       /* DATA */

descCcb_t descCcbTM = { 0, 0, 0, maxCbTM, NULL,          /* arg pendiente */
    { cbNulo, cbNulo, cbNulo, cbNulo, cbNulo, cbNulo }
} ;                                                               /* DATA */

void far isr_timer ( )
{
    pindx_t indProcesoActual ;

    save_DS0() ;                            /* guarda el DS anterior (SO1) */
    setraw_DS() ;           /* establece el DS correspondiente al programa */

    indProcesoActual = *ptrIndProcesoActual ;

//  plot('t', 0, contadorTimer0()) ; */  /* mejor antes de llamar rti bios */

    /* simulamos la interrupcion con el vector guardado */
    /* necesario para el timeout de leerSectorBIOS */
    /* int 13h con ah = 02h */

    ptrVIOrg[nVIntTimer]() ; /* (pushf + int) da la int. por servida (pic) */

    /*  printCarBIOS('.') ; */
    /*  printStrBIOS("\n contTics = ") ; */
    /*  printLDecBIOS(contTics, 5) ; */

    contTics++ ;

    argCbTimer.contTics = contTics ;
    argCbTimer.contRodajas = *ptrContRodajas ;
    argCbTimer.contTicsRodaja = *ptrContTicsRodaja ;
    argCbTimer.ticsPorRodaja = ticsPorRodaja ;

    atenderCcb((ccb_t)&descCcbTM) ;               /* problema si cambia DS */

    procesarTicColaTimer() ;

    if (indProcesoActual != -1)           /* se esta ejecutando un proceso */
    {
        if (++(*ptrContTicsRodaja) == ticsPorRodaja)
        {
            if ((ptrC2cPFR[PPreparados].numElem > 0) &&
                    (*ptrNivelActivacionSO1 == 1))           /* 17-10-2010 */
            {
                ptrDescProceso[indProcesoActual].trama =
                    (trama_t far *)MK_FP(*ptrSS_Proceso, *ptrSP_Proceso) ;
                ptrDescProceso[indProcesoActual].estado = preparado ;
                encolarPC2c(indProcesoActual, (ptrC2c_t)&ptrC2cPFR[PPreparados]) ;
                *ptrHayTic = TRUE ;
                buscarNuevoProcesoActual() ;              /* hayTic = TRUE */
            }
            else
            {
                (*ptrContRodajas)++ ;
                *ptrContTicsRodaja = 0 ;
            }
        }
//      plot('e', 0, contadorTimer0()) ;        /* no hay cambio de estado */
    }
    else                                         /* indProcesoActual == -1 */
    {
        if (ptrC2cPFR[PPreparados].numElem > 0) /* va a haber cambio de estado */
            ptrDescProceso[ptrC2cPFR[PPreparados].primero].estado = ejecutandose ;     /* truco */
//      plot('e', 0, contadorTimer0()) ;
        if (ptrC2cPFR[PPreparados].numElem > 0)
            ptrDescProceso[ptrC2cPFR[PPreparados].primero].estado = preparado ;
    }

    restore_DS0() ;                               /* restaura el DS de SO1 */
}

int finishTimer ( void )
{
	asm sti ;      /* ya que se bloqueo al proceso con sus ints. inhibidas */
	                                     /* antes de esperarDesinstalacion */
//  exit(0) ;              /* obligaria a meter la biblioteca ll_s_so1.lib */
    return(0) ;
}

void finCodeDriver ( void )   /* marca el fin del codigo propio del driver */
{
}

/* ----------------------------------------------------------------------- */
/*                      seccion de programa de usuario                     */
/* ----------------------------------------------------------------------- */

void mostrarFormato ( void )
{
    printf(" formato: TIMER [ -i [ tpr ] | -q | -u | -k | -s | -c tpr | -h | tpr ] ") ;
}

int formato ( void )
{
    mostrarFormato() ;
    return(-1) ;
}

int help ( void )
{
    printf("\n\n") ;
    mostrarFormato() ;
    printf(
        ""                                                               "\n"
        ""                                                               "\n"
        " instala/desinstala el driver del timer"                        "\n"
        ""                                                               "\n"
        " opciones: (por defecto -i y tpr = 18)"                         "\n"
        ""                                                               "\n"
        "     tpr : establece los tics por rodaja"                       "\n"
        "      -i : instala el driver (usar &)"                          "\n"
        "      -q : instala sin mensajes de salida (&)"                  "\n"
        "      -u : desintala el driver"                                 "\n"
        "      -k : desintala el driver (matando)"                       "\n"
        "      -s : muestra el estado del timer"                         "\n"
        "      -c : cambia los tics por rodaja"                          "\n"
        "      -h : muestra este help"                                   "\n"
    ) ;
    return(0) ;
}

int integrarTimer ( word_t ticsPR, bool_t conMensajes )
{
    int i ;
    dfs_t dfs ;
    descRecurso_t dR ;
    word_t nuevoDS ;

    if (ticsPR > ticsPorRodajaMax)
    {
        printf(" ticsPorRodaja demasiado grande (%i)", ticsPR) ;
        return(-1) ;
    }

    dR.tipo = rDCaracteres ;
    strcpy(dR.nombre, "TIMER") ;
//  dR.ccb = (ccb_t)&descCcbTM ;
    dR.ccb = MK_FP(_DS, FP_OFF(&descCcbTM)) ;        /* problema si cambia DS */
    dR.ccb->arg = MK_FP(_DS, FP_OFF(&argCbTimer)) ;  /* problema si cambia DS */
    dR.pindx = getpindx() ;
    dR.numVI = 1 ;
    dR.nVInt[0] = nVIntTimer ;
    dR.irq[0] = IRQ_TIMER ;
    dR.isr[0] = (isr_t)MK_FP(_CS, FP_OFF(isr_timer)) ;

    dR.open      = (open_t)     MK_FP(_CS, FP_OFF(openTimer)) ;
    dR.release   = (release_t)  MK_FP(_CS, FP_OFF(releaseTimer)) ;
    dR.read      = (read_t)     MK_FP(_CS, FP_OFF(readTimer)) ;
    dR.aio_read  = (aio_read_t) MK_FP(_CS, FP_OFF(aio_readTimer)) ;
    dR.write     = (write_t)    MK_FP(_CS, FP_OFF(writeTimer)) ;
    dR.aio_write = (aio_write_t)MK_FP(_CS, FP_OFF(aio_writeTimer)) ;
    dR.lseek     = (lseek_t)    MK_FP(_CS, FP_OFF(lseekTimer)) ;
    dR.fcntl     = (fcntl_t)    MK_FP(_CS, FP_OFF(fcntlTimer)) ;
    dR.ioctl     = (ioctl_t)    MK_FP(_CS, FP_OFF(ioctlTimer)) ;

    dR.eliminar  = (eliminar_t) MK_FP(_CS, FP_OFF(eliminarTimer)) ;

    rec_timer = crearRecurso(&dR) ;

    if (rec_timer >= 0)
    {
        dfs = crearFichero("TIMER", rec_timer, 0, fedCaracteres) ;

        if (dfs >= 0)
        {
            inicPC2c(&colaTimer, &eTimer, maxProcesos + 0, FALSE) ;
            for ( i = 0 ; i < maxProcesos ; i ++ ) ticsPorLeer[i] = 0 ;
            ticsPorRodaja = ticsPR ;
            if (conMensajes)
                printf(
                    ""                                                   "\n"
                    ""                                                   "\n"
                    " recurso TIMER instalado (fichero: TIMER)"          "\n"
                    ""                                                   "\n"
                    " ticsPorRodaja = %i"                                "\n",
                    ticsPorRodaja
                ) ;
            return(0) ;
        }
        switch(dfs)
        {
        case -1 : printf(" tipo de fichero erroneo") ; break ;
        case -2 : printf(" numero de recurso erroneo") ; break ;
        case -3 : printf(" TIMER nombre de fichero ya existente") ; break ;
        case -4 : printf(" no hay descriptores de fichero libres") ; break ;
        default : printf(" no ha podido crearse el fichero TIMER") ;
        }
        destruirRecurso("TIMER", TRUE) ;                        /* matando */
        return(0) ;
    }
    switch(rec_timer)
    {
    case -1 : printf(" tipo de recurso erroneo") ; break ;
    case -2 : printf(" demasiados vectores de interrupcion") ; break ;
    case -3 : printf(" TIMER nombre de recurso ya existente") ; break ;
    case -4 : printf(" no hay descriptores de recurso libres") ; break ;
    case -5 : printf(" numero de vector de interrupcion ya usado") ; break ;
    default : printf(" no ha podido crearse el recurso TIMER") ;
    }
    return(-1) ;
}

int desintegrarTimer ( void )
{
    return(0) ;
}

int instalarTimer ( word_t ticsPR, bool_t conMensajes )
{
    int res ;
    res = comprobarAmpersand() ;
    if (res != 0) return(res) ;
    obtenInfoSO1(dirDescSO1) ;               /* obtenemos los datos de SO1 */
    res = integrarTimer(ticsPR, conMensajes) ;
    if (res != 0) return(res) ;
#if (!REDUCIR_DRIVER)
    esperarDesinstalacion(0) ;                       /* bloquea el proceso */
#else
	asm cli ;                              /* inhibimos las interrupciones */
    colaTimer.e =               /* actualizamos el segmento en colaTimer.e */
	    MK_FP(
		    _CS + (((word_t)finCodeDriver + 0x000F) >> 4),
            FP_OFF(colaTimer.e)
        ) ;			
    esperarDesinstalacion(                           /* bloquea el proceso */
//      FP_OFF(dirDescSO1) + sizeof(descSO1_t)
//          + sizeof(rec_timer)                /* se necesita en readTimer */
//          + sizeof(eTimer) + sizeof(colaTimer)
//          + sizeof(ticsPorLeer) + sizeof(dirDestino)
//          + sizeof(ticsPorRodaja) + sizeof(ticsVerReloj)
//          + sizeof(contTicsReloj) + sizeof(contTics)
//          + sizeof(buf)
//          + sizeof(argCbTimer)
//          + sizeof(descCcbTM) + maxCbTM*sizeof(callBack_t),   /* tamDATA */
        FP_OFF(&descCcbTM)
            + sizeof(descCcbTM) + maxCbTM*sizeof(callBack_t),   /* tamDATA */
        FP_OFF(finCodeDriver),                            /* finCodeDriver */
        FP_OFF(finishTimer),                               /* finishDriver */
		0x0000                                                  /* tamPila */ 
    ) ;
/*  se retorna a finishTimer */		
#endif
/*  solo se llega aqui en el caso esperarDesinstalacion(0) */
    res = desintegrarTimer() ;
    return(res) ;
}

int main ( int argc, char * argv [ ] )
{
    int res ;
    int dfTimer ;
    dword_t contTics ;
    dword_t contRodajas ;
    int contTicsRodaja ;
    word_t ticsPorRodaja ;
    if (argc > 3)       return(formato()) ;
    else if (argc == 1) return(instalarTimer(ticsPorRodajaPorDefecto, TRUE)) ;
    else if (argc == 2)
    {
        if (!strcmpu(argv[1], "-h"))      return(help()) ;
        else if (!strcmpu(argv[1], "-i")) return(instalarTimer(ticsPorRodajaPorDefecto, TRUE)) ;
        else if (!strcmpu(argv[1], "-q")) return(instalarTimer(ticsPorRodajaPorDefecto, FALSE)) ;
        else if (!strcmpu(argv[1], "-s"))
        {
            dfTimer = open("TIMER", O_RDONLY) ;
            if (dfTimer < 0)
            {
                printf(" recurso TIMER no instalado todavia ") ;
                return(-1) ;
            }
            else
            {
                aio_read(dfTimer, (pointer_t)&contTics, 4) ;
                aio_read(dfTimer, (pointer_t)&contRodajas, 4) ;
                aio_read(dfTimer, (pointer_t)&contTicsRodaja, 2) ;
                aio_read(dfTimer, (pointer_t)&ticsPorRodaja, 2) ;
                close(dfTimer) ;
                printf(
                    ""                                                                         "\n"
                    ""                                                                         "\n"
                    " contTics = %lu contRodajas = %lu contTicsRodaja = %i ticsPorRodaja = %u" "\n",
                    contTics, contRodajas, contTicsRodaja, ticsPorRodaja
                ) ;
                return(0) ;
            }
        }
        else if ((!strcmpu(argv[1], "-u")) || 
                 (!strcmpu(argv[1], "-k")))
        {
            res = destruirRecurso("TIMER", tolower(argv[1][1]) == 'k') ;  
            switch (res)
            {
            case  0 : printf(" recurso TIMER desinstalado") ; break ;
            case -1 : printf(" recurso TIMER no existe") ; break ;
            case -2 : printf(" recurso TIMER en uso") ; break ;
            case -3 : printf(" fichero TIMER no puede destruirse") ; break ;
            default : printf(" TIMER no ha podido desinstalarse") ;
            }
            return(res) ;
        }
    }
    if ((argc == 2) ||
        ((argc == 3) &&
         (!strcmpu(argv[1], "-i") || !strcmpu(argv[1], "-c"))
        )
       )
    {
        strcpy(comando[0], argv[argc-1]) ;
        inicScanner() ;
        obtenSimb() ;
        if (simb == s_numero)
        {
            if (num == 0)
            {
                printf(" ticsPorRodaja debe ser > 0") ;
                exit(-1) ;
            }
            else if (!strcmpu(argv[1], "-c"))
            {
                dfTimer = open("TIMER", O_WRONLY) ;
                if (dfTimer < 0)
                {
                    printf(" recurso TIMER no instalado todavia") ;
                    return(-1) ;
                }
                else
                {
                    if ((res = (int)lseek(dfTimer, 10, SEEK_SET)) == 10)
                        res = aio_write(dfTimer, (pointer_t)&num, sizeof(num)) ;
                    close(dfTimer) ;
                    if (res < 0) return(-1) ;
                    else return(0) ;
                }
            }
            else return(instalarTimer(num, TRUE)) ;
        }
    }
    else if (!strcmpu(argv[1], "-c"))
    {
        strcpy(comando[0], argv[argc-1]) ;
        inicScanner() ;
        obtenSimb() ;
        if (simb == s_numero)
        {
            if (num == 0)
            {
                printf(" ticsPorRodaja debe ser > 0") ;
                return(-1) ;
            }
            else if (!strcmpu(argv[1], "-c"))
            {
                dfTimer = open("TIMER", O_WRONLY) ;
                if (dfTimer < 0)
                {
                    printf(" recurso TIMER no instalado todavia") ;
                    return(-1) ;
                }
                else
                {
                    if ((res = (int)lseek(dfTimer, 10, SEEK_SET)) == 10)
                        res = aio_write(dfTimer, (pointer_t)&num, sizeof(num)) ;
                    close(dfTimer) ;
                    if (res < 0) return(-1) ;
                    else return(0) ;
                }
            }
            else return(instalarTimer(num, TRUE)) ;
        }
    }
    return(formato()) ;
}