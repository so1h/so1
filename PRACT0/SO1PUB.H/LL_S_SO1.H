/* ----------------------------------------------------------------------- */
/*                               ll_s_so1.h                                */
/* ----------------------------------------------------------------------- */
/*   biblioteca de funciones de interfaz de las llamadas al sistema SO1    */
/* ----------------------------------------------------------------------- */

#ifndef LL_S_SO1_H
#define LL_S_SO1_H

#include <so1pub.h\tipos.h>
#include <so1pub.h\def_proc.h>
#include <so1pub.h\def_mem.h>
#include <so1pub.h\def_sf.h>
#include <so1pub.h\def_rat.h>
#include <so1pub.h\fcntl.h>
#include <so1pub.h\ll_s_exc.h>                            /* exec, killpid */
#include <so1pub.h\ll_s_sig.h>         /* kill, sigaction, sigalarm, pause */ 


/* ----------------------------------------------------------------------- */
/* -                                                                     - */
/* -     Llamadas al sistema implementadas mediante traps (int 0x60)     - */
/* -     ===========================================================     - */
/* ----------------------------------------------------------------------- */

/* Numero de vector de interrupcion utilizado por el sistema SO1.          */

#define nVIntSO1     0x60

pid_t createProcess ( const char far * nombre, /* declarada en ll_s_exec.h */
                      const char far * comando ) ;

/* ----------------------------------------------------------------------- */
/* Esta llamada al sistema toma como primer parametro una cadena de        */
/* caracteres nombre. Si esa cadena de caracteres corresponde a un fichero */
/* ejecutable (SO1) existente en el directorio raiz de la unidad de la que */
/* se ha arrancado, y si existe memoria libre suficiente, se crea un nuevo */
/* proceso que ejecuta el fichero una vez cargado en la nueva particion de */
/* memoria creada. El proceso creado hereda todos los ficheros que tenia   */
/* abiertos el proceso padre. El segundo parametro str es una cadena de    */
/* caracteres de la cual pueden extraerse los argumentos con los que se    */
/* ejecuta el programa.                                                    */
/* ----------------------------------------------------------------------- */


pid_t fork ( void ) ;

/* ----------------------------------------------------------------------- */
/* Esta llamada al sistema toma crea un nuevo proceso identico al proceso  */
/* que ejecuta la llamada al sistema fork (es decir ejecuta el mismo       */
/* codigo, con los mismos datos y pila, copiados y reubicados en una nueva */
/* particion de memoria que se supone disponible). La unica diferencia que */
/* se manifiesta al proceso que ejecuta la llamada (padre) y al proceso    */
/* que ha sido creado, es que fork devuelve como resultado en ese caso, un */
/* cero (0) al proceso hijo, y el pid del proceso hijo, al proceso padre.  */
/* El proceso hijo hereda todos los ficheros que tenia abiertos el proceso */
/* padre.                                                                  */
/* Si por algun motivo no se puede crear el proceso hijo, la llamada fork  */
/* devuelve un valor negativo indicando el tipo de error ocurrido.         */
/* ----------------------------------------------------------------------- */


int exec ( const char far * nombre,          /* declarada en ll_s_exec.h */
           const char far * comando ) ;

/* ----------------------------------------------------------------------- */
/* Esta llamada al sistema toma como primer parametro una cadena de        */
/* caracteres nombre. Si esa cadena de caracteres corresponde a un fichero */
/* ejecutable (SO1) existente en el directorio raiz de la unidad de la que */
/* se ha arrancado, y si existe memoria suficiente para cargar el fichero  */
/* con su codigo, datos y pila, el proceso que ejecuta esta llamada (exec) */
/* deja de ejecutar su anterior codigo, datos y pila para pasar a ejecutar */
/* el nuevo codigo, datos y pila desde el principio. El segundo parametro  */
/* str es una cadena de caracteres de la cual pueden extraerse los         */
/* argumentos con los que se el proceso pasa a ejecutar el nuevo codigo,   */
/* datros y pila.                                                          */
/* ----------------------------------------------------------------------- */


pid_t waitpid ( pid_t pid, int far * statloc );/* declarada en ll_s_exec.h */

/* ----------------------------------------------------------------------- */
/* El proceso que ejecuta esta llamada queda bloqueado hasta que el        */
/* proceso hijo con el pid indicado termine. Si pid = -1 el bloqueo es     */
/* tan solo hasta que termine cualquiera de sus hijos. La llamada retorna  */
/* el pid del proceso hijo terminado.                                      */
/* ----------------------------------------------------------------------- */


void exit ( int status ) ;                     /* declarada en ll_s_exec.h */

