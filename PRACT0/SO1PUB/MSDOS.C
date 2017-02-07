/* ----------------------------------------------------------------------- */
/*                                  msdos.c                                */
/* ----------------------------------------------------------------------- */
/*                                                                         */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\tipos.h>
#include <so1pub.h\msdos.h>
#include <so1pub.h\strings.h>

#define nVIntMSDOS 0x21     /* numero del vector del interrupcion de MSDOS */

void finProgDOS ( void ) {
  asm mov ah,4ch                    /* Llamada a MS-DOS: Terminar Programa */
  asm mov al,00h
  asm int 21h
}

#pragma warn +struct

bool_t hayMSDOS ( void ) {
  pointer_t far * ptrDos =
    (pointer_t far *)(nVIntMSDOS*4) ;
  return(**ptrDos != 0xCF) ;              /* 0xCF = codigo maquina de IRET */
}

word_t versionMSDOS ( void ) {
  word_t version ;
  asm mov ah,30h
  asm int 21h
  asm mov version,ax               /* al = numero mayor, ah = numero menor */
  return(version) ;
}

word_t segPSP ( void ) {
  word_t PSP ;
  asm mov ah,51h                                                    /* PSP */
  asm int nVIntMSDOS
  asm mov PSP,bx
  return(PSP) ;
}

pointer_t entornoMSDOS ( void ) {
  word_t segEntorno ;
  pointer_t ptrEntorno ;
  segEntorno = *((word_t far *)pointer(segPSP(), 0x002c)) ;
  ptrEntorno = pointer(segEntorno, 0x0000) ;
  return(ptrEntorno) ;
}

pointer_t valorMSDOS ( char * str ) {
  pointer_t ptrEntorno ;
  int i, j ;
  ptrEntorno = entornoMSDOS() ;
  i = 0 ;
  do {
    j = 0 ;
    while ((ptrEntorno[i] != (char)0) &&
           (ptrEntorno[i] != '=') &&
           (ptrEntorno[i] == str[j])) {
      i++ ;
      j++ ;
    }
    if ((ptrEntorno[i++] == '=') && (str[j] == (char)0))
      break ;
    else {
      while (ptrEntorno[i] != (char)0) i++ ;
    }
  } while (ptrEntorno[++i] != (char)0) ;

  return((pointer_t)&ptrEntorno[i]) ;

}

int argcMSDOS ( void ) {
  word_t tamCmdLine ;
  pointer_t ptrCmdLine ;
  int argc = 1 ;
  int i = 0 ;

  tamCmdLine = (word_t)*pointer(segPSP(), 0x0080) ;
  ptrCmdLine = pointer(segPSP(), 0x0081) ;

  while (i < tamCmdLine) {
    while ((i < tamCmdLine) && (ptrCmdLine[i] == ' ')) i++ ;
    if ((i < tamCmdLine) && (ptrCmdLine[i] != ' ')) argc++ ;
    while ((i < tamCmdLine) && (ptrCmdLine[i] != ' ')) i++ ;
  }
  return(argc) ;
}

bool_t argvMSDOS ( word_t n, char * str ) {

  word_t segEntorno ;
  pointer_t ptrEntorno ;
  word_t tamCmdLine ;
  pointer_t ptrCmdLine ;
  int i, j, k ;

  segEntorno = *((word_t far *)pointer(segPSP(), 0x002c)) ;
  ptrEntorno = pointer(segEntorno, 0x0000) ;
  tamCmdLine = (word_t)*pointer(segPSP(), 0x0080) ;
  ptrCmdLine = pointer(segPSP(), 0x0081) ;

  if (n == 1) {
    i = 0 ;
    do {
      while (ptrEntorno[i] != (char)0) i++ ;
    } while (ptrEntorno[++i] != (char)0) ;
    i++ ;
     i = i + 2 ;
    j = 0 ;
    while ((str[j++] = ptrEntorno[i++]) != (char)0) ;
    return(TRUE) ;
  }
  else {
    i = 0 ;
    j = 1 ;
    while ((i < tamCmdLine) && (j < n)) {
      while ((i < tamCmdLine) && (ptrCmdLine[i] == ' ')) i++ ;
      if ((i < tamCmdLine) && (ptrCmdLine[i] != ' ')) j++ ;
      if (j == n) {
        k = 0 ;
        while ((i < tamCmdLine) && (ptrCmdLine[i] != ' '))
          str[k++] = ptrCmdLine[i++] ;
         str[k] = (char)0 ;
        return(TRUE) ;
      }
    }
    return(FALSE) ;
  }
}

