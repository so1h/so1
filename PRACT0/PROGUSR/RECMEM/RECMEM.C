/* ----------------------------------------------------------------------- */
/*                                recmem.c                                 */
/* ----------------------------------------------------------------------- */
/*                implementacion de recurso RECMEM (memoria)               */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\ajustusr.h>          /* save_DS0, setraw_DS, restore_DS */
#include <so1pub.h\comundrv.h>  /* ptrIndProcesoActual, comprobarAmpersand */
#include <so1pub.h\ll_s_so1.h>    /* biblioteca de llamadas al sistema SO1 */
#include <so1pub.h\stdio.h>                                      /* printf */
#include <so1pub.h\ctype.h>                                     /* toupper */
#include <so1pub.h\strings.h>                           /* strcpy, strcmpu */
#include <so1pub.h\scanner.h>  /* inicScanner, obtenCar, comando, car, pos */
#include <so1pub.h\memory.h>                                     /* memcpy */

#define REDUCIR_DRIVER TRUE     /* TRUE ==> reducimos la memoria utilizada */
//#define REDUCIR_DRIVER FALSE

#pragma option -w-use /* (comundrv.h) warning: 'x' declared but never used */

rindx_t rec_mem ;                                        /* esta en el BSS */

/* ----------------------------------------------------------------------- */
/*                   seccion de implementacion del driver                  */
/* ----------------------------------------------------------------------- */

#pragma warn -par

int far openMem ( int dfs, modoAp_t modo )
{
    return(dfs) ;
}

int far releaseMem ( int dfs )
{
    return(0) ;
}

#define tamMem (0xFFFF0L + 0xFFFFL)

int far readMem ( int dfs, pointer_t dir, word_t nbytes )
{
    trama_t far * tramaProceso ;
    pindx_t indProcesoActual ;
    int df ;
    word_t pos ;

    save_DS0() ;                            /* guarda el DS anterior (SO1) */
    setraw_DS() ;           /* establece el DS correspondiente al programa */

    tramaProceso = *ptrTramaProceso ;
    indProcesoActual = *ptrIndProcesoActual ;
    df = tramaProceso->BX ;
    pos = (word_t)ptrDescProceso[indProcesoActual].tfa[df].pos ;
	
	memcpy(dir, MK_FP(pos >> 4, ((word_t)pos) & 0x00FF), nbytes) ;	
	
    restore_DS0() ;                               /* restaura el DS de SO1 */
    return(nbytes) ;
}

int far aio_readMem ( int dfs, pointer_t dir, word_t nbytes )
{
    return(0) ;
}

int far writeMem ( int dfs, pointer_t dir, word_t nbytes )
{
    trama_t far * tramaProceso ;
    pindx_t indProcesoActual ;
    int df ;
    word_t pos ;

    save_DS0() ;                            /* guarda el DS anterior (SO1) */
    setraw_DS() ;           /* establece el DS correspondiente al programa */

    tramaProceso = *ptrTramaProceso ;
    indProcesoActual = *ptrIndProcesoActual ;
    df = tramaProceso->BX ;
    pos = (word_t)ptrDescProceso[indProcesoActual].tfa[df].pos ;
	
	memcpy(MK_FP(pos >> 4, ((word_t)pos) & 0x00FF), dir, nbytes) ;
	
    restore_DS0() ;                               /* restaura el DS de SO1 */
    return(nbytes) ;
}

int far aio_writeMem ( int dfs, pointer_t dir, word_t nbytes ) 
{
    return(0) ;
}

long far lseekMem ( int dfs, long pos, word_t whence )
{
    int df ;
    long res = -1L ;
    long posActual ;
    trama_t far * tramaProceso ;
    pindx_t indProcesoActual ;

    save_DS0() ;                            /* guarda el DS anterior (SO1) */
    setraw_DS() ;           /* establece el DS correspondiente al programa */

    tramaProceso = *ptrTramaProceso ;
    df = tramaProceso->BX ;
    indProcesoActual = *ptrIndProcesoActual ;
    posActual = ptrDescProceso[indProcesoActual].tfa[df].pos ;
    switch (whence)
    {
    case SEEK_SET : posActual = pos       ; break ;
    case SEEK_CUR : posActual += pos      ; break ;
    case SEEK_END : posActual = tamMem + pos ; break ;
    default       : { restore_DS0() ; return(-2L) ; }
    }
    if ((0 <= posActual) && (posActual <= tamMem))
        res = posActual ;
    else
        res = -1L ;
	
    restore_DS0() ;                               /* restaura el DS de SO1 */
    return(res) ;
}

int far fcntlMem ( int dfs, word_t cmd, word_t arg )
{
    return(0) ;
}

int far ioctlMem ( int dfs, word_t request, word_t arg )
{
    return(0) ;
}

int far eliminarMem ( pindx_t pindx )
{
    return(0) ;
}

int finishMem ( void )
{
//  exit(0) ;              /* obligaria a meter la biblioteca ll_s_so1.lib */
    return(0) ;
}

void finCodeDriver ( void )   /* marca el fin del codigo propio del driver */
{
}

#define maxCbMEM 0

descCcb_t descCcbMEM = { 0, 0, 0, maxCbMEM, NULL } ;                 /* DATA */

#pragma warn +par

/* ----------------------------------------------------------------------- */
/*                      seccion de programa de usuario                     */
/* ----------------------------------------------------------------------- */

