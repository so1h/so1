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
#include <so1.h\sf.h>                                 /* strncmp, strncmpu */

/* ----------------- descriptores de unidades logicas -------------------- */

int unidadLogicaActual = -1 ;

int unidadFisicaActual = -1 ;

descUnidadLogica_t descUnidadLogica [ maxUL ] ;

descUnidadFisica_t descUnidadFisica [ maxUF ] ;

/* ------------------------------- FAT12/16 ------------------------------ */
/*            valores correspondientes a la unidad logica actual           */
/* ----------------------------------------------------------------------- */

word_t sectoresPorPista ;
word_t cabezas ;

word_t sectoresPorCluster ;
word_t bytesPorSector ;
word_t bytesPorCluster ;
word_t entradasPorSector ;
word_t sectoresPorCluster ;
word_t entradasPorCluster ;
word_t entradasDirRaiz ;
word_t sectoresDirRaiz ;
dword_t primerSector ;
dword_t primerSectorFAT ;
dword_t primerSectorDirRaiz ;
dword_t primerSectorDatos ;

dword_t tamFAT ;
dword_t entradasFAT ;
dword_t clusteres ;
word_t far * FAT ;               /* representa la tabla FAT desempaquetada */
//               /* se inicializa en inicSF apuntando a un segmento lejano */
//                /* con el fin de no gastar memoria del segmento de datos */

/* ----------------------------------------------------------------------- */

int numFichAbiertos ;

descriptor_de_fichero_t tablaFichAbiertos [ maxFichAbiertos ] ;

static victimaASustituir = 0 ;          /* para la politica de sustitucion */

/* leerSector de una unidad logica cualquiera (disquete o disco duro) */

int leerSectorUL ( dword_t sectorLogico, byte_t unidadLogica, pointer_t dir )
{
    int unidadFisica ;
    byte_t unidadBIOS ;
    word_t sector ;
    word_t cabeza ;
    word_t cilindro ;
    CSH_t CSH ;

//printStrBIOS("\n leerSectorUL(") ;
//printLDecBIOS(sectorLogico, 4) ;
//printStrBIOS(", ") ;
//printDecBIOS(unidadLogica, 1) ;
//printStrBIOS(", ") ;
//printPtrBIOS(dir) ;
//printStrBIOS(")") ;

    if (unidadLogica >= maxUL) return(-1) ;
    else
    {
        unidadFisica = descUnidadLogica[unidadLogica].ufindx ;
        if (unidadFisica < 0) return(-2) ;
        else
        {
            unidadBIOS = descUnidadFisica[unidadFisica].unidadBIOS ;
//printStrBIOS(" unidadBIOS = 0x") ;
//printHexBIOS(unidadBIOS, 2) ;
            if (unidadBIOS < 0x80)
            {
                sector = (word_t)(sectorLogico % sectoresPorPista) + 1 ;
                cabeza = (word_t)((sectorLogico / sectoresPorPista) % cabezas) ;
                cilindro = (word_t)(sectorLogico / (sectoresPorPista * cabezas)) ;
                CSH.h = cabeza ;
                CSH.cs = ((cilindro & 0x00FF) << 8) | ((cilindro & 0x0300) >> 2) | (sector & 0x003F) ;

//printStrBIOS(" (S, C, H) = (") ;
//printDecBIOS(sector, 2) ;
//printStrBIOS(", ") ;
//printDecBIOS(cabeza, 2) ;
//printStrBIOS(", ") ;
//printDecBIOS(cilindro, 2) ;
//printStrBIOS(")") ;
//leerTeclaBIOS() ;

                return(leerSectorCSH((CSH_t *)&CSH, unidadBIOS, dir)) ;
            }
            else
                return(leerSectorLBA(sectorLogico, unidadBIOS, dir)) ;
        }
    }
}

/* lectura de sector con reintento usando un bufer seguro */

int leerSectorSO1 ( dword_t sectorLogico, byte_t unidadLogica, pointer_t dir )
{
    int err ;
    err = leerSectorUL(sectorLogico, unidadLogica, dir) ;
    if (err == 0x09)
    {
        err = leerSectorUL(sectorLogico, unidadLogica, ptrBuferSO1) ;
        memcpy_fd(dir, ptrBuferSO1, 512) ;
    }
    return(err) ;
}

