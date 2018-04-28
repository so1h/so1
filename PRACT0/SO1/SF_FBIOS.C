/* ----------------------------------------------------------------------- */
/*                               sf_ftbios.h                               */
/* ----------------------------------------------------------------------- */
/* recurso SF sencillo FAT12/16 implementado haciendo uso del BIOS int 13h */
/* ----------------------------------------------------------------------- */

/* En el caso de este recurso/driver de sistema de ficheros tipo FAT12/16, */ 
/* todas las operaciones de ficheros open, read, write, ... se implementan */
/* haciendo uso de la int 13h ofrecida por el BIOS. Realmente puesto que   */
/* dispone ya del recurso DB de los dispositivos de bloques reconocidos    */
/* el BIOS el acceso a los discos se realizara a traves de los ficheros    */
/* especiales de dispositivo: FDA, FDB, HDA, HDA1, ... , HDB, HDB1, ...    */
/* que ofrece el recurso DB. Este driver es el unico que puede utilizarse  */
/* cuando SO1 arranca de forma nativa a traves de un MBR/PBR sobre un      */
/* ordenador fisico o sobre qemu, bochs, DOSBox, ... En algunos casos se   */
/* podria utilizar teniendo MSDOS por debajo. Es el caso de NTVDM o de     */
/* MSDOS accediendo al disquete/disco duro con el BIOS. No obstante en el  */
/* caso del MSDOS Player no es posible (de momento su uso).                */  

#include <so1pub.h\def_sf.h>  /* maxUL, maxUF, descUnidad(Fisica/Logica)_t */
#include <so1pub.h\def_proc.h>                            /* fedCaracteres */
#include <so1pub.h\bios_0.h>                /* leerTeclaBIOS, printCarBIOS */
#include <so1pub.h\strings.h>                                    /* strcpy */
#include <so1pub.h\msdos.h>                         /* getDiskDOS, readDOS */
#include <so1pub.h\fcntl.h>                                     /* O_CREAT */
#include <so1.h\ajustes.h>                                       /* CS_SO1 */
#include <so1.h\procesos.h>                            /* indProcesoActual */
#include <so1.h\recursos.h>                                    /* crearRec */
#include <so1.h\units.h>                       /* inicUnits, comprobarFats */
#include <so1.h\fatunit.h> 
#include <so1.h\interrup.h>                                       /* VIOrg */
#include <so1.h\plot.h>                                            /* plot */
#include <so1.h\sf.h>                                   /* formatearNombre */

#include <so1.h\sf_fbios.h> 

extern rindx_t rec_sf ; /* para evitar el conflicto con SF_MSDOS */

extern dfs_t dfs_sf ; /* para evitar el conflicto con SF_MSDOS */

#pragma warn -par

static int far openSF ( int dfs, modoAp_t modo ) 
{
    char far * nombre = MK_FP(tramaProceso->ES, tramaProceso->BX) ;
	
	
	
    int df2 ;                               /* descriptor de fichero MSDOS */
    int modoDOSExt = 0x2000 ;
    word_t accion = 0x0001 ;
    word_t error ;

return(-1) ;

#if (FALSE)
    if (modo & O_WRONLY) 
	{
        modoDOSExt = 0x2001 ;
        if (modo & O_CREAT) accion |= 0x0010 ;
        if (modo & O_TRUNC) accion = (accion & 0xFFF0) | 0x0002 ;
    }

    df2 = extendedOpenDOS(nombre, modoDOSExt, 0, &accion, &error) ; /* atr normal */
    if (df2 >= 0) 
	{
        dfs = crearFich(nombre, rec_sf, df2, fRegular) ;     /* menor = df2 */
        return(dfs) ;
    }
#endif	
    return(-1) ;
}

static int far releaseSF ( int dfs ) 
{
//  printStrBIOS("\n release dfs = ") ; printDecBIOS(dfs, 1) ;
    closeDOS(descFichero[dfs].menor) ;       /* se cierra el fichero MSDOS */
    destruirFich(dfs) ;
    return(0) ;
}

static int far readSF ( int dfs, pointer_t dir, word_t nbytes )
{
    int n ;
    n = readDOS(descFichero[dfs].menor, (char far *)dir, nbytes) ;
    return(n) ;
}

static int far aio_readSF ( int dfs, pointer_t dir, word_t nbytes ) 
{
    return(0) ;
}

static int far writeSF ( int dfs, pointer_t dir, word_t nbytes ) 
{
    int n ;
    n = writeDOS(descFichero[dfs].menor, (char far *)dir, nbytes) ;
    return(n) ;
}

static int far aio_writeSF ( int dfs, pointer_t dir, word_t nbytes ) 
{
    return(0) ;
}

static long far lseekSF ( int dfs, long pos, word_t whence ) 
{
    long posFich = (dword_t)pos ;
    lseekDOS(descFichero[dfs].menor, (dword_t *)&posFich, whence) ;
    return(posFich) ;
}

static int far fcntlSF ( int dfs, word_t cmd, word_t arg ) 
{
    return(0) ;
}

static int far ioctlSF ( int dfs, word_t request, word_t arg ) 
{
    return(0) ;
}

static int far eliminarSF ( pindx_t pindx )
{
    return(0) ;
}

/* ----------------------------------------------------------------------- */

#pragma warn +par

#define maxCbSF 0

static descCcb_t descCcbSF = { 0, 0, 0, maxCbSF, NULL } ;

int inicSF_FATBIOS ( void ) 
{
    descRecurso_t dR ;

    inicUnits() ;
	
	assert(comprobarUnits(unidadBIOS()) == 0, 
		   "\a\n inicSF_FATBIOS(): ERROR comprobarUnits ") ;
	
	inicFatUnit() ;
	
	inicTablaFichAbiertos() ; 
		
    dR.tipo = rSF ;
    strcpy(dR.nombre, "SF_FATBIOS") ;
    dR.ccb = (ccb_t)&descCcbSF ;
    dR.ccb->arg = NULL ;
    dR.pindx = indProcesoActual ;
    dR.numVI = 0 ;

    dR.open      = (open_t)     MK_FP(_CS, FP_OFF(openSF)) ;
    dR.release   = (release_t)  MK_FP(_CS, FP_OFF(releaseSF)) ;
    dR.read      = (read_t)     MK_FP(_CS, FP_OFF(readSF)) ;
    dR.aio_read  = (aio_read_t) MK_FP(_CS, FP_OFF(aio_readSF)) ;
    dR.write     = (write_t)    MK_FP(_CS, FP_OFF(writeSF)) ;
    dR.aio_write = (aio_write_t)MK_FP(_CS, FP_OFF(aio_writeSF)) ;
    dR.lseek     = (lseek_t)    MK_FP(_CS, FP_OFF(lseekSF)) ;
    dR.fcntl     = (fcntl_t)    MK_FP(_CS, FP_OFF(fcntlSF)) ;
    dR.ioctl     = (ioctl_t)    MK_FP(_CS, FP_OFF(ioctlSF)) ;

    dR.eliminar  = (eliminar_t) MK_FP(_CS, FP_OFF(eliminarSF)) ;

    rec_sf = crearRec(&dR) ;

    dfs_sf = crearFich("SF", rec_sf, 0, fedCaracteres) ;

    return(0) ;
}