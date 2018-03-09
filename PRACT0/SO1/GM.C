/* ----------------------------------------------------------------------- */
/*                                   gm.c                                  */
/* ----------------------------------------------------------------------- */
/*   Gestor de Memoria: Particiones variables (lista de bloques libres)    */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\bios_0.h>   /* memBIOS, printStrBIOS, leerTeclaBIOS ... */
#include <so1pub.h\msdos.h>                                      /* segPSP */
#include <so1pub.h\strings.h>                                    /* strcpy */
#include <so1.h\ajustsp.h>                                      /* SP0_SO1 */
#include <so1.h\ajustes.h>                      /* CS_SO1, DS_SO1, modoSO1 */
#include <so1.h\recursos.h>                         /* crearRec, crearFich */
#include <so1.h\procesos.h>               /* descProceso, indProcesoActual */
#include <so1pub.h\def_mem.h>                               /* ptrBloque_t */
#include <so1.h\gm.h>

rindx_t rec_gm ;

dfs_t dfs_gm ;

ptrBloque_t listaLibres ;            /* lista de bloques libres (cabecera) */

word_t tamBloqueMax ;                          /* tamanio del mayor bloque */

word_t k_buscarBloque ( word_t tam )
{
    ptrBloque_t ptrBloqueIz, ptrBloque, ptrBloqueDe ;
    ptrBloque_t ptrBloqueNuevo ;
    word_t segmento ;
    word_t segmentoNuevo ;
    word_t tamOriginal ;         /* tamanio original del bloque encontrado */

    if (tam > tamBloqueMax) return(0x0000) ;

    ptrBloque = MK_FP(listaLibres->sig, 0x0000) ;
    while (ptrBloque->tam < tam)
        ptrBloque = MK_FP(ptrBloque->sig, 0x0000) ;

    segmento = FP_SEG(ptrBloque) ;

//  printStrBIOS("\n k_buscarBloque: segmento = ") ;
//  printHexBIOS(segmento, 4) ;
//  leerTeclaBIOS() ;

    tamOriginal = ptrBloque->tam ;

    ptrBloqueIz = MK_FP(ptrBloque->ant, 0x0000) ;
    ptrBloqueDe = MK_FP(ptrBloque->sig, 0x0000) ;

    if (tamOriginal > tam) {
        segmentoNuevo = FP_SEG(ptrBloque) + tam ;
        ptrBloqueNuevo = MK_FP(segmentoNuevo, 0x0000) ;
        ptrBloqueNuevo->tam = tamOriginal - tam ;
        ptrBloqueNuevo->sig = ptrBloque->sig ;
        ptrBloqueNuevo->ant = ptrBloque->ant ;
        ptrBloqueIz->sig = FP_SEG(ptrBloqueNuevo) ;
        ptrBloqueDe->ant = FP_SEG(ptrBloqueNuevo) ;
        ptrBloque->tam = tam ;
    }
    else                                           /* ptrBloque-tam == tam */
    {
        ptrBloqueIz->sig = ptrBloque->sig ;
        ptrBloqueDe->ant = ptrBloque->ant ;
    }

    if (tamOriginal == tamBloqueMax)            /* actualizar tamBloqueMax */
    {
        tamBloqueMax = 0 ;
        ptrBloque = MK_FP(listaLibres->sig, 0x0000) ;
        while (ptrBloque != listaLibres)
        {
            if (ptrBloque->tam > tamBloqueMax)
                tamBloqueMax = ptrBloque->tam ;
            ptrBloque = MK_FP(ptrBloque->sig, 0x0000) ;
        }
    }

//  printStrBIOS(" tamBloqueMax = ") ;
//  printHexBIOS(tamBloqueMax, 4) ;
//  printStrBIOS(" Ps") ;

    return(segmento) ;
}

