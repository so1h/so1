/* ----------------------------------------------------------------------- */
/*                                retardo.c                                */
/* ----------------------------------------------------------------------- */
/*           implementacion del recurso RETARDO (retardo activo)           */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\comundrv.h>                /* dirDescSO1, segDatos, ... */
#include <so1pub.h\ll_s_so1.h>    /* biblioteca de llamadas al sistema SO1 */
#include <so1pub.h\escribir.h>            /* escribirStr, escribirDec, ... */
#include <so1pub.h\strings.h>                           /* strcpy, strcmpu */
#include <so1pub.h\scanner.h>  /* inicScanner, obtenCar, comando, car, pos */
#include <so1pub.h\biosdata.h>                              /* ptrBiosArea */

#pragma option -w-use /* (comundrv.h) warning: 'x' declared but never used */

rindx_t rec_retardo ;

/* ----------------------------------------------------------------------- */
/*                   seccion de implementacion del driver                  */
/* ----------------------------------------------------------------------- */

bool_t retardando [ maxProcesos ] ;        /* procesos retardandose */

dword_t nVueltasRetardo ;

#define cuerpoVuelta {                                                       \
  for ( j = 0 ; j < 10 ; j++ ) {                                             \
    asm mov dx,0x1234 ;                                                      \
    asm mov ax,0x5678 ;                                                      \
    asm mov bx,0xFFFF ;                                                      \
    asm div bx ;                                                             \
  }                                                                          \
  nVueltas++ ;                                                               \
}

void retardar ( dword_t nVueltas )
{
    dword_t n = nVueltas ;
    dword_t i ;
    word_t j ;
    nVueltas = 0 ;
    for ( i = 0 ; i < n ; i++ ) cuerpoVuelta ;
}

int far retardarProceso ( void )
{
    word_t DS_Retardo = *((word_t far *)pointer(_CS, (word_t)segDatos)) ;
    int res ;
    asm push ds
    asm mov ds,DS_Retardo                    /* establecemos DS del driver */
    if (!retardando[*ptrIndProcesoActual]) res = -1 ;
    else
    {
        retardar(nVueltasRetardo) ;
        res = 0 ;
    }
    asm pop ds                               /* restauramos DS del proceso */
    return(res) ;
}

#pragma warn -par

int far openRetardo ( int dfs, modoAp_t modo )
{
    word_t DS_Retardo = *((word_t far *)pointer(_CS, (word_t)segDatos)) ;
    pindx_t indProcesoActual ;
    int res = 0 ;

    asm push ds
    asm mov ds,DS_Retardo

    indProcesoActual = *ptrIndProcesoActual ;
    if (retardando[indProcesoActual]) res = -1 ;
    else retardando[indProcesoActual] = TRUE ;

    asm pop ds
    return(res) ;
}

int far releaseRetardo ( int dfs )
{
    word_t DS_Retardo = *((word_t far *)pointer(_CS, (word_t)segDatos)) ;
    pindx_t indProcesoActual ;
    int res = 0 ;

    asm push ds
    asm mov ds,DS_Retardo

    indProcesoActual = *ptrIndProcesoActual ;
    if (!retardando[indProcesoActual]) res = -1 ;
    else retardando[indProcesoActual] = FALSE ;

    asm pop ds
    return(res) ;
}

int far readRetardo ( int dfs, pointer_t dir, word_t nbytes )
{
    word_t DS_Retardo = *((word_t far *)pointer(_CS, (word_t)segDatos)) ;
    pindx_t indProcesoActual ;
    int res = 4 ;
    dword_t far * ptrDWord = (dword_t far *)dir ;

    asm push ds
    asm mov ds,DS_Retardo

    indProcesoActual = *ptrIndProcesoActual ;
    if (!retardando[indProcesoActual]) res = -1 ;    /* fichero no abierto */
    else if (nbytes != 4) res = -2 ;                  /* nbytes incorrecto */
    else
        switch (dir[0])
        {
        case 0x00 :
            ptrDWord[0] = nVueltasRetardo ;
            break ;
        case 0x01 :
            ptrDWord[0] = (dword_t)
                          pointer(_CS, (word_t)retardarProceso) ;
            break ;
        default :
            res = -3 ;                   /* codigo de operacion incorrecto */
        }

    asm pop ds
    return(res) ;
}

int far aio_readRetardo ( int dfs, pointer_t dir, word_t nbytes )
{
    return(readRetardo(dfs, dir, nbytes)) ;
}

