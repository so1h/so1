/* ----------------------------------------------------------------------- */
/*                               ejecutar.c                                */
/* ----------------------------------------------------------------------- */
/*            busqueda, carga y ejecucion de comandos externos             */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\caracter.h>
#include <so1pub.h\def_proc.h>
#include <so1pub.h\scanner.h>                                /* tamComando */
#include <so1pub.h\msdos.h>                  /* lssekMSDOS, SEEK_SET_MSDOS */
#include <so1pub.h\strings.h>
#include <so1pub.h\bios_0.h>
#include <so1pub.h\copia.h>                                  /* copiaLarga */
#include <so1.h\ejecutar.h>
#include <so1.h\gm.h>                  /* k_buscarBloque, k_devolverBloque */
#include <so1.h\procesos.h>
#include <so1.h\ajustes.h>
#include <so1.h\sf.h>

//#include <so1.h\so1dbg.h>                            /* puntoDeParadaSO1 */

pid_t kk_fork ( void ) {
  word_t segmento ;
  pindx_t pindx ;
  pointer_t origen ;
  pointer_t destino ;
  word_t SS_NuevoProceso ;
  word_t tam = descProceso[indProcesoActual].tam ;

  if (c2cPFR[DPOcupados].numElem >= (maxProcesos-1))
    return(-1) ;
  if ((segmento = k_buscarBloque(tam)) == 0x0000)
    return(-2) ;

  origen = MK_FP(descProceso[indProcesoActual].CSProc, 0x0000) ;
  destino = MK_FP(segmento, 0x0000) ;
  copiaLarga(origen, destino, (dword_t)16*(dword_t)tam) ;

  pindx = desencolarPC2c((ptrC2c_t)&c2cPFR[DPLibres]) ;
  encolarPC2c(pindx, (ptrC2c_t)&c2cPFR[DPOcupados]) ;

  origen = (pointer_t)&descProceso[indProcesoActual] ;
  destino = (pointer_t)&descProceso[pindx] ;
  copia(origen, destino, sizeof(descProceso_t)) ;

  SS_NuevoProceso = segmento
    + (seg((pointer_t)descProceso[indProcesoActual].trama)
         - descProceso[indProcesoActual].CSProc) ;

  descProceso[pindx].trama = (trama_t far *) MK_FP(
    SS_NuevoProceso,
    off((pointer_t)descProceso[indProcesoActual].trama)
  ) ;
  descProceso[pindx].CSProc = segmento ;

  descProceso[pindx].trama->DS = SS_NuevoProceso ;
  descProceso[pindx].trama->ES = SS_NuevoProceso ;
//descProceso[pindx].trama->DI = descProceso[indProcesoActual].trama->DI ;
//descProceso[pindx].trama->SI = descProceso[indProcesoActual].trama->SI ;
//descProceso[pindx].trama->BP = descProceso[indProcesoActual].trama->BP ;
//descProceso[pindx].trama->SP = descProceso[indProcesoActual].trama->SP ;
//descProceso[pindx].trama->BX = descProceso[indProcesoActual].trama->BX ;
//descProceso[pindx].trama->DX = descProceso[indProcesoActual].trama->DX ;
//descProceso[pindx].trama->CX = descProceso[indProcesoActual].trama->CX ;
//descProceso[pindx].trama->AX = descProceso[indProcesoActual].trama->AX ;
//descProceso[pindx].trama->IP = descProceso[indProcesoActual].trama->IP ;
  descProceso[pindx].trama->CS = segmento ;
//descProceso[pindx].trama->Flags = descProceso[indProcesoActual].trama->Flags ;

  registrarEnPOrdenados(pindx) ;

  descProceso[pindx].pid = nuevoPid() ;
  descProceso[pindx].hpindx = -1 ;
  inicPC2c(&descProceso[pindx].c2cHijos, &e2PFR.e2Hijos, maxProcesos + pindx, TRUE) ;
  descProceso[pindx].ppindx = indProcesoActual ;
  encolarPC2c(pindx, (ptrC2c_t)&descProceso[indProcesoActual].c2cHijos) ;

  return(pindx) ;

}

