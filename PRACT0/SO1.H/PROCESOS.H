/* ----------------------------------------------------------------------- */
/*                               procesos.h                                */
/* ----------------------------------------------------------------------- */
/*                           Gestion de procesos                           */
/* ----------------------------------------------------------------------- */

#ifndef PROCESOS_H
#define PROCESOS_H

#include <so1pub.h\def_proc.h>

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

extern c2c_t bloqueadosRaton ;

extern dobleEnlace_t e2BloqueadosRaton [ maxProcesos + 1 ] ;

pid_t nuevoPid ( void ) ;

pindx_t indice ( pid_t pid ) ;

pindx_t far sigProceso ( void ) ;              /* planificador de procesos */

void encolar ( cola_t * cola, int pindx ) ;

void encolarAlt ( cola_t * cola, int pindx ) ;

void encolarFar ( cola_t far * cola, int pindx ) ;

void eliminar ( cola_t * cola, int pindx ) ;

void eliminarAlt ( cola_t * cola, int pindx ) ;

void eliminarFar ( cola_t far * cola, int pindx ) ;

int far activarProceso ( pindx_t pindx ) ;

void registrarEnPOrdenados ( pindx_t pindx ) ;

pindx_t crearProceso (       word_t     segmento,
                             word_t     tam,
                             dword_t    tamFich,
                       const char far * programa,
                       const char far * comando,
                             pindx_t    pindx ) ;

#define crearProcesoMacro(segmento, tam, tamFich, programa, comando, pindx)  \
  crearProceso(segmento, tam, tamFich, programa, comando, pindx)

void inicProcesos ( void ) ;

void resetPids ( void ) ;

int terminarProcIndx ( pindx_t pindx ) ;          /* termina proceso pindx */

int eliminarProcIndx ( pindx_t pindx ) ;          /* elimina proceso pindx */

int matarProcIndx ( pindx_t pindx ) ;                /* mata proceso pindx */

#define semMax 10

typedef struct {
  unsigned int valor ;                          /* valor del semaforo */
  c2c_t cola ;          /* cola de procesos bloqueados en el semáforo */
} sem_t ;

extern sem_t semaforo[semMax] ;                           /* tabla de semaforos */

void inicSemaforos ( void ) ;

#define Capacidad 2  /* Capacidad de los buzones, puede ser 0, 1, 2, ... */
#define mboxMax 10                               /* 10 buzones (mailbox) */

typedef struct {
  c2c_t Remitentes ;
  c2c_t Destinatarios ;
#if (Capacidad != 0)                          /* compilación condicional */
  unsigned char bufer[Capacidad][16] ;
  unsigned int in ;          /* bufer[in]  es la primera entrada libre   */
  unsigned int out ;         /* bufer[out] es la primera entrada ocupada */
  unsigned int numMensajes ;
#endif                       /*            si numMensajes > 0            */
} mbox_t ;

extern mbox_t buzon [mboxMax] ;                                      /* buzones */

void inicBuzones ( void ) ;

#endif /* PROCESOS_H */







