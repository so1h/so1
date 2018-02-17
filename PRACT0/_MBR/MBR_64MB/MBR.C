/* ----------------------------------------------------------------------- */
/*                                   mbr.c                                 */
/* ----------------------------------------------------------------------- */
/*                       Registro de Arranque Maestro                      */
/*   con una tabla de particiones conteniendo una particion activa FAT16   */
/* ----------------------------------------------------------------------- */

/*                              Version SMALL                              */

#include <so1pub.h\tipos.h>               /* byte_t, word_t, dword_t, retf */
#include <so1pub.h\asmproc.h>                  /* beginAsmProc, endAsmProc */
#include <_mbr\mbr_2\mbr.h>

partes_del_MBR ; /* (macro) debe ser la primera declaracion de datos (bss) */
                     /* direcciones del segmento de datos: 0x0000 a 0x01FF */

/* codigo [ 0x01B8 ] */                   /* debe ser el primer dato (BSS) */ /* 0x0000 */
/* WinDiskSig */                              /* debe contener 0x12345678L */ /* 0x01B8 */
/* relleno */                                      /* debe contener 0x0000 */ /* 0x01BC */
/* descParticion [ maxParticiones ] */                                        /* 0x01BE */
/* signatura */                                    /* debe contener 0xAA55 */ /* 0x01FE */

byte_t  activa ;                                              /*   1 bytes */ /* 0x0200 */
byte_t  cabezaInicial ;                                       /*   1 bytes */
//byte_t  sectorInicial ;                                     /*   1 bytes */
//byte_t  cilindroInicial ;                                   /*   1 bytes */
word_t  scInicial ;                   /* sector y cilindro */ /*   2 bytes */
byte_t  tipo ;                                                /*   1 bytes */
byte_t  cabezaFinal ;                                         /*   1 bytes */
byte_t  sectorFinal ;                                         /*   1 bytes */
byte_t  cilindroFinal ;                                       /*   1 bytes */
dword_t primerSector ;                                        /*   4 bytes */
dword_t sectores ;                                            /*   4 bytes */

byte_t unidadBIOS ;     /* el BIOS deja en DL en numero de la unidad de la */ /* 0x0210 */
                       /* que ha aceptado arrancar: 0x00, 0x01, 0x80, 0x81 */
                               /* (el registro DL no se modifica en MBR.C) */

#define saltarA( segmento, desplazamiento )                                  \
  asm {                                                                      \
    push segmento ;       /* apilamos el segmento                          */\
    push desplazamiento ; /* apilamos el desplazamiento                    */\
    retf ;                /* retorno lejano: desapila segmento       en CS */\
  } ;                     /*                 desapila desplazamiento en IP */

void main ( void ) ;             /* declaracion forward de la funcion main */

void startBin ( void ) {            /* punto de entrada del programa (MBR) */
  asm {
//  cli ;             /* inhibimos las interrupciones (ya estan inhibidas) */
    mov ax,dirLinealCargaMBR SHR 4 ;                          /* CS = 0000 */
    mov ds,ax ;                                               /* DS = 07C0 */
    mov ax,0x9000
    mov ss,ax ;                                               /* SS = 9000 */
    mov sp,ax ;              /* establecemos la pila SS:SP = 0x9000:0x9000 */
  }
  saltarA(ds, OFFSET main) ;      /* se cede el control a main (07C0:main) */
}

/* ------------------------- funciones auxiliares ------------------------ */

#define setVideoMode()                                                       \
  asm {                                                                      \
    mov ax, 0x0003 ;                 /* Modo de video normal (texto 80x25) */\
    int 0x10 ;                                                               \
  } ;

void printCar ( char car ) {  asm {
    mov al,car ;     /* car -> caracter a escribir                         */
    mov bl,0x07 ;    /* 0x07 -> atributo normal                            */
    mov ah,0x0e ;    /* 0x0e -> escribir caracter                          */
    int 0x10 ;       /* 0x10 -> servicios BIOS de manejo de la pantalla    */
  }
}

#define opcion 0
//#define opcion 1

#if (opcion == 0)                                              /* opcion 0 */

beginAsmProc(msg) {
  db 0ah, 0dh, 'MBR P.P.L.R. (C) 2016', 0ah, 0dh, 0 ;
} endAsmProc(msg) ;

beginAsmProc(msg_Load) {
  db 0ah, 0dh, 'Loading PBR ...', 0ah, 0dh, 0 ;
} endAsmProc(msg_Load) ;