int inicSF ( byte_t unidadBIOS )             /* asigna memoria para la FAT */
{

    word_t segmento ;
    byte_t tipo ;                           /* tipo de la particion activa */
    mbr_t far * mbr ;
    boot_t far * boot ;
    CSH_t CSH ;
    int ufindx ;
    int ulindx ;
    int error ;
    int i ;

    /* Se necesita un bufer de 512 bytes para la lectura segura de los     */ 
	/* sectores mediante la INT13 (DMA).                                   */

//    segBuferSO1 = segBuferSeguro() ;                                 /* GM */
//    ptrBuferSO1 = MK_FP(segBuferSO1, 0x0000) ;
    printStrBIOS("\n ptrBuferSO1 = 0x") ;
    printPtrBIOS(ptrBuferSO1) ;

    mbr = (mbr_t far *)ptrBuferSO1 ;
    boot = (boot_t far *)ptrBuferSO1 ;

    for ( ufindx = 0 ; ufindx < maxUF ; ufindx++ )     /* descUnidadFisica */
    {
        descUnidadFisica[ufindx].unidadBIOS = -1 ;
        descUnidadFisica[ufindx].contUL = 0 ;
        for ( i = 0 ; i < maxParticiones ; i++ )
        {
            descUnidadFisica[ufindx].descParticion[i].tipo = 0x00 ;
            descUnidadFisica[ufindx].ulindx[i] = -1 ;
        }
    }
    for ( ulindx = 0 ; ulindx < maxUL ; ulindx++ )     /* descUnidadLogica */
    {
        descUnidadLogica[ulindx].ufindx = -1 ;
        descUnidadLogica[ulindx].indParticion = -1 ;
        descUnidadLogica[ulindx].letra = '$' ;
    }
    if (unidadBIOS >= 0x80)                                  /* Disco duro */
    {
        unidadFisicaActual = 2 + (unidadBIOS - 0x80) ;
        unidadLogicaActual = unidadFisicaActual ;
        descUnidadLogica[unidadLogicaActual].letra = 'C' + unidadFisicaActual ;
        descUnidadFisica[unidadFisicaActual].contUL = 1 ;    /* de momento */
    }
    else                                                       /* Disquete */
    {
        unidadFisicaActual = unidadBIOS  ;
        unidadLogicaActual = unidadFisicaActual ;
        descUnidadLogica[unidadLogicaActual].letra = 'A' + unidadFisicaActual ;
        descUnidadFisica[unidadFisicaActual].contUL = 1 ;
    }
    descUnidadFisica[unidadFisicaActual].unidadBIOS = unidadBIOS ;
    descUnidadFisica[unidadFisicaActual].ulindx[0] = unidadLogicaActual ;
    descUnidadLogica[unidadLogicaActual].ufindx = unidadFisicaActual ;
    /* sector logico 0 => sector fisico 1 */
    CSH.h = 0 ;
    CSH.cs = 0x0001 ;                              /* C = 0, S = 1 y H = 0 */
//  printStrBIOS("\n CSH.h = ") ;
//  printHexBIOS(CSH.h, 2) ;
//  printStrBIOS("\n CSH.cs = ") ;
//  printHexBIOS(CSH.cs, 4) ;
//  asm { @: jmp @ ; }

    error = leerSectorCSH((CSH_t *)&CSH, unidadBIOS, (pointer_t)boot) ;  /* BR */
    if (error)
    {
        printStrBIOS("\n error al leer el sector logico 0: error = ") ;
        printHexBIOS(error, 2) ;
        leerTeclaBIOS() ;
        rebootBIOS() ;
    }

    if ((boot->signatura[0] != 0x55) || (boot->signatura[1] != 0xAA))
    {
        printStrBIOS("\n error: sector 0 sin signatura (0x55, 0xAA) ") ;
//      printCarBIOS('(') ;
//      printHexBIOS(boot->signatura[0], 2) ;
//      printStrBIOS(", ") ;
//      printHexBIOS(boot->signatura[1], 2) ;
//      printCarBIOS(')') ;
        leerTeclaBIOS() ;
        rebootBIOS() ;
    } ;
    if ((boot->instJMP[0] != 0xEB) || (boot->instJMP[1] < 0x3C) || (boot->instNOP != 0x90) ||
            (boot->signaturaExt != 0x29))       /* debe tratarse de un MBR */
	{                                  /* copiamos la tabla de particiones */
        memcpy(&descUnidadFisica[unidadFisicaActual].descParticion,
               &mbr->descParticion,
               4*sizeof(descParticion_t)
        ) ;
        for ( i = 0 ; i < maxParticiones ; i++ )
        {
            tipo = descUnidadFisica[unidadFisicaActual].descParticion[i].tipo ;
            if ((tipo != 0x00) &&
                    (descUnidadFisica[unidadFisicaActual].descParticion[i].activa == 0x80))
                break ;
        }
        if (i >= maxParticiones)
        {
            printStrBIOS("\n error: tabla de particiones sin particion activa") ;
            leerTeclaBIOS() ;
            rebootBIOS() ;
        }
        if ((tipo != 0x01) && (tipo != 0x04))
        {
            printStrBIOS("\n error: tipo de particion no soportado 0x") ;
            printHexBIOS(tipo, 2) ;
            leerTeclaBIOS() ;
            rebootBIOS() ;
        }
        descUnidadLogica[unidadLogicaActual].indParticion = i ;
        primerSector = descUnidadFisica[unidadFisicaActual].descParticion[i].primerSector ;

        error = leerSectorUL(primerSector, unidadLogicaActual, (pointer_t)boot) ;  /* BR */
        if (error)
        {
            printStrBIOS("\n error al leer el primer sector de la particion: error = ") ;
            printHexBIOS(error, 2) ;
            printStrBIOS("\n primer sector = ") ;
            printLHexBIOS(primerSector, 4) ;
            leerTeclaBIOS() ;
            rebootBIOS() ;
        }

        if ((boot->signatura[0] != 0x55) || (boot->signatura[1] != 0xAA))
        {
            printStrBIOS("\n error: primer sector de la particion sin signatura (0x55, 0xAA) ") ;
            leerTeclaBIOS() ;
            rebootBIOS() ;
        }
        if ((boot->instJMP[0] != 0xEB) || (boot->instJMP[1] < 0x3C) || (boot->instNOP != 0x90) ||
                (boot->signaturaExt != 0x29))
        {
            printStrBIOS("\n error: sector de arranque de la particion, incorrecto ") ;
            leerTeclaBIOS() ;
            rebootBIOS() ;
        }
        if ((strncmp(boot->SF, "FAT12   ", 8) && (tipo == 0x01)) ||
            (strncmp(boot->SF, "FAT16   ", 8) && (tipo == 0x04)))
        {
            printStrBIOS("\n el tipo de sistema de ficheros no corresponde a \"") ;
            for ( i = 0 ; i < 8 ; i++ ) printCarBIOS(boot->SF[i]) ;
            printCarBIOS('\"') ;
            leerTeclaBIOS() ;
            rebootBIOS() ;
        }
    }
    else    /* en el sector 0 de la unidad fisica hay un BPB en vez de MBR */
    {
        descUnidadFisica[unidadFisicaActual].descParticion[i].activa = 0x80 ;
        descUnidadFisica[unidadFisicaActual].descParticion[i].primerSector = 0 ;
        for ( i = 1 ; i < maxParticiones ; i++ )
            descUnidadFisica[unidadFisicaActual].descParticion[i].tipo = 0x00 ;
        descUnidadLogica[unidadLogicaActual].indParticion = 1 ;
        if (!strncmp(boot->SF, "FAT12   ", 8)) tipo = 0x01 ;
        else if (!strncmp(boot->SF, "FAT16   ", 8)) tipo = 0x04 ;
        else
        {
            printStrBIOS("\n error: tipo de particion no soportado 0x") ;
            printHexBIOS(tipo, 2) ;
            leerTeclaBIOS() ;
            rebootBIOS() ;
        }
        descUnidadFisica[unidadFisicaActual].descParticion[i].tipo = tipo ;
    }
    if (boot->unidad != unidadBIOS)
    {
        printStrBIOS("\n unidad de arranque distinta de la indicada en el sector de arranque ") ;
        leerTeclaBIOS() ;
        rebootBIOS() ;
    }
                                       /* copiamos la tabla de particiones */
    memcpy(&descUnidadLogica[unidadLogicaActual].BPB, &boot->BPB, sizeof(BPB_t)) ;

    sectoresPorPista = descUnidadLogica[unidadLogicaActual].BPB.sectoresPorPista ;

    cabezas = descUnidadLogica[unidadLogicaActual].BPB.cabezas ;

    sectoresPorCluster = descUnidadLogica[unidadLogicaActual].BPB.sectoresPorCluster ;

    bytesPorSector = descUnidadLogica[unidadLogicaActual].BPB.bytesPorSector ;

    bytesPorCluster = bytesPorSector*sectoresPorCluster ;

    entradasPorSector = bytesPorSector/bytesPorEntrada ;

    entradasPorCluster = entradasPorSector*sectoresPorCluster ;

    entradasDirRaiz = descUnidadLogica[unidadLogicaActual].BPB.entradasDirRaiz ;

    sectoresDirRaiz = entradasDirRaiz/entradasPorSector ;

    primerSector =
        descUnidadFisica[unidadFisicaActual].descParticion[i].primerSector ;

    if (primerSector != descUnidadLogica[unidadLogicaActual].BPB.primerSector)
    {
        printStrBIOS("\n primerSector de la TP (") ;
        printLDecBIOS(primerSector, 1) ;
        printStrBIOS(") difiere del que figura en el BPB (") ;
        printLDecBIOS(descUnidadLogica[unidadLogicaActual].BPB.primerSector, 1) ;
        printCarBIOS(')') ;
        leerTeclaBIOS() ;
        rebootBIOS() ;
    }

    primerSectorFAT =
        primerSector + descUnidadLogica[unidadLogicaActual].BPB.sectoresReservados ;

    primerSectorDirRaiz =
        primerSectorFAT
        + descUnidadLogica[unidadLogicaActual].BPB.sectoresPorFAT
        * descUnidadLogica[unidadLogicaActual].BPB.numeroDeFATs ;

    primerSectorDatos =
        primerSectorDirRaiz + sectoresDirRaiz ;

    tamFAT =
        (dword_t)descUnidadLogica[unidadLogicaActual].BPB.sectoresPorFAT
        * (dword_t)bytesPorSector ;

    if (tipo == 0x01)
        entradasFAT = (tamFAT*2)/3 ;                              /* FAT12 */
    else /* tipo == 0x04 */
        entradasFAT = tamFAT/2 ;                                  /* FAT16 */

    clusteres =
        descUnidadLogica[unidadLogicaActual].BPB.sectores16
        - descUnidadLogica[unidadLogicaActual].BPB.sectoresReservados
        - (descUnidadLogica[unidadLogicaActual].BPB.sectoresPorFAT
           * descUnidadLogica[unidadLogicaActual].BPB.numeroDeFATs)
        - (entradasDirRaiz/entradasPorSector)
        + 2 ; /* clusteres 0 y 1 ya que el primer cluster de datos es el 2 */

    /* reservamos memoria para la tabla FAT desempaquetada */

//printStrBIOS("\n tamFAT = ") ;
//printLHexBIOS(tamFAT, 8) ;
//printStrBIOS(" entradasFAT = ") ;
//printLDecBIOS(entradasFAT, 1) ;

    segmento =  k_buscarBloque((word_t)(((2*entradasFAT)+15)/16)) ;  /* GM */
    FAT = (word_t far *)MK_FP(segmento, 0x0000) ;

    printStrBIOS(" FAT = ") ;
    printPtrBIOS((pointer_t)FAT) ;

    if (((tipo == 0x01) && (cargaFAT12_Ok(unidadLogicaActual) != 0)) ||
            ((tipo == 0x04) && (cargaFAT16_Ok(unidadLogicaActual) != 0)))
    {
        printStrBIOS("\n error al cargar la FAT de la unidad logica ") ;
        printHexBIOS(unidadLogicaActual, 1) ;
        leerTeclaBIOS() ;
        rebootBIOS() ;
    }

    return(0) ;
}

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

