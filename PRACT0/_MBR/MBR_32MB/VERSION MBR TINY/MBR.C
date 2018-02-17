/* ----------------------------------------------------------------------- */
/*                                   mbr.c                                 */
/* ----------------------------------------------------------------------- */
/*                       Registro de Arranque Maestro                      */
/*   con una tabla de particiones conteniendo una particion activa FAT16   */
/* ----------------------------------------------------------------------- */

/*                              Version TINY                               */

#include <so1pub.h\tipos.h>               /* byte_t, word_t, dword_t, retf */
#include <so1pub.h\carsctrl.h>                              /* CR, LF, ESC */
//#include "mbr.h"
#include <_mbr\mbr.h>

#ifndef IDE
asm .model TINY                             /* modelo de programacion TINY */
#endif

#define MBRDEBUG FALSE                      /* desactiva cierta depuracion */
//#define MBRDEBUG TRUE                        /* activa cierta depuracion */

#if !(MBRDEBUG)
#define tamDATA 0xB2        /* tamanio del segmento de datos inicializados */
#else
#define tamDATA 0x8C        /* tamanio del segmento de datos inicializados */
#endif

/* IMPORTANTE: el valor concreto del segmento de datos de MBR.C hay que    */
/* tomarlo de MBR.MAP tras compilar una primera vez (poniendo en tamDATA   */
/* un valor cualquiera, por ejemplo 0). Tras conocer el valor correcto     */
/* presente en MBR.MAP hay que establecer ese valor en tamDATA y volver a  */
/* compilar. Mientras no se aniadan/eliminen/modifiquen variables globales */
/* no es necesario volver a calcular/actualizar el valor de tamDATA.       */

/* El codigo del MBR lo unico que tiene que hacer es tomar de DL el numero */
/* de la unidad BIOS de la que se esta arrancando, luego buscar la primera */
/* particion activa de la tabla de particiones, y tras hacer alguna        */
/* comprobacion, cargar el sector de arranque de la (primera) particion    */
/* activa (presente en el primer sector logico de la particion) en la      */
/* direccion 0000:7C00. Finalmente debe ceder el control al sector de      */
/* arranque cargado en esa direccion.                                      */

byte_t unidadBIOS = '?' ;  /* el BIOS deja en DL en numero de la unidad de */
                    /* la que ha aceptado arrancar: 0x00, 0x01, 0x80, 0x81 */

descParticion_t descParticion [ maxParticiones ] ;              /* forward */

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
    mov ax,segCargaMBR ;
    mov ds,ax ;                                               /* DS = 07C0 */
    mov ss,ax ;                                               /* SS = 07C0 */
    mov sp,0xFFFE ;          /* establecemos la pila SS:SP = 0x07C0:0xFFFE */
//  mov bp,sp ; /* innecesario ya que las funciones llamadas lo establecen */
  }
#ifndef IDE
  saltarA(ax, OFFSET main) ;      /* se cede el control a main (07C0:main) */
#else
  asm mov cx,OFFSET main ;
//saltarA(ax, cx) ;               /* se cede el control a main (07C0:main) */
  asm push ax ;
  asm push cx ;
  asm add ax,tamDATA ;                        /* modelo SMALL ajustamos DS */
  asm mov ds,ax
  asm retf ;
#endif
}

/* ------------------------- funciones auxiliares ------------------------ */

#define setVideoMode()                                                       \
  asm {                                                                      \
    mov ax, 0x0003 ;                 /* Modo de video normal (texto 80x25) */\
    int 0x10 ;                                                               \
  } ;

void printCar ( char car ) {
  asm {
    mov al,car ;     /* car -> caracter a escribir                         */
    mov bl,07H ;     /* 07H -> atributo normal                             */
    mov ah,0eH ;     /* 0eH -> escribir caracter                           */
    int 10H ;        /* 10H -> servicios BIOS de manejo de la pantalla     */
  }
}

void printStr ( char * str ) {
  static char car ;               /* consume menos bytes del MBR asi (BSS) */
  while ((car = *str++) != (char)0) {
    if (car == '\n')
      printCar(CR) ;
    printCar(car) ;
  }
}

