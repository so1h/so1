/* ----------------------------------------------------------------------- */
/*                               comundrv.h                                */
/* ----------------------------------------------------------------------- */
/*                  fragmento comun a todos los drivers                    */
/* ----------------------------------------------------------------------- */

#ifndef COMUNDRV_H
#define COMUNDRV_H

#include <so1pub.h\ajustusr.h>                                 /* valor_DS */
#include <so1pub.h\tipos.h>                                        /* NULL */
#include <so1pub.h\c2c.h>                                         /* c2c_t */
#include <so1pub.h\ptrc2c.h>
#include <so1pub.h\def_proc.h>                        /* descProceso_t ... */

#define dirDescSO1 ((descSO1_t far *)&ptrIndProcesoActual)

/* campos de descSO1 declarados como variables: NO MODIFICAR               */

/* se inicializan todas las variables para que vayan al segmento de datos  */
/* (DATA) de manera que esten mas proximas al segmento de codigo (CODE)    */

static pindx_t              far * ptrIndProcesoActual      = NULL ;
static descProceso_t        far * ptrDPActual              = NULL ;
static trama_t        far * far * ptrTramaProceso          = NULL ;
static bool_t               far * ptrActivarAlEpilogo      = NULL ;
static descProceso_t        far * ptrDescProceso           = NULL ; /* tabla de procesos */
static word_t                     tamDescProceso           = 0x0000 ;
static descFichero_t        far * ptrDescFichero           = NULL ; /* tabla de ficheros */
static descRecurso_t        far * ptrDescRecurso           = NULL ; /* tabla de recursos */
static c2c_t                far * ptrC2cPFR                = NULL ;
static int                  far * ptrNivelActivacionSO1    = NULL ;
static bool_t               far * ptrEnHalt                = NULL ;
static bool_t               far * ptrHayTic                = NULL ;
static ccb_t                far * ptrCcbAlEpilogo          = NULL ;
static word_t               far * ptrSS_Proceso            = NULL ;
static word_t               far * ptrSP_Proceso            = NULL ;
static word_t                     SP0_SO1                  = 0x0000 ;
static dword_t              far * ptrContRodajas           = NULL ; /* */
static int                  far * ptrContTicsRodaja        = NULL ; /* */
static rti_t                far * ptrVIOrg                 = NULL ;

static sigProceso_t               sigProceso               = NULL ;
static activarProceso_t           activarProceso           = NULL ;
static buscarNuevoProcesoActual_t buscarNuevoProcesoActual = NULL ;
static bloquearProcesoActual_t    bloquearProcesoActual    = NULL ; /* no retorna */

/* fin campos de descSO1 declarados como variables inicializadas */

int comprobarAmpersand ( void ) ; /* mira si hay un & al final del comando */

#endif /* COMUNDRV_H */