bool_t k_devolverBloque ( word_t segmento, word_t tam )
{

    ptrBloque_t ptrBloqueIz, ptrBloque, ptrBloqueDe ;

    if (FP_SEG(listaLibres) <= segmento)
        return(FALSE) ;

    ptrBloqueDe = MK_FP(listaLibres->sig, 0x0000) ;
    while (FP_SEG(ptrBloqueDe) < segmento)
        ptrBloqueDe = MK_FP(ptrBloqueDe->sig, 0x0000) ;

    if (FP_SEG((pointer_t)ptrBloqueDe) < segmento+tam)
        return(FALSE) ;

    ptrBloque = MK_FP(segmento, 0x0000) ;

    ptrBloque->tam = tam ;
    ptrBloque->sig = FP_SEG(ptrBloqueDe) ;
    ptrBloque->ant = ptrBloqueDe->ant ;

    ptrBloqueIz = MK_FP(ptrBloqueDe->ant, 0x0000) ;

    ptrBloqueDe->ant = segmento ;

    ptrBloqueIz->sig = segmento ;

    if (((segmento+tam) == FP_SEG(ptrBloqueDe)) &&
        (ptrBloqueDe != listaLibres))
    {
        ptrBloque->tam += ptrBloqueDe->tam ;
        ptrBloque->sig = ptrBloqueDe->sig ;
        ptrBloqueDe = MK_FP(ptrBloqueDe->sig, 0x0000) ;
        ptrBloqueDe->ant = segmento ;
    }

    if (((FP_SEG(ptrBloqueIz)+ptrBloqueIz->tam) == segmento) &&
        (ptrBloqueIz != listaLibres)) {
        ptrBloqueIz->tam += ptrBloque->tam ;
        ptrBloqueIz->sig = ptrBloque->sig ;
        ptrBloqueDe = MK_FP(ptrBloque->sig, 0x0000) ;
        ptrBloqueDe->ant = FP_SEG(ptrBloqueIz) ;
        ptrBloque = ptrBloqueIz ;
    }

    if (ptrBloque->tam > tamBloqueMax)          /* actualizar tamBloqueMax */
        tamBloqueMax = ptrBloque->tam ;

//  printStrBIOS("\n k_devolverBloque: tamBloqueMax = ") ;
//  printHexBIOS(tamBloqueMax, 4) ;
//  printStrBIOS(" Ps") ;

    return(TRUE) ;
}

#pragma warn -par

int far openGM ( int dfs, modoAp_t modo )
{
    return(0) ;
}

int far releaseGM ( int dfs )
{
    return(0) ;
}

int far readGM ( int dfs, pointer_t dir, word_t nbytes )
{
    word_t far * tam = (word_t far *)dir ;
    word_t far * segmento = (word_t far *)dir ;
    if (nbytes == 2)
    {
        *segmento = k_buscarBloque(*tam) ;
        return(nbytes) ;
    }
    return(-1) ;
}

int far aio_readGM ( int dfs, pointer_t dir, word_t nbytes )
{
    word_t far * ptrWord = (word_t far *)dir ;
    word_t segmento ;
    word_t tam ;
    switch (nbytes)
    {
    case 2 : *ptrWord = tamBloqueMax ; return(nbytes) ;
    case 4 : segmento = ptrWord[0] ;
             tam = ptrWord[1] ;
             if (k_devolverBloque(segmento, tam))
               return(nbytes) ;
    default : ;
    }
    return(-1) ;
}

int far writeGM ( int dfs, pointer_t dir, word_t nbytes )
{
    return(0) ;
}

int far aio_writeGM ( int dfs, pointer_t dir, word_t nbytes )
{
    return(writeGM(dfs, dir, nbytes)) ;
}

long far lseekGM ( int dfs, long pos, word_t whence )
{
    return(-1L) ;
}

int far fcntlGM ( int dfs, word_t cmd, word_t arg )
{
    return(0) ;
}

int far ioctlGM ( int dfs, word_t request, word_t arg )
{
    return(0) ;
}

int far eliminarGM ( pindx_t pindx )
{
    return(0) ;
}

#pragma warn +par

#define maxCbGM 0

descCcb_t descCcbGM = { 0, 0, 0, maxCbGM, NULL } ;

