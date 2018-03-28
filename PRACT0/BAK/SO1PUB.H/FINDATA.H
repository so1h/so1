/***************************************************************************/
/*                               findata.c                                 */
/***************************************************************************/
/*    Funcion nula en el segmento de datos para determinar algun punto     */
/*           del segmento de datos en el momento del enlazado              */
/***************************************************************************/

/* Si en el enlazado se pone finCode.obj como ultimo fichero objeto con    */
/* codigo (instrucciones, funciones) y finData.obj como ultimo fichero     */
/* objeto con datos (variables inicializadas) el simbolo finData toma como */
/* valor el numero de bytes de datos inicializados generados, que irian    */
/* desde 0 hasta finData-1. El desplazamiento de comienzo del segmento BSS */
/* de datos no inicializados estaria en finData ajustandolo a la primera   */
/* direccion par.                                                          */
/* CS:(((finCode + (finCode % 2)) + (finData + (finData % 2))) seria la    */
/* direccion del primer dato no inicializado.                              */
/* DS tomaria el valor CS + ((finCode + (finCode % 2)) / 16)               */
/* DS:((finCode + ((finCode % 2)) % 16) + (finData + (finData % 2))) seria */
/* la direccion del primer dato.no inicializado.                           */

#ifndef FINDATA_H
#define FINDATA_H

/* finData:                                                                */
/*   Funcion en el segmento de datos sin ningun codigo (y sin retorno).    */
/*   Apropiada para determinar un punto del segmento de datos en el        */
/*   momento del enlazado.                                                 */

void finData ( void ) ;

#endif /* FINDATA_H */