int cargarSectoresFAT ( byte_t unidadLogica, pointer_t dir )
{
    dword_t sectorLogico = primerSectorFAT ;
    pointer_t ptr = dir ;
    int i ;
    for ( i = 0 ; i < descUnidadLogica[unidadLogica].BPB.sectoresPorFAT ; i++ )
    {
        leerSectorUL(sectorLogico, unidadLogica, ptr) ;
        incPtr((pointer_t *)&ptr, 512) ;
        sectorLogico++ ;
    }
    return(0) ;
}

void desempaquetarFAT12 ( pointer_t bufer, word_t numeroDeEntradas, word_t far * FAT )
{
    pointer_t ptr = bufer ;
    int i ;
    for ( i = 0 ; i < numeroDeEntradas ; i = i + 2 )
    {
        FAT[i] = (256*ptr[1]+ptr[0]) & 0x0FFF ;
        FAT[i+1] = ((word_t)(256*ptr[2]+ptr[1])) >> 4 ;
        incPtr((pointer_t *)&ptr, 3) ;
    }
}

int cargaFAT12_Ok ( byte_t unidadLogica )
{
    pointer_t bytesFAT ;       /* bufer de lectura de la FAT12 empaquetada */
    word_t segmento ;
    segmento =  k_buscarBloque((word_t)((tamFAT+15)/16)) ;           /* GM */
    bytesFAT = (pointer_t)MK_FP(segmento, 0x0000) ;
    cargarSectoresFAT(unidadLogica, bytesFAT) ; /* lectura FAT12 empaquetada */
    if ((bytesFAT[0] != descUnidadLogica[unidadLogica].BPB.tipoDeMedio) ||
            (bytesFAT[1] != 0xFF) ||
            (bytesFAT[2] != 0xFF))
        return(-1) ;                              /* signatura de la FAT12 */
    desempaquetarFAT12(bytesFAT, (word_t)entradasFAT, FAT) ;
    k_devolverBloque(segmento, (word_t)((tamFAT+15)/16)) ;
    return(0) ;
}

