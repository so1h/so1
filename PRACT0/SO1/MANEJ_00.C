/* ----------------------------------------------------------------------- */
/*                               manej_00.c                                */
/* ----------------------------------------------------------------------- */
/*          manejador de las llamadas al sistema del grupo AH = 00         */
/* ----------------------------------------------------------------------- */

/* ----------------------------------------------------------------------- */
/* AX = 0x0000 createProcess                                               */
/* AX = 0x0001 fork                                                        */
/* AX = 0x0002 exec                                                        */
/* AX = 0x0003 waitpid                                                     */
/* AX = 0x0004 exit                                                        */
/* AX = 0x0005 getpid                                                      */
/* AX = 0x0006 getpindx                                                    */
/* AX = 0x0007 getppid()                                                   */
/* AX = 0x0008 getuid()                                                    */
/* AX = 0x0009 setuid(uid)                                                 */
/* AX = 0x000a getgid()                                                    */
/* AX = 0x000b setuid(gid)                                                 */
/* AX = 0x000c killpid                                                     */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\tipos.h>                              /* pid_t, pindx_t */
#include <so1pub.h\strings.h>
#include <so1pub.h\ptrc2c.h>
#include <so1pub.h\def_proc.h>                                 /* rec_hijo */
#include <so1.h\ajustes.h>
#include <so1.h\ejecutar.h>                        /* kk_fork, preEjecutar */
#include <so1.h\procesos.h>
#include <so1.h\blockpr.h>
#include <so1.h\bios.h>
#include <so1.h\dokill.h>

void bloquearEsperandoHijo ( pindx_t hpindx ) {
  descProceso[indProcesoActual].hpindx = hpindx ;
  bloquearProcesoActual(rec_hijo) ;
}

