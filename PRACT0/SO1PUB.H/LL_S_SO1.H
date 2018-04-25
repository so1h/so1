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

#define nVIntSO1      0x60

/* Codigos de operacion (en el registro AX) de las llamadas al sistema     */        

/* ======================================================================= */
/* gestion de procesos  (AX = 0x00xx)                                      */
/* ======================================================================= */

#define CREATEPROCESS         0x0000   /* createProcess(nombre, comando)   */
#define FORK                  0x0001   /* fork()                           */
#define EXEC                  0x0002   /* exec(nombre, comando)            */   
#define WAITPID               0x0003   /* waitpid(pid, &status)            */
#define EXIT                  0x0004   /* exit(status)                     */
#define GETPID                0x0005   /* getpid()                         */
#define GETPINDX              0x0006   /* getpindx()                       */  
#define GETPPID               0x0007   /* getppid()                        */
#define GETUID                0x0008   /* getuid()                         */ 
#define SETUID                0x0009   /* setuid(uid)                      */
#define GETGID                0x000a   /* getgid()                         */ 
#define SETGID                0x000b   /* setgid(gid)                      */
#define KILLPID               0x000c   /* killpid(pid)                     */ 
#define THREAD                0x000d   /* thread(funcion, SP0, arg)        */
#define YIELD                 0x000e   /* yield()                          */

/* ======================================================================= */
/* ops. de ficheros     (AX = 0x01xx)                                      */
/* ======================================================================= */

#define OPEN                  0x0100   /* open(nombre, modoAp)             */
#define CLOSE                 0x0101   /* close(df)                        */
#define READ                  0x0102   /* read(df, dir, n)                 */
#define AIO_READ              0x0103   /* aio_read(df, dir, n)             */
#define WRITE                 0x0104   /* write(df, dir, n)                */
#define AIO_WRITE             0x0105   /* aio_write(df, dir, n)            */
#define LSEEK                 0x0106   /* lseek(df, pos, whence)           */  
#define FCNTL                 0x0107   /* fcntl(df, cmd, ... )             */ 
#define IOCTL                 0x0108   /* ioctl(df, request, ... )         */  

/* ======================================================================= */
/* manejo de drivers    (AX = 0x02xx)                                      */
/* ======================================================================= */

#define OBTENINFOSO1          0x0200   /* obtenInfoSO1(dSO1)               */
#define CREARRECURSO          0x0201   /* crearRecurso(dR)                 */
#define CREARFICHERO          0x0202   /* crearFichero(nombre, rindx)      */
#define ESPERARDESINSTALACION 0x0203   /* esperarDesinstalacion( ... )     */
#define DESTRUIRRECURSO       0x0204   /* destruirRecurso(nombre, matando) */
#define ENCOLARCCBRECURSO     0x0205   /* encolarCcbRecurso(cb, nombre)    */
#define ELIMINARCCBRECURSO    0x0206   /* eliminarCcbRecurso(cb, nombre)   */                

/* ======================================================================= */
/* obtenInfo y otras    (AX = 0x03xx)                                      */
/* ======================================================================= */

#define OBTENINFOPS           0x0300   /* obtenInfoPS(&dP,&e2FFR,&c2cPFR)  */
#define OBTENINFOMEM          0x0301   /* obtenInfoMEM(&dP,&e2FFR, ... )   */
#define OBTENINFOPFR          0x0302   /* obtenInfoPFR(&dP,&dF,&dR, ... )  */
#define OBTENINFOFAB          0x0303   /* obtenInfoFAB(&df)                */    
#define OBTENINFOINFO         0x0304   /* obtenInfoINFO(&info)             */ 
#define GETDISK               0x0305   /* getdisk()                        */
#define FINDFIRST             0x0306   /* listarDirectorio(unidad, rec)    */
#define FINDNEXT              0x0307   /* unidadLogicaLista(unidadLogica)  */

/* ======================================================================= */
/* analizarProcesos     (AX = 0x04xx)                               (plot) */
/* ======================================================================= */

#define ACTIVARTRAZA          0x0400    /* activarTraza(numInstr,pid)      */
#define ANALIZARPROCESOS      0x0401    /* analizarProcesos(Ok)            */

/* ======================================================================= */
/* paso de mensajes     (AX = 0x05xx)                         (tipo minix) */
/* ======================================================================= */

#define SEND                  0x0500    /* send(pindx, &msj)               */
#define RECEIVE               0x0501    /* receive(pindx, &msj)            */
#define SENDREC               0x0502    /* sendrec(pindx, &msj)            */
#define NOTIFY                0x0503    /* notify(pindx)                   */
#define ECHO                  0x0506    /* echo(pindx, &msj)               */

/* ======================================================================= */
/* user2system          (AX = 0x06xx)                       (experimental) */
/* ======================================================================= */

#define USER2SYSTEM           0x0600    /* user2system()                   */

