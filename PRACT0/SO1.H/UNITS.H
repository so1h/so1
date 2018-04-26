/* ----------------------------------------------------------------------- */
/*                                 units.h                                 */
/* ----------------------------------------------------------------------- */
/*                                                                         */
/* ----------------------------------------------------------------------- */

#ifndef UNITS_H
#define UNITS_H

#include <so1pub.h\def_sf.h>  /* maxUL, maxUF, descUnidad(Fisica/Logica)_t */

/* ----------------- descriptores de unidades logicas -------------------- */

extern int unidadFisicaArranque ;

extern int unidadLogicaArranque ;

extern descUnidadLogica_t descUnidadLogica [ maxUL ] ;

extern descUnidadFisica_t descUnidadFisica [ maxUF ] ;

void inicUnits ( void ) ;            /* inicializa descUnidad con d_bloque */

/* comprobarUnits comprueba signaturas, MBRs, BPBs y tipos de particion    */
/* inicializando unidadFisicaArranque y unidadLogicaArranque               */    

int comprobarUnits ( byte_t unidadBIOS ) ;         

#endif /* UNITS_H */