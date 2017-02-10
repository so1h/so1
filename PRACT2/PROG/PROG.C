
#define TRUE        1

#define CR   (char)13                                  /* Retorno de Carro */

typedef char far * pointer_t ;

#define MK_FP(s, d)                                    /* make far pointer */\
  (pointer_t)((((long)s) << 16) | ((long)d))

#ifndef TASM32                        /* TASM32 no esta definida en el IDE */
#define retf db 0xCB  /* necesario pues BASM no admite la instruccion retf */
#endif

/* Definiciones para acceder a la memoria de video en modo texto.          */

#define DirMemVideoColor MK_FP(0xB800, 0x0000)      /* direccion B800:0000 */

#define DirMemVideoMono  MK_FP(0xB000, 0x0000)      /* direccion B000:0000 */

typedef unsigned char byte_t ;

typedef struct {
  char car ;
  char atr ;
} pos_t ;

typedef struct {
  pos_t t [25][80] ;
} pantalla_t ;

void main ( void ) ;                                            /* forward */

void finCodigo ( void ) ;                                       /* forward */

void startBin ( void ) {    /* Inicializacion de los registros de segmento */

  asm mov ax,cs ;                                                            \
  asm mov bx,OFFSET(finCodigo)+3 ;  /* 3 bytes de finCodigo(): FINBSS + C3 */\
  asm mov dx,0001h ;                                                         \
  asm and dx,bx ;                                                            \
  asm add bx,dx ;                                /* alineamiento a palabra */\
  asm shr bx,4 ;                                                             \
  asm add ax,bx ;                                                            \
  asm mov ds,ax                                       /* de datos: DS */
  asm mov ss,ax                                       /* de pila:  SS = DS */
  asm mov es,ax                                       /* extra:    ES = DS */

  asm mov sp,8000h                   /* Inicializaci¢n del puntero de pila */
  asm mov bp,sp

  main() ;                               /* saltamos al programa principal */

}

pantalla_t far * ptrPant ;

void inicPantalla ( void ) {

  char far * ptr = (char far *)MK_FP(0x0000, 0x0449) ;     /* 0x00000449 ; */

  int enColor = (*ptr != (char)7) ;

  if (enColor)
    ptrPant = (pantalla_t far *)DirMemVideoColor ;
  else
    ptrPant = (pantalla_t far *)DirMemVideoMono ;

}

void pantallazo ( char car, char atr,
                  int lin0, int lin1,
                  int col0, int col1 ) {
  int l, c ;
  for ( l = lin0 ; l <= lin1 ; l++ ) {
    for ( c = col0 ; c <= col1 ; c++ ) {
      ptrPant->t[l][c].car = car ;
      ptrPant->t[l][c].atr = atr ;
    }
  }
}

char leerTecla ( void ) {
  char car ;
  asm mov ah,00h                      /* Llamada al BIOS: Leer del teclado */
  asm int 16h
  asm mov car,al                        /* al = caracter ascii introducido */
  return(car) ;
}

void printCar ( char car ) {
  asm mov al,car
  asm mov bh,07h
  asm mov ah,0eh        /* Llamada al BIOS: Escribir caracter por pantalla */
  asm int 10h
}

void finProgDOS ( void ) {
  asm mov ah,4ch                    /* Llamada a MS-DOS: Terminar Programa */
  asm mov al,00h
  asm int 21h
}

void reinicioBIOS ( void ) {
  asm int 19h
}

void callFarInicioROM ( void ) {
  asm {
    push 0F000h ;                   /* equivalente a call F000:FFF0 reboot */
    push 0FFF0h ;
    retf ;
  }
}

void printStr ( char * str ) {
  char car ;
  while ((car = *str++) != (char)0) {
    printCar(car) ;
    if (car == '\n')
      printCar(CR) ;
  }
}

void establecerModoDeVideo ( byte_t modo ) {
  asm mov ah,00h
  asm mov al,modo
  asm int 10h
}

typedef struct {
  byte_t modo ;
  byte_t ancho ;
  byte_t pagina ;
} infoVideo_t ;

byte_t modoDeVideo ( infoVideo_t * infoVideo ) {
  byte_t modo ;
  byte_t ancho ;
  byte_t pagina ;
  asm mov ah,0Fh
  asm int 10h
  asm mov modo,al
  asm mov ancho,ah
  asm mov pagina,bh
  if (infoVideo != (infoVideo_t *)0) {
    infoVideo->modo = modo ;
    infoVideo->ancho = ancho ;
    infoVideo->pagina = pagina ;
  }
  return(modo) ;
}

void mostrarModoDeVideo ( void ) {

  infoVideo_t infoVideo ;

  modoDeVideo(&infoVideo) ;

  printStr("\n") ;
  printStr("modo de video = ") ;
  printCar('0' + infoVideo.modo) ;
  printStr(" ancho = ") ;
  printCar('0' + infoVideo.ancho / 10) ;
  printCar('0' + infoVideo.ancho % 10) ;
  printStr(" pagina = ") ;
  printCar('0' + infoVideo.pagina) ;
  printStr("\n") ;

}

void main( void ) {

  char car ;

  mostrarModoDeVideo() ;

/*  establecerModoDeVideo(3) ; */
/*  mostrarModoDeVideo() ;     */

  inicPantalla() ;

  printStr("\n") ;
  printStr("SO1: Hola mundo.\n") ;
  printStr("\n") ;
  printStr("(T) - Terminar el programa (DOS ) \n") ;
  printStr("(R) - Reinicio con int 19h (BIOS) \n") ;
  printStr("(C) - call far a F000:FFF0 (ROM ) \n") ;

  pantallazo(' ', 0x3f, 19, 23, 41, 78) ;   /* (ciano << 4) + brigth white */
  while (TRUE) {
    car = leerTecla() ;
    pantallazo(car, 0x17, 20, 22, 42, 77) ;         /* (blue << 4) + white */
    switch (car) {
      case 't' :
      case 'T' : finProgDOS () ; break ;    /* peligro arrancando con dosbox */
      case 'r' :
      case 'R' : reinicioBIOS() ; break ;
      case 'c' :
      case 'C' : callFarInicioROM() ; break ;
      default : ;
    }
  }
}

void finCodigo ( void ) {          /* permite ajustar el segmento de datos */
}


