/* ----------------------------------------------------------------------- */
/*                                 reloj.c                                 */
/* ----------------------------------------------------------------------- */
/*          implementacion de recurso RELOJ (reloj en tiempo real)         */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\ajustusr.h>          /* save_DS0, setraw_DS, restore_DS */
#include <so1pub.h\comundrv.h>  /* ptrIndProcesoActual, comprobarAmpersand */
#include <so1pub.h\ll_s_so1.h>    /* biblioteca de llamadas al sistema SO1 */
#include <so1pub.h\stdio.h>                                      /* printf */
#include <so1pub.h\ctype.h>                                     /* toupper */
#include <so1pub.h\strings.h>                           /* strcpy, strcmpu */
#include <so1pub.h\scanner.h>  /* inicScanner, obtenCar, comando, car, pos */
#include <so1pub.h\cmos.h>                                     /* leerCmos */

#define REDUCIR_DRIVER TRUE     /* TRUE ==> reducimos la memoria utilizada */
//#define REDUCIR_DRIVER FALSE

#pragma option -w-use /* (comundrv.h) warning: 'x' declared but never used */

rindx_t rec_reloj ;                                      /* esta en el BSS */

/* ----------------------------------------------------------------------- */
/*                   seccion de implementacion del driver                  */
/* ----------------------------------------------------------------------- */

void leerHoras ( word_t * horas )
{
    *horas = leerCmos(CMOS_RTC_HOURS) ;                             /* BCD */
}

void leerMinutos ( word_t * minutos )
{
    *minutos = leerCmos(CMOS_RTC_MINUTES) ;                         /* BCD */
}

void leerSegundos ( word_t * segundos )
{
    *segundos = leerCmos(CMOS_RTC_SECONDS) ;                        /* BCD */
}

void leerHora ( word_t * horas, word_t * minutos, word_t * segundos )
{
    leerHoras(horas) ;
    leerMinutos(minutos) ;
    leerSegundos(segundos) ;
}

#pragma warn -par

int far openReloj ( int dfs, modoAp_t modo )
{
    return(dfs) ;
}

int far releaseReloj ( int dfs )
{
    return(0) ;
}

#define tamBuf 8

int far readReloj ( int dfs, pointer_t dir, word_t nbytes )
{
    /* El proceso lee los bytes del fichero virtual: HH:MM:SS */

    trama_t far * tramaProceso ;
    pindx_t indProcesoActual ;
    int df ;
    word_t w [3] ;
    word_t pos ;
    int i ;
    char car ;
    char buf [tamBuf] ;

    save_DS0() ;                            /* guarda el DS anterior (SO1) */
    setraw_DS() ;           /* establece el DS correspondiente al programa */

    tramaProceso = *ptrTramaProceso ;
    indProcesoActual = *ptrIndProcesoActual ;
    df = tramaProceso->BX ;
    pos = (word_t)ptrDescProceso[indProcesoActual].tfa[df].pos ;
    if (pos >= tamBuf)
        nbytes = 0 ;
    else
    {
        leerHora((word_t *)&w[0], (word_t *)&w[1], (word_t *)&w[2]) ;
        for ( i = 0 ; i < tamBuf ; i++ )
        {
            switch (i % 3)
            {
            case 0 : car = '0' + (w[i/3] >> 4) ; break ;
            case 1 : car = '0' + (w[i/3] & 0x000F) ; break ;
            case 2 : car = ':' ;
            }
            buf[i] = car ;
        }
        if (pos + nbytes > tamBuf) nbytes = tamBuf - pos ;
        for ( i = 0 ; i < nbytes ; i++ )
            dir[i] = buf[pos + i] ;
    }

    restore_DS0() ;                               /* restaura el DS de SO1 */
    return(nbytes) ;
}

int far aio_readReloj ( int dfs, pointer_t dir, word_t nbytes )
{
    return(readReloj(dfs, dir, nbytes)) ;
}

int far writeReloj ( int dfs, pointer_t dir, word_t nbytes )
{
    return(0) ;
}