int far writeRetardo ( int dfs, pointer_t dir, word_t nbytes )
{
    word_t DS_Retardo = *((word_t far *)pointer(_CS, (word_t)segDatos)) ;
    pindx_t indProcesoActual ;
    int res = 4 ;
    dword_t far * ptrDWord = (dword_t far *)dir ;

    asm push ds
    asm mov ds,DS_Retardo

    indProcesoActual = *ptrIndProcesoActual ;
    if (!retardando[indProcesoActual]) res = -1 ;    /* fichero no abierto */
    else if (nbytes != 4) res = -2 ;
    else nVueltasRetardo = ptrDWord[0] ;

    asm pop ds
    return(res) ;
}

int far aio_writeRetardo ( int dfs, pointer_t dir, word_t nbytes )
{
    return(-1) ;
}

long int far lseekRetardo ( int dfs, long int pos, word_t whence )
{
    return((long int)-1) ;
}

int far fcntlRetardo ( int dfs, word_t cmd, word_t arg )
{
    return(-1) ;
}

int far ioctlRetardo ( int dfs, word_t request, word_t arg )
{
    return(-1) ;
}

int far eliminarRetardo ( pindx_t pindx )
{
    return(0) ;
}

#pragma warn +par

/* ----------------------------------------------------------------------- */
/*                      seccion de programa de usuario                     */
/* ----------------------------------------------------------------------- */

void mostrarFormato ( void )
{
    escribirStr(
        " formato: RETARDO [ -i | -q | -u | -h ] "
    ) ;
}

int formato ( void )
{
    mostrarFormato() ;
    return(-1) ;
}

int help ( void )
{
    escribirStr(
        "\n"
        "\n"
    ) ;
    mostrarFormato() ;
    escribirStr(
        "\n"
        "\n"
        " instala/desinstala el driver de retardo activo \n"
        "\n"
        " opciones: (por defecto -i)                     \n"
        "\n"
        "      -i : instala el driver (usar &)           \n"
        "      -q : instala sin mensajes de salida (&)   \n"
        "      -u : desintala el driver                  \n"
        "      -h : muestra este help                    \n"
    ) ;
    return(0) ;
}

descProceso_t descProceso [ maxProcesos ] ;

e2PFR_t e2PFR ;

c2c_t c2cPFR [ numColasPFR ] ;

#define maxCbRT 0

descCcb_t descCcbRT = { 0, 0, 0, maxCbRT, NULL } ;

/* valorRetardo calcula cuantas vueltas hay que dar en un bucle de         */
/* de espera de referencia (ver: cuerpoVuelta y retardar) para conseguir   */
/* una espera de 1 tic (55 milisegundos) suponiendo las interrupciones     */
/* inhibidas. Evidentemente cuando un proceso ejecuta retardar(1) su espera*/
/* va a ser mayor ya que las interrupciones del timer estaran permitidas.  */
/* Ademas pueden producirse a consecuencia de las interrupciones el cambio */
/* del proceso en ejecucion. Por tanto para los procesos la espera siempre */
/* va a ser mayor al tiempo que podria esperarse con las interrupciones    */
/* inhibidas.                                                              */

dword_t valorRetardo ( void )  /* permite las interrupciones temporalmente */
{
    word_t j ;
    dword_t nVueltas ;
    byte_t contTics0 ;
    byte_t contTics1 ;
    asm
    {
        pushf ;
        sti ;
    }
    contTics0 = (byte_t)ptrBiosArea->BIOS_timer ;
    while ((byte_t)ptrBiosArea->BIOS_timer == contTics0) { } ;
    nVueltas = 0 ;
    contTics1 = (byte_t)ptrBiosArea->BIOS_timer ;
    while ((byte_t)ptrBiosArea->BIOS_timer == contTics1) cuerpoVuelta ;
    asm popf
    return(nVueltas) ;                          /* 1 Tics = 0,055 segundos */
}

