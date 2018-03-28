/* ----------------------------------------------------------------------- */
/*                                   db.h                                  */
/* ----------------------------------------------------------------------- */
/*                         dispositivos de bloques                         */
/* ----------------------------------------------------------------------- */

#ifndef DB_H
#define DB_H

#include <so1pub.h\tipos.h> /* rindx_t, byte_t, word_t, dword_t, pointer_t */

extern rindx_t rec_db ;

void inicDB ( void ) ;

void * DB ( void * arg ) ;

#define ptrBuferSO1 ((pointer_t)MK_FP(segBuferSO1, 0x0000))     

word_t segBuferSeguro ( void ) ;                      /* seguro con el DMA */

extern word_t segBuferSO1 ;    /* bufer para leer de forma segura sectores */
                                    /* con el BIOS (ver error 0x09 int 13) */

int opSectorDB ( dword_t sectorLogico, int db, pointer_t dir, byte_t cmd ) ;

typedef struct {
    char nombre [ 5 ] ;                 /* FDA, FDB, HDA, HDA1, ... , HDB4 */
	byte_t  unidadBIOS ;                         /* 0x00, 0x01, 0x80, 0x81 */
	byte_t  tipoUnidad ;                          /* CMOS_DISK_DRIVE1_TYPE */
    word_t  bytesPorSector ;                                        /* 512 */
    byte_t  sectoresPorPista ;                    
    byte_t  cabezas ;
	word_t  cilindros ;
	dword_t primerSector ;
	dword_t numSectores ;        /* numero de sectores del disco/particion */
} d_bloque_t ;

typedef struct {  
    pindx_t   pindx ;                    /* proceso que solicita el sector */
    dword_t   sectorLogico ;                    /* numero de sector logico */
    int       db ;                      /* dispositivo de bloques (unidad) */
    pointer_t dir ;                                /* direccion de memoria */
    byte_t    cmd ;                                             /* comando */
} peticionDB_t ;
									
#endif /* DB_H */