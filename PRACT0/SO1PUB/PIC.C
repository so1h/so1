/* ----------------------------------------------------------------------- */
/*                                   pic.c                                 */
/* ----------------------------------------------------------------------- */
/*                                                                         */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\puertos.h>
#include <so1pub.h\pic.h>

ptrTVI_t ptrTVI = (ptrTVI_t)0x00000000L ;   /* puntero a la TVI: 0000:0000 */

word_t valorIMR ( void ) {
  word_t res ;
  asm in al,PORT_PIC2_IMR
  asm mov ah,al
  asm in al,PORT_PIC1_IMR
  asm mov res,ax
  return(res) ;
}

void establecerIMR ( word_t nuevoIMR ) {
  asm mov ax,nuevoIMR
  asm out PORT_PIC1_IMR,al
  asm mov al,ah
  asm out PORT_PIC2_IMR,al
}

void mask_pic1 ( byte_t irq ) {
  outportb(PORT_PIC1_DATA, inportb(PORT_PIC1_DATA) | irq) ;
}

void mask_pic2 ( byte_t irq ) {
  outportb(PORT_PIC2_DATA, inportb(PORT_PIC2_DATA) | irq) ;
}

void unmask_pic1 ( byte_t irq ) {
  outportb(PORT_PIC1_DATA, inportb(PORT_PIC1_DATA) & ~irq) ;
}

void unmask_pic2 ( byte_t irq ) {
  outportb(PORT_PIC2_DATA, inportb(PORT_PIC2_DATA) & ~irq) ;
}

byte_t get_pic1_isr ( void ) {           /* 0x0B == select OCW1 + read ISR */
  outportb(PORT_PIC1_CMD, 0x0B) ;
  return(inportb(PORT_PIC1_CMD)) ;
}

byte_t get_pic2_isr ( void ) {           /* 0x0B == select OCW1 + read ISR */
  outportb(PORT_PIC2_CMD, 0x0B) ;
  return(inportb(PORT_PIC2_CMD)) ;
}

void set_pics ( byte_t irq0, byte_t irq8 ) {

                               /* Send ICW1 (select OCW1 + will send ICW4) */

    outportb(PORT_PIC1_CMD, 0x11) ;
    outportb(PORT_PIC2_CMD, 0x11) ;

     /* Send ICW2 (base irqs: 0x08-0x0f for irq0-7, 0x70-0x77 for irq8-15) */

    outportb(PORT_PIC1_DATA, irq0) ;
    outportb(PORT_PIC2_DATA, irq8) ;
                                           /* Send ICW3 (cascaded pic ids) */
    outportb(PORT_PIC1_DATA, 0x04) ;
    outportb(PORT_PIC2_DATA, 0x02) ;
                                           /* Send ICW4 (enable 8086 mode) */
    outportb(PORT_PIC1_DATA, 0x01) ;
    outportb(PORT_PIC2_DATA, 0x01) ;

                               /* Mask all irqs (except cascaded PIC2 irq) */

    outportb(PORT_PIC1_DATA, ~PIC1_IRQ2) ;
    outportb(PORT_PIC2_DATA, ~0) ;
}

void pic_setup ( void ) {
  set_pics(0x08, 0x70) ;
}

void enable_hwirq ( int hwirq, rti_t rti ) {
  int vector;
  if (hwirq < 8) {
    unmask_pic1(1 << hwirq);
    vector = 0x08 + hwirq;
  } else {
    unmask_pic2(1 << (hwirq - 8));
    vector = 0x70 + hwirq - 8;
  }
  ptrTVI[vector] = rti ;
}


