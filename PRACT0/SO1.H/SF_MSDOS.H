/* ----------------------------------------------------------------------- */
/*                                sf_msdos.h                               */
/* ----------------------------------------------------------------------- */
/*         recurso SF sencillo implementado haciendo uso de MSDOS          */
/* ----------------------------------------------------------------------- */

/* En el caso de este recurso/driver de sistema de ficheros, se cumple que */ 
/* todas las operaciones de ficheros open, read, write, ... se implementan */
/* haciendo uso de las llamadas al sistema de ficheros correspondientes a  */
/* MSDOS (int 21), suponiendo que MSDOS esta presente, como sucede cuando  */
/* SO1 se ejecuta sobre MSDOS, NTVDM, MSDOS Player o DOSBox (por tanto se  */
/* lanza el sistema ejecutando un fichero ejecutable de msdos SO1.EXE o    */
/* SO1.COM).                                                               */ 

#ifndef SF_MSDOS_H
#define SF_MSDOS_H

extern rindx_t rec_sf ;                     /* recurso sistema de ficheros */

extern dfs_t dfs_sf ;                       /* descriptor del fichero "SF" */

int inicSF_MSDOS (void ) ;       /* sistema de ficheros SF basado en MSDOS */

//int inicMinisfMSDOS ( void ) ;      

//int inicMinisfFAT ( void ) ;   /* sistema de ficheros propio de tipo FAT */

#endif /* SF_MSDOS_H */