/* ----------------------------------------------------------------------- */
/* Esta llamada al sistema termina el proceso (en ejecucion) que la        */
/* ejecuta, tratando de devolver al proceso padre el codigo de terminacion */
/* status. Si el proceso padre esta esperando a recibir ese codigo de      */
/* terminacion habiéndose bloqueado con waitpid el proceso termina         */
/* inmediatamente, la ventana correspondiente al proceso se cierra y la    */
/* particion de memoria que utilizaba se libera, devolviendose dicha       */
/* memoria al sistema. Sie proceso padre no esta bloqueado en waitpid, el  */
/* proceso que hace el exit se queda bloqueado (zombie) a la espera de que */
/* el padre haga el waitpid correspondiente.                               */
/* ----------------------------------------------------------------------- */


pid_t getpid ( void ) ;

/* ----------------------------------------------------------------------- */
/* Esta llamada al sistema devuelve como resultado el pid del proceso que  */
/* la ejecuta.                                                             */
/* ----------------------------------------------------------------------- */


pindx_t getpindx ( void ) ;

/* ----------------------------------------------------------------------- */
/* Esta llamada al sistema devuelve como resultado el pindx del proceso    */
/* que la ejecuta.                                                         */
/* ----------------------------------------------------------------------- */


pid_t getppid ( void ) ;

/* ----------------------------------------------------------------------- */
/* Esta llamada al sistema devuelve como resultado el pid del proceso      */
/* padre del proceso que la ejecuta.                                       */
/* ----------------------------------------------------------------------- */


uid_t getuid ( void ) ;

/* ----------------------------------------------------------------------- */
/* Esta llamada al sistema devuelve como resultado el uid del proceso      */
/* que la ejecuta.                                                         */
/* ----------------------------------------------------------------------- */


int setuid ( uid_t uid ) ;                     /* declarada en ll_s_exec.h */

/* ----------------------------------------------------------------------- */
/* Esta llamada al sistema establece el uid del proceso que la ejecuta.    */
/* ----------------------------------------------------------------------- */


gid_t getgid ( void ) ;

/* ----------------------------------------------------------------------- */
/* Esta llamada al sistema devuelve como resultado el gid del proceso      */
/* que la ejecuta.                                                         */
/* ----------------------------------------------------------------------- */


int setgid ( gid_t gid ) ;                     /* declarada en ll_s_exec.h */

/* ----------------------------------------------------------------------- */
/* Esta llamada al sistema establece el gid del proceso que la ejecuta.    */
/* ----------------------------------------------------------------------- */


int killpid ( int pid ) ;            /* mata directamente sin enviar señal */                    

/* ----------------------------------------------------------------------- */
/* Si pid > 0 y corresponde al identificador de un proceso del sistema,    */
/* esta llamada destruye al proceso cuyo identificador de proceso es pid y */
/* retorna como resultado un 0.                                            */
/* Si pid = -1 esta llamada destruye todos los procesos menos el proceso   */
/* con pid 0 que corresponde al sistema SO1 (nucleo).                      */
/* Si pid = 0 la llamada simplemente retorna el valor -1, ya que no se     */
/* permite matar al proceso 0.                                             */
/* En cualquier otro caso la llamada al sistema retorna el valor -2.       */
/* ----------------------------------------------------------------------- */


#define SEEK_SET   0        /* lseek se posiciona desde el principio       */
#define SEEK_CUR   1        /* lseek se posiciona desde la posision actual */
#define SEEK_END   2        /* lseek se posiciona desde el final           */

int open ( const char far * nombre, modoAp_t modoAp ) ;
                                               /* declarada en ll_s_exec.h */
int close ( int df ) ;                         /* declarada en ll_s_exec.h */

int read ( int df, pointer_t dir, word_t nbytes ) ;

int aio_read ( int df, pointer_t dir, word_t nbytes ) ;

int write ( int df, pointer_t dir, word_t nbytes ) ;
                                               /* declarada en ll_s_exec.h */
int aio_write ( int df, pointer_t dir, word_t nbytes ) ;

long int lseek ( int df, long int pos, word_t whence  ) ;
                                               /* declarada en ll_s_exec.h */
int fcntl ( int df, word_t cmd, word_t arg ) ;
                                               
int ioctl ( int df, word_t request, word_t arg ) ;
                                               /* declarada en ll_s_exec.h */
/* drivers */

void obtenInfoSO1 ( descSO1_t far * descSO1 ) ;

rindx_t crearRecurso ( descRecurso_t far * dR ) ;

dfs_t crearFichero ( const char far * nombre,
                     rindx_t          rindx,
                     word_t           menor,
                     tipoFichero_t    tipo ) ;

int esperarDesinstalacion ( word_t tamDATA, ... ) ;

int destruirRecurso ( char far * nombre ) ;

int encolarCcbRecurso ( callBack_t cb, const char far * nombre ) ;

int eliminarCcbRecurso ( callBack_t cb, const char far * nombre ) ;


typedef struct {
  descProceso_t far * descProceso ;
  descFichero_t far * descFichero ;
  descRecurso_t far * descRecurso ;
  e2PFR_t       far * e2PFR ;
  c2c_t         far * c2cPFR ;
} bloquePFR_t ;