int far aio_writeReloj ( int dfs, pointer_t dir, word_t nbytes )
{
    return(0) ;
}

long far lseekReloj ( int dfs, long pos, word_t whence )
{
    int df ;
    long res = -1L ;
    long posNueva ;

    save_DS0() ;                            /* guarda el DS anterior (SO1) */
    setraw_DS() ;           /* establece el DS correspondiente al programa */

    switch (whence)
    {
    case SEEK_SET :
        if ((0 <= pos) && (pos <= (long int)tamBuf))
            res = pos ;
        break ;
    case SEEK_CUR :
        df = (*ptrTramaProceso)->BX ;
        posNueva = ptrDescProceso[*ptrIndProcesoActual].tfa[df].pos + pos ;
    case SEEK_END :
        if (whence == SEEK_END) posNueva = (long int)tamBuf + pos ;
        if (posNueva <= (long int)0)
            res = 0 ;
        else if (posNueva > (long int)tamBuf)
            res = (long int)tamBuf ;
        else
            res = posNueva ;
        break ;
    default :
        ;
    }

    restore_DS0() ;                               /* restaura el DS de SO1 */
    return(res) ;
}

int far fcntlReloj ( int dfs, word_t cmd, word_t arg )
{
    return(0) ;
}

int far ioctlReloj ( int dfs, word_t request, word_t arg )
{
    return(0) ;
}

int far eliminarReloj ( pindx_t pindx )
{
    return(0) ;
}

int finishReloj ( void )
{
//  exit(0) ;              /* obligaria a meter la biblioteca ll_s_so1.lib */
    return(0) ;
}

void finCodeDriver ( void )   /* marca el fin del codigo propio del driver */
{
}

#define maxCbRL 0

descCcb_t descCcbRL = { 0, 0, 0, maxCbRL, NULL } ;                 /* DATA */

#pragma warn +par

/* ----------------------------------------------------------------------- */
/*                      seccion de programa de usuario                     */
/* ----------------------------------------------------------------------- */

void mostrarFormato ( void )
{
    printf(" formato: RELOJ [ -i | -q | -u | -k | -h ] ") ;
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
        " instala/desinstala el driver del reloj"                        "\n"
        ""                                                               "\n"
        " opciones: (por defecto -i)"                                    "\n"
        ""                                                               "\n"
        "      -i : instala el driver (usar &)"                          "\n"
        "      -q : instala sin mensajes de salida (&)"                  "\n"
        "      -u : desintala el driver"                                 "\n"
        "      -k : desintala el driver (matando)"                       "\n"
        "      -h : muestra este help"                                   "\n"
    ) ;
    return(0) ;
}

