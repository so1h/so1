/* ----------------------------------------------------------------------- */
/*                               comundrv.h                                */
/* ----------------------------------------------------------------------- */
/*                  fragmento cumun a todos los drivers                    */
/* ----------------------------------------------------------------------- */

#ifndef COMUNDRV_H
#define COMUNDRV_H

#include <so1pub.h\tipos.h>
#include <so1pub.h\c2c.h>
#include <so1pub.h\ptrc2c.h>
#include <so1pub.h\def_proc.h>

#define dirDescSO1 ((descSO1_t far *)&ptrIndProcesoActual)

/* campos de descSO1 declarados como variables: NO MODIFICAR */

static pindx_t              far * ptrIndProcesoActual ;
static trama_t        far * far * ptrTramaProceso ;
static bool_t               far * ptrActivarAlEpilogo ;
static descProceso_t        far * ptrDescProceso ;    /* tabla de procesos */
static word_t                     tamDescProceso ;
static descFichero_t        far * ptrDescFichero ;    /* tabla de ficheros */
static descRecurso_t        far * ptrDescRecurso ;    /* tabla de recursos */
static c2c_t                far * ptrC2cPFR ;
static int                  far * ptrNivelActivacionSO1 ;
static bool_t               far * ptrEnHalt ;
static bool_t               far * ptrHayTic ;
static ccb_t                far * ptrCcbAlEpilogo ;
static word_t               far * ptrSS_Proceso ;
static word_t               far * ptrSP_Proceso ;
static word_t                     SP0_SO1 ;
static dword_t              far * ptrContRodajas ;                      /* */
static int                  far * ptrContTicsRodaja ;                   /* */
static rti_t                far * ptrVIOrg ;
static sigProceso_t               sigProceso ;
static activarProceso_t           activarProceso ;
static buscarNuevoProcesoActual_t buscarNuevoProcesoActual ;
static bloquearProcesoActual_t    bloquearProcesoActual ;    /* no retorna */

/* fin campos de descSO1 declarados como variables */

static void segDatos ( void ) {       /* vamos a guardar aqui el valor del */
//asm db 'DS'                                         /* segmento de datos */
  asm db 0x44, 0x42 ;                                 /* segmento de datos */
}                                         /* dentro del segmento de codigo */

#endif /* COMUNDRV_H */
