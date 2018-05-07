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

#include <so1.h\sf_fbios.h>  
#include <so1.h\bios.h>                    /* leerSectorCSH, leerSectorLBA */
#include <so1.h\db.h>                                       /* ptrBuferSO1 */
#include <so1pub.h\memory.h>                          /* memcpy, memcpy_fd */
#include <so1pub.h\debug.h>                          /* assert */

#include <so1.h\blockpr.h>                        /* bloquearProcesoActual */
#include <so1pub.h\ll_s_u2s.h>                 /* user2system, system2user */

extern rindx_t rec_sf ; /* para evitar el conflicto con SF_MSDOS */

extern dfs_t dfs_sf ; /* para evitar el conflicto con SF_MSDOS */

pindx_t pindxSrvSF ;

/* ----------------------------------------------------------------------- */

int numFichAbiertos ;

descriptor_de_fichero_t tablaFichAbiertos [ maxFichAbiertos ] ;

static victimaASustituir = 0 ;          /* para la politica de sustitucion */

int inicTablaFichAbiertos ( void )
{
    int i ;
    numFichAbiertos = 0 ;
    victimaASustituir = 0 ;
    for ( i = 0 ; i < maxFichAbiertos ; i++ )
        tablaFichAbiertos[i].libre = TRUE ;
    return(0) ;
}

dword_t numSectores ( dword_t tam )
{
    return((tam+511)/512) ;
}

int findFirstSo1 ( int unidadLogica, ffblk_t far * ffblk )
{
    ffblk->unidadLogica = unidadLogica ;
    ffblk->eindx = 0 ;         /* siguiente entrada del directorio, a leer */
    return(findNextSo1(ffblk)) ;
}

int findNextSo1 ( ffblk_t far * ffblk )     /* obtiene el siguiente sector */
{                                                   /* del directorio raiz */
    entrada_t far * entrada ;                           /* nEntrsPorSector */
    dword_t sectorLogico ;
    word_t i ;
    byte_t unidadLogica = ffblk->unidadLogica ;
	int db = descUnidadLogica[unidadLogica].db ;

    entrada = (entrada_t far *)ffblk->buffer ;

    sectorLogico = primerSectorDirRaiz + (ffblk->eindx/entradasPorSector) ;

    while (ffblk->eindx < entradasDirRaiz)
    {
//      leerSectorUL(sectorLogico, unidadLogica, (pointer_t)entrada) ;
        opSectorDB(sectorLogico, db, (pointer_t)entrada, cmd_read_sector) ;		
        for ( i = 0 ; i < entradasPorSector ; i++ )
            if ((entrada[i].nombre[0] != 0x00) && (entrada[i].nombre[0] != 0xE5))
            {
                ffblk->bindx = i ;
                return(0) ;
            }
        ffblk->eindx += 16 ;
        sectorLogico++ ;
    }
    return(1) ;
}

bool_t formatearNombre ( const char far * nombre, char * formateado )
{
    word_t i, j, k, numPtos, posPto ;
    char car ;
    for ( i = 0 ; i < 11 ; i++ ) formateado[i] = ' ' ;
    formateado[11] = (char)0 ;
    i = 0 ;
    j = 0 ;
    posPto = 0 ;
    numPtos = 0 ;
    while ((car = nombre[i]) != ((char)0))
    {
        if ((('A' <= car) && (car <= 'Z')) || (('0' <= car) && (car <= '9')) ||
                (car == '_') || (car == '-'))
            j++ ;
        else if (car == '.')
        {
            posPto = i ;
            numPtos++ ;
        }
        else return(FALSE) ;              /* caracter extra¤o en el nombre */
        i++ ;
    }
    if ((i == 0) ||
        (j == 0) ||
        (numPtos > 1) ||
        ((numPtos == 1) &&
         ((posPto == 0) ||
          (posPto == (i-1))))) return(FALSE) ;
		  
    if ((i == j) && (i <= 8)) 
    {
        for ( k = 0 ; k < i ; k++ ) formateado[k] = nombre[k] ;
        return(TRUE) ;
    }
    if ((numPtos == 1) && (i == (j + 1)) &&
            (j <= 11) && ((i - posPto) <= 4))
    {
        for ( k = 0 ; k < posPto ; k++ ) formateado[k] = nombre[k] ;
        for ( k = posPto+1 ; k < i ; k++ )
            formateado[7+(k-posPto)] = nombre[k] ;
        return(TRUE) ;
    }
    return(FALSE) ;    /* varios ptos, tam nombre > 11 o tam extension > 3 */
}