int integrarReloj ( bool_t conMensajes )
{

    int dfs ;
    descRecurso_t dR ;

    dR.tipo = rDCaracteres ;
    strcpy(dR.nombre, "RELOJ") ;
    dR.ccb = (ccb_t)&descCcbRL ;
    dR.ccb->arg = NULL ;
    dR.pindx = getpindx() ;
    dR.numVI = 0 ;

    dR.open      = (open_t)      MK_FP(_CS, FP_OFF(openReloj)) ;
    dR.release   = (release_t)   MK_FP(_CS, FP_OFF(releaseReloj)) ;
    dR.read      = (read_t)      MK_FP(_CS, FP_OFF(readReloj)) ;
    dR.aio_read  = (aio_read_t)  MK_FP(_CS, FP_OFF(aio_readReloj)) ;
    dR.write     = (write_t)     MK_FP(_CS, FP_OFF(writeReloj)) ;
    dR.aio_write = (aio_write_t) MK_FP(_CS, FP_OFF(aio_writeReloj)) ;
    dR.lseek     = (lseek_t)     MK_FP(_CS, FP_OFF(lseekReloj)) ;
    dR.fcntl     = (fcntl_t)     MK_FP(_CS, FP_OFF(fcntlReloj)) ;
    dR.ioctl     = (ioctl_t)     MK_FP(_CS, FP_OFF(ioctlReloj)) ;

    dR.eliminar  = (eliminar_t)  MK_FP(_CS, FP_OFF(eliminarReloj)) ;

    rec_reloj = crearRecurso(&dR) ;

    if (rec_reloj >= 0)
    {
        dfs = crearFichero("RELOJ", rec_reloj, 0, fedCaracteres) ;

        if (dfs >= 0)
        {
            if (conMensajes)
                printf("\n\n recurso RELOJ instalado (fichero: RELOJ) \n") ;
            return(0) ;
        }
        switch(dfs)
        {
        case -1 : printf(" tipo de fichero erroneo") ; break ;
        case -2 : printf(" numero de recurso erroneo") ; break ;
        case -3 : printf(" RELOJ nombre de fichero ya existente") ; break ;
        case -4 : printf(" no hay descriptores de fichero libres") ; break ;
        default : printf(" no ha podido crearse el fichero RELOJ") ;
        }
        destruirRecurso("RELOJ", TRUE) ;                        /* matando */
        return(0) ;
    }
    switch(rec_reloj)
    {
    case -1 : printf(" tipo de recurso erroneo") ; break ;
    case -2 : printf(" demasiados vectores de interrupcion") ; break ;
    case -3 : printf(" RELOJ nombre de recurso ya existente") ; break ;
    case -4 : printf(" no hay descriptores de recurso libres") ; break ;
    case -5 : printf(" numero de vector de interrupcion ya usado") ; break ;
    default : printf(" no ha podido crearse el recurso RELOJ") ;
    }
    return(-1) ;
}

int desintegrarReloj ( void )
{
    return(0) ;
}

int instalarReloj ( bool_t conMensajes )
{
    int res ;
    res = comprobarAmpersand() ;
    if (res != 0) return(res) ;
    obtenInfoSO1(dirDescSO1) ;               /* obtenemos los datos de SO1 */
    res = integrarReloj(conMensajes) ;
    if (res != 0) return(res) ;
#if (!REDUCIR_DRIVER)
    esperarDesinstalacion(0) ;                       /* bloquea el proceso */
#else
    esperarDesinstalacion(                           /* bloquea el proceso */
//      FP_OFF(dirDescSO1) + sizeof(descSO1_t)
//          + sizeof(descCcbRL) + 0*sizeof(callBack_t),         /* tamDATA */
        FP_OFF(&descCcbRL)
            + sizeof(descCcbRL) + 0*sizeof(callBack_t),         /* tamDATA */
        FP_OFF(finCodeDriver),                            /* finCodeDriver */
        FP_OFF(finishReloj),                               /* finishDriver */
		0x0000                                                  /* tamPila */ 
    ) ;
/*  se retorna a finishReloj */		
#endif
/*  solo se llega aqui en el caso esperarDesinstalacion(0) */
    res = desintegrarReloj() ;
    return(res) ;
}

int main ( int argc, char * argv [ ] )
{
    int res ;
    if (argc > 2)                     return(formato()) ;
    else if (argc == 1)               return(instalarReloj(TRUE)) ;
    else if (!strcmpu(argv[1], "-h")) return(help()) ;
    else if (!strcmpu(argv[1], "-i")) return(instalarReloj(TRUE)) ;
    else if (!strcmpu(argv[1], "-q")) return(instalarReloj(FALSE)) ;
    else if ((!strcmpu(argv[1], "-u")) || 
             (!strcmpu(argv[1], "-k")))
    { 
        res = destruirRecurso("RELOJ", tolower(argv[1][1]) == 'k') ;  
        switch (res)
        {
        case  0 : printf(" recurso RELOJ desinstalado") ; break ;
        case -1 : printf(" recurso RELOJ no existe") ; break ;
        case -2 : printf(" recurso RELOJ en uso") ; break ;
        case -3 : printf(" fichero RELOJ no puede destruirse") ; break ;
        default : printf(" RELOJ no ha podido desinstalarse") ;
        }
        return(res) ;
    }
    else return(formato()) ;
}
