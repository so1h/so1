/* ----------------------------------------------------------------------- */
/*                                 mbot.c                                  */
/* ----------------------------------------------------------------------- */
/*   toma el control desde grub y reubica el programa a la dir 0060:0000   */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\fincode.h>                                   /* finCode */
#include <so1pub.h\findata.h>                                   /* finData */
#include <so1pub.h\finbss.h>                                     /* finBSS */
#include <so1pub.h\multiboot.h>

#define LOAD_ADDR 0x00220000

void relleno ( void ) {
//asm { nop ; nop ; }                                      /* para alinear */
  asm { nop ; }                                            /* para alinear */
}

#define MBOOT_ENTRY OFFSET mbheader + sizeof(multiboot_header_t) ;

//#define tamFichBin 40146          /* numero de bytes en el fichero so1.bin */
#define tamFichBin 40844          /* numero de bytes en el fichero so1.bin */

//#define numNops 15
#define numNops 12



void mbheader ( void ) {
  asm {

  extrn finBSS:NEAR ;

//db numNops dup (0x90) ; /* nop == 0x90 */ /* para alinear elegir numNops */
  db 0x90, 0x90, 0x90, 0x90  ;           /* nop == 0x90 */ /* para alinear */
  db 0x90, 0x90, 0x90, 0x90  ; 
  db 0x90, 0x90, 0x90, 0x90  ; 

header_addr:
  dd MULTIBOOT_HEADER_MAGIC ;                                     /* magic */
  dd MULTIBOOT_HEADER_FLAGS ;                                     /* flags */
  dd -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS) ;      /* checksum */

  dd LOAD_ADDR + OFFSET header_addr ;                       /* header_addr */
  dd LOAD_ADDR + OFFSET header_addr ;                       /* header_addr */

  dd LOAD_ADDR ; /* a gusto del consumidor */                 /* load_addr */
  dd LOAD_ADDR + tamFichBin ;                             /* load_end_addr */
//dd LOAD_ADDR + tamFichBin + OFFSET finBSS ;  /* (es mayor)  bss_end_addr */
  dd LOAD_ADDR + tamFichBin + 0x00010000 ;     /* (es mayor)  bss_end_addr */
  dd LOAD_ADDR + OFFSET multiboot_entry ;                    /* entry_addr */

  dd 0x00000000 ;                                             /* mode_type */
  dd 0x00000050 ; /* 80 */                                        /* width */
  dd 0x00000019 ; /* 25 */                                       /* height */
  dd 0x00000000 ;                                                 /* depth */
  }

  asm {      /* multibootEntry debe ser sobreescrito por codigo de 32 bits */
multiboot_entry:                          /* concretamente por mboot32.bin */

  db 'multiboot entry'
  db (0x1F7-196) dup (0x20) ;

  }
}

