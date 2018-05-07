/* ----------------------------------------------------------------------- */
/*                               sf_ftbios.h                               */
/* ----------------------------------------------------------------------- */
/* recurso SF sencillo FAT12/16 implementado haciendo uso del BIOS int 13h */
/* ----------------------------------------------------------------------- */

/* En el caso de este recurso/driver de sistema de ficheros tipo FAT12/16, */ 
/* todas las operaciones de ficheros open, read, write, ... se implementan */
/* haciendo uso de la int 13h ofrecida por el BIOS. Realmente puesto que   */
/* dispone ya del recurso DB de los dispositivos de bloques reconocidos    */
/* el BIOS el acceso a los discos se realizara a traves de los ficheros    */
/* especiales de dispositivo: FDA, FDB, HDA, HDA1, ... , HDB, HDB1, ...    */
/* que ofrece el recurso DB. Este driver es el unico que puede utilizarse  */
/* cuando SO1 arranca de forma nativa a traves de un MBR/PBR sobre un      */
/* ordenador fisico o sobre qemu, bochs, DOSBox, ... En algunos casos se   */
/* podria utilizar teniendo MSDOS por debajo. Es el caso de NTVDM o de     */
/* MSDOS accediendo al disquete/disco duro con el BIOS. No obstante en el  */
/* caso del MSDOS Player no es posible (de momento su uso).                */  

#ifndef SF_FATBIOS_H
#define SF_FATBIOS_H

#include <so1pub.h\tipos.h>

extern rindx_t rec_sf ;                     /* recurso sistema de ficheros */

extern dfs_t dfs_sf ;                       /* descriptor del fichero "SF" */

int inicSF_FATBIOS ( void ) ;  

void * SF ( void * arg ) ;            /* funcion ejecutada por el servidor */

extern pindx_t pindxSrvSF ;

typedef struct {
  bool_t libre ;                  /* descriptor de fichero libre u ocupado */
  entrada_t entrada ;              /* entrada del fichero en el directorio */
  byte_t unidadLogica ;     /* unidad logica donde se encuentra el fichero */
  int sectorLogico ;               /* sector logico ocupado por la entrada */
  int numEntrada ;                   /* numero de la entrada en ese sector */
} descriptor_de_fichero_t ;

typedef struct {
  char      ff_attrib ;
  unsigned  ff_ftime ;
  unsigned  ff_fdate ;
  long      ff_fsize ;
  char      ff_name [13] ;
  int       unidadLogica ;
  byte_t    bindx ;    /* indice (0..15) de la entrada actual en el buffer */
  word_t    eindx ; /* indice (0.. ) de la entrada actual en el directorio */
  pointer_t buffer ;  /* puntero a un bufer local del proceso de 512 bytes */
} ffblk_t ;

extern int numFichAbiertos ;

extern descriptor_de_fichero_t tablaFichAbiertos [ maxFichAbiertos ] ;

int inicTablaFichAbiertos ( void ) ;

bool_t formatearNombre ( const char far * nombre, char * formateado ) ;

long tamFichero ( int df ) ;

int abrirFichero ( const char far * nombre, byte_t unidadLogica ) ;

int cerrarFichero ( int df )      ;

word_t tamProceso ( int df ) ;

int cargaFicheroAbierto_Ok ( int df, word_t segmento ) ;

int cargaFichero_Ok ( char * nombre, byte_t unidad, word_t segmento ) ;


int findFirstSo1 ( int unidadLogica, ffblk_t far * ffblk ) ;

int findNextSo1 ( ffblk_t far * ffblk ) ;

#endif /* SF_FATBIOS_H */