/* ----------------------------------------------------------------------- */
/*                                ll_s_exc.c                               */
/* ----------------------------------------------------------------------- */
/* Subconjunto de rutinas de interfaz de las llamadas al sistema que se    */
/* utilizan desde SO1 al ser necesarias para la creación del proceso       */
/* inicial INIC. La idea es no tener que enlazar el resto de rutinas de    */
/* interfaz contenidas en ll_s_so1.c para obtener SO1.BIN, de manera que   */
/* se reduzca asi el tamaño de SO1.BIN.                                    */
/* ----------------------------------------------------------------------- */

#ifndef LL_S_EXEC_H
#define LL_S_EXEC_H

#include <so1pub.h\tipos.h>
#include <so1pub.h\def_proc.h>
#include <so1pub.h\ll_s_exc.h>                                 /* nVIntSO1 */

/* ----------------------------------------------------------------------- */
/* exec(nombre, comando)                                                   */
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

pid_t createProcess ( const char far * nombre,               /* ax = 0000h */
                      const char far * comando ) {

  word_t hpid ;
  asm {
    les bx,nombre ;                                       /* ES:BX         */
    mov cx,es ;                                           /* CX:BX nombre  */
    les dx,comando ;                                      /* ES:DX comando */
    mov ax,0000h ;
    int nVIntSO1 ;
    mov hpid,dx ;
  }
  return(hpid) ;
}

/* ----------------------------------------------------------------------- */
/* exec(nombre, comando)                                                   */
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

int exec ( const char far * nombre,                        /* ax = 0002h */
           const char far * comando ) {
  word_t err ;
  asm {
    les bx,nombre ;                                       /* ES:BX         */
    mov cx,es ;                                           /* CX:BX nombre  */
    les dx,comando ;                                      /* ES:DX comando */
    mov ax,0002h ;
    int nVIntSO1 ;
    mov err,dx ;
  }
  return(err) ;
}


/* ----------------------------------------------------------------------- */
/* waitpid(pid, &statloc)                                                  */
/* ----------------------------------------------------------------------- */
/* El proceso que ejecuta esta llamada queda bloqueado hasta que el        */
/* proceso hijo con el pid indicado termine. Si pid = -1 el bloqueo es     */
/* tan solo hasta que termine cualquiera de sus hijos. La llamada retorna  */
/* el pid del proceso hijo terminado y en statloc el codigo de terminacion */
/* del proceso hijo. waitpid mira previamente a ver si el proceso hijo     */
/* esta zombie, en cuyo caso no se bloquea. Si &statloc vale NULL el       */
/* proceso padre indica al hijo que no va a esperar a que termine, por lo  */
/* que el hijo muere inmediatamente con exit sin intentar comunicar su     */
/* estado de terminación al padre.                                         */
/* ----------------------------------------------------------------------- */

pid_t waitpid ( pid_t pid, int far * statloc ) {             /* ax = 0003h */
  pid_t pidDifunto ;
  int status ;
  bool_t noStatus = (statloc == (int far *)NULL) ;  /* el hijo puede morir */
  if ((pid < 0) &&                                 /* sin enviar el status */
      ((pid != -1) || noStatus))
    return(-1) ;
  asm {
    mov bx,pid ;
    mov dx,noStatus ;
    mov ax,0003h ;
    int nVIntSO1 ;
  }
  if (noStatus) return(0) ;
  asm {
    mov status,ax ;
    mov pidDifunto,bx ;
  }
  *statloc = status ;
  return(pidDifunto) ;
}

/* ----------------------------------------------------------------------- */
/* exit(status)                                                            */
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

void exit ( int status ) {                                   /* ax = 0004h */
  asm {
    mov bx,status ;
    mov ax,0004h ;
    int nVIntSO1 ;
  }
}

/* ----------------------------------------------------------------------- */
/* setuid(uid)                                                             */
/* ----------------------------------------------------------------------- */
/* Esta llamada al sistema establece el uid del proceso que la ejecuta.    */
/* ----------------------------------------------------------------------- */

