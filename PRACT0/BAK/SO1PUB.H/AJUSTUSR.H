/* ----------------------------------------------------------------------- */
/*                                ajustusr.h                               */
/* ----------------------------------------------------------------------- */
/*    Ajuste de los registros de segmento y salto al programa principal    */
/* ----------------------------------------------------------------------- */

#ifndef AJUSTUSR_H
#define AJUSTUSR_H

#include <so1pub.h\fincode.h>                                   /* finCode */
#include <so1pub.h\startdata.h>                               /* startData */
#include <so1pub.h\findata.h>                                   /* finData */
#include <so1pub.h\finbss.h>                                    /* finBSS  */

#undef SEG   /* no queremos usar la macro SEG sino el operador SEG de TASM */
                       /* Turbo Assembler Version 5 Quick Reference pag 11 */

#define startUserCode     /* solo se ejecuta en so1.bin, so1.com y so1.exe */\
asm                             /* pero siempre es necesario como cabecera */\
{                                                                            \
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

#define valor_DS (_CS + ((cabecera_t far *)MK_FP(_CS, 0x0000))->segDatosSR) 
   
#define offSegDatosSR 3    
   
#define setraw_DS()                                                          \
asm                                                                          \
{                                                                            \
    mov ax,word ptr cs:[3] ;       /* 3 es el desplazamiento de segDatosSR */\
    mov dx,cs ;                                                              \
    add ax,dx ;                                                              \
    mov ds,ax ;                                                              \
}

#define set_DS()                                                             \
{                                                                            \
    asm { push ax ; push dx ; }                                              \
    setraw_DS() ;                                                            \
    asm { pop dx ; pop ax ; }                                                \
}   

#define save_DS0()                                                           \
asm                                                                          \
{                                                                            \
    push ds ;                                                                \
}

#define restore_DS0()                                                        \
asm                                                                          \
{                                                                            \
    pop ds ;                                                                 \
}
   
#define offset_SPInicial 0x001A

#define valor_SPInicial *((word_t far *)MK_FP(_CS, offset_SPInicial))

#define atexit( finish ) {                                                   \
  manejador_t far * ptrManejadorAtExit =                                     \
    (manejador_t far *)MK_FP(                                              \
       _DS,                                                                  \
       valor_SPInicial-2             /* ver crearProceso en SO1\PROCESOS.C */\
    ) ;                                                                      \
  *ptrManejadorAtExit = (manejador_t)finish ;                                \
}

#endif /* AJUSTUSR_H */