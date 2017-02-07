/* ----------------------------------------------------------------------- */
/*                                mboot32.c                                */
/* ----------------------------------------------------------------------- */
/*    toma el control desde grub y reubica el programa a la dir 6000:0000  */
/* ----------------------------------------------------------------------- */

/* AVISO: En este modulo no se declaran variables globales, ya que el      */
/* codigo producido va a insertarse directamente en el codigo de so1.bin.  */
/* Eso simplifica calcular el espacio que debe reservarse en mboot.c para  */
/* insertar en dentro de el mboot32,bin en la posicion AJMB.               */

/* definiciones como en so1pub.h\tipos.h */

typedef enum { FALSE, TRUE } bool_t ;               /* FALSE = 0, TRUE = 1 */
                                                     /* sizeof(bool_t) = 2 */
typedef unsigned char byte_t ;                            /* byte (8 bits) */
                                                     /* sizeof(byte_t) = 1 */
typedef unsigned short int word_t ;                   /* palabra (16 bits) */
                                                     /* sizeof(word_t) = 1 */
typedef unsigned long int dword_t ;             /* doble palabra (32 bits) */
                                                    /* sizeof(dword_t) = 1 */
/* definiciones como so1pub.h\def_proc.h */

typedef struct {                                /* ver start en AJUSTUSR.C */
  byte_t magicbyte[3] ;     /* deben ser los bytes 0x8C 0xC8 y 0xBB (o 8E) */
  word_t finCodigo ;                         /* fin del codigo del fichero */
  byte_t relleno1[22] ;
  word_t SP0 ;
  byte_t relleno2[5] ;                            /* debe ser el byte 0xC3 */
  word_t desplMain ;
  byte_t relleno3[2] ;                    /* deben ser los bytes 0x53 0xBA */
  word_t desplFinish ;
} cabecera_t ;                           /* cabecera de fichero ejecutable */

/* definiciones nuevas */

#include "..\so1pub.h\multiboot.h"

void startBin32 ( void ) {       /* para que cmain tenga un desplazamiento */
  asm db AJMB-5 dup(?) ;   /* respecto a startBin32 igual a multibootEntry */
}                                                     /* dentro de so1.bin */

/* cmain tiene respecto de startBin32 el mismo desplazamiento que la       */
/* función multibootEntry de mboot.c. Ese desplazamiento puede calcularse  */
/* del siguiente modo:                                                     */
/*                                                                         */
/*          AJMB = OFFSET mbheader + sizeof(multiboot_header_t)            */
/*                                                                         */
/* El valor de OFFSET mbheader puede verse en el fichero so1bin.map y el   */
/* valor sizeof(multiboot_header_t) es 0x30 = 48. El símbolo AJMB debe     */
/* tener ese valor cuando se compila mbott32.c con el bcc32.               */

void cmain ( unsigned long magic, unsigned long addr ) ;

void multibootEntry ( void ) {
  asm {
    pushd 0x00000000 ;                                     /* Reset EFLAGS */
    popf ;                                     /* interrupciones inhibidas */

    push ebx ; /* Push the pointer to the Multiboot information structure. */ /* addr */
    push eax ;                                    /* Push the magic value. */ /* magic */
    call cmain ;                       /* Now enter the C main function... */

@B: hlt ;      /* por si se retorna de cmain (cosa que no debiera ocurrir) */
    jmp @B ;
  }
}

void printStr ( byte_t * * ptr, char * str ) ;

void reubicado ( void ) ;

void cmain ( unsigned long magic, unsigned long addr ) {

  /* copia so1.bin (cargado en 0x00220000) a 0x00000600                    */
  /* establece una GDT (con segmento de codigo y datos como la ya usada)   */
  /* se salta a un nuevo segmento de código de 16 bits                     */
  /* pasa a modo real                                                      */
  /* cede el control a 0060:0000                                           */

//  multiboot_header_t * multiboot_header ;
//  multiboot_info_t * mbi = (multiboot_info_t *)addr ;
//  cabecera_t * ptrCab = (cabecera_t *)origen ;
  dword_t * origen = (dword_t *)startBin32 ;
  dword_t * destino = (dword_t *)0x00000600 ;
  dword_t tamFichero = 0x10000 ;                                  /* 64 KB */
//  dword_t tamFichero = multiboot_header->load_end_addr - multiboot_header->load_addr ;
  dword_t dirReubicada ;
  dword_t despl ;
  int i ;
  unsigned char unidadBIOS ;
  dword_t unidadBIOS_SI ;
  unidadBIOS = *((unsigned char *)(addr + 15)) ;     /* ver estandar mboot */
  unidadBIOS_SI = (dword_t)unidadBIOS ;
  asm mov esi,unidadBIOS_SI ;

//  byte_t * ptr = (byte_t *)0x000B8000 ;
//  printStr((byte_t * *)&ptr, "Hola") ;

//  multiboot_header =
//    (multiboot_header_t *)((dword_t)&multibootEntry - sizeof(multiboot_header_t)) ;
//  origen = (dword_t *)multiboot_header->load_addr ;

  /* copiar so1.bin de 0x00220000 a 0x00000600 */

  for ( i = 0 ; i < (int)(tamFichero/sizeof(dword_t)) ; i++ )
    destino[i] = origen[i] ;

//  printStr((byte_t * *)&ptr, "B") ;

  asm mov edx,esi ;                                       /* dl = unidadBIOS */
//  asm mov unidadBIOS_SI,esi ;
//  unidadBIOS = (unsigned char)unidadBIOS_SI ;
//  *((unsigned char *)(0x00000600 + 0x000000C8)) = unidadBIOS ; /*ajustes.c */
                                                    /* para informar a SO1 */
  despl = (dword_t)reubicado - (dword_t)origen ;
  dirReubicada = (dword_t)destino + despl ;
  asm jmp dirReubicada ;

}