void obtenInfoPS ( descProceso_t far * descProceso,         /* ax = 0601h */
                   e2PFR_t       far * e2PFR,
                   c2c_t         far * c2cPFR ) ;

/* ----------------------------------------------------------------------- */
/* Esta llamada al sistema copia la tabla de procesos en la direccion      */
/* indicada por descProceso. Tambien copia los enlaces y colas necesarios  */
/* para conocer el orden los procesos preparados.                          */
/* ----------------------------------------------------------------------- */


void obtenInfoMEM ( descProceso_t far * descProceso,         /* ax = 0602h */
                    e2PFR_t       far * e2PFR,
                    c2c_t         far * c2cPFR,
                    ptrBloque_t       * listaLibres,
                    word_t            * tamBlqMax ) ;

/* ----------------------------------------------------------------------- */
/* Esta llamada al sistema copia la tabla de procesos en la direccion      */
/* indicada por descProceso. Tambien copia los enlaces y colas necesarios  */
/* para conocer el orden los procesos preparados. Finalmente copia la      */
/* lista de bloques libres a partir de la direcci¢n listaLibres.           */
/* ----------------------------------------------------------------------- */


void obtenInfoPFR ( descProceso_t far * descProceso,         /* ah = 0603h */
                    descFichero_t far * descFichero,
                    descRecurso_t far * descRecurso,
                    e2PFR_t       far * e2PFR,
                    c2c_t         far * c2cPFR ) ;

/* ----------------------------------------------------------------------- */
/* Esta llamada al sistema copia la tabla de procesos en la direccion      */
/* indicada por descProceso, la tabla de ficheros abiertos del sistema en  */
/* la direccion descFichero y la tabla de recursos del sistema en la       */
/* direccion descRecurso. Tambien copia los enlaces y colas necesarios     */
/* para conocer el orden los procesos preparados.                          */
/* ----------------------------------------------------------------------- */


void obtenInfoFAB ( descriptor_de_fichero_t far * df ) ;

/* ----------------------------------------------------------------------- */
/* Esta llamada al sistema copia la tabla de ficheros abiertos en la       */
/* direccion indicada por df.                                              */
/* ----------------------------------------------------------------------- */


void obtenInfoINFO ( info_t far * info ) ;

/* ----------------------------------------------------------------------- */
/* Esta llamada al sistema copia informacion diversa del sistema a partir  */
/* de la direccion indicada por df.                                        */
/* ----------------------------------------------------------------------- */

/* getdisk devuelve el numero de la unidad logica actual de SO1            */

int getdisk ( void ) ;

int findFirst ( byte_t unidadLogica, ffblk_t * ffblk ) ;

int findNext ( ffblk_t * ffblk ) ;


char leer ( int df ) ;

char leerListo ( int df ) ;

char leerAsciiListo ( int df ) ;


void activarTraza ( word_t numInstr, pid_t pid ) ;

/* ----------------------------------------------------------------------- */
/* Si numInstr = 0 esta llamada al sistema desactiva el flag de traza del  */
/* proceso con identificador de proceso pid, si pid > 0, o de todos los    */
/* procesos, si pid = -1. En el caso de que numInstr > 0 esta llamada      */
/* activa el flag de traza del proceso pid, si pid > 0, o de todos los     */
/* procesos, si pid = -1. En el caso de que numInstr > 0, ademas se        */
/* establece  el numero maximo de instrucciones ejecutadas sin ser         */
/* visualizadas por el puerto serie 0, al valor numInstr.                  */
/* ----------------------------------------------------------------------- */


void analizarProcesos ( bool_t Ok ) ;

/* ----------------------------------------------------------------------- */
/* Habilita el envio de informacion por el puerto COM1 para representar el */
/* seguimiento del comportamiento de los procesos del sistema. Dicha       */
/* informaci¢n se envia periodicamente cada tic de reloj.                  */
/* ----------------------------------------------------------------------- */


/* ----------------------------------------------------------------------- */
/* -                                                                     - */
/* -         Llamadas al sistema implementadas mediante ficheros         - */
/* -         ===================================================         - */
/* ----------------------------------------------------------------------- */

word_t ll_buscarBloque ( word_t tam ) ;

bool_t ll_devolverBloque ( word_t segmento, word_t tam ) ;

word_t ll_tamBloqueMax ( void ) ;

dword_t retardoActivo ( dword_t nVueltas ) ;

/* ----------------------------------------------------------------------- */
/* Si nVueltas = 0 esta llamada provoca que el proceso ejecute un bucle    */
/* que provoca la demora establecida en SO1 como estandar.                 */
/* Si nVueltas = 1 esta llamada retorna el numero de vueltas que lleva a   */
/* cabo el bucle anteriormente mencionado.                                 */
/* Si nVueltas > 1 esta llamada establece en SO1 el numero de vueltas que  */
/* se utiliza en el bucle de retardo mencionado                            */
/* ----------------------------------------------------------------------- */

#endif /* LL_S_SO1_H */







