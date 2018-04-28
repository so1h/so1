/* ----------------------------------------------------------------------- */
/*                                  sf.c                                   */
/* ----------------------------------------------------------------------- */
/*                                                                         */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\tipos.h>                                      /* incPtr */
#include <so1pub.h\carsctrl.h>
#include <so1pub.h\memory.h>                          /* memcpy, memcpy_fd */
#include <so1pub.h\strings.h>                /* strlcpy, strncmp, strncmpu */
#include <so1pub.h\msdos.h>
#include <so1.h\ajustes.h>                                   /* unidadBIOS */
#include <so1.h\bios.h>                    /* leerSectorCSH, leerSectorLBA */
#include <so1.h\gm.h>
#include <so1.h\procesos.h>
#include <so1.h\db.h>                                       /* ptrBuferSO1 */
#include <so1.h\units.h>  
#include <so1.h\fatunit.h>  
#include <so1.h\sf.h>                                 /* strncmp, strncmpu */

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

#if (FALSE)

int unidadLogicaListaSo1 ( int unidadLogica )
{

    boot_t far * boot = (boot_t far *)ptrBuferSO1 ;
    int unidadFisica ;
    char strSF [9] ;
    int err = 0 ;

    if ((unidadLogica < 0) || (unidadLogica >= maxUL)) return(-1) ;
    unidadFisica = descUnidadLogica[unidadLogica].ufindx ;
    if (unidadFisica < 0) return(-1) ;

//printStrBIOS("\n unidadLogicaListaSo1(") ;
//printDecBIOS(unidadLogica, 1) ;
//printStrBIOS(")") ;

    err = leerSectorUL(primerSector, unidadLogica, (pointer_t)boot) ;
//printStrBIOS(" err = ") ;
//printIntBIOS(err, 1) ;
    if (err) return(-2) ;

    if ((boot->instJMP[0] != 0xEB) || (boot->instJMP[1] != 0x3C) || (boot->instNOP != 0x90) ||
            (boot->signatura[0] != 0x55) || (boot->signatura[1] != 0xAA)) return(-3) ;

    if ((boot->BPB.bytesPorSector != 512) ||
        (boot->BPB.sectoresPorCluster != 1) ||
//      (boot->BPB.sectoresReservados != 1) ||
        (boot->BPB.numeroDeFATs != 2) ||
//      (boot->BPB.entradasDirRaiz != 224) ||
//      (boot->BPB.sectores16 != 2880) ||
//      (boot->BPB.tipoDeMedio != 0xF0) ||
//      (boot->BPB.sectoresPorFAT != 9) ||
//      (boot->BPB.sectoresPorPista != 18) ||
//      (boot->BPB.cabezas != 2) ||
//      (boot.BPB.primerSector != 0) ||
        (boot->BPB.sectores != 0) ||
        (strncmp((char far *)&(boot->SF), "FAT12   ", 8) &&
         strncmp((char far *)&(boot->SF), "FAT16   ", 8)))
    {

//   printStrBIOS("\n Formato de disquete no esperado") ;
        return(-4) ;
    }

//  memcpy(&BPB[unidadLogica], &boot->BPB, sizeof(BPB_t)) ;

    strlcpy((char far *)&strSF, (char far *)&(boot->SF), 8) ;
    if (!strncmp((char far *)&strSF, "FAT12   ", 8))
    {
        if (cargaFAT12_Ok(unidadLogica) != 0) return (-5) ;
    }
    else if (!strncmp((char far *)&strSF, "FAT16   ", 8))
    {
        if (cargaFAT16_Ok(unidadLogica) != 0) return (-6) ;
    }
    else
        return(-7) ;
    return(0) ;
}

#endif

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
{
    /* del directorio raiz */
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

    if (strncmp((char far *)tablaFichAbiertos[df].entrada.nombre,
                (char far *)entrada[tablaFichAbiertos[df].numEntrada].nombre,
                      11))
        return(FALSE) ;

    if (tablaFichAbiertos[df].entrada.primerCluster !=
            entrada[tablaFichAbiertos[df].numEntrada].primerCluster)
        return(FALSE) ;

    i = 0 ;
    cluster = tablaFichAbiertos[df].entrada.primerCluster ;
    while ((cluster & 0xFF8) != 0xFF8)
    {
        cluster = FAT[cluster] ;
        i++ ;

        if (i > clusteres)
        {
//    if (i > BPB[tablaFichAbiertos[df].unidadLogica].sectores16) {
            /*
                  printStrWin(win, "\n la FAT tiene un ciclo\n") ;
            */
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