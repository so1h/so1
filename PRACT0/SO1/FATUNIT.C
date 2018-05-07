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
#include <so1.h\db.h>                           /* ptrBuferSO1, opSectorDB */
#include <so1.h\units.h>                  /* unidad(Fisica/Logica)Arranque */ 
#include <so1.h\fatunit.h> 

int unidadLogicaActual = -1 ;

int unidadFisicaActual = -1 ;

/* ------------------------------- FAT12/16 ------------------------------ */
/*            valores correspondientes a la unidad logica enuso           */
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

int inicFatUnit ( void )                     /* asigna memoria para la FAT */
{
    word_t segmento ;
	byte_t tipo ;
	int uFActual ;
	int uLActual ;
	int i ;
	
	descUnidadFisica_t * ptrDUFisica ;
	descUnidadLogica_t * ptrDULogica ;
	descParticion_t * ptrDParticion ;
	BPB_t * ptrBPB ; 
	
    uFActual = unidadFisicaActual = unidadFisicaArranque ;	
    uLActual = unidadLogicaActual = unidadLogicaArranque ;	
	
	ptrDUFisica = (descUnidadFisica_t *)&descUnidadFisica[uFActual] ;
	ptrDULogica = (descUnidadLogica_t *)&descUnidadLogica[uLActual] ;
	ptrDParticion = (descParticion_t *)&ptrDUFisica->descParticion ;
	ptrBPB = (BPB_t *)&ptrDULogica->BPB ;

    sectoresPorPista = ptrBPB->sectoresPorPista ;

    cabezas = ptrBPB->cabezas ;

    sectoresPorCluster = ptrBPB->sectoresPorCluster ;

    bytesPorSector = ptrBPB->bytesPorSector ;

    bytesPorCluster = bytesPorSector*sectoresPorCluster ;

    entradasPorSector = bytesPorSector/bytesPorEntrada ;

    entradasPorCluster = entradasPorSector*sectoresPorCluster ;

    entradasDirRaiz = ptrBPB->entradasDirRaiz ;

    sectoresDirRaiz = entradasDirRaiz/entradasPorSector ;

	i = ptrDULogica->indParticion ;
	
    primerSector = ptrDParticion[i].primerSector ;

    if (primerSector != ptrBPB->primerSector)
    {
        printStrBIOS("\n primerSector de la TP (") ;
        printLDecBIOS(primerSector, 1) ;
        printStrBIOS(") difiere del que figura en el BPB (") ;
        printLDecBIOS(ptrBPB->primerSector, 1) ;
        printCarBIOS(')') ;
        leerTeclaBIOS() ;
        rebootBIOS() ;
    }

    primerSectorFAT =
        primerSector + ptrBPB->sectoresReservados ;

    primerSectorDirRaiz =
        primerSectorFAT
        + ptrBPB->sectoresPorFAT
        * ptrBPB->numeroDeFATs ;

    primerSectorDatos =
        primerSectorDirRaiz + sectoresDirRaiz ;

    tamFAT = (dword_t)ptrBPB->sectoresPorFAT * (dword_t)bytesPorSector ;

    if (d_bloque[ptrDULogica->db].unidadBIOS >= 0x80)	     /* disco duro */
	    if ((tipo = ptrDParticion[i].tipo) == 0x04)
            entradasFAT = tamFAT/2 ;                              /* FAT16 */
		else return(-1) ;
    else 
	{	
        tipo = 0x01 ; 
        entradasFAT = (tamFAT*2)/3 ;                              /* FAT12 */
	}
		
	if ((ptrBPB->numeroDeFATs == 0) ||
        (ptrBPB->numeroDeFATs > 2))		
    {
        printStrBIOS("\n numero de FATs (") ;
        printDecBIOS(ptrBPB->numeroDeFATs, 1) ;
        printStrBIOS(") erroneo ") ;
        leerTeclaBIOS() ;
        rebootBIOS() ;
	}		
		
    clusteres =
        ptrBPB->sectores16
        - ptrBPB->sectoresReservados
        - (ptrBPB->sectoresPorFAT * ptrBPB->numeroDeFATs)
        - (entradasDirRaiz/entradasPorSector)
        + 2 ; /* clusteres 0 y 1 ya que el primer cluster de datos es el 2 */

//  /* reservamos memoria para la tabla FAT desempaquetada */

//  printStrBIOS("\n tamFAT = ") ;
//  printLHexBIOS(tamFAT, 8) ;
//  printStrBIOS(" entradasFAT = ") ;
//  printLDecBIOS(entradasFAT, 1) ;

    segmento = k_buscarBloque(                                       /* GM */
	    (word_t)(((sizeof(word_t)*entradasFAT)+15)/16)
	) ;  
    FAT = (word_t far *)MK_FP(segmento, 0x0000) ;

    printStrBIOS(" FAT = ") ;
    printPtrBIOS((pointer_t)FAT) ;
//  printStrBIOS(" tipo = 0x") ;
//  printHexBIOS(tipo, 4) ;
	
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

int cargarSectoresFAT ( byte_t unidadLogica, pointer_t dir )
{
	descUnidadLogica_t * ptrDULogica ;
    dword_t sectorLogico = primerSectorFAT ;
    pointer_t ptr ;
    int i ;	
	
	ptrDULogica = (descUnidadLogica_t *)&descUnidadLogica[unidadLogica] ;
    ptr = dir ;
    for ( i = 0 ; i < ptrDULogica->BPB.sectoresPorFAT ; i++ )
    {
//      leerSectorUL(sectorLogico, unidadLogica, ptr) ; /* danger err 0x09 */
//      leerSectorSO1(sectorLogico, unidadLogica, ptr) ;      /* no danger */
        opSectorDB(sectorLogico, ptrDULogica->db, ptr, cmd_read_sector) ;
        incPtr((pointer_t *)&ptr, 512) ;
        sectorLogico++ ;
    }
    return(0) ;
}

void desempaquetarFAT12 ( pointer_t    bufer, 
                          word_t       numeroDeEntradas, 
						  word_t far * FAT )
{
    pointer_t ptr = bufer ;
    int i ;
    for ( i = 0 ; i < numeroDeEntradas ; i = i + 2 )
    {
//      FAT[i] = ((ptr[1] << 8) + ptr[0]) & 0x0FFF ;
//      FAT[i+1] = ((word_t)((ptr[2] << 8) + ptr[1])) >> 4 ;
//      incPtr((pointer_t *)&ptr, 3) ;
        FAT[i] = (*((word_t far *)ptr++)) & 0x0FFF ;
 		FAT[i+1] = (*((word_t far *)ptr)) >> 4 ;
		ptr += 2 ;
    }
}

int cargaFAT12_Ok ( byte_t unidadLogica )
{
    pointer_t bytesFAT ;       /* bufer de lectura de la FAT12 empaquetada */
    word_t segmento ;
    segmento =  k_buscarBloque((word_t)((tamFAT + 15)/16)) ;         /* GM */
    bytesFAT = (pointer_t)MK_FP(segmento, 0x0000) ;
    printStrBIOS(" bytes packed FAT12 = ") ;
    printPtrBIOS((pointer_t)bytesFAT) ;
    cargarSectoresFAT(unidadLogica, bytesFAT) ;    /* lectura FAT12 packed */
    if ((bytesFAT[0] != descUnidadLogica[unidadLogica].BPB.tipoDeMedio) ||
        (bytesFAT[1] != 0xFF) ||
        (bytesFAT[2] != 0xFF))
        return(-1) ;                              /* signatura de la FAT12 */
    desempaquetarFAT12(bytesFAT, (word_t)entradasFAT, FAT) ;
    k_devolverBloque(segmento, (word_t)((tamFAT + 15)/16)) ;
    return(0) ;
}

int cargaFAT16_Ok ( byte_t unidadLogica )
{
    cargarSectoresFAT(unidadLogica, (pointer_t)FAT) ;  /* lectura de FAT16 */
    if ((FAT[0] != (0x0FF00 + descUnidadLogica[unidadLogica].BPB.tipoDeMedio)) ||
        (FAT[1] != 0xFFFF))
        return(-1) ;                              /* signatura de la FAT16 */
    return(0) ;
}