void reubicado ( void ) {

//  byte_t * ptr = (byte_t *)(0x000B8000 + 16) ; printStr((byte_t **)&ptr, "F") ;

  asm {
    jmp @F0 ;
gdt: ;
    // Dummy descriptor 0x00.
    dw 0x0000 ; // Limit  15:00
    dw 0x0000 ; // Base   15:00
    db 0x00 ;   // Base   23:16
    db 0x00 ;   // Access byte (descriptor type)
    db 0x00 ;   // Limits 19:16, Flags
    db 0x00 ;   // Base   31:24

    // Code descriptor 0x08.
    dw 0xFFFF ; // Limit 15:0
    dw 0x0000 ; // Base  15:0
    db 0x00 ;   // Base  23:16
    db 0x9A ;   // Code, Present, Non-conforming, Exec/read (1,0,0,1,1010)
    db 0xCF ;   // G=1, D=1, 0, AVL=0, 1111=F: Limit/Length (1,1,0,0,1111)
    db 0x00 ;   // Base  31:24

    // Data descriptor 0x10.
    dw 0xFFFF ; // Limit 15:00
    dw 0x0000 ; // Base  15:00
    db 0x00 ;   // Base  23:16
    db 0x92 ;   // Data, Present, Writeable (1,0,0,1,0010)
    db 0xCF ;   // G=1, D=1, 0, AVL=0, 1111=F: Limit/Length (1,1,0,0,1111)
    db 0x00 ;   // Base  31:24

    // Code descriptor 0x18.
    dw 0xFFFF ; // Limit 15:0
    dw 0x0000 ; // Base  15:0
    db 0x00 ;   // Base  23:16
    db 0x9A ;   // Code, Present, Non-conforming, Exec/read (1,0,0,1,1010)
    db 0x00 ;   // G=0, D=0, 0, AVL=0, 1111=F: Limit/Length (1,1,0,0,0000)
    db 0x00 ;   // Base  31:24

    // Data descriptor 0x20.
    dw 0xFFFF ; // Limit 15:00
    dw 0x0000 ; // Base  15:00
    db 0x00 ;   // Base  23:16
    db 0x92 ;   // Data, Present, Writeable (1,0,0,1,0010)
    db 0x00 ;   // G=0, D=0, 0, AVL=0, 1111=F: Limit/Length (1,1,0,0,0000)
    db 0x00 ;   // Base  31:24

gdt_end:
gdtr: ;
    dw (gdt_end - gdt - 1) ;
    dd 0x0600 + OFFSET gdt - OFFSET startBin32 ;
idtr: ;
    dw 0x03ff ;
    dd 0x00000000 ;
@F0: ;
  }

  asm mov ebx,0x0600 + OFFSET gdtr - OFFSET startBin32 ;
  asm lgdt [bx] ;

  asm mov ebx,0x0600 + OFFSET idtr - OFFSET startBin32 ;
  asm lidt [bx]

  asm {
    db 0xEA ;                                       /* jmp far 0x0010:@FF0 */
    dd 0x0600 + OFFSET @F1 - OFFSET startBin32 ;
    dw 0x0008 ;
@F1: ;
  }

  asm mov ax,0x0020 ;                    /* selector del segmento de datos */

  asm mov ds,ax ;
  asm mov es,ax ;
  asm mov fs,ax ;
  asm mov gs,ax ;
  asm mov ss,ax ;

  asm {
    db 0xEA ;                                      /* jmp far 0x0018:@FFF0 */
    dd 0x0600 + OFFSET @F2 - OFFSET startBin32 ;
    dw 0x0018 ;
@F2: ;
  }

  asm {                                   /* abandonamos el modo protegido */
    mov eax,cr0 ;
    and eax, 0xFFFFFFFEL ;
    mov cr0,eax ;
  }

  asm {
    db 0xEA ;                                     /* jmp far 0x0060:0x0000 */
    dw 0x0000 ;
    dw 0x0060 ;
  }
}

/*

void printStr ( byte_t * * ptr, char * str ) {
  while (*str != (char)0) {
    *(*ptr) = *str++ ;
    *ptr += 2 ;
  }
}

*/