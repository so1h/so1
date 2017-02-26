/* ----------------------------------------------------------------------- */
/*                           c0fs.c (ajustes.c)                            */
/* ----------------------------------------------------------------------- */
/*    Ajuste de los registros de segmento y salto al programa principal    */
/* ----------------------------------------------------------------------- */

#include <so1.h\ajustes.h>
#include <so1pub.h\fincode.h>                                   /* finCode */
#include <so1pub.h\startcode.h>
#include <so1pub.h\startdata.h>                               /* startSata */
#include <so1pub.h\startbss.h>
#include <so1pub.h\findata.h>                                   /* finData */
#include <so1pub.h\finbss.h>                                     /* finBSS */
#include <so1pub.h\ajustusr.h>                            /* startUserCode */
#include <so1.h\ajustsp.h>                       /* SP0_SO1, SPInicial ... */
#include <so1.h\main.h>                                             /* so1 */

#include <so1pub.h\bios_0.h>

/* Este modulo, tras tomar el control salta a la función (externa) main    */

/* Puntos de entrada del programa:                                         */
/*                                                                         */
/*   startBin(): (2 casos)                                                 */
/*     el sector de arranque carga so1.bin en CS:0000 -> sistema operativo */
/*     desde DOS/Windows  se carga so1.com en CS:0100 -> sistema operativo */
/*          (so1.com se obtiene simplemente renombrando so1.bin)           */
/*                                                                         */
/*   startExe():                                                           */
/*     desde DOS/Windows  se carga so1.exe en CS:0000 -> sistema operativo */
/*                                                                         */
/*   main():                                                               */
/*     desde SO1 se carga so1.bin como un nuevo -----> programa de usuario */
/*                                                                         */
/* En todos los casos al final se cede el control a main(), de manera que  */
/* en main() hay que contemplar las cuatro posibilidades siguientes:       */
/*                                                                         */
/*    1) so1.bin (boot), 2) so1.com, 3) so1.exe y 4) so1.bin (comando).    */
/*                                                                         */
/* En el caso 2) (so1.com) es necesario modificar CS de forma que el nuevo */
/* CS sea tal que (nuevo) CS:0000 == (antiguo) CS:0100, ya que MSDOS carga */
/* el codigo de los ficheros COM a partir de CS:100. Para saber si se esta */
/* en el caso 2) hay que comprobar que al tomar el control:                */
/*                                                                         */
/*   CS = DS = ES = SS = PSP (con linea de comandos = so1.com)             */
/*   IP = 0x0100                                                           */
/*   SP = 0xFFFE (y se ha apilado 0x0000)                                  */
/*                                                                         */
/* En el caso 3) (so1.exe) debemos hacer que el linker no reubique las     */
/* direcciones indicadas en la cabecera del fichero exe (o parchear).      */
/* Se cumple:                                                              */
/*                                                                         */
/*   DS = ES = PSP (con linea de comandos = so1.exe)                       */
/*   CS, IP, SS y SP establecidos en la cabecera del programa (linker)     */
/*       (duda: ¿se apila una direccion completa en la pila o no?)         */
/*            ¿MSDOS apila CS:0000 en la carga de so1.exe?                 */
/*                                                                         */
/* En el caso 1) so1.bin (boot) la única suposicion es que se ha cedido el */
/* control al programa en CS:0000.                                         */

/* Se presupone IP alineado a paragrafo: IP & 0x000F = 0x0000              */

#define tomarIP( ) \
  asm { \
/*  call $+3 ; *//* apilamos IP correspondiente a la siguiente instruccion */\
    db 0xE8, 0x00, 0x00 ; /* call $+3 no aceptado en algunos ensambladores */\
    pop bx ;                           /* BX = IP de la instruccion pop bx */\
  }

#define tomarIPstartBin( ) \
  tomarIP() ; \
/*asm sub bx,$-1 ; */                               /* BX = IP en startBin */\
  asm db 0x81, 0x0EB, 0x0E, 0x00 ;                  /* BX = IP en startBin */

#define SPInicialProvisional 0x7FE

  /* macro para habilitar una pila provisional en una zona segura despues  */
  /* del codigo, los datos y el bss                                        */

#if (FALSE)  
  
#define habilitarPilaProvisional( ) \
  asm { \
    mov cx,0x0100 ;            /* por si hay cabecera EXE (max 4096 bytes) */\
    mov ax,finCode ; \
    mov bx,ax ; \
    and bx,0x0001 ; \
    add ax,bx ; \
    mov bx,finBSS ; \
    add ax,bx ; \
    jnc short @@ ;                                  /* suma menor de 64K ; */\
    mov cx,0x1100 ; \
@@: \
    shr ax,4 ; \
    add ax,cx ; \
    mov ss,ax ;  /* SS = ((finCode + (finCode % 2) + finBSS + 1000h) >> 4) */\
    mov sp,SPInicialProvisional ;        /* SS:SP pila inicial provisional */\
}

#endif

#define habilitarPilaProvisional( ) \
  asm { \
    mov ax,cs ; \
     add ax,0x3000 ;                 /* segmento 192 Ks (0x30000) posterior */\
     mov ss,ax ; \
     mov sp,SPInicialProvisional ;        /* SS:SP pila inicial provisional */\
}