/* pindx == -1 si se quiere crear un nuevo proceso (descProceso nuevo, pid */
/*             nuevo y bloque de memoria nuevo)                            */
/* pindx != -1 si se quiere mantener el mismo descProceso y el mismo pid   */
/*             pero siendo el bloque de memoria nuevo.                     */

pindx_t preEjecutar ( const char far * nombre,
                      const char far * comando,
                            pindx_t    pindx ) {

  int i, n ;
  int df ;
  pindx_t pindxAux ;
  word_t tam ;
  word_t segmento ;
  dword_t tamFich ;
  dword_t posFich ;
  char nombreCompleto [ tamComando ] ;
  char nombreFormateado [ 12 ] ;
  pointer_t ptr ;
  int unidad ;
  int unidad0 ;
  int unidad1 ;
  word_t accion ;
  word_t error ;
  char nombreBackup [12] ;                                    /* v. local */
  char comandoBackup [tamComando] ;                           /* v. local */

  copiarStrHasta(nombre, nombreBackup, 12) ;
  copiarStrHasta(comando, comandoBackup, tamComando) ;

#if (FALSE)
//if (pindx != -1) {                                              /* exec */
  printStrBIOS("\n ejecutar: nombre = ") ;
  printStrBIOS((char far *)nombre) ;
  printStrBIOS("\n ejecutar: comando = ") ;
  printStrBIOS((char far *)comando) ;
  printStrBIOS("\n ejecutar: nombreBackup = ") ;
  printStrBIOS((char far *)nombreBackup) ;
  printStrBIOS("\n ejecutar: comandoBackup = ") ;
  printStrBIOS((char far *)comandoBackup) ;
//}
#endif

  if (!formatearNombre(nombre, (char *)&nombreFormateado))
    return(-1) ;

#if (FALSE)
  printStrBIOS("\n ejecutar: nombre formateado = \"") ;
  printStrBIOS((char *)&nombreFormateado) ;
  printStrBIOS("\"") ;
#endif

  i = 0 ;
  while (nombre[i] != (char)0) {
    nombreCompleto[i] = nombre[i] ;
    i++ ;
  }
  nombreCompleto[i] = (char)0 ;

  if (iguales(&nombreFormateado[8], "   ")) {     /* extension por defecto */
    copiarStr(".BIN", &nombreCompleto[i]) ;
    copiarStr("BIN", &nombreFormateado[8]) ;
  }

#if (FALSE)
  printStrBIOS("\n ejecutar: nombreCompleto = ") ;
  printStrBIOS(nombreCompleto) ;
  printStrBIOS("\n ejecutar: nombre formateado = \"") ;
  printStrBIOS(nombreFormateado) ;
  printStrBIOS("\"") ;
#endif

  if (!iguales(&nombreFormateado[8], "BIN"))
    return(-2) ;

  if (unidadLogicaActual >= 0) {
    if ((df = abrirFichero(nombreCompleto, unidadLogicaActual)) == -1)
      return(-5) ;
  }
  else {                                     /* openDOS(nombreCompleto, 0) */
    unidad = getdiskDOS() ;
    if (('A' < nombreCompleto[0]) && (nombreCompleto[0] <= 'Z') && (nombreCompleto[1] == ':')) {
      unidad0 = nombreCompleto[0]-'A' ;
      setdiskDOS(unidad0) ;
      unidad1 = getdiskDOS() ;
      setdiskDOS(unidad) ;
      if (unidad1 != unidad0) {
        return(-4) ;
      }
    }
    accion = 1 ;
    if ((df = extendedOpenDOS((pointer_t)nombreCompleto, 0x2000, 0, &accion, &error)) == -1)
      return(-5) ;
  }

  if (unidadLogicaActual >= 0)
    tamFich = tamFichero(df) ;
  else {
    posFich = (dword_t)0 ;
    lseekDOS(df, (dword_t *)&posFich, SEEK_END_MSDOS) ;
    tamFich = posFich ;
    posFich = (dword_t)0 ;
    lseekDOS(df, (dword_t *)&posFich, SEEK_SET_MSDOS) ;    /* volvemos a 0 */
  }

#if (FALSE)
  printStrBIOS("\n tamFich = ") ;
  printLDecBIOS(tamFich, 1) ;
  printStrBIOS(" tamBloqueMax = ") ;
  printHexBIOS(tamBloqueMax, 4) ;
  printStrBIOS(" 16*tamBloqueMax = ") ;
  printLHexBIOS(16L*(long)tamBloqueMax, 8) ;
#endif

  if (pindx >= 0) {                           /* pindx == indProcesoActual */

    /* Se va a devolver el bloque ocupado por el proceso que ejecuta exec  */
    /* pudiendo ser sobreescrito al cargar el nuevo programa. Por ese      */
    /* motivo ya no va poder utilizarse el espacio de direcionamiento del  */
    /* proceso actual que ejecuta el exec, en particular cualquier         */
    /* variable del proceso.                                               */

    k_devolverBloque(descProceso[pindx].CSProc, descProceso[pindx].tam) ;
  }

  if ((tamFich > (dword_t)(16L*(long)tamBloqueMax)) ||
      ((tam = tamProceso(df)) == 0x0000) ||
      ((segmento = k_buscarBloque(tam)) == 0x0000)) {
#if (FALSE)
  printStrBIOS("\n df = ") ;
  printDecBIOS(df, 1) ;
  printStrBIOS("\n tam = 0x") ;
  printHexBIOS(tam, 4) ;
  printStrBIOS("\n segmento = 0x") ;
  printHexBIOS(segmento, 4) ;
#endif
    if (unidadLogicaActual >= 0)
      cerrarFichero(df) ;
    else
      closeDOS(df) ;
    return(-6) ;
  }

#if (FALSE)
  printStrBIOS("\n tam = ") ;
  printDecBIOS(tam, 1) ;
  printStrBIOS("\n segmento = ") ;
  printHexBIOS(segmento, 4) ;
#endif

  if (unidadLogicaActual >= 0) { /* pueden cargarse ficheros de mas de 64K */
    if (!cargaFicheroAbierto_Ok(df, segmento)) {
      k_devolverBloque(segmento, tam) ;
      cerrarFichero(df) ;
      return(-9) ;
    }
    cerrarFichero(df) ;
  }
  else {
    word_t s = segmento ;
    ptr = pointer(s, 0x0000) ;
    posFich = (dword_t)0 ;
    lseekDOS(df, (dword_t *)&posFich, SEEK_SET_MSDOS) ;          /* whence */
    while ((n = readDOS(df, (char far *)ptr, 32*512)) > 0) {
      s = s + (32*512)/16 ;
      ptr = pointer(s, 0x0000) ;
    }
    closeDOS(df) ;
    if (n < 0) return(-9) ;
  }

#if (FALSE)
printStrBIOS("\n antes de crearProceso\n") ;
if (pindx != -1) {                                                 /* exec */
  printStrBIOS("\n ejecutar: nombre = ") ;
  printStrBIOS((char far *)nombre) ;
  printStrBIOS("\n ejecutar: comando = ") ;
  printStrBIOS((char far *)comando) ;
  puntoDeParadaSO1() ;                                 /* puntoDeParadaSO1 */
}
#endif

  if ((pindxAux = crearProcesoMacro(
                    segmento, tam, tamFich, nombreBackup, comandoBackup, pindx)) < 0) {
    k_devolverBloque(segmento, tam) ;
    return(-10) ;
  }

#if (FALSE)
  printStrBIOS("\n despues de crearProceso") ;
#endif

  if ((pindx >= 0) && (pindxAux != pindx)) return(-11) ;

  return(pindxAux) ;

}

