/* ----------------------------------------------------------------------- */
/*                                 reloj.c                                 */
/* ----------------------------------------------------------------------- */
/*          implementacion de recurso RELOJ (reloj en tiempo real)         */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\comundrv.h>                               /* dirDescSO1 */
#include <so1pub.h\ll_s_so1.h>    /* biblioteca de llamadas al sistema SO1 */
#include <so1pub.h\escribir.h>
#include <so1pub.h\carsctrl.h>                                       /* FF */
#include <so1pub.h\strings.h>                           /* strcpy, strcmpu */
#include <so1pub.h\scanner.h>
#include <so1pub.h\puertos.h>
#include <so1pub.h\cmos.h>                                     /* leerCmos */

#pragma option -w-use /* (comundrv.h) warning: 'x' declared but never used */

rindx_t rec_reloj ;

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
    return(0) ;
}

int far releaseReloj ( int dfs )
{
    return(0) ;
}

#define tamBuf 8

int far readReloj ( int dfs, pointer_t dir, word_t nbytes )
{

    /* El proceso lee los bytes del fichero virtual: HH:MM:SS */

    word_t DS_Reloj = *((word_t far *)pointer(_CS, (word_t)segDatos)) ;

    trama_t far * tramaProceso ;
    pindx_t indProcesoActual ;
    int df ;
    word_t w [3] ;
    word_t pos ;
    int i ;
    char car ;
    char buf [tamBuf] ;

    asm push ds
    asm mov ds,DS_Reloj

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
            case 0 :
                car = '0' + (w[i/3] >> 4) ;
                break ;
            case 1 :
                car = '0' + (w[i/3] & 0x000F) ;
                break ;
            case 2 :
                car = ':' ;
            }
            buf[i] = car ;
        }
        if (pos + nbytes > tamBuf) nbytes = tamBuf - pos ;
        for ( i = 0 ; i < nbytes ; i++ )
            dir[i] = buf[pos + i] ;
    }

    asm pop ds
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

long int far lseekReloj ( int dfs, long int pos, word_t whence )
{

    word_t DS_Reloj = *((word_t far *)pointer(_CS, (word_t)segDatos)) ;

    int df ;
    long int res = (long int)-1 ;
    long int posNueva ;

    asm push ds
    asm mov ds,DS_Reloj

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

    asm pop ds
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

#pragma warn +par

/* ----------------------------------------------------------------------- */
/*                      seccion de programa de usuario                     */
/* ----------------------------------------------------------------------- */

void mostrarFormato ( void )
{
    escribirStr(" formato: RELOJ [ -i | -q | -u | -h ] ") ;
}

void formato ( void )
{
    mostrarFormato() ;
    exit(-1) ;
}

void help ( void )
{
    escribirLn() ;
    escribirLn() ;
    mostrarFormato() ;
    escribirStr("\n\n") ;
    escribirStr(" instala/desinstala el driver del reloj \n\n") ;
    escribirStr(" opciones: (por defecto -i)             \n\n") ;
    escribirStr("      -i : instala el driver (usar &)   \n") ;
    escribirStr("      -u : desintala el driver          \n") ;
    escribirStr("      -h : muestra este help            \n") ;
    exit(0) ;
}

descProceso_t descProceso [ maxProcesos ] ;

e2PFR_t e2PFR ;

c2c_t c2cPFR [ numColasPFR ] ;

#define maxCbRL 0

descCcb_t descCcbRL = { 0, 0, 0, maxCbRL, NULL } ;

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

    dR.open      = (open_t)pointer(_CS, (word_t)openReloj) ;
    dR.release   = (release_t)pointer(_CS, (word_t)releaseReloj) ;
    dR.read      = (read_t)pointer(_CS, (word_t)readReloj) ;
    dR.aio_read  = (aio_read_t)pointer(_CS, (word_t)aio_readReloj) ;
    dR.write     = (write_t)pointer(_CS, (word_t)writeReloj) ;
    dR.aio_write = (aio_write_t)pointer(_CS, (word_t)aio_writeReloj) ;
    dR.lseek     = (lseek_t)pointer(_CS, (word_t)lseekReloj) ;
    dR.fcntl     = (fcntl_t)pointer(_CS, (word_t)fcntlReloj) ;
    dR.ioctl     = (ioctl_t)pointer(_CS, (word_t)ioctlReloj) ;

    dR.eliminar  = (eliminar_t)pointer(_CS, (word_t)eliminarReloj) ;

    rec_reloj = crearRecurso(&dR) ;

    if (rec_reloj >= 0)
    {

        dfs = crearFichero("RELOJ", rec_reloj, 0, fedCaracteres) ;

        if (dfs >= 0)
        {
            if (conMensajes)
                escribirStr("\n\n recurso RELOJ instalado (fichero: RELOJ) \n") ;
            return(0) ;
        }
        switch(dfs)
        {
        case -1 :
            escribirStr(" tipo de fichero erroneo") ;
            break ;
        case -2 :
            escribirStr(" numero de recurso erroneo") ;
            break ;
        case -3 :
            escribirStr(" RELOJ nombre de fichero ya existente") ;
            break ;
        case -4 :
            escribirStr(" no hay descriptores de fichero libres") ;
            break ;
        default :
            escribirStr(" no ha podido crearse el fichero RELOJ") ;
        }
        destruirRecurso("RELOJ") ;
        return(0) ;
    }
    switch(rec_reloj)
    {
    case -1 :
        escribirStr(" tipo de recurso erroneo") ;
        break ;
    case -2 :
        escribirStr(" demasiados vectores de interrupcion") ;
        break ;
    case -3 :
        escribirStr(" RELOJ nombre de recurso ya existente") ;
        break ;
    case -4 :
        escribirStr(" no hay descriptores de recurso libres") ;
        break ;
    case -5 :
        escribirStr(" numero de vector de interrupcion ya usado") ;
        break ;
    default :
        escribirStr(" no se ha podido crear el recurso RELOJ") ;
    }
    return(-1) ;
}