void far startBin ( void ) {  /* se llega aqui como so1.bin o como so1.com */
  asm cli ;
  /* mediante las opciones del linker inicialmente SP = 0xFFFE.            */
  /* como esa pila podria dar algun problema establecemos provisionalmente */
  /* una pila valida en memoria mas alla del BSS del programa.             */
  habilitarPilaProvisional() ;
  tomarIPstartBin() ;                               /* BX = IP en startBin */
  asm {
//deducirModoSO1: ;                                 /* (so1.bin o so1.com) */
    mov cx,modoSO1_Bin ;       /* en principio SO1 se ejecuta como so1.bin */
    cmp bx,0x0000 ;                    /* ¿ IP == 0000 ? => so1.bin (boot) */
    jne short NoEs0 ;
    mov ax,cs ;
    jmp short ok ;
NoEs0:
    mov ax,0x000F ;
    and ax,bx ;
    jz short multiplo16 ;
    cli ;
    hlt ;  /* error fatal, so1 cargado en una direccion CS:XXXY con Y != 0 */
multiplo16:
    cmp bx,0x7C00 ;                                          /* 2015-06-21 */
    jge bootSLX ;                                            /* 2015-06-21 */
    mov cx,modoSO1_Com ;      /* efectivamente SO1 se ejecuta como so1.com */
    jmp normalizar:                                          /* 2015-06-21 */
bootSLX:                                                     /* 2015-06-21 */
    mov cx,modoSO1_Bs ;                                      /* 2015-06-21 */
normalizar:
    mov ax,cs ;                            /* normalizamos CS:IP (so1.com) */
    shr bx,4 ;
    add ax,bx ;
ok:
    mov bx,OFFSET startCommon ;                     /* jmp far startCommon */
    push ax ;                                                  /* apilamos */
    push bx ;                                                  /* apilamos */
  }
}                                            /* asm retf */ /* desapilamos */

void startExe ( void ) {                                      /* (so1.exe) */
  asm cli ;                                                   /* (=======) */
  /* mediante las opciones del linker inicialmente SP = 0xFFFE.            */
  /* como esa pila podria dar algun problema establecemos provisionalmente */
  /* una pila valida en memoria mas alla del BSS del programa.             */
  habilitarPilaProvisional() ;
  asm {
    mov cx,modoSO1_Exe ;
    mov bx,OFFSET startCommon ;                    /* jmp near startCommon */
    push bx ;                                                  /* apilamos */
  }
}                                                                   /* ret */

void startCommon ( void ) {
  asm {
#if defined(TASM32)
    mov word ptr cs:[OFFSET modoSO1 + 2],cx ;
    mov word ptr cs:[OFFSET desplCab + 2],OFFSET startCabecera ;
    mov byte ptr cs:[OFFSET unidadBIOS + 2],dl ;
#else
    db 2eh, 89h, 0eh ; dw OFFSET modoSO1 + 2 ;
    db 2eh, 0c7h, 06h ; dw OFFSET desplCab + 2, OFFSET startCabecera ;
    db 2eh, 88h, 16h ; dw OFFSET unidadBIOS + 2 ;
#endif
    mov bx,OFFSET startCabecera ;                /* jmp near startCabecera */
    push bx ;                                                  /* apilamos */
  }
}                                                                   /* ret */

void startCabecera ( void ) {
  startUserCode                       /* establece nueva pila DS:SPInicial */
  asm {
    mov bx,OFFSET main ;                                  /* jmp near main */
    push bx ;                                                  /* apilamos */
    mov dx,OFFSET finish ;   /* solo para que figure finish en la cabecera */
  }
}                                                                   /* ret */

word_t CS_SO1 ;                               /* segmento de codigo de SO1 */

word_t DS_SO1 ;                                /* segmento de datos de SO1 */

word_t BSS_SO1 ;                                    /* comienzo BSS de SO1 */

word_t IMRInicial ;            /* mascara de interrupcion inicial del 8259 */

void finish ( void ) {                          /* main debe retornar aqui */
  /* exit(0) ; */             /* esta función no se usa, pero por simetria */
}                                                            /* ver atexit */

void segDatos ( void ) {              /* vamos a guardar aqui el valor del */
  /* asm db 'DS' */
  asm dw 0x0000       /* aqui se guardara el segmento de datos (DS) de SO1 */
                          /* en BCC 5.02 no funciona ni db 'DS' ni ds 'DS' */
}                                         /* dentro del segmento de codigo */

#pragma warn -rvl           /* para evitar: Function should return a value */

modoSO1_t modoSO1 ( void ) {  /* modo en que se ejecuta SO1: bin, exe, ... */
  asm jmp short calculo1
  asm dw modoSO1_Cmd                                   /* valor de modoSO1 */
calculo1:
#if defined(TASM32)
  asm mov ax,word ptr cs:[OFFSET modoSO1 + 2]           /* return(modoSO1) */
#else
  asm db 2eh, 0a1h ; asm dw OFFSET modoSO1 + 2 ;
#endif
}

word_t desplCab ( void ) {  /* desplazam. de la cabecera (cab en startBin) */
  asm jmp short calculo2
  asm dw 0x0000                                       /* valor de desplCab */
calculo2:
#if defined(TASM32)
  asm mov ax,word ptr cs:[OFFSET desplCab + 2]         /* return(desplCab) */
#else
  asm db 2eh, 0a1h ; asm dw OFFSET desplCab + 2 ;
#endif
}

byte_t unidadBIOS ( void ) {       /* numero BIOS de la unidad de arranque */
  asm jmp short calculo3
  asm db 0xFF                                       /* valor de unidadBIOS */
calculo3:
#if defined(TASM32)
  asm mov ax,word ptr cs:[OFFSET unidadBIOS + 2]     /* return(unidadBIOS) */
#else
  asm db 2eh, 0a1h ; asm dw OFFSET unidadBIOS + 2 ;
#endif
}

#pragma warn +rvl

