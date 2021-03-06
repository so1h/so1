/***************************************************************************/
/*                               pbrfat16.c                                */
/***************************************************************************/
/* Sector de arranque para una particion de disco duro con formato FAT16,  */
/* que comienza en el sector 63, con un unico sector reservado (el propio  */
/* sector de arranque), con 2 FATs que ocupan 254 sectores cada una, 512   */
/* entradas en el directorio raiz, con 512 bytes por sector y clusters     */
/* de un sector. Se realiza la carga (sin hacer ninguna comprobacion) del  */
/* fichero almacenado de forma contigua que comienza en el sector logico   */
/* numero 63 + 1 + 2*254 + (512/(512/32)) = 64 + 508 + 32 = 604,           */
/* presuponiendose que el fichero de carga se ha escrito en el disquete    */
/* ocupando bloques contiguos. El numero de bloques del fichero se toma de */
/* la primera entrada del directorio raiz. Tras la carga de ese fichero en */
/* la dirección 0x600 se cede inmediatamente el control a esa direccion    */
/* como direccion segmentada 0x0060:0x0000 (CS:IP).                        */
/***************************************************************************/
/* El BIOS carga del sector de arranque en CS:IP = 0000:7C00.              */
/* Se establece la pila en SS:SP = 9000:9000.                              */
/* Se establece CS = DS = 07C0 saltandose a main.                          */
/* Se reubica el sector en CS = DS = SS = 9000 saltandose a reubicado.     */
/* Se lee el primer sector del directorio raiz en 9000:0200.               */
/* Se obtiene el numero de sectores del primer fichero (entrada).          */
/* Se lee ese numero de primeros sectores de datos sobre 0060:0000.        */
/* Se salta a la direccion CS:IP = 0060:0000.                              */
/***************************************************************************/

/*                              Version TINY                               */

#include <so1pub.h\carsctrl.h>                              /* CR, LF, ESC */
#include <so1pub.h\tipos.h>                      /* byte_t, word_t, dword_t*/
                                                   /* pointer_t, address_t */
#include "_bpbfat16\bpbfat16.h"                          /* campos del BPB */

typedef struct {
  camposBPB(volatile)
} bpb_t ;

#define tamBS 512

typedef struct {                      /* estructura del sector de arranque */
  bpb_t bpb ;
  char codigo [ tamBS - sizeof(bpb_t) - 2 ] ;
  byte_t signatura [ 2 ] ;
} bootsector_t ;                                              /* 512 bytes */

#if (sizeof(bootsector_t) != tamBS)
#error el tipo bootsector_t debe ocupar exactamente 512 bytes (un sector)
#endif

#define tamData 0x7B     /* obtenido del fichero PBRFAT16.MAP tras enlazar */

#define CARGA7C00 TRUE               /* direccion de carga fijada a 0x7C00 */
//#define CARGA7C00 FALSE     /* direccion de carga detectada en ejecucion */
                                               /* al ejecutar BOOTPLUS.exe */
#if defined(JWASM) || defined(TASM32)
asm .model TINY                             /* modelo de programacion TINY */
#endif

typedef struct {                                  /* estructura (registro) */
  byte_t inf ;                            /* byte bajo:        bits  7.. 0 */
  word_t med ;                            /* word de en medio: bits 23.. 8 */
  byte_t sup ;                            /* byte superior:    bits 31..24 */
} reg_t ;

typedef union {                                                   /* union */
  dword_t  dword ;                           /* doble palabra: bits 31.. 0 */
  reg_t    reg ;
} tam_t ;

typedef struct {
  byte_t  nombre [8] ;                                         /*  8 bytes */
  byte_t  ext [3] ;                                            /*  3 bytes */
  byte_t  atr ;                                                /*  1 byte  */
  byte_t  reservado [10] ;                                     /* 10 bytes */
  word_t  horaModif ;                                          /*  2 bytes */
  word_t  fechaModif ;                                         /*  2 bytes */
  word_t  cluster ;                                            /*  2 bytes */
  tam_t   tam ;                                                /*  4 bytes */
} entrada_t ;                                           /* total: 32 bytes */

#define C_primerSectorDirRaiz \
  (C_primerSector + C_sectoresReservados \
     + (C_numeroDeFATs*C_sectoresPorFAT))          /* 63 + 1 + 2*254 = 572 */

#define C_tamEntrada sizeof(entrada_t)             /*             32 bytes */

#define C_entradasPorSector \
  (C_bytesPorSector/C_tamEntrada)                  /* 512/32 = 16 entradas */

