/***************************************************************************/
/*                              startCode.h                                */
/***************************************************************************/
/*    Funcion nula en el segmento de codigo para determinar algun punto    */
/*                 del codigo en el momento del enlazado                   */
/***************************************************************************/

#ifndef STARTCODE_H
#define STARTCODE_H

/* startCode:                                                              */
/*   Funcion en el segmento de codigo sin ningun codigo (y sin retorno).   */
/*   Apropiada para determinar un punto del codigo en el momento del       */
/*   enlazado entrada a un fichero ejecutable                              */

void startCode ( void ) ;

#endif /* STARTCODE_H */