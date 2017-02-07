/* ----------------------------------------------------------------------- */
/*                                  cmos.c                                 */
/* ----------------------------------------------------------------------- */
/*                                                                         */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\puertos.h>
#include <so1pub.h\cmos.h>

byte_t leerCmos ( byte_t reg ) {
  byte_t dato ;
  reg |= NMI_DISABLE_BIT ;
  asm pushf
  asm cli
  outportb(PORT_CMOS_INDEX, reg) ;
  dato = inportb(PORT_CMOS_DATA) ;
  asm popf
  return(dato) ;
}

void escribirCmos ( byte_t reg, byte_t valor ) {
  reg |= NMI_DISABLE_BIT ;
  asm pushf
  asm cli
  outportb(PORT_CMOS_INDEX, reg) ;
  outportb(PORT_CMOS_DATA, valor) ;
  asm popf
}

byte_t bcdToInt ( byte_t valorBCD ) {
  return((valorBCD >> 4)*10 + (valorBCD & 0x0F)) ;
}