void so1_manejador_00 ( void ) {                      /* ah = 00 ; int SO1 */

  static int i ;
  static pid_t pid ;
  static pid_t hpid ;
  static pindx_t pindx ;
  static pindx_t hpindx ;
  static bool_t noStatus ;

  switch (tramaProceso->AL) {

    case 0x00 :                              /* createProcess */ /* 0x0000 */
#if (FALSE)
  printStrBIOS("\n manej_00 createProcess ") ;
//printStrBIOS(" pid = ") ;
//printDecBIOS(descProceso[indProcesoActual].pid, 1) ;
#endif
    case 0x01 :                                       /* fork */ /* 0x0001 */

      if (c2cPFR[DPOcupados].numElem >= (maxProcesos-1)) {
        tramaProceso->DX = -10 ;                        /* codigo de error */
        break ;
      }

      descProceso[indProcesoActual].trama = tramaProceso ;

      if (tramaProceso->AL == 0x01)                                /* fork */
        pindx = kk_fork() ;
      else {                                              /* createProcess */
        pindx = preEjecutar(
          MK_FP(tramaProceso->CX, tramaProceso->BX),      /* CX:BX nombre  */
          MK_FP(tramaProceso->ES, tramaProceso->DX),      /* ES:DX comando */
          -1                                          /* createProcess: -1 */
        ) ;
      }

      if (pindx >= 0) {

             /* avisar por consola cuando quede un unico descProceso libre */

        if (c2cPFR[DPOcupados].numElem == (maxProcesos-2)) {

          printStrBIOS("\n solo queda un descriptor de proceso libre ") ;

//        printStrBIOS("\n c2cPFR[DPOcupados].numElem = ") ;
//        printDecBIOS(c2cPFR[DPOcupados].numElem, 1) ;
//        printStrBIOS(" maxProcesos = ") ;
//        printDecBIOS(maxProcesos, 1) ;
//        printStrBIOS(" pindx = ") ;
//        printDecBIOS(pindx, 1) ;

        }

        /* imponemos que se ejecute a continuacion en la CPU primero el    */
        /* proceso hijo pindx y luego el proceso padre indProcesoActual.   */
        /* Para ello apilamos en la cola intermedia PUrgentes, la cual el  */
        /* planificador incorpora a la cola de preparados desapilando de   */
        /* PUrgentes y apilando en PPreparados.                            */
        /* esto es crucial en las inicializaciones (INICCODE.C).           */

        descProceso[pindx].estado = preparado ;
//      encolarPC2c(pindx, (ptrC2c_t)&c2cPFR[PUrgentes]) ;
        apilarPC2c(pindx, (ptrC2c_t)&c2cPFR[PUrgentes]) ;

        descProceso[indProcesoActual].estado = preparado ;
//      encolarPC2c(indProcesoActual, (ptrC2c_t)&c2cPFR[PUrgentes]) ;
        apilarPC2c(indProcesoActual, (ptrC2c_t)&c2cPFR[PUrgentes]) ;

        /* se ejecutara pindx antes que indProcesoActual */
        if (tramaProceso->AL == 0x01)                             /* fork */
          descProceso[pindx].trama->DX = 0x0000 ;              /* hijo DX */
        tramaProceso->DX = descProceso[pindx].pid ;           /* padre DX */
        buscarNuevoProcesoActual() ;   /* ¡no se retorna de esta funcion! */
      }                                /* =============================== */
      else
        tramaProceso->DX = pindx ;                     /* codigo de error */
      break ;

    case 0x02 :                                       /* exec */ /* 0x0002 */
#if (FALSE)
  printStrBIOS("\n manej_00 exec ") ;
#endif
      pindx = preEjecutar(
        MK_FP(tramaProceso->CX, tramaProceso->BX),        /* CX:BX nombre  */
        MK_FP(tramaProceso->ES, tramaProceso->DX),        /* ES:DX comando */
        indProcesoActual                         /* exec: indProcesoActual */
      ) ;

#if (FALSE)
  printStrBIOS("\n pindx = ") ;
  printIntBIOS(pindx, 1) ;
  printStrBIOS(" pid = ") ;
  printIntBIOS(descProceso[pindx].pid, 1) ;
  printStrBIOS(" estado = ") ;
  printIntBIOS(descProceso[pindx].estado, 1) ;
  printStrBIOS("\n CSProc = 0x") ;
  printHexBIOS(descProceso[pindx].CSProc, 4) ;
  printStrBIOS("\n programa = ") ;
  printStrBIOS("\"") ;
  printStrHastaBIOS(descProceso[pindx].programa, 12, FALSE) ;
  printStrBIOS("\"") ;
  printStrBIOS("\n comando = \"") ;
  printStrBIOS((char *)&descProceso[pindx].comando) ;
  printStrBIOS("\"") ;
#endif

      if (pindx < -5) {                                     /* error fatal */
        descProceso[indProcesoActual].estado = bloqueado ;
        descProceso[indProcesoActual].esperandoPor = rec_zombie ;
        eliminarProcIndx(indProcesoActual) ;
        eliminarPC2c(descProceso[indProcesoActual].ppindx,
                     (ptrC2c_t)&descProceso[indProcesoActual].c2cHijos) ;
        buscarNuevoProcesoActual() ;    /* ¡no se retorna de esta funcion! */
      }                                 /* =============================== */
      else if (pindx < 0)                                    /* error leve */
        break ;                              /* el procesoActual se retoma */
      else         /* el proceso recubierto pasa a (comienza su) ejecucion */
        activarProceso(pindx) ;           /* no se retorna de esta funcion */

                                                                 /* 0x0003 */
    case 0x03 :                                                 /* waitpid */
      hpid = tramaProceso->BX ;                            /* pid del hijo */
      hpindx = indice(hpid) ;
      if ((hpid >= 0) && (hpindx == -1))                 /* hpid no existe */
        tramaProceso->BX = -1 ;                              /* pidDifunto */
      else {
        if (hpid == -1) {                          /* buscar hijos zombies */
          i = 0 ;
          while ((i < maxProcesos) &&
                 ((descProceso[i].ppindx != indProcesoActual) ||   /* hijo */
                  (descProceso[i].estado != bloqueado) ||
                  (descProceso[i].esperandoPor != rec_zombie)))  /* zombie */
            i++ ;
          if (i < maxProcesos) hpindx = i ;          /* primer hijo zombie */
          else hpindx = -1 ;
        }
        if ((hpindx != -1) &&             /* terminar de matar hijo zombie */
            (descProceso[hpindx].ppindx == indProcesoActual) &&
            (descProceso[hpindx].estado == bloqueado) &&
            (descProceso[hpindx].esperandoPor == rec_zombie)) {
          tramaProceso->AX = descProceso[hpindx].status ;        /* status */
          tramaProceso->BX = descProceso[hpindx].pid ;       /* pidDifunto */
          eliminarProcIndx(hpindx) ;

          eliminarPC2c(hpindx, (ptrC2c_t)&descProceso[indProcesoActual].c2cHijos) ;

        }
        else {                                   /* no hay hijo(s) zombies */
          noStatus = tramaProceso->DX ;
          if (hpid != -1) {
            if (noStatus) {
              descProceso[hpindx].noStatus = TRUE ;
              tramaProceso->BX = 0 ;                         /* pidDifunto */
            }
            else bloquearEsperandoHijo(hpindx) ;
          }
          else if (noStatus) {
            i = 0 ;
            while (i < maxProcesos) {
              if (descProceso[i].ppindx == indProcesoActual)       /* hijo */
                descProceso[i].noStatus = TRUE ;
              i++ ;
            }
            tramaProceso->BX = 0 ;                           /* pidDifunto */
          }
          else {
            i = 0 ;
            while ((i < maxProcesos) &&
                   (descProceso[i].ppindx != indProcesoActual))    /* hijo */
              i++ ;
            if (i < maxProcesos) {                       /* hay algun hijo */
              descProceso[indProcesoActual].hpindx = -1 ;
/*
              if (indProcesoActual == indProcesoDeSuperficie)
                focalizarWin(descProceso[i].win) ;
*/
              bloquearProcesoActual(rec_hijo) ;
            }
            else                                           /* no hay hijos */
              tramaProceso->BX = -1 ;                        /* pidDifunto */
          }
        }
      }
      break ;
                                                                 /* 0x0004 */
    case 0x04 :                                                    /* exit */
      doKill(indProcesoActual) ;
      break ;
                                                                 /* 0x0005 */
    case 0x05 :                                                  /* getpid */
      tramaProceso->BX = descProceso[indProcesoActual].pid ;
      break ;
                                                                 /* 0x0006 */
    case 0x06 :                                                /* getpindx */
      tramaProceso->BX = indProcesoActual ;
      break ;
                                                                 /* 0x0007 */
    case 0x07 :                                                 /* getppid */
      tramaProceso->BX =
        descProceso[descProceso[indProcesoActual].ppindx].pid ;
      break ;
                                                                 /* 0x0008 */
    case 0x08 :                                                  /* getuid */
      tramaProceso->BX =
        descProceso[indProcesoActual].uid ;
      break ;
                                                                 /* 0x0009 */                                                                 /* 0x0008 */
    case 0x09 :                                                  /* setuid */
      descProceso[indProcesoActual].uid = tramaProceso->BX ;
      break ;
                                                                 /* 0x000a */
    case 0x0a :                                                  /* getgid */
      tramaProceso->BX =
        descProceso[indProcesoActual].gid ;
      break ;
                                                                 /* 0x000b */
    case 0x0b :                                                  /* setgid */
      descProceso[indProcesoActual].gid = tramaProceso->BX ;
      break ;
                                                                 /* 0x000c */
    case 0x0c :                                                 /* killpid */
      pid = (pid_t)tramaProceso->DX ;
      if (pid >= 0) {                              /* matar el proceso pid */
        i = indice(pid) ;
        tramaProceso->BX = (word_t)((i != -1) ? matarProcIndx(i) : -2) ;
      }
      else {                                   /* matar todos los procesos */
        for ( i = 1 ; i < maxProcesos ; i++ )
          matarProcIndx(i) ;
        tramaProceso->BX = 0 ;
        if (indProcesoActual > 0)
          buscarNuevoProcesoActual() ;
      }
      break ;

    default : ;

  }

}