int integrarRetardo ( bool_t conMensajes )
{

    int i ;
    int dfs ;
    descRecurso_t dR ;

    dR.tipo = rDCaracteres ;
    strcpy(dR.nombre, "RETARDO") ;
    dR.ccb = (ccb_t)&descCcbRT ;
    dR.ccb->arg = NULL ;
    dR.pindx = getpindx() ;
    dR.numVI = 0 ;

    dR.open      = (open_t)pointer(_CS, (word_t)openRetardo) ;
    dR.release   = (release_t)pointer(_CS, (word_t)releaseRetardo) ;
    dR.read      = (read_t)pointer(_CS, (word_t)readRetardo) ;
    dR.aio_read  = (aio_read_t)pointer(_CS, (word_t)aio_readRetardo) ;
    dR.write     = (write_t)pointer(_CS, (word_t)writeRetardo) ;
    dR.aio_write = (aio_write_t)pointer(_CS, (word_t)aio_writeRetardo) ;
    dR.lseek     = (lseek_t)pointer(_CS, (word_t)lseekRetardo) ;
    dR.fcntl     = (fcntl_t)pointer(_CS, (word_t)fcntlRetardo) ;
    dR.ioctl     = (ioctl_t)pointer(_CS, (word_t)ioctlRetardo) ;

    dR.eliminar  = (eliminar_t)pointer(_CS, (word_t)eliminarRetardo) ;

    rec_retardo = crearRecurso(&dR) ;

    nVueltasRetardo = valorRetardo() ;                  /* dividimos por 2 */
    /* para compensar las ints */
    for ( i = 0 ; i < maxProcesos ; i ++ ) retardando[i] = FALSE ;

    if (rec_retardo >= 0)
    {

        dfs = crearFichero("RETARDO", rec_retardo, 0, fedCaracteres) ;

        if (dfs >= 0)
        {
            if (conMensajes)
            {
                escribirStr(
                    "\n"
                    "\n recurso RETARDO instalado (fichero: RETARDO) "
                    "\n\n nVueltasRetardo = "
                ) ;
                escribirLDec(nVueltasRetardo, 1) ;
                escribirStr("\n") ;
            }
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
            escribirStr(" RETARDO nombre de fichero ya existente") ;
            break ;
        case -4 :
            escribirStr(" no hay descriptores de fichero libres") ;
            break ;
        default :
            escribirStr(" no se ha podido crear el fichero RETARDO") ;
        }
        destruirRecurso("RETARDO") ;
        return(0) ;
    }
    switch(rec_retardo)
    {
    case -1 :
        escribirStr(" tipo de recurso erroneo") ;
        break ;
    case -2 :
        escribirStr(" demasiados vectores de interrupcion") ;
        break ;
    case -3 :
        escribirStr(" RETARDO nombre de recurso ya existente") ;
        break ;
    case -4 :
        escribirStr(" no hay descriptores de recurso libres") ;
        break ;
    case -5 :
        escribirStr(" numero de vector de interrupcion ya usado") ;
        break ;
    default :
        escribirStr(" no ha podido crearse el recurso RETARDO") ;
    }
    return(-1) ;
}

int desintegrarRetardo ( void )
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

int instalarRetardo ( bool_t conMensajes )
{
    int res ;
    res = comprobarAmpersand() ;
    if (res != 0) return(res) ;
    obtenInfoSO1(dirDescSO1) ;               /* obtenemos los datos de SO1 */
    *((word_t far *)pointer(_CS, (word_t)segDatos)) = _DS ;   /* guardo DS */
    res = integrarRetardo(conMensajes) ;
    if (res != 0) return(res) ;
    esperarDesinstalacion() ;                        /* bloquea el proceso */
    res = desintegrarRetardo() ;
    return(res) ;
}

int main ( int argc, char * argv [ ] )
{
    int res ;
    if (argc > 2) return(formato()) ;
    else if (argc == 1) exit(instalarRetardo(TRUE)) ;
    else if (!strcmpu(argv[1], "-h")) return(help()) ;
    else if (!strcmpu(argv[1], "-i")) return(instalarRetardo(TRUE)) ;
    else if (!strcmpu(argv[1], "-q")) return(instalarRetardo(FALSE)) ;
    else if (!strcmpu(argv[1], "-u"))
    {
        res = destruirRecurso("RETARDO") ;
        switch (res)
        {
        case  0 :
            escribirStr(" recurso RETARDO desinstalado") ;
            break ;
        case -1 :
            escribirStr(" recurso RETARDO no existe") ;
            break ;
        case -2 :
            escribirStr(" recurso RETARDO en uso") ;
            break ;
        case -3 :
            escribirStr(" fichero RETARDO no puede destruirse") ;
            break ;
        default :
            escribirStr(" RETARDO no ha podido desinstalarse") ;
        }
        return(res) ;
    }
    else return(formato()) ;
}
