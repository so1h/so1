/* ----------------------------------------------------------------------- */
/*                                  msdos.c                                */
/* ----------------------------------------------------------------------- */
/*                                                                         */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\tipos.h>                                   /* address_t */
#include <so1pub.h\strings.h>                                    /* strcmp */
#include <so1pub.h\msdos.h>

void finProgDOS ( int exitCode ) {
  signed char code = (signed char)exitCode ;
  asm {
      mov al,code ;
	  mov ah,4ch ;                  /* Llamada a MS-DOS: Terminar Programa */
      int 21h ;
  }
}

#pragma warn +struct

#if (FALSE)

void mostrarVIMSDOS ( void ) {

  address_t far * ptrVIDos = (address_t far *)(nVIntMSDOS*4) ;

  pointer_t VIDos = *((pointer_t far *)ptrVIDos) ;

  clrScrBIOS() ;

  printStrBIOS("\n ptrVIDos = ") ;
  printHexBIOS(ptrVIDos, 8) ;
  printLnBIOS() ;

  printStrBIOS("\n *ptrVIDos = ") ;
  printHexBIOS(ptrVIDos->segment, 4) ;
  printCarBIOS(':') ;
  printHexBIOS(ptrVIDos->offset, 4) ;
  printLnBIOS() ;

  printStrBIOS("\n VIDos    = ") ;
  printLHexBIOS(VIDos, 8) ;
  printLnBIOS() ;
  printStrBIOS("\n (((dword_t)VIDos) >> 16) = ") ;
  printLHexBIOS((((dword_t)VIDos) >> 16), 8) ;
  printLnBIOS() ;

  printStrBIOS("\n *VIDos   = ") ;
  printHexBIOS(*VIDos, 2) ;
  printLnBIOS() ;
  printLnBIOS() ;

}

#endif

word_t versionMSDOS ( void ) ;                                  /* forward */

bool_t hayMSDOS ( void ) {

  address_t far * ptrVIDos = (address_t far *)(nVIntMSDOS*4) ;

  pointer_t VIDos = *((pointer_t far *)ptrVIDos) ;

#if (0)
  mostrarVIMSDOS() ;
#endif

  if ((*VIDos) == 0xCF) {                 /* 0xCF = codigo maquina de IRET */
    /* el MSDOS Player de Takeda Toshiya tiene MSDOS pero sin embargo el   */
	/* vector de interrupción apunta a un IRET ya que la máquina virtual   */
	/* es la que captura el trap: int 21H. Por tanto para saber si hay     */
	/* MSDOS (emulado) vamos a llamar a versionMSDOS. Si versionMSDOS      */
    /* modifica AX eso significa que efectivamente hay MSDOS.              */	
	asm mov ax,0x0000 ;        /* versionMSDOS pone inicialmente AH a 0x30 */ 
	return(versionMSDOS() != 0x3000) ; 
  }

  if (((LINEAL(VIDos)) >> 16) == 0xF)     /* el vector apunta al BIOS      */
                                          /* direccion 0x000Fxxxx          */
    if (LINEAL(VIDos) > 0x000F14A0)
      return (FALSE) ;
                                 /* !!!! DOSBox tiene el vector 0x000F14A0 */
  return (TRUE) ;

}

word_t versionMSDOS ( void ) {
  word_t version ;
  asm {
	  mov ah,30h ;
      int 21h ;
      mov version,ax ;             /* al = numero mayor, ah = numero menor */
  }
  return(version) ;
}

