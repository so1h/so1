/* ----------------------------------------------------------------------- */
/*                                 k_msj.c                                 */
/* ----------------------------------------------------------------------- */
/*    envio de mensajes desde el nucleo (suplantando al proceso actual)    */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\ll_s_so1.h>     /* SEND, RECEIVE, SENDREC, NOTIFY, ECHO */
#include <so1.h\procesos.h>                                /* tramaProceso */
#include <so1.h\blockpr.h>                        /* bloquearProcesoActual */
#include <so1.h\k_msj.h> 

/* las funciones k_send, k_receive, k_sendrec y k_notify permiten al       */
/* nucleo enviar/recibir/sendrec/notify un mensaje como si lo enviara el   */
/* proceso actualmente en ejecucion. Se podria pensar en hacer lo mismo    */
/* para un proceso cualquiera pindx haciendo previamente:                  */
/*                                                                         */
/*   indProcesoActual = pindx ;                                            */
/*   trapaPrcceso = descProceso[pindx].trama ; ...                         */
/*                                                                         */
/* No obstante hay que tener mucho cuidado ya que si el proceso actual     */
/* ha llamado al nucleo mediante una llamada al sistema, k_send provoca    */
/* que se ejecute para el proceso la llamada al sistema send. Si hay un    */
/* bloqueo a consecuencia de esa llamada sera el proceso actual el que se  */
/* bloquee, y cuando se desbloquee retornara a la instruccion siguiente al */
/* trap. No obstante si k_send se ejecuta sin bloqueo, el retorno es a la  */
/* instruccion siguiente al k_send. Lo mismo en el caso de un k_receive y  */
/* en el caso de un k_sendrec. En el caso de k_notify no hay complicacion  */
/* ya que es no bloqueante. Podemos utilizar k_notify desde una rutina de  */
/* tratamiento de interrupcion para notificar esas interrupciones a los    */
/* procesos.                                                               */
/*                                                                         */
/* En el caso de un bloqueo hay que tener en cuenta que los registros del  */
/* proceso se han modificado, para cuando continue con su ejecucion.       */
/*                                                                         */
/* En cuanto al mensaje, en send el puntero es near (local al proceso)     */
/* pero en k_send se trata de un puntero far que puede apuntar a cualquier */
/* sitio, en particular a una zona de memoria dentro del driver que        */
/* ejecuta el k_send.                                                      */   

extern void so1_manejador_05 ( void ) ;

#define salvarRegsBXAX() \
	word_t reg_BX ;                                                          \
	word_t reg_AX ;                                                          \
	reg_BX = tramaProceso->BX ;                                              \
	reg_AX = tramaProceso->AX ;                                              \

#define salvarRegs() \
	word_t reg_ES ;                                                          \
	word_t reg_DX ;                                                          \
	word_t reg_BX ;                                                          \
	word_t reg_AX ;                                                          \
    reg_ES = tramaProceso->ES ;                                              \
	reg_DX = tramaProceso->DX ;                                              \
	reg_BX = tramaProceso->BX ;                                              \
	reg_AX = tramaProceso->AX ;                                              \
	
#define restaurarRegsESDX() \
	tramaProceso->ES = reg_ES ;                                              \
	tramaProceso->DX = reg_DX ;                                              \
	
#define restaurarRegsBXAX() \
	tramaProceso->BX = reg_BX ;                                              \
	tramaProceso->AX = reg_AX ;                                              \

#define restaurarRegs() \
    restaurarRegsESDX() ;                                                    \
    restaurarRegsBXAX() ;                                                    \

void k_send ( pindx_t pindx, mensaje_t * msj ) 
{	
    salvarRegs() ;
	tramaProceso->ES = DS_SO1 ;
	tramaProceso->DX = FP_OFF(msj) ;
	tramaProceso->BX = pindx ; 
	tramaProceso->AX = SEND ; 
    so1_manejador_05() ;
	restaurarRegs() ;
}

void k_receive ( pindx_t pindx, mensaje_t * msj )    /* pindx puede ser ANY */
{
	salvarRegs() ;
	tramaProceso->ES = DS_SO1 ;
	tramaProceso->DX = FP_OFF(msj) ;
	tramaProceso->BX = pindx ; 
	tramaProceso->AX = RECEIVE ; 
    so1_manejador_05() ;
	restaurarRegs() ;
}

void k_sendrec ( pindx_t pindx, mensaje_t * msj ) 
{
	salvarRegs() ;
	tramaProceso->ES = DS_SO1 ;
	tramaProceso->DX = FP_OFF(msj) ;
	tramaProceso->BX = pindx ; 
	tramaProceso->AX = SENDREC ; 
    so1_manejador_05() ;
	restaurarRegs() ;
}

void k_notify ( pindx_t pindx ) 
{
    salvarRegsBXAX() ;
	tramaProceso->BX = pindx ; 
	tramaProceso->AX = NOTIFY ; 
    so1_manejador_05() ;
    restaurarRegsBXAX() ;
}