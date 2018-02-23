/* ----------------------------------------------------------------------- */
/*                                  sf.h                                   */
/* ----------------------------------------------------------------------- */
/*                                                                         */
/* ----------------------------------------------------------------------- */

#ifndef SF_H
#define SF_H

#include <so1pub.h\def_sf.h>
#include <so1pub.h\tipos.h>

/* ----------------- descriptores de unidades logicas -------------------- */

extern int unidadFisicaActual ;

extern int unidadLogicaActual ;

extern descUnidadLogica_t descUnidadLogica [maxUL] ;

extern descUnidadFisica_t descUnidadFisica [maxUF] ;

#if (TRUE)
#define tamFAT12 (9*512)                                         /* 4,5 KB */

#define nEntFAT (((9*512)/3)*2)          /* FAT12: 3 bytes cada 2 entradas */
                                                    /* 6*512 = 3K entradas */
#define nEntFATMax (2880-(1+2*9+(224/(512/16))) + 2)     /* 2880-1-18-14+2 */
                                                /* 2880-31 = 2749 entradas */
#define nEntrsPorSector (512/32)      /* entradas de directorio por sector */
#endif

/* ------------------------------- FAT12/16 ------------------------------ */
/*            valores correspondientes a la unidad logica actual           */
/* ----------------------------------------------------------------------- */

extern word_t sectoresPorPista ;
extern word_t cabezas ;

extern word_t sectoresPorCluster ;
extern word_t bytesPorSector ;
extern word_t bytesPorCluster ;
extern word_t entradasPorSector ;
extern word_t entradasPorCluster ;
extern word_t entradasDirRaiz ;
extern word_t sectoresDirRaiz ;

extern dword_t primerSector ;
extern dword_t primerSectorFAT ;
extern dword_t primerSectorDirRaiz ;
extern dword_t primerSectorDatos ;

extern dword_t tamFAT ;
extern dword_t entradasFAT ;
extern dword_t clusteres ;
extern word_t far * FAT ;        /* representa la tabla FAT desempaquetada */
                 /* se inicializa en inicSF apuntando a un segmento lejano */
                  /* con el fin de no gastar memoria del segmento de datos */

/* ----------------------------------------------------------------------- */

int inicSF ( byte_t unidadBIOS ) ;           /* asigna memoria para la FAT */

extern int numFichAbiertos ;

//extern word_t segBuferSO1 ;        /* bufer para leer sectores de disquete */

//extern pointer_t ptrBuferSO1 ;

int leerSectorSO1 ( dword_t sectorLogico, byte_t unidadLogica, pointer_t dir ) ;

extern descriptor_de_fichero_t tablaFichAbiertos [ maxFichAbiertos ] ;

int inicTablaFichAbiertos ( void ) ;

int unidadLogicaListaSo1 ( int unidadLogica ) ;

int cargaFAT12_Ok ( byte_t unidadLogica ) ;

int cargaFAT16_Ok ( byte_t unidadLogica ) ;

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