long tamFichero ( int df )
{
    if ((df < 0) || (maxFichAbiertos <= df)) return(-1) ;
    if (tablaFichAbiertos[df].libre) return(-1) ;
    return(tablaFichAbiertos[df].entrada.tam) ;
}

int abrirFichero ( const char far * nombre, byte_t unidadLogica )
{

    entrada_t far * entrada = (entrada_t far *)MK_FP(segBuferSO1, 0x0000) ;
    char nombreFormateado [12] ;
    dword_t sectorLogico ;
    word_t i, j, k ;
	int db = descUnidadLogica[unidadLogica].db ;

    if (!formatearNombre(nombre, (char *)nombreFormateado)) return(-1) ;

    for ( i = 0 ; i < maxFichAbiertos ; i++ )
        if ((!tablaFichAbiertos[i].libre) &&
                (tablaFichAbiertos[i].unidadLogica == unidadLogica) &&
//              (!strncmp(tablaFichAbiertos[i].entrada.nombre, (char *)&nombreFormateado, 11)))
                (!strncmpu((char far *)tablaFichAbiertos[i].entrada.nombre,
                           (char far *)&nombreFormateado, 11)))
            return(i) ;

    if (numFichAbiertos == maxFichAbiertos)
    {
        tablaFichAbiertos[victimaASustituir].libre = TRUE ;
        victimaASustituir = (victimaASustituir + 1) % maxFichAbiertos ;
        numFichAbiertos-- ;
    }

    sectorLogico = primerSectorDirRaiz ;
    for ( i = 0 ; i < sectoresDirRaiz ; i++)
    {
//      leerSectorUL(sectorLogico, unidadLogica, (pointer_t)entrada) ;
        opSectorDB(sectorLogico, db, (pointer_t)entrada, cmd_read_sector) ;				
        for ( j = 0 ; j < entradasPorSector ; j++ )
            if ((entrada[j].nombre[0] != 0x00) &&
                    (entrada[j].nombre[0] != 0xE5) &&
                    ((entrada[j].atr & 0x08) == 0x00) &&   /* no etiqueta de volumen */
                    ((entrada[j].atr & 0x10) != 0x10) &&                  /* fichero */
                    (!strncmpu((char far *)entrada[j].nombre,
                               (char far *)&nombreFormateado, 11)))
            {
                k = 0 ;
                while (!tablaFichAbiertos[k].libre) k++ ;
                tablaFichAbiertos[k].libre = FALSE ;
                memcpy(&tablaFichAbiertos[k].entrada, &entrada[j], sizeof(entrada_t)) ;
                tablaFichAbiertos[k].unidadLogica = unidadLogica ;
                tablaFichAbiertos[k].sectorLogico = (word_t)sectorLogico ;
                tablaFichAbiertos[k].numEntrada = j ;
                numFichAbiertos++ ;
                return(k) ;
            }
        sectorLogico++ ;
    }
    return(-1) ;
}

int cerrarFichero ( int df )
{
//  entrada_t entrada[nEntrsPorSector] ;
    if ((0 <= df) && (df < maxFichAbiertos) &&
            (!tablaFichAbiertos[df].libre))
    {

//  leerSector(tablaFichAbiertos[df].sectorlogico,
//             tablaFichAbiertos[df].unidad,
//             &entrada) ;
//  entrada[df] = tablaFichAbiertosdfid].entrada ;

        tablaFichAbiertos[df].libre = TRUE ;
        return(0) ;
    }
    return(-1) ;
}

/* tamProceso: calcula el tamaño del proceso expresado en paragrafos a     */
/* partir de la informacion presente en la cabecera del fichero ejecutable */

