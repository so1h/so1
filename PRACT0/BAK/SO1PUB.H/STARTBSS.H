/***************************************************************************/
/*                               startbss.h                                */
/***************************************************************************/
/*    Funcion nula en el segmento BSS para determinar algun punto de       */
/*                ese segmento en el momento del enlazado                  */
/***************************************************************************/

#ifndef STARTBSS_H
#define STARTBSS_H

/* startBSS:                                                               */
/*   Funcion en el segmento BSS sin ningun codigo (y sin retorno).         */
/*   Apropiada para determinar un punto del BSS en el momento del          */
/*   enlazado.                                                             */

void startBSS ( void ) ;

#endif /* STARTBSS_H */