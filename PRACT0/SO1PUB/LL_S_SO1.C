/* ----------------------------------------------------------------------- */
/*                               ll_s_so1.c                                */
/* ----------------------------------------------------------------------- */
/*   biblioteca de funciones de interfaz de las llamadas al sistema SO1    */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\ll_s_so1.h>

/* ----------------------------------------------------------------------- */
/* -                                                                     - */
/* -     Llamadas al sistema implementadas mediante traps (int 0x60)     - */
/* -     ===========================================================     - */
/* ----------------------------------------------------------------------- */

/* ======================================================================= */
/*  AX                   Gestion de procesos (AH = 0)                      */
/* ======================================================================= */
/* 0000: createProcess(nombre, comando)                                    */
/* 0001: fork()                                                            */
/* 0002: exec(nombre, comando)                                             */
/* 0003: waitpid(pid, &statloc)                                            */
/* 0004: exit(status)                                                      */
/* 0005: getpid()                                                          */
/* 0006: getpindx()                                                        */
/* 0007: getppid()                                                         */
/* 0008: getuid()                                                          */
/* 0009: setuid()                                                          */
/* 000a: getgid()                                                          */
/* 000b: setgid()                                                          */
/* 000c: kill(pid)                                                         */
/* ======================================================================= */


/* ----------------------------------------------------------------------- */
/* createProcess(nombre, comando)                                          */
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
                      const char far * comando ) ;       /* en ll_s_exec.h */


/* ----------------------------------------------------------------------- */
/* fork()                                                                  */
/* ----------------------------------------------------------------------- */
/* Esta llamada al sistema crea un nuevo proceso identico al proceso       */
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