word_t tamProceso ( int df )
{

//entrada_t far * entrada = (entrada_t far *)ptrBuferSO1 ;
    entrada_t entrada [nEntrsPorSector] ;
    word_t cluster ;
    word_t sectorlogico ;
    cabecera_t far * cabecera ;
    word_t despl ;
    word_t n ;
    word_t tamCodigo ;
    word_t tamPila ;
	int db ;

    if (df < 0) return(0x0000) ;
    if (unidadLogicaActual < 0)
    {
        n = readDOS(df, (char *)&entrada, 512) ;
//  n = readDOSFar(df, (pointer_t)entrada, 512) ;
        if (n < sizeof(cabecera_t))
        {
            return(0x0000) ;
        }
    }
    else
    {
        if (maxFichAbiertos <= df) return(0x0000) ;
        if (tablaFichAbiertos[df].libre) return(0x0000) ;
//      leerSectorSO1(tablaFichAbiertos[df].sectorLogico,
//                    tablaFichAbiertos[df].unidadLogica,
//                    (pointer_t)&entrada) ;
        db = descUnidadLogica[tablaFichAbiertos[df].unidadLogica].db ;
        opSectorDB(tablaFichAbiertos[df].sectorLogico, db, (pointer_t)entrada, cmd_read_sector) ;
        if (strncmp((char far *)tablaFichAbiertos[df].entrada.nombre,
                    (char far *)entrada[tablaFichAbiertos[df].numEntrada].nombre,
                    11))
            return(0x0000) ;
//      printStrBIOS("\n tamProceso B") ;
//      printStrBIOS("\n (pointer_t)&entrada = ") ;
//      printPtrBIOS((pointer_t)&entrada) ;
//      printStrBIOS("\n sectorLogico = ") ;
//      printDecBIOS(tablaFichAbiertos[df].sectorLogico, 1) ;
//      printStrBIOS("\n unidadLogica = 0x") ;
//      printHexBIOS(tablaFichAbiertos[df].unidadLogica, 4) ;
//      printStrBIOS("\n numEntrada = ") ;
//      printDecBIOS(tablaFichAbiertos[df].numEntrada, 1) ;
        if (tablaFichAbiertos[df].entrada.primerCluster !=
                entrada[tablaFichAbiertos[df].numEntrada].primerCluster)
            return(0x0000) ;

        cluster = tablaFichAbiertos[df].entrada.primerCluster ;
        sectorlogico = (word_t)primerSectorDatos + (cluster-2) ;
//      leerSectorSO1(sectorlogico, tablaFichAbiertos[df].unidadLogica, (pointer_t)&entrada) ;
        opSectorDB(sectorlogico, db, (pointer_t)entrada, cmd_read_sector) ;
    }

    cabecera = (cabecera_t far *)&entrada ;
    if (strncmp((char far *)cabecera->magicbyte,
                (char far *)ptrMagicByteUsr, 3))          /* AJUSTES */
    {
        if (strncmp((char far *)cabecera->magicbyte,
                    (char far *)ptrMagicByteSO1, 3))
            return(0x0000) ;                        /* cabecera incorrecta */
        cabecera = (cabecera_t far *)((pointer_t)&entrada + desplCab()) ;
    }
    if ((cabecera->magicbyte[ 5] != ptrMagicByteUsr[ 5]) ||   /* ver AJUSTES.H */
            (cabecera->magicbyte[ 8] != ptrMagicByteUsr[ 8]) ||
            (cabecera->magicbyte[11] != ptrMagicByteUsr[11]))
        return(0x0000) ;                            /* cabecera incorrecta */

    despl = (16*cabecera->segDatosSR) + cabecera->startData ;
    if (despl & 0x0001) despl++ ;                   /* alineamos a palabra */

    tamCodigo = (despl >> 4) ;                            /* en paragrafos */
    if (cabecera->SP0 <= 0xFFF0)
        tamPila = (cabecera->SP0+15)/16 ;
    else
        tamPila = 0x0FFF ;

    return (tamCodigo + tamPila) ;

}