char leerTecla ( void ) {
//static char car ;               /* consume menos bytes del MBR asi (BSS) */
  asm {
    mov ah,00H ;         /* 00H -> leer siguiente tecla pulsada            */
    int 16H ;            /* 16H -> servicios BIOS de teclado               */
//  mov car,al ;         /* El caracter ascii se nos devuelve en AL        */
  }
//return(car) ;
  return(_AL) ;
}

#define reboot()                               /* reiniciamos el ordenador */\
  asm int 0x19 ;                                         /* BIOS bootstrap */

int resetControler ( void ) {
  asm {
    mov ah,00H ;        /* 00H -> reset del controlador                    */
    int 13h ;           /* BIOS: E/S de disco                              */
    jc resetError ;     /* el flag de acarreo C se activa en caso de error */
  }
  return(0) ;
resetError:
  return(1) ;
}

/* leerSector: sect es el numero de sector logico                          */
/*             unidad es el numero de la unidad (0 = A:, 1 = B:)           */
/*             dir es la direccion del bufer de memoria                    */

int leerPrimerSector ( int part, int unidadBIOS, pointer_t dir ) {

//static byte_t sector ;              /* numero de sector fisico   (1..63) */
  static byte_t cabeza ;              /* numero de cabeza         (0..255) */
//static byte_t cilindro ;            /* numero de cilindro      (0..1023) */
                                                       /* FEDCBA9876543210 */
  static word_t sc ;    /* sector y cilindro codificados: ccccccccCCssssss */

  cabeza   = descParticion[part].cabezaInicial ;
//sector   = descParticion[part].sectorInicial ;
//cilindro = descParticion[part].cilindroInicial ;

  sc = *((word_t *)&descParticion[part].sectorInicial) ;/* sector+cilindro */
                                                            /* codificados */
  asm {
    les bx,dir ;                   /* pone en ES:BX la direccion del bufer */
    mov dl,unidadBIOS ;           /* numero de unidad indicado por el BIOS */
    mov dh,cabeza ;                                              /* cabeza */
    mov cx,sc ;                            /* codificacion sector/cilindro */
    mov al,1 ;                                /* numero de sectores a leer */
    mov ah,02h ;                            /* 02H -> lectura de un sector */
    int 13h ;                                        /* BIOS: E/S de disco */
    jc errorAlLeer ;    /* el flag de acarreo C se activa en caso de error */
  }
  return(0) ;
errorAlLeer:
  return(1) ;
}

/* ------------------------- programa principal -------------------------- */

void reubicado ( void ) ;

void main ( void ) {                         /* CS = DS = SS = segCargaMBR */

  static pointer_t origen  = (pointer_t)dirCargaMBR ; /* 0x07C0:0 = 0x7C00 */
  static pointer_t destino = (pointer_t)MK_FP(0x0060,0x0000) ;   /* 0x0600 */

  static int i ; /* consume menos bytes del MBR como variable global (BSS) */
                                                /* que como variable local */
  asm mov unidadBIOS,dl                      /* guardamos dl en unidadBIOS */

  for ( i = 0 ; i < tamMBR ; i++ )      /* reubicacion del mbr a 0060:0000 */
    *destino++ = *origen++ ;

#ifndef IDE
  saltarA(0x0060, OFFSET reubicado) ;   /* Cedemos el control al sector de */
                               /* arranque reubicado en el segmento 0x0060 */
#else
  asm mov cx,OFFSET reubicado ;
  saltarA(0x0060, cx) ;           /* se cede el control a main (07C0:main) */
#endif
}

/* ---------- codigo que se ejecuta tras la reubicacion del MBR ---------- */

#define mostrarInfo() {                      \
      msgDebug[2] += i ;                     \
      msgDebug[4] += (unidadBIOS >> 4) ;     \
      msgDebug[5] += (unidadBIOS & 0x01) ;   \
      msgDebug[7] += descParticion[i].tipo ; \
      printStr(msgDebug) ;                   \
      /* leerTecla() ; */                    \
} ;