word_t segPSP ( void ) {
  word_t PSP ;
  asm {
	  mov ah,51h ;                                                  /* PSP */
      int 21h ;
      mov PSP,bx ;
  }
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
  char car ;
  ptrEntorno = entornoMSDOS() ;
  i = 0 ;
  do {
    j = 0 ;
    car = ptrEntorno[i] ;
    while ((car != (char)0) &&
           (car != '=') &&
           (car == str[j])) {
      i++ ;
      j++ ;
      car = ptrEntorno[i] ;
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

bool_t hayDOSBox ( void ) {

  address_t far * ptrVIDos = (address_t far *)(nVIntMSDOS*4) ;

  pointer_t VIDos = *((pointer_t far *)ptrVIDos) ;

  if (LINEAL(VIDos) == 0x000F14A0)
      return (TRUE) ;            /* !!!! DOSBox tiene el vector 0x000F14A0 */
  if (!hayMSDOS()) return(FALSE) ;
  return (!strcmp((char far *)valorMSDOS("DOSBOX"), "DOSBOX")) ;

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

  if (n == 0) {
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
    i = 0 ;                           /* indice del caracter en ptrCmdLine */
    j = 0 ;                                 /* índice del argumento actual */
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
      else {
        i++ ;
        while ((i < tamCmdLine) && (ptrCmdLine[i] != ' ')) i++ ;
      }
    }
    return(FALSE) ;
  }
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
    return (!strcmp((char far *)valorMSDOS("OS"), "Windows_NT")) ;
  }
  return(FALSE) ;
}

int openDOS ( pointer_t nombre, byte_t modo ) {
  int df ;
  asm {
	  push ds ;
      mov al,modo ;                                /* 0 = R, 1 = W, 2 = RW */
      lds dx,nombre ;
      mov ah,3dh ;
      int 21h ;
      pop ds ;
      mov df,ax ;
      jc openFailed ;
  }
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
  asm {
	  push ds ;
      mov bx,modo ;                                /* 0 = R, 1 = W, 2 = RW */
      mov cx,atr ;
      mov dx,fci ;
      lds si,nombre
      mov al,00h ;
      mov ah,6ch ;
      int 21h ;
      pop ds ;
      mov res,ax ;
      mov fci,cx ;
      jc eopenFailed ;
  }
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
  asm {
	  mov bx,df ;
      mov ah,3eh ;
      int 21h ;
      mov err,ax ;
      jc closeFailed ;
  }
  return(err) ;
closeFailed:
  return(-1) ;
}

int commitFileDOS ( int df ) {
  int err ;
  asm {
	  mov bx,df ;
      mov ah,68h ;
      int 21h ;
      mov err,ax ;
      jc commitFailed ;
  }
  return(err) ;
commitFailed:
  return(-1) ;
}

int createDOS ( pointer_t nombre, word_t atributo ) {      /* FA_ARCH, ... */
  int df ;
  asm {
	  push ds ;
      mov cx,atributo ;                                        /* 00AD0SHR */
      lds dx,nombre ;
      mov ah,3ch ;
      int 21h ;
	  pop ds ;
      mov df,ax ;
      jc creatFailed ;
  }
  return(df) ;
creatFailed:
  return(-1) ;
}

int readDOS ( int df, char far * buf, int n ) {
  int leidos ;
  asm {
      mov bx,df ;
	  les dx,buf ;
      mov cx,n ;
      mov ah,3fh ;
      push ds ;
      push es ;
      pop ds ;
      int 21h ;
      pop ds ;
      mov leidos,ax ;
      jc readFailed ;
  }
  return(leidos) ;
readFailed:
  return(-1) ;
}

int writeDOS ( int df, char far * buf, int n ) {
  int escritos ;
  asm {
      mov bx,df ;
	  les dx,buf ;
      mov cx,n ;                /* DOS 3.0+ n = 0 trunca (o rellena) hasta */
      mov ah,40h ;               /* la posicion actual del fichero (lseek) */
      push ds ;
      push es ;
      pop ds ;
      int 21h ;
      pop ds ;
      mov escritos,ax ;
      jc writeFailed ;
  }
  return(escritos) ;
writeFailed:
  return(-1) ;
}

int lseekDOS ( int df, dword_t * pos, int whence ) {
  word_t posL = *((word_t *)pos) ;
  word_t posH = (word_t)((*((dword_t *)pos)) >> 16) ;
  byte_t whenceByte = whence ;
  asm {
	  mov bx,df ;
      mov dx,posL ;
      mov cx,posH ;
      mov al,whenceByte ;
      mov ah,42h ;
      int 21h ;
      jc lseekFailed ;
      mov posL,ax ;
      mov posH,dx ;
  }	  
  *pos = (((dword_t)posH) << 16) + (dword_t)posL ;
  return(0) ;
lseekFailed:
  return(-1) ;
}

int getdiskDOS ( void ) {
  unsigned char drive ;
  asm {
	  mov ah,19h ;
      int 21h ;
      mov drive,al ;
  }
  return((int)drive) ;
}

int setdiskDOS ( int drive ) {
  unsigned char ndrives ;
  asm {
	  mov dx,drive ;
      mov ah,0eh ;
      int 21h ;
      mov ndrives,al ;
  }
  return((int)ndrives) ;
}

int getcurdirDOS ( int drive, char far * directorio ) {
  drive++ ;
  asm {
	  push ds ;
      mov ah,47h ;
      mov dx,drive ;
      lds si,directorio ;
      int 21h ;
      pop ds ;
      jc getcurdirFailed ;
  }
  return(0) ;
getcurdirFailed:
  return(-1) ;
}

int chdirDOS ( const char far * path ) {
  asm {
	  push ds ;
      mov ah,3bh ;
      lds dx,path ;
      int 021h ;
      pop ds ;
      jc chdirFailed ;
  }
  return(0);
chdirFailed:
  return(-1) ;
}

int findfirstDOS ( const char far * pathname,
                   struct ffblk far * ffblk,
                   int attrib ) {
  asm {
	  push ds ;

      mov ah,2fh ;                                   /* get (and save) DTA */
      int 21h ;
      push es ;
      push bx ;

      mov ah,01Ah ;
      lds dx,ffblk ;
      int 021h ;                          /* Set the disk transfer address */

      mov ah,04Eh ;
      mov cx,attrib ;
      lds dx,pathname ;
      int 021h ;                               /* Find first matching file */

      pushf ;                                  /* save state of carry flag */
      pop cx ;
      xchg ax,bx ;                                     /* save return code */

      mov ah,01Ah ;                                         /* restore DTA */
      pop dx ;
      pop ds ;
      int 21h ;

      push cx ;
      popf ;
      pop ds ;

      jc findfirstFailed ;
  }
  return(0) ;
findfirstFailed:
  return(-1) ;
}

int findnextDOS ( struct ffblk far * ffblk ) {
  asm {
	  push ds ;

      mov ah,2fh ;                                   /* get (and save) DTA */
      int 21h ;
      push es ;
      push bx ;

      mov ah,01Ah ;
      lds dx,ffblk ;
      int 021h ;                          /* Set the disk transfer address */

      mov ah,04Fh ;
      int 021h ;                                /* Find next matching file */

      pushf ;                                  /* save state of carry flag */
      pop cx ;
      xchg ax,bx ;                                     /* save return code */

      mov ah,01Ah ;                                         /* restore DTA */
      pop dx ;
      pop ds ;
      int 21h ;

      push cx ;
      popf ;
      pop ds ;

      jc findnextFailed ;
  }
  return(0) ;
findnextFailed:
  return(-1) ;
}