beginAsmProc(msg_ReadError) {
//db 0ah, 0dh, 'PBR Read error', 0ah, 0dh, 0ah, 0dh, 0 ;
  db 0ah, 0dh, 'E', 0ah, /* 0dh, */ 0ah, 0dh, 0 ;
} endAsmProc(msg_ReadError) ;

beginAsmProc(msg_NoPartition) {
//db 0ah, 0dh, 'No active partition found', 0ah, 0dh, 0ah, 0dh, 0 ;
  db 0ah, 0dh, 'No A P Found', 0ah, /* 0dh, */ 0ah, 0dh, 0 ;
} endAsmProc(msg_NoPartition) ;

beginAsmProc(msg_PartitionError) {
//db 0ah, 0dh, 'Partition table error', 0ah, 0dh, 0ah, 0dh, 0 ;
  db 0ah, 0dh, 'PT Error', 0ah, /* 0dh, */ 0ah, 0dh, 0 ;
} endAsmProc(msg_PartitionError) ;

#else                                                          /* opcion 1 */

void msg ( void ) {
  asm db 0ah, 0dh, 'MBR P.P.L.R. (C) 2016', 0ah, 0dh, 0 ;
}

void msg_Load ( void ) {
  asm db 0ah, 0dh, 'Loading PBR ...', 0ah, 0dh, 0 ;
}

void msg_ReadError ( void ) {
//asm db 0ah, 0dh, 'PBR Read error', 0ah, 0dh, 0ah, 0dh, 0 ;
  asm db 0ah, 0dh, 'E', 0ah, /* 0dh, */ 0ah, 0dh, 0 ;
}

void msg_NoPartition ( void ) {
//asm db 0ah, 0dh, 'No active partition found', 0ah, 0dh, 0ah, 0dh, 0 ;
  asm db 0ah, 0dh, 'No A P Found', 0ah, /* 0dh, */ 0ah, 0dh, 0 ;
}

void msg_PartitionError ( void ) {
//asm db 0ah, 0dh, 'Partition table error', 0ah, 0dh, 0ah, 0dh, 0 ;
  asm db 0ah, 0dh, 'PT Error', 0ah, /* 0dh, */ 0ah, 0dh, 0 ;
}
#endif


char car ;                        /* consume menos bytes del MBR asi (BSS) */

void printStr ( char * str ) {
  while ((car = *str++) != (char)0) {
    if (car == '\n')                  /* '\n' caracter de avance de linea  */
      printCar('\r') ;                /* '\r' caracter de retorno de carro */
    printCar(car) ;
  }
}

#define printStrMacro(str) \
  asm \
  { \
    push offset _ ## str ; \
    call near ptr _printStr ; \
    pop cx ; \
  }

char leerTecla ( void ) {
//static char car ;               /* consume menos bytes del MBR asi (BSS) */
  asm {
    mov ah,0x00 ;        /* 0x00 -> leer siguiente tecla pulsada           */
    int 0x16 ;           /* 0x16 -> servicios BIOS de teclado              */
//  mov car,al ;         /* El caracter ascii se nos devuelve en AL        */
  }
//return(car) ;
  return(_AL) ;
}

#define reboot()                               /* reiniciamos el ordenador */\
  asm int 0x19 ;                                         /* BIOS bootstrap */

int resetControler ( void ) {
  asm {
    mov ah,0x00 ;       /* 0x00 -> reset del controlador                   */
    int 0x13 ;          /* BIOS: E/S de disco                              */
    jc resetError ;     /* el flag de acarreo C se activa en caso de error */
  }
  return(0) ;
resetError:
  return(1) ;
}

/* leerPrimerSector:                                                       */
/*     unidadBIOS es el numero de la unidad (0 = A:, 80 = C:)              */
/*     dir es la direccion del bufer de memoria                            */

//byte_t sector ;                     /* numero de sector fisico   (1..63) */
//byte_t cabeza ;                     /* numero de cabeza         (0..255) */
//byte_t cilindro ;                   /* numero de cilindro      (0..1023) */
                                                       /* FEDCBA9876543210 */
//word_t sc ;           /* sector y cilindro codificados: ccccccccCCssssss */

pointer_t dirCarga ;

#define leerPrimerSector( part, unidadBIOS, dir ) leerPrimerSectorF()

