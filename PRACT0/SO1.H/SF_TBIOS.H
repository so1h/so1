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

extern rindx_t rec_sf ;                     /* recurso sistema de ficheros */

extern dfs_t dfs_sf ;                       /* descriptor del fichero "SF" */

int inicSF_FATBIOS ( byte_t unidadBIOS ) ;  

#endif /* SF_FATBIOS_H */