/* ----------------------------------------------------------------------- */
/*                                ajustusr.h                               */
/* ----------------------------------------------------------------------- */
/*    Ajuste de los registros de segmento y salto al programa principal    */
/* ----------------------------------------------------------------------- */

#ifndef AJUSTUSR_H
#define AJUSTUSR_H

#include <so1pub.h\fincode.h>                                   /* finCode */
#include <so1pub.h\startdata.h>                               /* startSata */
#include <so1pub.h\findata.h>                                   /* finData */
#include <so1pub.h\finbss.h>                                    /* finBSS  */

#undef SEG   /* no queremos usar la macro SEG sino el operador SEG de TASM */
                       /* Turbo Assembler Version 5 Quick Reference pag 11 */

#define startUserCode     /* solo se ejecuta en so1.bin, so1.com y so1.exe */\
                                /* pero siempre es necesario como cabecera */\
  asm {                                                                      \
    mov ax,cs ;                                                              \
    mov bx,SEG(startData) ;  /* valor del segmento de datos (sin reubicar) */\
    mov cx,OFFSET(finData) ;                           /* tam. del la DATA */\
    mov dx,OFFSET(finBSS) ;                            /* tam. del BSS     */\
    mov dx,OFFSET(startData) ;          /* ultimos bytes de codigo tras DS */\
    add ax,bx ;        /* ax contiene ahora el valor del segmento de datos */\
    pushf ;                                                                  \
    pop bx ;                                                 /* bx = flags */\
    cli ;                                                                    \
    mov ds,ax ;                              /* segmento de datos: DS */     \
    mov es,ax ;                              /* segmento extra:    ES = DS */\
    mov ss,ax ;                              /* segmento de pila:  SS = DS */\
    mov sp,SPInicial ;       /* Inicializacion puntero de pila del proceso */\
    mov bp,sp ;                                                              \
    push bx ;                                     /* restauramos los flags */\
    popf ;                                                                   \
  }

#define offset_SPInicial 0x001A

#define valor_SPInicial *((word_t far *)pointer(_CS, offset_SPInicial))

#define atexit( finish ) {                                                   \
  manejador_t far * manejador =                                              \
    (manejador_t far *)pointer(                                              \
       _DS,                                                                  \
       valor_SPInicial-6                                    /* SPInicial-6 */\
    ) ;                                                                      \
  *manejador = finish ;                                                      \
}

#endif /* AJUSTES_H */