void reubicado ( void ) {

#if !(MBRDEBUG)
  static char msg [ ] = "\nMBR P.P.L.R. (C) 2016\n" ;
#else
  static char msgDebug [ ] = "\np0u00t0\n" ;
#endif
  static char msg_Load [ ] = "\nLoading PBR ...\n" ;
  static char msg_NoPartition [ ] = "\nNo active partition found\n\n" ;
  static int i ; /* consume menos bytes del MBR como variable global (BSS) */
                                                /* que como variable local */
#if !(MBRDEBUG)
  static char msg_PartitionError [ ] = "\nPartition table error\n\n" ;
#endif

  setVideoMode() ;

#if !(MBRDEBUG)
  printStr((char *)msg) ;
#endif

  for ( i = 0 ; i < 4 ; i++ ) {
    if (descParticion[i].activa == 0x80) {
#if (MBRDEBUG)
      mostrarInfo() ;
#endif
      printStr((char *)msg_Load) ;
      resetControler() ;                 /* Hacer un reset del controlador */
      leerPrimerSector(i, unidadBIOS, MK_FP(0x0000, 0x7C00)) ; /* leer PBR */
      asm {
        mov ax,cs ;
        mov ds,ax ;
        mov dl,unidadBIOS ;             /* pasamos la unidad al PBR en DL. */
      }                                 /* Se cede el control al sector de */
      saltarA(0x0000, 0x7C00) ;         /* arranque de la particion activa */
    }                                   /* cargado en 0000:7C00 = 0x7C00   */
#if !(MBRDEBUG)
    else if (descParticion[i].activa != 0x00)
      printStr((char *)msg_PartitionError) ;
#endif
  }

  printStr((char *)msg_NoPartition) ;   /* no hay ninguna particion activa */
  leerTecla() ;
  reboot() ;

#ifndef IDE
  asm org tamMBR-(tamDATA+2) ;     /* alargamos la funcion hasta completar */
                                     /* los 512 bytes entre codigo y datos */
#else
  asm {                              /************ añadir bytes necesarios */
    db 0x00, 0x00, 0x00 ;
  }
#endif
}

dword_t WinDiskSig = 0x12345678L ;

word_t relleno1 = 0x0000 ;

/* Particion de casi 32 MB en pen drive con geometria:                     */
/* (C, H, S, B) = (7, 255, 63, 512)                                        */
/* Primera particion: 0                                                    */
/* Sector 0 ==> MBR                                                        */
/* Primer sector de la particion 0 ==> Primer sector de la pista 1         */
/* (C, H, S) = (0, 1, 1) primer sector logico = 0*255*63+1*63+(1-1) = 63   */
/* 32MB = 32*1024*1024 Bytes = 64*1024 sectores = 65536 sectores           */
/* Ultimo sector de la particion 0 ==> 65536-1 = 65535                     */
/* C = 65535/(255*63) = 65535/16065 = 4,                                   */
/*     resto = 65535-(4*16065) = 65535-64260 = 1275                        */
/* H = 1275/63 = 20                                                        */
/*     resto = 1275-20*63 = 1275-1260 = 15                                 */
/* S = 15+1 = 16                                                           */
/* (C, H, S) = (4, 20, 16)                                                 */
/* Numero de sectores en la particion = 65535-63+1 = 65473                 */

/*  0x80 => Activa, 0x00 no activa */              /* tabla de particiones */
/*  |     H, S, C iniciales de la particion           ==================== */
/*  |     |  |  |  0x04 tipo de la particion (FAT16), 0x00 p. vacia        */
/*  |     |  |  |  |      H,  S,  C finales de la particion                */
/*  |     |  |  |  |      |   |   |  63 primer sector logico de la p.      */
/*  |     |  |  |  |      |   |   |   |  65536 numero de sectores de la p. */

descParticion_t descParticion [ maxParticiones ] = {
  { 0x80, 1, 1, 0, 0x04, 20, 16,  4, 63, 65473L },            /*  16 bytes */
  { 0x00, 0, 0, 0, 0x00,  0,  0,  0,  0,     0L },            /*  16 bytes */
  { 0x00, 0, 0, 0, 0x00,  0,  0,  0,  0,     0L },            /*  16 bytes */
  { 0x00, 0, 0, 0, 0x00,  0,  0,  0,  0,     0L }             /*  16 bytes */
} ;                                                           /*  64 bytes */

byte_t signatura [ 2 ] = {
  0x55, 0xAA                 /* Deben ser los dos ultimos bytes del sector */
} ;

