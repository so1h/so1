/* ----------------------------------------------------------------------- */
/*                                fatunit.h                                */
/* ----------------------------------------------------------------------- */
/*                                                                         */
/* ----------------------------------------------------------------------- */

#ifndef FATUNIT_H
#define FATUNIT_H

#include <so1pub.h\def_sf.h>  /* maxUL, maxUF, descUnidad(Fisica/Logica)_t */
#include <so1pub.h\tipos.h>

extern int unidadFisicaActual ;

extern int unidadLogicaActual ;

#if (TRUE)
#define tamFAT12 (9*512)                                         /* 4,5 KB */

#define nEntFAT (((9*512)/3)*2)          /* FAT12: 3 bytes cada 2 entradas */
                                                    /* 6*512 = 3K entradas */
#define nEntFATMax (2880-(1+2*9+(224/(512/16))) + 2)     /* 2880-1-18-14+2 */
                                                /* 2880-31 = 2749 entradas */
#define nEntrsPorSector (512/32)      /* entradas de directorio por sector */
#endif 

/* ------------------------------- FAT12/16 ------------------------------ */
/*            valores correspondientes a la unidad logica en uso           */
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

int inicFatUnit ( void ) ;                   /* asigna memoria para la FAT */

int cargaFAT12_Ok ( byte_t unidadLogica ) ;

int cargaFAT16_Ok ( byte_t unidadLogica ) ;

#endif /* FATUNIT_H */