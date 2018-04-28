/* ----------------------------------------------------------------------- */
/*                                  sf.h                                   */
/* ----------------------------------------------------------------------- */
/*                                                                         */
/* ----------------------------------------------------------------------- */

#ifndef SF_H
#define SF_H

#include <so1pub.h\def_sf.h>  /* maxUL, maxUF, descUnidad(Fisica/Logica)_t */
#include <so1pub.h\tipos.h>

extern int numFichAbiertos ;

extern descriptor_de_fichero_t tablaFichAbiertos [ maxFichAbiertos ] ;

int inicTablaFichAbiertos ( void ) ;

int unidadLogicaListaSo1 ( int unidadLogica ) ;

dword_t numSectores ( dword_t tam ) ;

int findFirstSo1 ( int unidadLogica, ffblk_t far * ffblk ) ;

int findNextSo1 ( ffblk_t far * ffblk ) ;

bool_t formatearNombre ( const char far * nombre, char * formateado ) ;

long tamFichero ( int df ) ;

int abrirFichero ( const char far * nombre, byte_t unidadLogica ) ;

int cerrarFichero ( int df )      ;

word_t tamProceso ( int df ) ;

int cargaFicheroAbierto_Ok ( int df, word_t segmento ) ;

int cargaFichero_Ok ( char * nombre, byte_t unidad, word_t segmento ) ;

#endif /* SF_H */