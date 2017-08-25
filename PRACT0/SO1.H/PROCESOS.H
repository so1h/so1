/* ----------------------------------------------------------------------- */
/*                               procesos.h                                */
/* ----------------------------------------------------------------------- */
/*                           Gestion de procesos                           */
/* ----------------------------------------------------------------------- */

#ifndef PROCESOS_H
#define PROCESOS_H

#include <so1pub.h\tipos.h>                             /* word_t, dword_t */
#include <so1pub.h\const.h>            /* maxProcesos, dfsMax, maxRecursos */
#include <so1pub.h\c2c.h>                                         /* c2c_t */
#include <so1pub.h\ccb.h>                                         /* ccb_t */
#include <so1pub.h\def_trm.h>                                   /* trama_t */
#include <so1pub.h\def_proc.h>           /* pid_t, pindx_t, camposPublicos */ 
//                                        /* descFichero_t, descRecurso_t, */
//                                 /* numColasPFR, e2PFR_t, pid_t, pindx_t */

typedef struct {

  camposPublicos /* campos publicos del descriptor de proceso (DEF_PROC.H) */

  /* añadir a partir de aqui nuevos campos */

} descProcesoExt_t ;                    /* descriptor de proceso extendido */

extern descProcesoExt_t descProceso [ maxProcesos ] ; /* tabla de procesos */

extern descFichero_t descFichero [ dfsMax ] ;         /* tabla de ficheros */
                                                   /* abiertos del sistema */
extern descRecurso_t descRecurso [ maxRecursos ] ;

extern c2c_t c2cPFR [ numColasPFR ] ;                    /* indices cPFR_t */

extern e2PFR_t e2PFR ;

extern ccb_t ccbAlEpilogo ;    /* cola de callBacks al epilogo de las int. */

extern trama_t far * tramaProceso ;

extern trama_t far * tramaTarea ;

/* Pueden ocurrir interrupciones mientras se atiende una llamada al        */
/* sistema, por eso hay que distinguir entre la pila del proceso que hace  */
/* la llamada, y la pila del proceso/sistema interrumpido. Esto ocurre     */
/* por ejemplo cuando durante la llamada al sistema se leen sectores del   */
/* disco con la int 13 del bios. Dicha int 13 permite las interrupciones,  */
/* necesitando la interrupcion de reloj para medir los timeouts. Cuando    */
/* tiene lugar la interrupcion de reloj hay que recordar tanto la pila del */
/* proceso que hizo la llamada al sistema (SS_Proceso) como la del sistema */
/* (SS_ProcInt).                                                           */

extern int indProcesoActual ;           /* indice del proceso en ejecucion */

extern int indProcesoDeSuperficie ;

extern dword_t contRodajas ;                        /* contador de rodajas */

extern int contTicsRodaja ;              /* contador de tics de una rodaja */

extern word_t contadorTimer00 ; /* v. contadorTimer0() al pasar a ej. p.a. */

extern int contOcioso ;                      /* contador para reloj ocioso */

pid_t nuevoPid ( void ) ;

pindx_t indice ( pid_t pid ) ;

pindx_t far sigProceso ( void ) ;              /* planificador de procesos */

int far activarProceso ( pindx_t pindx ) ;

void registrarEnPOrdenados ( pindx_t pindx ) ;

pindx_t crearProceso (       word_t     segmento,
                             word_t     tam,
                             dword_t    tamFich,
                       const char far * programa,
                       const char far * comando,
                             pindx_t    pindx ) ;
							 
void inicProcesos ( void ) ;

void resetPids ( void ) ;

int terminarProcIndx ( pindx_t pindx ) ;          /* termina proceso pindx */

int eliminarProcIndx ( pindx_t pindx ) ;          /* elimina proceso pindx */

int matarProcIndx ( pindx_t pindx ) ;                /* mata proceso pindx */

#endif /* PROCESOS_H */








