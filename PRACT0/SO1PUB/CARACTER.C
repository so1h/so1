/* ----------------------------------------------------------------------- */
/*                               caracter.c                                */
/* ----------------------------------------------------------------------- */
/*                                                                         */
/* ----------------------------------------------------------------------- */

char dig [17] = "0123456789ABCDEF" ;

char mayuscula ( char letra ) {
  if (('a' <= letra) && (letra <= 'z')) {
    /* letra = 'A' + (letra - 'a') ; */
    letra = (letra & 0xDF) ;                                  /* bit 5 a 0 */
  }
  else if (letra == '¤') letra = '¥' ;
  return(letra) ;
}

char minuscula ( char letra ) {
  if (('A' <= letra) && (letra <= 'Z')) {
    /* letra = 'a' + (letra - 'A') ; */
    letra = (letra | 0x20) ;                                  /* bit 5 a 1 */
  }
  else if (letra == '¥') letra = '¤' ;
  return(letra) ;
}