#define C_sectoresDirRaiz \
  (C_entradasDirRaiz/C_entradasPorSector)          /* 512/16 = 32 entradas */

#define C_primerSectorDatos \
  (C_primerSectorDirRaiz + C_sectoresDirRaiz)            /* 572 + 32 = 604 */

void main ( void ) ;             /* declaracion forward de la funcion main */

void bootCode ( void ) {                  /* recibe el cotrol desde el BPB */
  asm {
//  cli ;        /* inhibimos las interrupciones (se suponen ya inhibidas) */
    mov sp,0x9000 ;              /* establecemos la pila SS:SP = 9000:9000 */
    mov ss,sp ;                                         /* por simplicidad */
//  mov bp,sp ;                                       /* SS:BP = 9000:9000 */
//  sti ;                   /* permitimos las interrupciones (innecesario) */

#if (CARGA7C00)
    mov ax,7c00h SHR 4 ;                                            /* 7C0 */
#else              /* funciona con carga del sector en cualquier paragrafo */
                         /* lo que es util para depurar BOOT_EXE con el TD */
             /* Se presupone IP alineado a paragrafo: IP & 0x000F = 0x0000 */
    call $+3 ;                         /* apilamos la dirección de retorno */
    pop bx ;                           /* BX = IP de pop bx                */
    sub bx,$-1 ;                       /* BX = IP donde se cargo el sector */
    shr bx,4 ;
    mov ax,cs ;
    add ax,bx ;            /* AX:0000 = direccion donde se cargo el sector */
#endif
    mov ds,ax ;
    push ax ;
#if defined(JWASM) || defined(TASM32)
    push OFFSET main ;                                /* retornamos a main */
#else
    mov ax,OFFSET main ;
    push ax ;
#endif
    retf ;                                 /* retorno lejano (a 07C0:main) */
  }                                             /* estableciendo CS a 07C0 */
}

void printCar ( char car ) {
  asm {
    mov al,car           /* car -> caracter a escribir                     */
    mov ah,0eH           /* 0eH -> escribir caracter                       */
    int 10H              /* 10H -> servicios BIOS de manejo de la pantalla */
  }
}

void printStr ( char * str ) {
  char car ;
  while ((car = *str++) != (char)0) {
    if (car == '\n') printCar('\r') ;
    printCar(car) ;
  }
}

char leerTecla ( void ) {
//char car ;
  asm {
    mov ah,00H                  /* 00H -> leer siguiente tecla pulsada     */
    int 16H                     /* 16H -> servicios BIOS de teclado        */
//  mov car,al                  /* El caracter ascii se nos devuelve en AL */
  }
//return(car) ;
  return(_AL) ;
}

#if (TRUE)
void rebootBIOS ( void ) {
  asm int 19h ;
}
#else
#define rebootBIOS( ) { \
  asm int 19h ; \
}
#endif

byte_t error = 0x00 ;

#if (FALSE)
byte_t resetController ( void ) {        /* innecesario, ya lo hace el MBR */
  asm {
    mov ah,00H          /* 00H -> reset del controlador                    */
    int 13h             /* BIOS: E/S de disco                              */
    jnc resetOk         /* el flag de acarreo C se activa en caso de error */
    mov error,ah
  }
resetOk:
  return(error) ;
}
#endif

typedef struct {
  word_t  tam ;                         /* tamaño del bloque de parametros */
  word_t  numSectores ;                       /* numero de sectores a leer */
  pointer_t dir ;                         /* bufer segmento:desplazamiento */
  dword_t sectorLogicoL ;             /* primeros 32 bits del sectorLogico */
  dword_t sectorLogicoH ;              /* ultimos 32 bits del sectorLogico */
} bloqueDeParametros_t ;

#define tamBDP sizeof(bloqueDeParametros_t)

bloqueDeParametros_t bloqueDeParametros = {
  tamBDP,                                                           /* tam */
  1,                                                        /* numSectores */
  (pointer_t)MK_FP(0x07C0, 0x0000),                                 /* dir */
  0x00000000,                                             /* sectorLogicoL */
  0x00000000                                              /* sectorLogicoH */
}  ;

/* leerSectorLBA: sector es el numero de sector logico                     */
/*                unidad es el numero de la unidad (0x00 = A:, 0x80 = C:)  */
/*                dir es la direccion del bufer de memoria                 */