int cargaFAT16_Ok ( byte_t unidadLogica )
{
    cargarSectoresFAT(unidadLogica, (pointer_t)FAT) ; /* lectura de la FAT16 */
    if ((FAT[0] != (0x0FF00 + descUnidadLogica[unidadLogica].BPB.tipoDeMedio)) ||
            (FAT[1] != 0xFFFF))
        return(-1) ;                              /* signatura de la FAT16 */
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
{
    /* del directorio raiz */
    entrada_t far * entrada ;                           /* nEntrsPorSector */
    dword_t sectorLogico ;
    word_t i ;
    byte_t unidadLogica = ffblk->unidadLogica ;

    entrada = (entrada_t far *)ffblk->buffer ;

    sectorLogico = primerSectorDirRaiz + (ffblk->eindx/entradasPorSector) ;

    while (ffblk->eindx < entradasDirRaiz)
    {
        leerSectorUL(sectorLogico, unidadLogica, (pointer_t)entrada) ;
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
        leerSectorUL(sectorLogico, unidadLogica, (pointer_t)entrada) ;
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
        leerSectorSO1(tablaFichAbiertos[df].sectorLogico,
                      tablaFichAbiertos[df].unidadLogica,
                      (pointer_t)&entrada) ;
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
        leerSectorSO1(sectorlogico, tablaFichAbiertos[df].unidadLogica, (pointer_t)&entrada) ;
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

    if ((df < 0) || (maxFichAbiertos <= df) ||
            (tablaFichAbiertos[df].libre))
        return(FALSE) ;

    leerSectorUL(tablaFichAbiertos[df].sectorLogico,
                 tablaFichAbiertos[df].unidadLogica,
                 (pointer_t)entrada) ;

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
        leerSectorSO1(sectorlogico, tablaFichAbiertos[df].unidadLogica, ptr) ;
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
        leerSectorUL(sectorlogico, unidad, (pointer_t)&entrada) ;
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
                    {
//          if (k > BPB[unidad].sectores16) {
                        /*          printStrWin(win, "\n la FAT tiene un ciclo\n") ; */
                        return(FALSE) ;
                    }
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
                    leerSectorUL(sectorlogico, unidad, ptr) ;
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



