/***************************************************************************/
/*                                finbss.h                                 */
/***************************************************************************/
/*    Funcion nula en el segmento BSS para determinar algun punto de       */
/*                ese segmento en el momento del enlazado                  */
/***************************************************************************/

/* Si en el enlazado se pone finCode.obj como ultimo fichero objeto con    */
/* codigo (instrucciones, funciones), finData.obj como ultimo fichero      */
/* objeto con datos (variables inicializadas) y finBSS como ultimo fichero */
/* con datos no inicializados, el simbolo finBSS toma como valor el numero */
/* de bytes de datos no inicializados generados, que irian desde 0 hasta   */
/* finBSS-1. El desplazamiento de comienzo del segmento BSS de datos no    */
/* inicializados estaria en finData ajustandolo a la primera direccion par.*/
/* CS:(((finCode + (finCode % 2)) + (finData + (finData % 2))) seria la    */
/* direccion del primer dato no inicializado.                              */
/* DS tomaria el valor CS + ((finCode + (finCode % 2)) / 16)               */
/* DS:((finCode + ((finCode % 2)) % 16) + (finData + (finData % 2))) seria */
/* la direccion del primer dato.no inicializado.                           */

#ifndef FINBSS_H
#define FINBSS_H

/* finBSS:                                                                 */
/*   Funcion en el segmento BSS sin ningun codigo (y sin retorno).         */
/*   Apropiada para determinar un punto del BSS en el momento del          */
/*   enlazado.                                                             */

void finBSS ( void ) ;

#endif /* FINBSS_H */