void mostrarFormato ( void )
{
    printf(" formato: RECMEM [ -i | -q | -u | -k | -h ] ") ;
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

int integrarMem ( bool_t conMensajes )
{

    int dfs ;
    descRecurso_t dR ;

    dR.tipo = rDB ;
    strcpy(dR.nombre, "MEM") ;
    dR.ccb = (ccb_t)&descCcbMEM ;
    dR.ccb->arg = NULL ;
    dR.pindx = getpindx() ;
    dR.numVI = 0 ;

    dR.open      = (open_t)      MK_FP(_CS, FP_OFF(openMem)) ;
    dR.release   = (release_t)   MK_FP(_CS, FP_OFF(releaseMem)) ;
    dR.read      = (read_t)      MK_FP(_CS, FP_OFF(readMem)) ;
    dR.aio_read  = (aio_read_t)  MK_FP(_CS, FP_OFF(aio_readMem)) ;
    dR.write     = (write_t)     MK_FP(_CS, FP_OFF(writeMem)) ;
    dR.aio_write = (aio_write_t) MK_FP(_CS, FP_OFF(aio_writeMem)) ;
    dR.lseek     = (lseek_t)     MK_FP(_CS, FP_OFF(lseekMem)) ;
    dR.fcntl     = (fcntl_t)     MK_FP(_CS, FP_OFF(fcntlMem)) ;
    dR.ioctl     = (ioctl_t)     MK_FP(_CS, FP_OFF(ioctlMem)) ;

    dR.eliminar  = (eliminar_t)  MK_FP(_CS, FP_OFF(eliminarMem)) ;

    rec_mem = crearRecurso(&dR) ;

    if (rec_mem >= 0)
    {
        dfs = crearFichero("MEM", rec_mem, 0, fedBloques) ;

        if (dfs >= 0)
        {
            if (conMensajes)
                printf("\n\n recurso MEM instalado (fichero: MEM) \n") ;
            return(0) ;
        }
        switch(dfs)
        {
        case -1 : printf(" tipo de fichero erroneo") ; break ;
        case -2 : printf(" numero de recurso erroneo") ; break ;
        case -3 : printf(" MEM nombre de fichero ya existente") ; break ;
        case -4 : printf(" no hay descriptores de fichero libres") ; break ;
        default : printf(" no ha podido crearse el fichero MEM") ;
        }
        destruirRecurso("MEM", TRUE) ;                          /* matando */
        return(0) ;
    }
    switch(rec_mem)
    {
    case -1 : printf(" tipo de recurso erroneo") ; break ;
    case -2 : printf(" demasiados vectores de interrupcion") ; break ;
    case -3 : printf(" MEM nombre de recurso ya existente") ; break ;
    case -4 : printf(" no hay descriptores de recurso libres") ; break ;
    case -5 : printf(" numero de vector de interrupcion ya usado") ; break ;
    default : printf(" no ha podido crearse el recurso MEM") ;
    }
    return(-1) ;
}

int desintegrarMem ( void )
{
    return(0) ;
}

int instalarMem ( bool_t conMensajes )
{
    int res ;
    res = comprobarAmpersand() ;
    if (res != 0) return(res) ;
    obtenInfoSO1(dirDescSO1) ;               /* obtenemos los datos de SO1 */
    res = integrarMem(conMensajes) ;
    if (res != 0) return(res) ;
#if (!REDUCIR_DRIVER)
    esperarDesinstalacion(0) ;                       /* bloquea el proceso */
#else
    esperarDesinstalacion(                           /* bloquea el proceso */
//      FP_OFF(dirDescSO1) + sizeof(descSO1_t)
//          + sizeof(descCcbMEM) + 0*sizeof(callBack_t),         /* tamDATA */
        FP_OFF(&descCcbMEM)
            + sizeof(descCcbMEM) + 0*sizeof(callBack_t),         /* tamDATA */
        FP_OFF(finCodeDriver),                            /* finCodeDriver */
        FP_OFF(finishMem),                                 /* finishDriver */
		0x0000                                                  /* tamPila */ 
    ) ;
/*  se retorna a finishMem */		
#endif
/*  solo se llega aqui en el caso esperarDesinstalacion(0) */
    res = desintegrarMem() ;
    return(res) ;
}

int main ( int argc, char * argv [ ] )
{
    int res ;
    if (argc > 2)                     return(formato()) ;
    else if (argc == 1)               return(instalarMem(TRUE)) ;
    else if (!strcmpu(argv[1], "-h")) return(help()) ;
    else if (!strcmpu(argv[1], "-i")) return(instalarMem(TRUE)) ;
    else if (!strcmpu(argv[1], "-q")) return(instalarMem(FALSE)) ;
    else if ((!strcmpu(argv[1], "-u")) || 
             (!strcmpu(argv[1], "-k")))
    { 
        res = destruirRecurso("MEM", tolower(argv[1][1]) == 'k') ;  
        switch (res)
        {
        case  0 : printf(" recurso MEM desinstalado") ; break ;
        case -1 : printf(" recurso MEM no existe") ; break ;
        case -2 : printf(" recurso MEM en uso") ; break ;
        case -3 : printf(" fichero MEM no puede destruirse") ; break ;
        default : printf(" MEM no ha podido desinstalarse") ;
        }
        return(res) ;
    }
    else return(formato()) ;
}