int leerPrimerSectorF ( void ) {

//cabeza   = cabezaInicial ;
//sector   = sectorInicial ;
//cilindro = cilindroInicial ;

//sc = *((word_t *)&sectorInicial) ;                    /* sector+cilindro */
                                                            /* codificados */
  asm {
    les bx,dirCarga ;              /* pone en ES:BX la direccion del bufer */
//  mov dl,unidadBIOS ;           /* numero de unidad indicado por el BIOS */
    mov dh,cabezaInicial ;                                       /* cabeza */
//  mov cx,sc ;                            /* codificacion sector/cilindro */
    mov cx,scInicial ;                     /* codificacion sector/cilindro */
    mov al,1 ;                                /* numero de sectores a leer */
    mov ah,0x02 ;                          /* 0x02 -> lectura de un sector */
    int 0x13 ;                                       /* BIOS: E/S de disco */
    jc errorAlLeer ;    /* el flag de acarreo C se activa en caso de error */
  }
  return(0) ;
errorAlLeer:
  return(1) ;
}

/* ------------------------- programa principal -------------------------- */

void reubicado ( void ) ;                                       /* forward */

byte_t * origen  ;

pointer_t destinoLejano ;

byte_t * origenDesc ;

byte_t * destinoDesc ;

int i, j ;       /* consume menos bytes del MBR como variable global (BSS) */
                                                /* que como variable local */

void main ( void ) {                                    /* CS = DS = 0x7C0 */

  origen  = (byte_t *)0x0000 ;                        /* 0x07C0:0 = 0x7C00 */
  destinoLejano = (pointer_t)MK_FP(_SS,0x0000) ;     /* 0x9000:0 = 0x90000 */

  for ( i = 0 ; i < tamMBR ; i++ )      /* reubicacion del mbr a 9000:0000 */
    *destinoLejano++ = *origen++ ;

  saltarA(ss, OFFSET reubicado) ;      /* Cedemos el control al sector de */
                               /* arranque reubicado en el segmento 0x0060 */
}

/* ---------- codigo que se ejecuta tras la reubicacion del MBR ---------- */

void reubicado ( void ) {                 /* CS = SS = 0x9000, DS = 0x7C00 */

  asm {
    push ss ;
    pop ds ;
  }                                               /* CS = DS = SS = 0x9000 */

  setVideoMode() ;

//printStr((char *)msg) ;
  printStrMacro(msg) ;
//leerTecla() ;

//asm mov unidadBIOS,dl ;                    /* guardamos dl en unidadBIOS */

  origenDesc = (byte_t *)&descParticion ;

  for ( i = 0 ; i < 4 ; i++ ) {

    destinoDesc = (byte_t *)&activa ;
    for ( j = 0 ; j < sizeof(descParticion_t) ; j++ )
      *destinoDesc++ = *origenDesc++ ;

    if (activa == 0x80) {
//    printStr((char *)msg_Load) ;
      printStrMacro(msg_Load) ;
//    leerTecla() ;
      resetControler() ;                 /* Hacer un reset del controlador */
//    leerPrimerSector(unidadBIOS, MK_FP(0x0000, 0x7C00)) ;    /* leer PBR */
      dirCarga = MK_FP(0x0000, 0x7C00) ;
      if (leerPrimerSector(i, _DL, dirCarga)) {
//      printStr((char *)msg_ReadError) ;
        printStrMacro(msg_ReadError) ;
        goto fin ;
      }
//    asm mov dl,unidadBIOS ;           /* pasamos la unidad al PBR en DL. */
                                        /* Se cede el control al sector de */
                                        /* arranque de la particion activa */
                                        /* cargado en 0000:7C00 = 0x7C00   */
//    asm xor ax,ax ;
//    asm mov ds,ax ;
//    asm mov ss,ax ;
//    asm mov sp,ax ;

      saltarA(ax, 0x7C00) ;
    }
    else if (activa != 0x00) {
//    printStr((char *)msg_PartitionError) ;
      printStrMacro(msg_PartitionError) ;
      goto fin ;
    }
  }

//printStr((char *)msg_NoPartition) ;   /* no hay ninguna particion activa */
  printStrMacro(msg_NoPartition) ;      /* no hay ninguna particion activa */

fin: ;
  leerTecla() ;
  reboot() ;

  asm org 0x01FF ;  /* para alinear a multiplo de 16 el tamanio del codigo */

}