void inicGM ( void )                             /* 1 paragrafo = 16 bytes */
{                          

    descRecurso_t dR ;

    word_t memDisponible ;             /* memoria disponible en paragrafos */
    word_t primerSegLibre ;
//  word_t far * ptrWord ;
    ptrBloque_t ptrBloque ;
    word_t sigSeg ;

    /* SO1 requiere espacio para su segmento de codigo (DS_SO1-CS_SO1) y   */
    /* para su segmento de datos que contiene la DATA, BSS y la pila, en   */
    /* total unos SP0_SO1 paragrafos.                                      */

    primerSegLibre = DS_SO1 + ((SP0_SO1 + 15)/16) ;  /* 1er seg. tras pila */

    descProceso[0].tam = primerSegLibre - CS_SO1 ;

    /* inicGM debe ocuparse de descontar de la memoria libre la memoria    */
    /* ocupada por SO1, ya que si no, las escrituras en los nodos de la    */
    /* lista de bloques libres podrían modificar los primeros bytes de     */
    /* codigo de SO1.                                                      */

    /* sigSeg = segmento siguiente al ultimo disponible por el proceso     */

    switch (modoSO1())
    {
    case modoSO1_Bin :                                   /* so1.bin (boot) */
    case modoSO1_Bs  :                          /* BOOT so1.bin (SYSLINUX) */
        sigSeg = memBIOS()*(1024/16) ;
        break ;
    case modoSO1_Com :
    case modoSO1_Exe :                                           /* hayDOS */
        sigSeg = *((word_t far *)MK_FP(segPSP(), 0x0002)) ;
        break ;
    default :
        printStrBIOS("\n inicGM() ERROR: modoSO1() = ") ;
        printHexBIOS(modoSO1(), 4) ;
        leerTeclaBIOS() ;
    }

    memDisponible = sigSeg - primerSegLibre ;                   /* sin SO1 */

    /* unico bloque libre inicial */

    ptrBloque = MK_FP(primerSegLibre, 0x0000) ;
    ptrBloque->tam = memDisponible-1 ;
    ptrBloque->sig = FP_SEG(ptrBloque) + ptrBloque->tam ;
    ptrBloque->ant = ptrBloque->sig ;

    /* cabecera */

    listaLibres = MK_FP(ptrBloque->sig, 0x0000) ;
    listaLibres->tam = 1 ;                                     /* 16 Bytes */
    listaLibres->sig = FP_SEG(ptrBloque) ;
    listaLibres->ant = listaLibres->sig ;

    tamBloqueMax = ptrBloque->tam ;

//  printStrBIOS(" \n inicGM: ptrBloque = ") ;
//  printPtrBIOS((pointer_t)ptrBloque) ;

    dR.tipo = rGM ;
    strcpy(dR.nombre, "GM") ;
    dR.ccb = (ccb_t)&descCcbGM ;
    dR.ccb->arg = NULL ;
    dR.pindx = indProcesoActual ;
    dR.numVI = 0 ;

    dR.open      = (open_t)     MK_FP(_CS, FP_OFF(openGM)) ;
    dR.release   = (release_t)  MK_FP(_CS, FP_OFF(releaseGM)) ;
    dR.read      = (read_t)     MK_FP(_CS, FP_OFF(readGM)) ;
    dR.aio_read  = (aio_read_t) MK_FP(_CS, FP_OFF(aio_readGM)) ;
    dR.write     = (write_t)    MK_FP(_CS, FP_OFF(writeGM)) ;
    dR.aio_write = (aio_write_t)MK_FP(_CS, FP_OFF(aio_writeGM)) ;
    dR.lseek     = (lseek_t)    MK_FP(_CS, FP_OFF(lseekGM)) ;
    dR.fcntl     = (fcntl_t)    MK_FP(_CS, FP_OFF(fcntlGM)) ;
    dR.ioctl     = (ioctl_t)    MK_FP(_CS, FP_OFF(ioctlGM)) ;

    dR.eliminar  = (eliminar_t) MK_FP(_CS, FP_OFF(eliminarGM)) ;

    rec_gm = crearRec(&dR) ;

    dfs_gm = crearFich("GM", rec_gm, 0, fedCaracteres) ;

}