/* ======================================================================= */
/* manejo de seniales   (AX = 0x07xx)                                      */
/* ======================================================================= */

#define KILL                  0x0700    /* kill(pid, sig)                  */ 
#define SIGACTION             0x0701    /* sigaction(sig, &act, &oact)     */
#define ALARM                 0x0702    /* alarm(seconds)                  */ 
#define PAUSE                 0x0703    /* pause()                         */ 
#define SIGSUSPEND            0x0704    /* sigsuspend(sig)                 */

/* ----------------------------------------------------------------------- */
/* -                                                                     - */
/* -         Llamadas al sistema implementadas mediante ficheros         - */
/* -         ===================================================         - */
/* ----------------------------------------------------------------------- */

/* ======================================================================= */
/*                        Gestion de memoria ("GM")                        */
/* ======================================================================= */

/* buscarBloque(tam)                          // read(dfGM, &w, 2)         */
/* devolverBloque(segmento, tam)              // read(dfGM, &dw, 4)        */
/* tamBloqueMax()                             // aio_read(dfGM, &tam, 2)   */  

/* ======================================================================= */
/*                        retardoActivo ("RETARDO")                        */
/* ======================================================================= */

/* ----------------------------------------------------------------------- */
/* retardoActivo(nVueltas)                    // read/write(dfRET, &dw, 4) */
/* ----------------------------------------------------------------------- */


pid_t createProcess ( const char far * nombre, 
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


int exec ( const char far * nombre,         
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


pid_t waitpid ( pid_t pid, int far * statloc ) ;

/* ----------------------------------------------------------------------- */
/* El proceso que ejecuta esta llamada queda bloqueado hasta que el        */
/* proceso hijo con el pid indicado termine. Si pid = -1 el bloqueo es     */
/* tan solo hasta que termine cualquiera de sus hijos. La llamada retorna  */
/* el pid del proceso hijo terminado.                                      */
/* ----------------------------------------------------------------------- */


void exit ( int status ) ;                  

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


int setuid ( uid_t uid ) ;                    

/* ----------------------------------------------------------------------- */
/* Esta llamada al sistema establece el uid del proceso que la ejecuta.    */
/* ----------------------------------------------------------------------- */


gid_t getgid ( void ) ;

/* ----------------------------------------------------------------------- */
/* Esta llamada al sistema devuelve como resultado el gid del proceso      */
/* que la ejecuta.                                                         */
/* ----------------------------------------------------------------------- */


int setgid ( gid_t gid ) ;                   

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
                                              
int close ( int df ) ;                        

int read ( int df, pointer_t dir, word_t nbytes ) ;

int aio_read ( int df, pointer_t dir, word_t nbytes ) ;

int write ( int df, pointer_t dir, word_t nbytes ) ;
                                            
int aio_write ( int df, pointer_t dir, word_t nbytes ) ;

long int lseek ( int df, long int pos, word_t whence  ) ;
                                             
int fcntl ( int df, word_t cmd, word_t arg ) ;
                                               
int ioctl ( int df, word_t request, word_t arg ) ;
                                             
/* drivers */

void obtenInfoSO1 ( descSO1_t far * descSO1 ) ;

rindx_t crearRecurso ( descRecurso_t far * dR ) ;

dfs_t crearFichero ( const char far * nombre,
                     rindx_t          rindx,
                     word_t           menor,
                     tipoFichero_t    tipo ) ;

int esperarDesinstalacion ( word_t tamDATA, ... ) ;

int destruirRecurso ( char far * nombre, bool_t matando ) ;

int encolarCcbRecurso ( callBack_t cb, const char far * nombre ) ;

int eliminarCcbRecurso ( callBack_t cb, const char far * nombre ) ;


typedef struct {
  descProceso_t far * descProceso ;
  descFichero_t far * descFichero ;
  descRecurso_t far * descRecurso ;
  e2PFR_t       far * e2PFR ;
  c2c_t         far * c2cPFR ;
} bloquePFR_t ;

void obtenInfoPS ( descProceso_t far * descProceso,        
                   e2PFR_t       far * e2PFR,
                   c2c_t         far * c2cPFR ) ;

/* ----------------------------------------------------------------------- */
/* Esta llamada al sistema copia la tabla de procesos en la direccion      */
/* indicada por descProceso. Tambien copia los enlaces y colas necesarios  */
/* para conocer el orden los procesos preparados.                          */
/* ----------------------------------------------------------------------- */


void obtenInfoMEM ( descProceso_t far * descProceso,       
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


void obtenInfoPFR ( descProceso_t far * descProceso,       
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

int procesarEntrada ( ffblk_t * ffblk ) ;              /* funcion auxiliar */

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

word_t buscarBloque ( word_t tam ) ;

bool_t devolverBloque ( word_t segmento, word_t tam ) ;

word_t tamBloqueMax ( void ) ;

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