bool_t hayDOSBox ( void ) {
  if (!hayMSDOS()) return(FALSE) ;
  return (iguales((char far *)valorMSDOS("DOSBOX"), "DOSBOX")) ;
}

bool_t hayWindowsNT ( void ) {
  word_t version ;
  word_t menor, mayor ;
  if (!hayMSDOS()) return(FALSE) ;
  version = versionMSDOS() ;
  mayor = version & 0x00FF ;
  menor = version >> 8 ;
/*
  printStr(" mayor = ") ;
  printDec(mayor, 1) ;
  printStr(" menor = ") ;
  printDec(menor, 1) ;
  printStr(" %OS% = ") ;
  printStrFar(valorMSDOS("OS")) ;
*/
  if ((mayor == 7) && (menor == 10))
    return(FALSE) ;                                          /* Windows 98 */
  if ((mayor == 5) && (menor == 0)) {
    /* return(valorMSDOS("OS")[0] == 'W') ; */
    return (iguales((char far *)valorMSDOS("OS"), "Windows_NT")) ;
  }
  return(FALSE) ;
}

int openDOS ( pointer_t nombre, byte_t modo ) {
  int df ;
  asm push ds
  asm mov al,modo                                  /* 0 = R, 1 = W, 2 = RW */
  asm lds dx,nombre
  asm mov ah,3dh
  asm int nVIntMSDOS
  asm pop ds
  asm mov df,ax
  asm jc openFailed
  return(df) ;
openFailed:
  return(-1) ;
}

                            /* para evitar INT 24h Critical Error Handling */

int extendedOpenDOS ( pointer_t    nombre,                   /* MSDOS 4.0+ */
                      word_t       modo,               /* 0df00000ishr0r/w */
                      word_t       atr,                        /* 00advshr */
                      word_t far * accion,             /* 0x01, 0x11, 0x12 */
                      word_t far * error ) {
  int df ;
  word_t fci = *accion ;               /* fci = function control indicator */
  word_t res ;
  asm push ds
  asm mov bx,modo                                  /* 0 = R, 1 = W, 2 = RW */
  asm mov cx,atr
  asm mov dx,fci
  asm lds si,nombre
  asm mov al,00h
  asm mov ah,6ch
  asm int nVIntMSDOS
  asm pop ds
  asm mov res,ax
  asm mov fci,cx
  asm jc eopenFailed
  *accion = fci ;
  *error = 0 ;
  df = res ;
  return(df) ;
eopenFailed:
  *error = res ;
  return(-1) ;
}

int closeDOS ( int df ) {
  int err ;
  asm mov bx,df
  asm mov ah,3eh
  asm int nVIntMSDOS
  asm mov err,ax
  asm jc closeFailed
  return(err) ;
closeFailed:
  return(-1) ;
}

int commitFileDOS ( int df ) {
  int err ;
  asm mov bx,df
  asm mov ah,68h
  asm int nVIntMSDOS
  asm mov err,ax
  asm jc commitFailed
  return(err) ;
commitFailed:
  return(-1) ;
}

int createDOS ( pointer_t nombre, word_t atributo ) {    /* FA_RDONLY, ... */
  int df ;
  asm mov cx,atributo                                          /* 00AD0SHR */
  asm lds dx,nombre
  asm mov ah,3ch
  asm int nVIntMSDOS
  asm mov df,ax
  asm jc creatFailed
  return(df) ;
creatFailed:
  return(-1) ;
}

int readDOS ( int df, char * buf, int n ) {
  int leidos ;
  asm mov bx,df
  asm mov dx,buf
  asm mov cx,n
  asm mov ah,3fh
  asm int nVIntMSDOS
  asm mov leidos,ax
  asm jc readFailed
  return(leidos) ;
readFailed:
  return(-1) ;
}

int readDOSFar ( int df, char far * buf, int n ) {
  int leidos ;
  asm les bx,buf
  asm mov dx,bx
  asm mov bx,df
  asm mov cx,n
  asm mov ah,3fh
  asm push ds
  asm push es
  asm pop ds
  asm int nVIntMSDOS
  asm pop ds
  asm mov leidos,ax
  asm jc readFailed
  return(leidos) ;
readFailed:
  return(-1) ;
}

