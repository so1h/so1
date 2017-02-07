/***************************************************************************/
/*                               fincode.h                                 */
/***************************************************************************/
/*    Funcion nula en el segmento de codigo para determinar algun punto    */
/*                 del codigo en el momento del enlazado                   */
/***************************************************************************/

/* Si en el enlazado se pone finCode.obj como ultimo fichero objeto con    */
/* codigo (instrucciones, funciones) el simbolo finCode toma como valor el */
/* numero de bytes de codigo generados, que irian desde 0 hasta finCode-1. */
/* El desplazamiento de comienzo del segmento de datos estaria en finCode  */
/* ajustandolo a la primera direccion par. (finCode + (finCode % 2)).      */
/* CS:((finCode + (finCode % 2)) seria la direccion del primer dato.       */
/* DS tomaria el valor CS + ((finCode + (finCode % 2)) / 16)               */
/* DS:((finCode + (finCode % 2)) % 16) seria la direccion del primer dato. */

#ifndef FINCODE_H
#define FINCODE_H

/* finCode:                                                                */
/*   Funcion en el segmento de codigo sin ningun codigo (y sin retorno).   */
/*   Apropiada para determinar un punto del codigo en el momento del       */
/*   enlazado.                                                             */

void finCode ( void ) ;

#endif /* FINCODE_H */