pid_t fork ( void ) {                                        /* ax = 0001h */
  pid_t hpid ;
#if (FALSE)
  if (dfGP == -1) dfGP = open("GP", O_RDONLY) ;
  read(dfGP, (pointer_t)&hpid, 2) ;
#else
  asm {
    mov ax,0001h ;
    int nVIntSO1 ;
    mov hpid,dx
  }
#endif
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

pid_t exec ( const char far * nombre,                        /* ax = 0002h */
             const char far * comando ) ;                /* en ll_s_exec.h */


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

pid_t waitpid ( pid_t pid, int far * statloc ) ;             /* ax = 0003h */
                                            /* implementada en ll_s_exec.h */

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

void exit ( int status ) ;                                   /* ax = 0004h */
                                            /* implementada en ll_s_exec.h */

/* ----------------------------------------------------------------------- */
/* pid_t getpid()                                                          */
/* ----------------------------------------------------------------------- */
/* Esta llamada al sistema devuelve como resultado el pid del proceso que  */
/* la ejecuta.                                                             */
/* ----------------------------------------------------------------------- */

pid_t getpid ( void ) {                                      /* ax = 0005h */
  pid_t pid ;
  asm {
    mov ax,0005h ;
    int nVIntSO1 ;
    mov pid,bx ;
  }
  return(pid) ;
}

/* ----------------------------------------------------------------------- */
/* pindx_t getpindx()                                                      */
/* ----------------------------------------------------------------------- */
/* Esta llamada al sistema devuelve como resultado el pindx del proceso    */
/* que la ejecuta.                                                         */
/* ----------------------------------------------------------------------- */

pindx_t getpindx ( void ) {                                  /* ax = 0006h */
  pindx_t pindx ;
  asm {
    mov ax,0006h ;
    int nVIntSO1 ;
    mov pindx,bx ;
  }
  return(pindx) ;
}

/* ----------------------------------------------------------------------- */
/* pid_t getppid()                                                         */
/* ----------------------------------------------------------------------- */
/* Esta llamada al sistema devuelve como resultado el pid del proceso      */
/* padre del proceso que la ejecuta.                                       */
/* ----------------------------------------------------------------------- */

pid_t getppid ( void ) {                                     /* ax = 0007h */
  pid_t ppid ;
  asm {
    mov ax,0007h ;
    int nVIntSO1 ;
    mov ppid,bx ;
  }
  return(ppid) ;
}

/* ----------------------------------------------------------------------- */
/* uid_t getuid()                                                          */
/* ----------------------------------------------------------------------- */
/* Esta llamada al sistema devuelve como resultado el uid del proceso      */
/* que la ejecuta.                                                         */
/* ----------------------------------------------------------------------- */

uid_t getuid ( void ) {                                      /* ax = 0008h */
  uid_t uid ;
  asm {
    mov ax,0008h ;
    int nVIntSO1 ;
    mov uid,bx ;
  }
  return(uid) ;
}

/* ----------------------------------------------------------------------- */
/* setuid(uid)                                                             */
/* ----------------------------------------------------------------------- */
/* Esta llamada al sistema establece el uid del proceso que la ejecuta.    */
/* ----------------------------------------------------------------------- */

int setuid ( uid_t uid ) ;                                   /* ax = 0009h */
                                            /* implementada en ll_s_exec.h */

/* ----------------------------------------------------------------------- */
/* gid_t getgid()                                                          */
/* ----------------------------------------------------------------------- */
/* Esta llamada al sistema devuelve como resultado el gid del proceso      */
/* que la ejecuta.                                                         */
/* ----------------------------------------------------------------------- */

pid_t getgid ( void ) {                                      /* ax = 000ah */
  gid_t gid ;
  asm {
    mov ax,000ah ;
    int nVIntSO1 ;
    mov gid,bx ;
  }
  return(gid) ;
}

/* ----------------------------------------------------------------------- */
/* setgid(uid)                                                             */
/* ----------------------------------------------------------------------- */
/* Esta llamada al sistema establece el gid del proceso que la ejecuta.    */
/* ----------------------------------------------------------------------- */

int setgid ( gid_t gid ) ;                                   /* ax = 000bh */
                                            /* implementada en ll_s_exec.h */

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

int kill ( int pid ) ;                                       /* ax = 000ch */
                                            /* implementada en ll_s_exec.h */

/* ======================================================================= */
/*  AX                operaciones de ficheros (AH = 1)                     */
/* ======================================================================= */
/* 0100: open(nombre, modoAp)                                              */
/* 0101: close(df)                                                         */
/* 0102: read(df, dir, nbytes)                                             */
/* 0103: aio_read(df, dir, nbytes)                                         */
/* 0104: write(df, dir, nbytes)                                            */
/* 0105: aio_write(df, dir, nbytes)                                        */
/* 0106: lseek(df, pos, whence)                                            */
/* 0107: fcntl(df, cmd, ...)                                               */
/* 0108: ioctl(df, request, ...)                                           */
/* ======================================================================= */

/* ----------------------------------------------------------------------- */
/* int open(nombre, modoAp)                                                */
/* ----------------------------------------------------------------------- */
/* Esta llamada al sistema abre el fichero con ese nombre y devuelve como  */
/* resultado el correspondiente descriptor de fichero.                     */
/* ----------------------------------------------------------------------- */

int open ( const char far * nombre, modoAp_t modoAp ) ;
                                            /* implementada en ll_s_exec.h */
/* ----------------------------------------------------------------------- */
/* int close(df)                                                           */
/* ----------------------------------------------------------------------- */
/* Esta llamada al sistema cierra el fichero correspondiente al descriptor */
/* de fichero indicado.                                                    */
/* ----------------------------------------------------------------------- */

int close ( int df ) ;                      /* implementada en ll_s_exec.h */

int read ( int df, pointer_t dir, word_t nbytes ) {
  int res ;
  asm {
    mov bx,df ;
    les dx,dir ;
    mov cx,nbytes ;
    mov ax,0102h ;
    int nVIntSO1 ;
    mov res,ax ;
  }
  return(res) ;
}

int aio_read ( int df, pointer_t dir, word_t nbytes ) {
  int res ;
  asm {
    mov bx,df ;
    les dx,dir ;
    mov cx,nbytes ;
    mov ax,0103h ;
    int nVIntSO1 ;
    mov res,ax ;
  }
  return(res) ;
}

int write ( int df, pointer_t dir, word_t nbytes ) ;
                                            /* implementada en ll_s_exec.h */

int aio_write ( int df, pointer_t dir, word_t nbytes ) {
  int res ;
  asm {
    mov bx,df ;
    les dx,dir ;
    mov cx,nbytes ;
    mov ax,0105h ;
    int nVIntSO1 ;
    mov res,ax ;
  }
  return(res) ;
}
                                            /* implementada en ll_s_exec.h */
long lseek ( int df, long pos, word_t whence  ) ; 

/* ----------------------------------------------------------------------- */
/* char leer(df)                                                           */
/* ----------------------------------------------------------------------- */
/* Esta llamada al sistema lee el siguiente byte del fichero indicado por  */
/* el descriptor de fichero df.                                            */
/* ----------------------------------------------------------------------- */

char leer ( int df ) {
  char car ;
  int n = read(df, (pointer_t)&car, 1) ;
  if (n < 1) car = (char)0 ;
  return(car) ;
}

/* ----------------------------------------------------------------------- */
/* char leerListo(df)                                                      */
/* ----------------------------------------------------------------------- */
/* Esta llamada al sistema lee el siguiente byte del fichero indicado por  */
/* el descriptor de fichero df.                                            */
/* ----------------------------------------------------------------------- */

char leerListo ( int df ) {
  char car ;
  int n = aio_read(df, (pointer_t)&car, 1) ;
  if (n < 1) car = (char)0 ;
  return(car) ;
}

char leerAsciiListo ( int df ) {
  char car ;
  int n = aio_read(df, (pointer_t)&car, 1) ;
  if (n < 1) car = (char)0 ;
  else if (car == (char)0) {
    aio_read(df, (pointer_t)&car, 1) ;
    return((char)0) ;
  }
  return(car) ;
}
                                          
int fcntl ( int df, word_t cmd, word_t arg  ) {
  word_t res ;
  asm {
    mov bx,df ;
    mov cx,cmd ;
    mov dx,arg ;
    mov ax,0107h ;
    int nVIntSO1 ;
    mov res,ax ;
  }
  return((bool_t)res) ;
}
                                            /* implementada en ll_s_exec.h */
int ioctl ( int df, word_t cmd, word_t arg  ) ;
	
/* ======================================================================= */
/*  AX            operaciones de manejo de drivers (AH = 2)                */
/* ======================================================================= */
/* 0200: obtenInfoSO1(dSO1)                                                */
/* 0201: rindx = crearRecurso(dR)                                          */
/* 0202: crearFichero(nombre, rindx)                                       */
/* 0203: esperarDesinstalacion()                                           */
/* 0204: destruirRecurso(nombre)                                           */
/* 0205: encolarCcbRecurso(cb, nombre)                                     */
/* 0206: eliminarCcbRecurso(cb, nombre)                                    */
/* ======================================================================= */

void obtenInfoSO1 ( descSO1_t far * descSO1 ) {
  asm {
    les bx,descSO1 ;
    mov ax,0200h ;
    int nVIntSO1 ;
  }
}

rindx_t crearRecurso ( descRecurso_t far * dR ) {
  rindx_t rindx ;
  asm les bx,dR
  asm mov ax,0201h
  asm int nVIntSO1
  asm mov rindx,ax
  return(rindx) ;
}

dfs_t crearFichero ( const char far * nombre,
                     rindx_t          rindx,
                     word_t           menor,
                     tipoFichero_t    tipo ) {
  int res ;
  asm {
    les bx,nombre ;
    mov cx,rindx ;
    mov dx,menor ;
    mov si,tipo ;
    mov ax,0202h ;
    int nVIntSO1 ;
    mov res,ax ;
  }
  return(res) ;
}

int esperarDesinstalacion ( void ) {
  int res ;
  asm {
    mov ax,0203h ;
    int nVIntSO1 ;
    mov res,ax ;
  }
  return(res) ;
}

int destruirRecurso ( char far * nombre ) {
  int res ;
  asm {
    les bx,nombre ;
    mov ax,0204h ;
    int nVIntSO1 ;
    mov res,ax ;
  }
  return(res) ;
}

int encolarCcbRecurso ( callBack_t cb, const char far * nombre ) {
  int res ;
  asm {
    les bx,nombre ;                                       /* ES:BX         */
    mov cx,es ;                                           /* CX:BX nombre  */
    les dx,cb ;                                           /* ES:DX cb      */
    mov ax,0205h ;
    int nVIntSO1 ;
    mov res,ax ;
  }
  return(res) ;
}

int eliminarCcbRecurso ( callBack_t cb, const char far * nombre ) {
  int res ;
  asm {
    les bx,nombre ;                                       /* ES:BX         */
    mov cx,es ;                                           /* CX:BX nombre  */
    les dx,cb ;                                           /* ES:DX cb      */
    mov ax,0206h ;
    int nVIntSO1 ;
    mov res,ax ;
  }
  return(res) ;
}

/* ======================================================================= */
/*  AX             obtenInfo y unidadLogicaLista (AH = 3)                  */
/* ======================================================================= */
/* 0300: obtenInfoPS(&descProceso,&e2FFR,&c2cPFR)                          */
/* 0301: obtenInfoMEM(&descProceso,&e2FFR,&c2cPFR,&listaLibres,&tamBlqMax) */
/* 0302: obtenInfoPFR(&descProc,&descFich,&descRec,&e2FFR,&c2cPFR)         */
/* 0303: obtenInfoFAB(&df)                                                 */
/* 0304: obtenInfoINFO(&info)                                              */
/* 0305: listarDirectorio(unidad, recursivo)                               */
/* 0306: unidadLoficaLista(unidadLogica)                                   */
/* ======================================================================= */

/* ----------------------------------------------------------------------- */
/* obtenInfoPS(&descProceso,&e2PFR,&c2cPFR)                                */
/* ----------------------------------------------------------------------- */
/* Esta llamada al sistema copia la tabla de procesos en la direccion      */
/* indicada por descProceso. Tambien copia los enlaces y colas necesarios  */
/* para conocer el orden los procesos preparados.                          */
/* ----------------------------------------------------------------------- */

void obtenInfoPS ( descProceso_t far * descProceso,         /* ax = 0300h */
                   e2PFR_t       far * e2PFR,
                   c2c_t         far * c2cPFR ) {

  bloquePFR_t bPFR ;

  bloquePFR_t far * ptrPFR = (bloquePFR_t far *)&bPFR ;

  bPFR.descProceso = descProceso ;
  bPFR.e2PFR = e2PFR ;
  bPFR.c2cPFR = c2cPFR ;

  asm {
    les bx,ptrPFR ;
    mov ax,0300h ;
    int nVIntSO1 ;
  }
}

/* ----------------------------------------------------------------------- */
/* obtenInfoMEM(&descProceso,&e2PFR,&c2c,&listaLibres,&tamBlqMax) ;        */
/* ----------------------------------------------------------------------- */
/* Esta llamada al sistema copia la tabla de procesos en la direccion      */
/* indicada por descProceso. Tambien copia los enlaces y colas necesarios  */
/* para conocer el orden los procesos preparados. Finalmente copia la      */
/* lista de bloques libres a partir de la direcci¢n listaLibres.           */
/* ----------------------------------------------------------------------- */

void obtenInfoMEM ( descProceso_t far * descProceso,         /* ax = 0301h */
                    e2PFR_t       far * e2PFR,
                    c2c_t         far * c2cPFR,
                    ptrBloque_t       * listaLibres,
                    word_t            * tamBlqMax ) {

  bloquePFR_t bPFR ;
  word_t segmento ;
  word_t tam ;

  bloquePFR_t far * ptrPFR = (bloquePFR_t far *)&bPFR ;

  bPFR.descProceso = descProceso ;
  bPFR.e2PFR = e2PFR ;
  bPFR.c2cPFR = c2cPFR ;

  asm {
    les bx,ptrPFR ;
    mov ax,0301h ;
    int nVIntSO1 ;

    mov segmento,ax ;
    mov tam,dx ;
  }
  *listaLibres = (ptrBloque_t)pointer(segmento, 0x0000) ;
  *tamBlqMax = tam ;

}

/* ----------------------------------------------------------------------- */
/* obtenInfoPFR(&descProc,&descFich,&descRec,&e2PFR,&c2c) ;                */
/* ----------------------------------------------------------------------- */
/* Esta llamada al sistema copia la tabla de procesos en la direccion      */
/* indicada por descProceso, la tabla de ficheros abiertos del sistema en  */
/* la direccion descFichero y la tabla de recursos del sistema en la       */
/* direccion descRecurso. Tambien copia los enlaces y colas necesarios     */
/* para conocer el orden los procesos preparados.                          */
/* ----------------------------------------------------------------------- */

void obtenInfoPFR ( descProceso_t far * descProceso,         /* ah = 0302h */
                    descFichero_t far * descFichero,
                    descRecurso_t far * descRecurso,
                    e2PFR_t       far * e2PFR,
                    c2c_t         far * c2cPFR ) {

  bloquePFR_t bPFR ;

  bloquePFR_t far * ptrPFR = (bloquePFR_t far *)&bPFR ;

  bPFR.descProceso = descProceso ;
  bPFR.descFichero = descFichero ;
  bPFR.descRecurso = descRecurso ;
  bPFR.e2PFR = e2PFR ;
  bPFR.c2cPFR = c2cPFR ;

  asm {
    les bx,ptrPFR ;
    mov ax,0302h ;
    int nVIntSO1 ;
  }
}

/* ----------------------------------------------------------------------- */
/* obtenInfoFAB(&df) ;                                                     */
/* ----------------------------------------------------------------------- */
/* Esta llamada al sistema copia la tabla de ficheros abiertos en la       */
/* direccion indicada por df.                                              */
/* ----------------------------------------------------------------------- */

void obtenInfoFAB ( descriptor_de_fichero_t far * df ) {     /* ax = 0303h */
  asm {
    les bx,df ;
    mov ax,0303h ;
    int nVIntSO1 ;
  }
}

/* ----------------------------------------------------------------------- */
/* obtenInfoINFO(&info)                                                    */
/* ----------------------------------------------------------------------- */
/* Esta llamada al sistema copia informacion diversa del sistema a partir  */
/* de la direccion indicada por df.                                        */
/* ----------------------------------------------------------------------- */

void obtenInfoINFO ( info_t far * info ) {                   /* ax = 0304h */
  asm {
    les bx,info
    mov ax,0304h
    int nVIntSO1
  }
}

int getdisk ( void ) {
  int unidadLogica ;
  asm {
    mov ax,0305h
    int nVIntSO1
    mov unidadLogica,ax
  }
  return(unidadLogica) ;
}

#if (FALSE)

/* ----------------------------------------------------------------------- */
/* int unidadLogicaLista(unidadLogica)                                     */
/* ----------------------------------------------------------------------- */
/* Esta llamada al sistema permite saber si la unidad indicada esta lista  */
/* para acceder fisicamente a ella.                                        */
/* ----------------------------------------------------------------------- */

int unidadLogicaLista ( byte_t unidadLogica ) {              /* ax = 0306h */
  int err ;
  asm {
    mov bl,unidadLogica ;
    mov ax,0305h ;
    int nVIntSO1 ;
    mov err,ax ;
  }
  return(err) ;
}

#endif

int procesarEntrada ( ffblk_t * ffblk ) {              /* funcion auxiliar */
  entrada_t far * entrada = (entrada_t far *)ffblk->buffer ;
  int i = ffblk->bindx ;
  int j, k ;
  ffblk->ff_attrib = entrada[i].atr ;
  ffblk->ff_ftime = entrada[i].hora ;
  ffblk->ff_fdate = entrada[i].fecha ;
  ffblk->ff_fsize = entrada[i].tam ;
  j = 0 ;
  while ((j < 8) && (entrada[i].nombre[j] != ' ')) {
    ffblk->ff_name[j] = entrada[i].nombre[j] ;
    j++ ;
  }
  if (entrada[i].ext[0] != ' ') {
    ffblk->ff_name[j] = '.' ;
    j++ ;
  }
  k = 0 ;
  while ((k < 3) && (entrada[i].ext[k] != ' ')) {
    ffblk->ff_name[j] = entrada[i].ext[k] ;
    k++ ;
    j++ ;
  }
  ffblk->ff_name[j] = (char)0 ;
  /* entrada[i].primerCluster ; */
  return(0) ;
}

/* ----------------------------------------------------------------------- */
/* findFirst(unidadLogica, &ffblk)                                         */
/* ----------------------------------------------------------------------- */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\bios_0.h>

int findFirst ( byte_t unidadLogica, ffblk_t * ffblk ) {
  int res ;
  asm {
    mov bl,unidadLogica ;
    mov dx,ffblk ;
    mov ax,0306h ;
    int nVIntSO1 ;                                     /* ffblk->bindx = 0 */
    mov res,ax ;
  }
  if (res == 0)
    procesarEntrada(ffblk) ;
  return(res) ;
}

/* ----------------------------------------------------------------------- */
/* findNext(&ffblk)                                                        */
/* ----------------------------------------------------------------------- */
/* ----------------------------------------------------------------------- */

int findNext ( ffblk_t * ffblk ) {
  entrada_t far * entrada = (entrada_t far *)ffblk->buffer ;
  int res ;
  int i ;
  for ( i = ++ffblk->bindx ; i < 16 ; i++ )
    if ((entrada[i].nombre[0] != 0x00) && (entrada[i].nombre[0] != 0xE5))
      break ;
  ffblk->bindx = i ;
  if (ffblk->bindx < 16) {
    procesarEntrada(ffblk) ;
    return(0) ;
  }
  else {
    ffblk->eindx += 16 ;
    asm {
      mov dx,ffblk ;
      mov ax,0307h ;
      int nVIntSO1 ;                                   /* ffblk->bindx = 0 */
      mov res,ax ;
    }
    if (res == 0)
      procesarEntrada(ffblk) ;
    return(res) ;
  }
}

/* ======================================================================= */
/*  AX             activarTraza y analizarProcesos (AH = 4)                */
/* ======================================================================= */
/* 0400: activarTraza(numInstr,pid)                                        */
/* 0401: analizarProcesos(Ok)                                              */
/* ======================================================================= */

/* ----------------------------------------------------------------------- */
/* activarTraza(numInstr, pid)                                             */
/* ----------------------------------------------------------------------- */
/* Si numInstr = 0 esta llamada al sistema desactiva el flag de traza del  */
/* proceso con identificador de proceso pid, si pid > 0, o de todos los    */
/* procesos, si pid = -1. En el caso de que numInstr > 0 esta llamada      */
/* activa el flag de traza del proceso pid, si pid > 0, o de todos los     */
/* procesos, si pid = -1. En el caso de que numInstr > 0, ademas se        */
/* establece  el numero maximo de instrucciones ejecutadas sin ser         */
/* visualizadas por el puerto serie 0, al valor numInstr.                  */
/* ----------------------------------------------------------------------- */

void activarTraza ( word_t numInstr,
                    pid_t pid ) {                            /* ax = 0700h */
  asm {
    mov bx,numInstr ;
    mov dx,pid ;
    mov ax,0400h ;
    int nVIntSO1 ;
  }
}

/* ----------------------------------------------------------------------- */
/* analizarProcesos(Ok)                                                    */
/* ----------------------------------------------------------------------- */
/* Habilita el envio de informacion por el puerto COM1 para representar el */
/* seguimiento del comportamiento de los procesos del sistema. Dicha       */
/* informaci¢n se envia periodicamente cada tic de reloj.                  */
/* ----------------------------------------------------------------------- */

void analizarProcesos ( bool_t Ok ) {                        /* ax = 0701h */
  asm {
    mov ax,Ok ;              /* pasamos a SO1 el parametro Ok en el flag Z */
    and ax,ax ;                                     /* activamos los flags */
    mov ax,0401h ;
    int nVIntSO1 ;
  }
}

/* ----------------------------------------------------------------------- */
/* -                                                                     - */
/* -         Llamadas al sistema implementadas mediante ficheros         - */
/* -         ===================================================         - */
/* ----------------------------------------------------------------------- */

/* ======================================================================= */
/*                        Gestion de memoria ("GM")                        */
/* ======================================================================= */

word_t ll_buscarBloque ( word_t tam ) {
  word_t w = tam ;
  int dfGM = open("GM", O_RDONLY) ;
  if (dfGM < 0) return(0x0000) ;
  read(dfGM, (pointer_t)&w, 2) ;
  return(w) ;
}

bool_t ll_devolverBloque ( word_t segmento, word_t tam ) {
  word_t w [2] ;
  int dfGM = open("GM", O_RDONLY) ;
  if (dfGM < 0) return(FALSE) ;
  w[0] = segmento ;
  w[1] = tam ;
  return(read(dfGM, (pointer_t)&w, 4) == 4) ;
}

word_t ll_tamBloqueMax ( void ) {
  word_t tam ;
  int dfGM = open("GM", O_RDONLY) ;
  if (dfGM < 0) return(0x0000) ;
  aio_read(dfGM, (pointer_t)&tam, 2) ;
  return(tam) ;
}

/* ======================================================================= */
/*                        retardoActivo ("RETARDO")                        */
/* ======================================================================= */

/* ----------------------------------------------------------------------- */
/* retardoActivo(nVueltas)                                                 */
/* ----------------------------------------------------------------------- */
/* Si nVueltas = 0 esta llamada provoca que el proceso ejecute un bucle    */
/* que provoca la demora establecida en SO1 como estandar.                 */
/* Si nVueltas = 1 esta llamada retorna el numero de vueltas que lleva a   */
/* cabo el bucle anteriormente mencionado.                                 */
/* Si nVueltas > 1 esta llamada establece en SO1 el numero de vueltas que  */
/* se utiliza en el bucle de retardo mencionado.                           */
/* ----------------------------------------------------------------------- */

dword_t retardoActivo ( dword_t nVueltas ) {
  int df ;
  dword_t res = (dword_t)0 ;
  retardarProceso_t retardarProceso ;
  dword_t nVueltasRetardo ;
  if ((df = open("RETARDO", O_RDONLY)) < 0) return((dword_t)-1) ;
  switch (nVueltas) {
  case (dword_t)0 :
    *((pointer_t)&retardarProceso) = 0x01 ;
    read(df, (pointer_t)&retardarProceso, 4) ;
    retardarProceso() ;
    break ;
  case (dword_t)1 :
    *((pointer_t)&nVueltasRetardo) = 0x00 ;
    read(df, (pointer_t)&nVueltasRetardo, 4) ;
    res = nVueltasRetardo ;
    break ;
  default :
    close(df) ;
    df = open("RETARDO", O_WRONLY) ;
    nVueltasRetardo = nVueltas ;
    write(df, (pointer_t)&nVueltasRetardo, 4) ;
    break ;
  }
  close(df) ;
  return(res) ;
}

/* --------------------------------------------------------------------- */
/*                               semaforos                               */
/* --------------------------------------------------------------------- */
/*           funciones de interfaz para el manejo de semaforos           */
/* --------------------------------------------------------------------- */

#define semMax 10

int sem_inic (unsigned int sem,     /* codigo de operacion en ax = 0500h */
              unsigned int valor  ) {
  if (sem >= semMax) return(-1) ;
  asm mov bx,sem
  asm mov dx,valor
  asm mov ax,0500h                                 /* AH = 05h y AL = 00 */
  asm int nVIntSO1     /* nVIntSO1 es el numero de vector de int. de SO1 */
  return(0) ;
}

int sem_bajar (unsigned int sem ) { /* codigo de operacion en ax = 0501h */
  if (sem >= semMax) return(-1) ;
  asm mov bx,sem
  asm mov ax,0501h                                 /* AH = 05h y AL = 01 */
  asm int nVIntSO1     /* nVIntSO1 es el numero de vector de int. de SO1 */
  return(0) ;
}

int sem_subir (unsigned int sem ) { /* codigo de operacion en ah = 0502h */
  if (sem >= semMax) return(-1) ;
  asm mov bx,sem
  asm mov ax,0502h                                 /* AH = 05h y AL = 02 */
  asm int nVIntSO1     /* nVIntSO1 es el numero de vector de int. de SO1 */
  return(0) ;
}

/* --------------------------------------------------------------------- */
/*                                 buzones                               */
/* --------------------------------------------------------------------- */
/*             funciones de interfaz para el paso de mensajes            */
/* --------------------------------------------------------------------- */

#define Capacidad 2  /* Capacidad de los buzones, puede ser 0, 1, 2, ... */
#define mboxMax 10                   /* 10 buzones (mailbox) como maximo */
int mbox_send ( unsigned   mbox,    /* codigo de operacion en ah = 0e00h */
                void far * msj ) {
  if (mbox >= mboxMax) return(-1) ;
  asm mov dx,mbox
  asm les bx,msj                   /* es = SEGMENT msj, bx = OFFSET  msj */
  asm mov ax,0600h
  asm int nVIntSO1     /* nVIntSO1 es el numero de vector de int. de SO1 */
  return(0) ;
}
int mbox_receive ( unsigned   mbox, /* codigo de operacion en ah = 0e01h */
                   void far * msj ) {
  if (mbox >= mboxMax) return(-1) ;
  asm mov dx,mbox
  asm les bx,msj                   /* es = SEGMENT msj, bx = OFFSET  msj */
  asm mov ax,0601h
  asm int nVIntSO1     /* nVIntSO1 es el numero de vector de int. de SO1 */
  return(0) ;
}