int leerSectorLBA ( dword_t sectorLogico, byte_t unidad, pointer_t dir ) {

//word_t inicioBDP = (word_t)&bloqueDeParametros ;
//word_t finBDP = (word_t)&bloqueDeParametros + tamBDP ;

//bloqueDeParametros.tam = tamBDP ;       /* sizeof(bloqueDeParamnetros_t) */
//bloqueDeParametros.numSectores = 1 ;
  bloqueDeParametros.dir = dir ;
  bloqueDeParametros.sectorLogicoL = sectorLogico ;
//bloqueDeParametros.sectorLogicoH = 0x00000000L ;

  asm {
//  mov si,inicioBDP ;
    mov si,OFFSET bloqueDeParametros ;
//  mov di,finBDP ;
    mov di,si ;
    add di,16 ;
    mov dl,unidad ;
    mov al,1 ;
    mov ah,42h ;        /* 42H -> EXTENDED READ                            */
    int 13h ;           /* http://hdebruijn.soo.dto.tudelft.nl/newpage/interupt/out-0700.htm#0651 */
    jc errorAlLeer ;    /* el flag de acarreo C se activa en caso de error */
    mov error,ah
  }
errorAlLeer:
  return(error) ;
}

void incAddr ( address_t * p, word_t i ) {
  if (i > (0xFFFF - p->offset)) p->segment += 0x1000 ;
  p->offset += i ;
}

void reubicado ( void ) ;                                       /* forward */

pointer_t origen  = (pointer_t)MK_FP(0x07C0, 0x0000) ;        /*   CS:0000 */
pointer_t destino = (pointer_t)MK_FP(0x9000, 0x0000) ;        /* 9000:0000 */

void main ( void ) {

  int i ;       /* Declaracion de otras variables locales que se necesiten */

#if (!CARGA7C00)
  origen = (pointer_t)MK_FP(_CS, 0x0000) ;                    /*   CS:0000 */
#endif

  for ( i = 0 ; i < tamBS ; i++ )    /* reubicacion del sector de arranque */
//  *destino++ = *origen++ ;
    destino[i] = origen[i] ;

  asm {
    push 9000H                 /* Cedemos el control al sector de arranque */
                                                              /* reubicado */
#if defined(JWASM) || defined(TASM32)
    push OFFSET reubicado
#else
    mov ax,OFFSET reubicado
    push ax
#endif
    retf
  }

}

entrada_t far * entrada = (entrada_t far *)MK_FP(0x9000, 0x0200) ;

pointer_t ptr = (pointer_t)MK_FP(0x0060, 0x0000) ;   /* 0060:0000 = 00600H */

word_t sectorlogico = C_primerSectorDatos ;      /* primer sector de datos */

void reubicado ( void ) {

  word_t nsects ;
  byte_t unidadBIOS ;
  int i ;

  asm {                   /* establecemos el segmento de datos DS igual CS */
    mov ax,cs
    mov ds,ax
  }

  asm mov unidadBIOS,dl ;           /* el BIOS puso el numero de la unidad */
                                    /* de la que arranca en el registro DL */
  if (unidadBIOS != unidad) {
    printCar('E') ;
    leerTecla() ;
    rebootBIOS() ;
  }

//resetController() ;              /* se supone ya hecho por parte del MBR */

              /* Cargar el S.O. a partir de la dirección 0600H escribiendo */
              /* un punto tras la lectura de cada sector                   */

  leerSectorLBA(C_primerSectorDirRaiz, C_unidad, (pointer_t)entrada) ;
  nsects = (entrada[0].tam.reg.med/2)+1 ;

/* grldr (0.4.4)  funciona, aunque no detecta el fichero de configuracion  */
/*   menu.lst hasta que no se ejecuta el comando: root (fd0). (3 MB Ok).   */
/* grldr (0.4.5c) funciona si cuenta con al menos 35 megas de RAM.         */
/* grldr (0.4.6a) no funciona con BOOT_9 (usar bootlace).                  */

  printStr(
    "\n"
    "PBR P.P.L.R. (C) 2016\n"
    "\n"
  ) ;

  for ( i = 0 ; i < nsects ; i++ ) {
    leerSectorLBA(sectorlogico++, C_unidad, ptr) ;          /* unidad 0x80 */
    printCar('.') ;
    incAddr((address_t *)&ptr, C_bytesPorSector) ;
  }

  asm {
    mov dl,unidadBIOS ;  /* Transmitimos el numero de la unidad de aranque */
    push 0060H        /* Cedemos el control al S.O. en 0060:0000 = 000600H */
    push 0000H
    retf
  }

}

#if defined(JWASM) || defined(TASM32)
asm {
_TEXT segment byte public 'CODE'
  org 510 - tamData ;
  db 0x55, 0xAA ;                      /* signatura del sector de arranque */
_TEXT ends
}
#endif