int cargaFicheroAbierto_Ok ( int df, word_t segmento )
{

    entrada_t far * entrada = (entrada_t far *)MK_FP(segBuferSO1, 0x0000) ;
    pointer_t ptr = MK_FP(segmento, 0x0000) ;
    word_t sectorlogico ;
//  word_t primerSectDatos ;
    word_t numEntrada ;
    word_t cluster ;
    word_t i ;
	int db ;
	
    if ((df < 0) || (maxFichAbiertos <= df) ||
            (tablaFichAbiertos[df].libre))
        return(FALSE) ;
		
	db = descUnidadLogica[tablaFichAbiertos[df].unidadLogica].db ;	
//  leerSectorUL(tablaFichAbiertos[df].sectorLogico,
//               tablaFichAbiertos[df].unidadLogica,
//               (pointer_t)entrada) ;
    opSectorDB(tablaFichAbiertos[df].sectorLogico, db, (pointer_t)entrada, cmd_read_sector) ;		

	numEntrada = tablaFichAbiertos[df].numEntrada ;
	
    if (strncmp((char far *)tablaFichAbiertos[df].entrada.nombre,
                (char far *)entrada[numEntrada].nombre,
                      11))
        return(FALSE) ;

	if (tablaFichAbiertos[df].entrada.primerCluster !=
            entrada[numEntrada].primerCluster)
        return(FALSE) ;
    i = 0 ;
    cluster = tablaFichAbiertos[df].entrada.primerCluster ;
    while ((cluster & 0xFF8) != 0xFF8)
    {
        cluster = FAT[cluster] ;
        i++ ;

        if (i > clusteres)
        {
            printStrBIOS("\n La FAT tiene un ciclo: clusteres = ") ;
            printLIntBIOS(clusteres, 1) ;
            printStrBIOS(" i = ") ;
            printIntBIOS(i, 1) ;
            printStrBIOS(" cluster = 0x") ;
            printHexBIOS(cluster, 4) ;
            return(FALSE) ;
        }
    }
	
    if ((i != numSectores(tablaFichAbiertos[df].entrada.tam)) &&
            ((tablaFichAbiertos[df].entrada.atr & 0x10) == 0x00))
    {
        /*
            printStrWin(win, "\n tamanio del fichero erroneo k = ") ;
            printDecWin(win, i, 1) ;
            printStrWin(win, "\n") ;
        */
        return(FALSE) ;
    }
//  primerSectDatos =
//    BPB[tablaFichAbiertos[df].unidadLogica].sectoresReservados +
//    BPB[tablaFichAbiertos[df].unidadLogica].numeroDeFATs *
//      BPB[tablaFichAbiertos[df].unidadLogica].sectoresPorFAT +
//    BPB[tablaFichAbiertos[df].unidadLogica].entradasDirRaiz/nEntrsPorSector ;

    i = 0 ;
    cluster = tablaFichAbiertos[df].entrada.primerCluster ;
    while ((cluster & 0xFF8) != 0xFF8)
    {
        sectorlogico = (word_t)primerSectorDatos + (cluster-2) ;
//      leerSectorSO1(sectorlogico, tablaFichAbiertos[df].unidadLogica, ptr) ;
        opSectorDB(sectorlogico, db, ptr, cmd_read_sector) ;
        incPtr((pointer_t *)&ptr, 512) ;
        cluster = FAT[cluster] ;
    }
    /* borrar
      particion[part].tamFichero = tablaFichAbiertos[df].entrada.tam ;
    */

    return(TRUE) ;
}

