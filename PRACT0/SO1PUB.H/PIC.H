/* ----------------------------------------------------------------------- */
/*                                   pic.h                                 */
/* ----------------------------------------------------------------------- */
/*             Controlador de Interrupciones Programable 8259A             */
/* ----------------------------------------------------------------------- */

#ifndef PIC_H
#define PIC_H

#include <so1pub.h\puertos.h>
#include <so1pub.h\tipos.h>

#define disable() asm cli ;

#define enable() asm sti ;

#define PIC1_IRQ0  (1 << 0)                /* PORT_PIC1 definicion de bits */
#define PIC1_IRQ1  (1 << 1)
#define PIC1_IRQ2  (1 << 2)
#define PIC1_IRQ5  (1 << 5)
#define PIC1_IRQ6  (1 << 6)

#define PIC2_IRQ8  (1 << 0)                /* PORT_PIC2 definicion de bits */
#define PIC2_IRQ12 (1 << 4)
#define PIC2_IRQ13 (1 << 5)
#define PIC2_IRQ14 (1 << 6)

#define IRQ_TIMER     0                                        /* INT 0x08 */
#define IRQ_TECLADO   1                                        /* INT 0x09 */
#define IRQ_PIC2      2                                        /* INT 0x0A */
#define IRQ_COM2_4    3                                        /* INT 0x0B */

#define IRQ_COM1_3    4                                        /* INT 0x0C */
#define IRQ_LPT2      5                                        /* INT 0x0D */
#define IRQ_DISQUETE  6                                        /* INT 0x0E */
#define IRQ_LPT1      7                                        /* INT 0x0F */

#define IRQ_RTC       8                                        /* INT 0x70 */
#define IRQ_REDIR     9                                        /* INT 0x71 */

#define IRQ_RATON    12                                        /* INT 0x74 */
#define IRQ_8087     13                                        /* INT 0x75 */
#define IRQ_DISCO    14                                        /* INT 0x76 */
#define IRQ_RESERVED 15                                        /* INT 0x77 */


extern ptrTVI_t ptrTVI ;                    /* puntero a la TVI: 0000:0000 */

word_t valorIMR ( void ) ;

void establecerIMR ( word_t nuevoIMR ) ;

                                           /* fin de interrupcion generico */

#define eoi_pic1()                                                /* macro */\
  asm mov al,PIC_EOI ;                                                       \
  asm out PORT_PIC1_OCW23,al ;                                               \
                                                              /* fin macro */

#define eoi_pic2()                                                /* macro */\
  asm mov al,PIC_EOI ;                                                       \
  asm out PORT_PIC2_OCW23,al ;                                               \
  eoi_pic1() ;                                                               \
                                                              /* fin macro */
void mask_pic1 ( byte_t irq ) ;

void mask_pic2 ( byte_t irq ) ;

void unmask_pic1 ( byte_t irq ) ;

void unmask_pic2 ( byte_t irq ) ;

byte_t get_pic1_isr ( void ) ;

byte_t get_pic2_isr ( void ) ;

void set_pics ( byte_t irq0, byte_t irq8 ) ;

void pic_setup ( void ) ;

void enable_hwirq ( int hwirq, rti_t rti ) ;

#endif /* PIC_H */

