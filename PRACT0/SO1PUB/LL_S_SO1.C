/* ----------------------------------------------------------------------- */
/*                               ll_s_so1.c                                */
/* ----------------------------------------------------------------------- */
/*   biblioteca de funciones de interfaz de las llamadas al sistema SO1    */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\ll_s_so1.h>
#include <so1pub\ll_s_sig.c>           /* kill, sigaction, sigalarm, pause */ 

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
/* 000c: killpid(pid)                                                      */
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
/* Si por algun motivo no puede crearse el proceso hijo, la llamada fork  */
/* devuelve un valor negativo indicando el tipo de error ocurrido.         */
/* ----------------------------------------------------------------------- */

pid_t fork ( void ) {                                        /* ax = 0001h */
#if (FALSE)
  pid_t hpid ;
  if (dfGP == -1) dfGP = open("GP", O_RDONLY) ;
  read(dfGP, (pointer_t)&hpid, 2) ;
  return(hpid) ;
#else
    asm 
	{
        mov ax,FORK ;
        int nVIntSO1 ;
    }
    return(_AX) ;
#endif
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

pid_t getpid ( void )                                        /* ax = 0005h */
{
    asm 
	{
        mov ax,GETPID ;
        int nVIntSO1 ;
    }
    return(_AX) ;
}

/* ----------------------------------------------------------------------- */
/* pindx_t getpindx()                                                      */
/* ----------------------------------------------------------------------- */
/* Esta llamada al sistema devuelve como resultado el pindx del proceso    */
/* que la ejecuta.                                                         */
/* ----------------------------------------------------------------------- */

pindx_t getpindx ( void )                                    /* ax = 0006h */
{
    asm 
	{
        mov ax,GETPINDX ;
        int nVIntSO1 ;
    }
    return(_AX) ;
}

/* ----------------------------------------------------------------------- */
/* pid_t getppid()                                                         */
/* ----------------------------------------------------------------------- */
/* Esta llamada al sistema devuelve como resultado el pid del proceso      */
/* padre del proceso que la ejecuta.                                       */
/* ----------------------------------------------------------------------- */

pid_t getppid ( void )                                       /* ax = 0007h */
{
    asm 
	{
        mov ax,GETPPID ;
        int nVIntSO1 ;
    }
    return(_AX) ;
}

/* ----------------------------------------------------------------------- */
/* uid_t getuid()                                                          */
/* ----------------------------------------------------------------------- */
/* Esta llamada al sistema devuelve como resultado el uid del proceso      */
/* que la ejecuta.                                                         */
/* ----------------------------------------------------------------------- */

uid_t getuid ( void )                                        /* ax = 0008h */
{
    asm 
	{
        mov ax,GETUID ;
        int nVIntSO1 ;
    }
    return(_AX) ;
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

pid_t getgid ( void )                                        /* ax = 000ah */
{
    asm 
	{
        mov ax,GETGID ;
        int nVIntSO1 ;
    }
    return(_AX) ;
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

int killpid ( int pid ) ;                                    /* ax = 000ch */
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

int read ( int df, pointer_t dir, word_t nbytes ) 
{
    asm {
        mov bx,df ;
        les dx,dir ;
        mov cx,nbytes ;
        mov ax,READ ;
        int nVIntSO1 ;
    }
    return(_AX) ;
}

int aio_read ( int df, pointer_t dir, word_t nbytes ) 
{
    asm {
        mov bx,df ;
        les dx,dir ;
        mov cx,nbytes ;
        mov ax,AIO_READ ;
        int nVIntSO1 ;
    }
    return(_AX) ;
}

int write ( int df, pointer_t dir, word_t nbytes ) ;
                                            /* implementada en ll_s_exec.h */

int aio_write ( int df, pointer_t dir, word_t nbytes ) 
{
    asm 
	{
        mov bx,df ;
        les dx,dir ;
        mov cx,nbytes ;
        mov ax,AIO_WRITE ;
        int nVIntSO1 ;
    }
    return(_AX) ;
}
                                            /* implementada en ll_s_exec.h */
long lseek ( int df, long pos, word_t whence  ) ; 

/* ----------------------------------------------------------------------- */
/* char leer(df)                                                           */
/* ----------------------------------------------------------------------- */
/* Esta llamada al sistema lee el siguiente byte del fichero indicado por  */
/* el descriptor de fichero df.                                            */
/* ----------------------------------------------------------------------- */

char leer ( int df ) 
{
  char car ;
  int n = read(df, (pointer_t)&car, 1) ;
  if (n < 1) car = '\0' ;
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
                                          
int fcntl ( int df, word_t cmd, word_t arg  ) 
{
    asm 
	{
        mov bx,df ;
        mov cx,cmd ;
        mov dx,arg ;
        mov ax,FCNTL ;
        int nVIntSO1 ;
    }
    return(_AX) ;
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
/* 0204: destruirRecurso(nombre, matando)                                  */
/* 0205: encolarCcbRecurso(cb, nombre)                                     */
/* 0206: eliminarCcbRecurso(cb, nombre)                                    */
/* ======================================================================= */

void obtenInfoSO1 ( descSO1_t far * descSO1 ) 
{
    asm 
	{
        les bx,descSO1 ;
        mov ax,OBTENINFOSO1 ;
        int nVIntSO1 ;
    }
}

rindx_t crearRecurso ( descRecurso_t far * dR ) 
{
    asm 
	{
		les bx,dR ;
        mov ax,CREARRECURSO ;
        int nVIntSO1 ;
	}
    return(_AX) ;
}

dfs_t crearFichero ( const char far * nombre,
                     rindx_t          rindx,
                     word_t           menor,
                     tipoFichero_t    tipo ) 
{
    asm 
	{
        les bx,nombre ;
        mov cx,rindx ;
        mov dx,menor ;
        mov si,tipo ;
        mov ax,CREARFICHERO ;
        int nVIntSO1 ;
    }
    return(_AX) ;
}

int esperarDesinstalacion ( word_t tamDATA, ... ) 
{
    if (tamDATA != 0x0000) 
	{
        word_t finCodeDriver ;
		word_t finishDriver ;
        word_t tamPila ;
	    finCodeDriver = *((word_t far *)((pointer_t)&tamDATA + 1*sizeof(word_t))) ;
	    finishDriver  = *((word_t far *)((pointer_t)&tamDATA + 2*sizeof(word_t))) ;
	    tamPila       = *((word_t far *)((pointer_t)&tamDATA + 3*sizeof(word_t))) ;
        asm 
     	{
	        mov cx,finCodeDriver ;
     	    mov dx,finishDriver ;
    	    mov si,tamPila ;
	    }
	}
	asm 
	{
	    mov bx,tamDATA ;
        mov ax,ESPERARDESINSTALACION ;
        int nVIntSO1 ;
    }
    return(_AX) ;
}

int destruirRecurso ( char far * nombre, bool_t matando ) 
{
    asm {
        les bx,nombre ;
	    mov cx,matando ;
        mov ax,DESTRUIRRECURSO ;
        int nVIntSO1 ;
    }
	return(_AX) ;
}

int encolarCcbRecurso ( callBack_t cb, const char far * nombre ) 
{
    asm {
        les bx,nombre ;                                   /* ES:BX         */
        mov cx,es ;                                       /* CX:BX nombre  */
        les dx,cb ;                                       /* ES:DX cb      */
        mov ax,ENCOLARCCBRECURSO ;
        int nVIntSO1 ;
    }
    return(_AX) ;
}

int eliminarCcbRecurso ( callBack_t cb, const char far * nombre ) 
{
    asm {
        les bx,nombre ;                                   /* ES:BX         */
        mov cx,es ;                                       /* CX:BX nombre  */
        les dx,cb ;                                       /* ES:DX cb      */
        mov ax,ELIMINARCCBRECURSO ;
        int nVIntSO1 ;
    }
    return(_AX) ;
}

/* ======================================================================= */
/*  AX             obtenInfo y unidadLogicaLista (AH = 3)                  */
/* ======================================================================= */
/* 0300: obtenInfoPS(&descProceso,&e2FFR,&c2cPFR)                          */
/* 0301: obtenInfoMEM(&descProceso,&e2FFR,&c2cPFR,&listaLibres,&tamBlqMax) */
/* 0302: obtenInfoPFR(&descProc,&descFich,&descRec,&e2FFR,&c2cPFR)         */
/////* 0303: obtenInfoFAB(&df)                                                 */
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
                   c2c_t         far * c2cPFR ) 
{
    bloquePFR_t bPFR ;

    bloquePFR_t far * ptrPFR = (bloquePFR_t far *)&bPFR ;

    bPFR.descProceso = descProceso ;
    bPFR.e2PFR = e2PFR ;
    bPFR.c2cPFR = c2cPFR ;

    asm 
	{
        les bx,ptrPFR ;
        mov ax,OBTENINFOPS ;
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
                    word_t            * tamBlqMax ) 
{
    bloquePFR_t bPFR ;
    word_t segmento ;
    word_t tam ;

    bloquePFR_t far * ptrPFR = (bloquePFR_t far *)&bPFR ;

    bPFR.descProceso = descProceso ;
    bPFR.e2PFR = e2PFR ;
    bPFR.c2cPFR = c2cPFR ;

    asm 
	{
        les bx,ptrPFR ;
        mov ax,OBTENINFOMEM ;
        int nVIntSO1 ;

        mov segmento,ax ;
        mov tam,dx ;
    }
    *listaLibres = (ptrBloque_t)MK_FP(segmento, 0x0000) ;
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
                    c2c_t         far * c2cPFR ) 
{
    bloquePFR_t bPFR ;

    bloquePFR_t far * ptrPFR = (bloquePFR_t far *)&bPFR ;

    bPFR.descProceso = descProceso ;
    bPFR.descFichero = descFichero ;
    bPFR.descRecurso = descRecurso ;
    bPFR.e2PFR = e2PFR ;
    bPFR.c2cPFR = c2cPFR ;

    asm 
	{
        les bx,ptrPFR ;
        mov ax,OBTENINFOPFR ;
        int nVIntSO1 ;
    }
}

#if (FALSE)
/* ----------------------------------------------------------------------- */
/* obtenInfoFAB(&df) ;                                                     */
/* ----------------------------------------------------------------------- */
/* Esta llamada al sistema copia la tabla de ficheros abiertos en la       */
/* direccion indicada por df.                                              */
/* ----------------------------------------------------------------------- */

void obtenInfoFAB ( descriptor_de_fichero_t far * df )       /* ax = 0303h */
{
    asm 
	{
        les bx,df ;
        mov ax,OBTENINFOFAB ;
        int nVIntSO1 ;
    }
}
#endif

/* ----------------------------------------------------------------------- */
/* obtenInfoINFO(&info)                                                    */
/* ----------------------------------------------------------------------- */
/* Esta llamada al sistema copia informacion diversa del sistema a partir  */
/* de la direccion indicada por df.                                        */
/* ----------------------------------------------------------------------- */

void obtenInfoINFO ( info_t far * info )                     /* ax = 0304h */
{
    asm 
    {
        les bx,info ;
        mov ax,OBTENINFOINFO ;
        int nVIntSO1 ;
    }
}

int getdisk ( void ) 
{
    asm {
        mov ax,GETDISK ;
        int nVIntSO1 ;
    }
    return(_AX) ;                                          /* unidadLogica */
}

int procesarEntrada ( ffblk_t * ffblk )                /* funcion auxiliar */
{
    entrada_t far * entrada = (entrada_t far *)ffblk->buffer ;
    int i = ffblk->bindx ;
    int j, k ;
    ffblk->ff_attrib = entrada[i].atr ;
    ffblk->ff_ftime = entrada[i].hora ;
    ffblk->ff_fdate = entrada[i].fecha ;
    ffblk->ff_fsize = entrada[i].tam ;
    j = 0 ;
    while ((j < 8) && (entrada[i].nombre[j] != ' ')) 
	{
        ffblk->ff_name[j] = entrada[i].nombre[j] ;
        j++ ;
    }
    if (entrada[i].ext[0] != ' ') {
        ffblk->ff_name[j] = '.' ;
        j++ ;
    }
    k = 0 ;
    while ((k < 3) && (entrada[i].ext[k] != ' ')) 
	{
        ffblk->ff_name[j] = entrada[i].ext[k] ;
        k++ ;
        j++ ;
    }
    ffblk->ff_name[j] = '\0' ;
    /* entrada[i].primerCluster ; */
    return(0) ;
}

/* ----------------------------------------------------------------------- */
/* findFirst(unidadLogica, &ffblk)                                         */
/* ----------------------------------------------------------------------- */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\bios_0.h>

int findFirst ( byte_t unidadLogica, ffblk_t * ffblk ) 
{
    int res ;
    asm 
	{
        mov bl,unidadLogica ;
        mov dx,ffblk ;
        mov ax,FINDFIRST ;
        int nVIntSO1 ;                                 /* ffblk->bindx = 0 */
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

int findNext ( ffblk_t * ffblk ) 
{
    entrada_t far * entrada = (entrada_t far *)ffblk->buffer ;
    int res ;
    int i ;
    for ( i = ++ffblk->bindx ; i < 16 ; i++ )
        if ((entrada[i].nombre[0] != 0x00) && (entrada[i].nombre[0] != 0xE5))
            break ;
    ffblk->bindx = i ;
    if (ffblk->bindx < 16) 
	{
        procesarEntrada(ffblk) ;
        return(0) ;
    }
    else 
	{
        ffblk->eindx += 16 ;
        asm 
		{
            mov dx,ffblk ;
            mov ax,FINDNEXT ;
            int nVIntSO1 ;                             /* ffblk->bindx = 0 */
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

void activarTraza ( word_t numInstr, pid_t pid )             /* ax = 0700h */
{
    asm 
	{
        mov bx,numInstr ;
        mov dx,pid ;
        mov ax,ACTIVARTRAZA ;
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

void analizarProcesos ( bool_t Ok )                          /* ax = 0701h */
{
    asm {
        mov ax,Ok ;          /* pasamos a SO1 el parametro Ok en el flag Z */
        and ax,ax ;                                 /* activamos los flags */
        mov ax,ANALIZARPROCESOS ;
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

word_t ll_buscarBloque ( word_t tam ) 
{
	word_t segmento ;
    int df = open("GM", O_WRONLY) ;
    if (df < 0) return(0x0000) ;
	segmento = ioctl(df, 0, tam) ;                   /* 0 ==> buscarBloque */
	close(df) ;
    return(segmento) ; 	
}

bool_t ll_devolverBloque ( word_t segmento, word_t tam ) 
{
	bool_t res ;
    int df = open("GM", O_WRONLY) ;
    if (df < 0) return(FALSE) ;
	res = ioctl(df, 1, tam) ;                      /* 1 ==> devolverBloque */
	close(df) ;
    return(res) ; 	
}

word_t ll_tamBloqueMax ( void ) 
{
	word_t tamBloqueMax ;
    int df = open("GM", O_WRONLY) ;
    if (df < 0) return(0x0000) ;
	tamBloqueMax = ioctl(df, 2, 0) ;
    close(df) ;	
    return(tamBloqueMax) ;                           /* 2 ==> tamBloqueMax */
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

dword_t retardoActivo ( dword_t nVueltas ) 
{
    int df ;
	dword_t res = 0L ;                            /* resultado por defecto */
    retardarProceso_t retardarProceso ;
    switch (nVueltas) {
    case 0L :                                                        /* 0L */
        if ((df = open("RETARDO", O_RDONLY)) < 0) return(-1L) ;
        retardarProceso = (retardarProceso_t)1L ;              /* envio 1L */
        read(df, (pointer_t)&retardarProceso, 4) ;
        retardarProceso() ;                /* bucle de retardo establecido */
        break ;
    case 1L :                                                        /* 1L */
        if ((df = open("RETARDO", O_RDONLY)) < 0) return(-1L) ;
        res = 0L ;                                             /* envio 0L */
        read(df, (pointer_t)&res, 4) ;            /* res = nVueltasRetardo */ 
        break ;
    default :                                                     /* 2L .. */
        if ((df = open("RETARDO", O_WRONLY)) < 0) return(-1L) ;
        write(df, (pointer_t)&nVueltas, 4) ;
        break ;
    }
    close(df) ;
    return(res) ;
}