int cargaFichero_Ok ( char * nombre, byte_t unidad, word_t segmento )
{
    entrada_t entrada[nEntrsPorSector] ;
    pointer_t ptr = MK_FP(segmento, 0x0000) ;
    char nombreFormateado [12] ;
    word_t sectorlogico ;
//  word_t primerSector ;
//  word_t primerSectDatos ;
//  word_t nSectsDirRaiz ;
    word_t cluster ;
    word_t i, j, k ;
	int db = descUnidadLogica[unidad].db ;
    /*
      ptrWindow_t win ;

        win = descProceso[indProcesoActual].win ;
    */
    if (!formatearNombre(nombre, (char *)&nombreFormateado))
    {
        return(FALSE) ; 
    }

//  primerSector = BPB[unidad].sectoresReservados +
//                   BPB[unidad].numeroDeFATs * BPB[unidad].sectoresPorFAT ;

//  nSectsDirRaiz = BPB[unidad].entradasDirRaiz/nEntrsPorSector ;

//  primerSectDatos = primerSector + nSectsDirRaiz ;

    sectorlogico = (word_t)primerSector ;       /* primer sector del directorio raiz */

    for ( i = 0 ; i < (entradasDirRaiz/entradasPorSector) ; i++)
    {
//      leerSectorUL(sectorlogico, unidad, (pointer_t)&entrada) ;
        opSectorDB(sectorlogico, db, (pointer_t)entrada, cmd_read_sector) ;		
        for ( j = 0 ; j < entradasPorSector ; j++ )
            if ((entrada[j].nombre[0] != 0x00) &&
                    (entrada[j].nombre[0] != 0xE5) &&
                    ((entrada[j].atr & 0x08) == 0x00) &&   /* no etiqueta de volumen */
                    ((entrada[j].atr & 0x10) != 0x10) &&                  /* fichero */
                    (!strncmp((char far *)entrada[j].nombre,
                              (char far *)&nombreFormateado, 11)))
            {
                k = 0 ;
                cluster = entrada[j].primerCluster ;
                while ((cluster & 0xFF8) != 0xFF8)
                {
                    cluster = FAT[cluster] ;
                    k++ ;
                    if (k > clusteres)
//                  if (k > BPB[unidad].sectores16) 
                        /*          printStrWin(win, "\n la FAT tiene un ciclo\n") ; */
                        return(FALSE) ;
                }
                if ((k != numSectores(entrada[j].tam)) &&
                        ((entrada[j].atr & 0x10) == 0x00))
                {
                    /*
                            printStrWin(win, "\n tamanio del fichero erroneo k = ") ;
                            printDecWin(win, k, 1) ;
                            printStrWin(win, "\n") ;
                    */
                    return(FALSE) ;
                }
                cluster = entrada[j].primerCluster ;
                while ((cluster & 0xFF8) != 0xFF8)
                {
                    sectorlogico = (word_t)primerSectorDatos + (cluster-2) ;
//                  leerSectorUL(sectorlogico, unidad, ptr) ;
                    opSectorDB(sectorlogico, db, ptr, cmd_read_sector) ;		
                    incPtr((pointer_t *)&ptr, 512) ;
                    cluster = FAT[cluster] ;
                }
                /* borrar
                      particion[part].tamFichero = entrada[j].tam ;
                */
                return(TRUE) ;
            }
        sectorlogico++ ;
    }
    return(FALSE) ;
}

/* ----------------------------------------------------------------------- */

#pragma warn -par

static int far openSF ( int dfs, modoAp_t modo ) 
{
	int df ;
    char far * nombre = MK_FP(tramaProceso->ES, tramaProceso->BX) ;

//  printStrBIOS("\n nombre = ") ;
//  printStrBIOS(nombre) ;
//  printStrBIOS(" uLActual = ") ;
//  printIntBIOS(unidadLogicaActual, 1) ;
//  printStrBIOS(" modo = ") ;
//  printHexBIOS(modo, 4) ;
    if (modo & O_RDONLY) 
	{
        df = abrirFichero(nombre, unidadLogicaActual) ;
//      printStrBIOS(" df = ") ;
//      printIntBIOS(df, 1) ;
        if (df >= 0) 
	    {
            dfs = crearFich(nombre, rec_sf, df, fRegular) ;     /* menor = df2 */
            return(dfs) ;
        }
	}
    return(-1) ;
}

static int far releaseSF ( int dfs ) 
{
//  printStrBIOS("\n release dfs = ") ; printDecBIOS(dfs, 1) ;
    cerrarFichero(descFichero[dfs].menor) ;            /* se cierra el fichero */
    destruirFich(dfs) ;
    return(0) ;
}

static int far readSF ( int dfs, pointer_t dir, word_t nbytes )
{
    int n ;
//  n = readDOS(descFichero[dfs].menor, (char far *)dir, nbytes) ;
//  return(n) ;
    return(0) ;
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
    strcpy(dR.nombre, "SF") ;
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

void * SF ( void * arg ) ;
//void * SF ( void * arg ) 
void * SF ( ) 
{
	asm cli ;
	
//  pindxSrvSF = getpindx() ;
    pindxSrvSF = indProcesoActual ;            /* pindx del servidor de SF */

        {
            macro_user2system() ; /* CF = 1 (system), CF = 0 (user) desbl. */
            if (_FLAGS & 0x0001)
            {
                tramaProceso->Flags &= 0xFFFE ;                  /* CF = 0 */
                bloquearProcesoActual(rec_sf) ;
            }  
        }
	
	for ( ; ; ) 
	{
		
		asm sti ;
		
	}
	return(0) ;
}