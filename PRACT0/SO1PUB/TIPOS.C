/* ----------------------------------------------------------------------- */
/*                                 tipos.c                                 */
/* ----------------------------------------------------------------------- */
/*                  Definiciones y tipos de datos basicos                  */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\tipos.h>

pointer_t pointer ( word_t seg, word_t off ) {
  uPtrAdr_t uPtrAdr ;
  uPtrAdr.adr.segment = seg ;
  uPtrAdr.adr.offset = off ;
  return(uPtrAdr.ptr) ;
}

word_t seg ( pointer_t p ) {
  uPtrAdr_t uPtrAdr ;
  uPtrAdr.ptr = p ;
  return(uPtrAdr.adr.segment) ;
}

word_t off ( pointer_t p ) {
  uPtrAdr_t uPtrAdr ;
  uPtrAdr.ptr = p ;
  return(uPtrAdr.adr.offset) ;
}

void incPtr ( pointer_t far * p, word_t i ) {
  word_t despl ;
  despl = ((address_t *)p)->offset ;
  asm {
    mov ax,despl
    add ax,i
    jnc incOk
  }
  ((address_t *)p)->segment += 0x1000 ;
incOk: ;
  asm mov despl,ax
  ((address_t *)p)->offset = despl ;
}

/* otra implementacion de inc */
/*
void inc ( pointer_t * p, word_t i ) {
  if (i > (0xFFFF - ((address_t *)p)->offset))
    ((address_t *)p)->segment += 0x1000 ;
  ((address_t *)p)->offset += i ;
}
*/