int desintegrarReloj ( void )
{
    return(0) ;
}

int comprobarAmpersand ( void )
{
    int i ;
    obtenInfoPS((descProceso_t far *)&descProceso, (e2PFR_t far *)&e2PFR, (c2c_t far *)&c2cPFR) ;

    for ( i = 0 ; i < tamComando ; i++ )
        comando[0][i] = descProceso[getpindx()].comando[i] ;

    inicScanner() ;
    while (car != (char)0) obtenCar() ;
    if (comando[0][pos] != '&') /* truco: crearProceso deja ahi &, <, >, | */
    {
        escribirStr("\n\n este driver debe ejecutarse con & \n") ;
        return(-1) ;
    }
    return(0) ;
}

int instalarReloj ( bool_t conMensajes )
{
    int res ;
    res = comprobarAmpersand() ;
    if (res != 0) return(res) ;
    obtenInfoSO1(dirDescSO1) ;               /* obtenemos los datos de SO1 */
    *((word_t far *)pointer(_CS, (word_t)segDatos)) = _DS ;   /* guardo DS */
    res = integrarReloj(conMensajes) ;
    if (res != 0) return(res) ;
    esperarDesinstalacion() ;                        /* bloquea el proceso */
    res = desintegrarReloj() ;
    return(res) ;
}

void main ( int argc, char * argv [ ] )
{
    int res ;
    if (argc > 2) formato() ;
    else if (argc == 1) exit(instalarReloj(TRUE)) ;
    else if (!strcmpu(argv[1], "-h")) help() ;
    else if (!strcmpu(argv[1], "-i")) exit(instalarReloj(TRUE)) ;
    else if (!strcmpu(argv[1], "-q")) exit(instalarReloj(FALSE)) ;
    else if (!strcmpu(argv[1], "-u"))
    {
        res = destruirRecurso("RELOJ") ;
        switch (res)
        {
        case  0 :
            escribirStr(" recurso RELOJ desinstalado") ;
            break ;
        case -1 :
            escribirStr(" recurso RELOJ no existe") ;
            break ;
        case -2 :
            escribirStr(" recurso RELOJ en uso") ;
            break ;
        case -3 :
            escribirStr(" fichero RELOJ no puede destruirse") ;
            break ;
        default :
            escribirStr(" RELOJ no ha podido desinstalarse") ;
        }
        exit(res) ;
    }
    else formato() ;
}