int writeDOS ( int df, char * buf, int n ) {
  int escritos ;
  asm mov bx,df
  asm mov dx,buf
  asm mov cx,n                  /* DOS 3.0+ n = 0 trunca (o rellena) hasta */
  asm mov ah,40h                 /* la posicion actual del fichero (lseek) */
  asm int nVIntMSDOS
  asm mov escritos,ax
  asm jc writeFailed
  return(escritos) ;
writeFailed:
  return(-1) ;
}

int writeDOSFar ( int df, char far * buf, int n ) {
  int escritos ;
  asm les bx,buf
  asm mov dx,bx
  asm mov bx,df
  asm mov cx,n
  asm mov ah,40h
  asm push ds
  asm push es
  asm pop ds
  asm int nVIntMSDOS
  asm pop ds
  asm mov escritos,ax
  asm jc writeFailed
  return(escritos) ;
writeFailed:
  return(-1) ;
}

int lseekDOS ( int df, dword_t * pos, int whence ) {
  word_t posL = *((word_t *)pos) ;
  word_t posH = (word_t)((*((dword_t *)pos)) >> 16) ;
  byte_t whenceByte = whence ;
  asm mov bx,df
  asm mov dx,posL
  asm mov cx,posH
  asm mov al,whenceByte
  asm mov ah,42h
  asm int nVIntMSDOS
  asm jc lseekFailed
  asm mov posL,ax
  asm mov posH,dx
  *pos = (((dword_t)posH) << 16) + (dword_t)posL ;
  return(0) ;
lseekFailed:
  return(-1) ;
}

int getdiskDOS ( void ) {
  unsigned char drive ;
  asm mov ah,19h
  asm int 21h
  asm mov drive,al
  return((int)drive) ;
}

int setdiskDOS ( int drive ) {
  unsigned char ndrives ;
  asm mov dx,drive
  asm mov ah,0eh
  asm int 21h
  asm mov ndrives,al
  return((int)ndrives) ;
}

int getcurdirDOS ( int drive, char * direc ) {

  char far * directorio = (char far *)direc ;

  drive++ ;
  asm push ds
  asm mov ah,47h
  asm mov dx,drive
  asm lds si,directorio
  asm int 21h
  asm pop ds
  asm jc getcurdirFailed
  return(0) ;
getcurdirFailed:
  return(-1) ;
}

int chdirDOS ( const char * path ) {

  char far * ptrFar = (char far *)path ;

  asm push ds
  asm mov ah,3bh
  asm lds dx,ptrFar
  asm int 021h
  asm pop ds
  asm jc chdirFailed
  return(0);
chdirFailed:
  return(-1) ;
}

int findfirstDOS ( const char * pathname,
                   struct ffblk * ffblk,
                   int attrib ) {

  char far * pathname1 = (char far *)pathname ;

  struct ffblk far * ffblk1 = (struct ffblk far *)ffblk ;

  asm push ds

  asm mov ah,2fh                                     /* get (and save) DTA */
  asm int 21h
  asm push es
  asm push bx

  asm mov ah,01Ah
  asm lds dx,ffblk1
  asm int 021h                            /* Set the disk transfer address */

  asm mov ah,04Eh
  asm mov cx,attrib
  asm lds dx,pathname1
  asm int 021h                                 /* Find first matching file */

  asm pushf                                    /* save state of carry flag */
  asm pop cx
  asm xchg ax,bx                                       /* save return code */

  asm mov ah,01Ah                                           /* restore DTA */
  asm pop dx
  asm pop ds
  asm int 21h

  asm push cx
  asm popf
  asm pop ds

  asm jc findfirstFailed
  return(0) ;
findfirstFailed:
  return(-1) ;
}

int findnextDOS ( struct ffblk * ffblk ) {

  struct ffblk far * ffblk1 = (struct ffblk far *) ffblk ;

  asm push ds

  asm mov ah,2fh                                     /* get (and save) DTA */
  asm int 21h
  asm push es
  asm push bx

  asm mov ah,01Ah
  asm lds dx,ffblk1
  asm int 021h                            /* Set the disk transfer address */

  asm mov ah,04Fh
  asm int 021h                                  /* Find next matching file */

  asm pushf                                    /* save state of carry flag */
  asm pop cx
  asm xchg ax,bx                                       /* save return code */

  asm mov ah,01Ah                                           /* restore DTA */
  asm pop dx
  asm pop ds
  asm int 21h

  asm push cx
  asm popf
  asm pop ds

  asm jc findnextFailed
  return(0) ;
findnextFailed:
  return(-1) ;
}