int setuid ( uid_t uid ) {                                   /* ax = 0009h */
  asm {
    mov bx,uid ;
    mov ax,0009h ;
    int nVIntSO1 ;
  }
  return(0) ;
}

/* ----------------------------------------------------------------------- */
/* setgid(uid)                                                             */
/* ----------------------------------------------------------------------- */
/* Esta llamada al sistema establece el gid del proceso que la ejecuta.    */
/* ----------------------------------------------------------------------- */

int setgid ( gid_t gid ) {                                   /* ax = 000bh */
  asm {
    mov bx,gid ;
    mov ax,000bh ;
    int nVIntSO1 ;
  }
  return(0) ;
}

/* ----------------------------------------------------------------------- */
/* int kill(pid)                                                           */
/* ----------------------------------------------------------------------- */
/* Si pid > 0 y corresponde al identificador de un proceso del sistema,    */
/* esta llamada destruye al proceso cuyo identificador de proceso es pid y */
/* retorna como resultado un 0.                                            */
/* Si pid = -1 esta llamada destruye todos los procesos menos el proceso   */
/* con pid 0 que corresponde a la consola del sistema SO1.                 */
/* Si pid = 0 la llamada simplemente retorna el valor -1, ya que no se     */
/* permite matar al proceso consola.                                       */
/* En cualquier otro caso la llamada al sistema retorna el valor -2.       */
/* ----------------------------------------------------------------------- */

int kill ( int pid ) {                                         /* ah = 0ch */
  int codRetorno ;
  asm {
    mov dx,pid ;
    mov ax,000ch ;
    int nVIntSO1 ;
    mov codRetorno,bx ;
  }
  return((int)codRetorno) ;
}

/* ----------------------------------------------------------------------- */
/* int open(nombre, modoAp)                                                */
/* ----------------------------------------------------------------------- */
/* Esta llamada al sistema abre el fichero con ese nombre y devuelve como  */
/* resultado el correspondiente descriptor de fichero.                     */
/* ----------------------------------------------------------------------- */

int open ( const char far * nombre, modoAp_t modoAp ) {
  int df ;
  asm {
    les bx,nombre ;
    mov dx,modoAp ;
    mov ax,0100h ;
    int nVIntSO1 ;
    mov df,ax ;
  }
  return(df) ;
}

/* ----------------------------------------------------------------------- */
/* int close(df)                                                           */
/* ----------------------------------------------------------------------- */
/* Esta llamada al sistema cierra el fichero correspondiente al descriptor */
/* de fichero indicado.                                                    */
/* ----------------------------------------------------------------------- */

int close ( int df ) {
  int res ;
  asm {
    mov bx,df ;
    mov ax,0101h ;
    int nVIntSO1 ;
    mov res,ax ;
  }
  return(res) ;
}

int write ( int df, pointer_t dir, word_t nbytes ) {
  int res ;
  asm {
    mov bx,df ;
    les dx,dir ;
    mov cx,nbytes ;
    mov ax,0104h ;
    int nVIntSO1 ;
    mov res,ax ;
  }
  return(res) ;
}

long lseek ( int df, long pos, word_t whence  ) {
  dword_t res ;
  word_t res_L ;
  word_t res_H ;
  asm {
    mov bx,df ;
    les dx,pos ;
    mov cx,whence ;
    mov ax,0106h ;
    int nVIntSO1 ;
    mov res_L,ax ;
    mov res_H,bx ;
  }
  res = (((dword_t)res_H) << 16) | res_L ;
  return(res) ;
}

word_t ioctl ( int df, word_t cmd, word_t arg  ) {
  int res ;
  asm {
    mov bx,df ;
    mov cx,cmd ;
    mov dx,arg ;
    mov ax,0108h ;
    int nVIntSO1 ;
    mov res,ax ;
  }
  return((bool_t)res) ;
}

#endif /* LL_S_EXEC_H */
