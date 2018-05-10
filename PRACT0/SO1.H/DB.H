/* ----------------------------------------------------------------------- */
/*                                   db.h                                  */
/* ----------------------------------------------------------------------- */
/*                         dispositivos de bloques                         */
/* ----------------------------------------------------------------------- */

#ifndef DB_H
#define DB_H

#include <so1pub.h\tipos.h> /* rindx_t, byte_t, word_t, dword_t, pointer_t */

extern rindx_t rec_db ;                  /* indice del recurso de los d.b. */

typedef struct {                             /* datos de un d.b. detectado */
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

#define dbMax 12                                  /* numero maximo de d.b. */

extern d_bloque_t d_bloque [ dbMax ] ;

int inicDB ( void ) ;                                    /* inicializacion */

//  /* inicDB devuelve el numero de dispositivos de bloques detectados     */

void * DB ( void * arg ) ;            /* funcion ejecutada por el servidor */

extern pindx_t pindxSrvDB ;                         /* indice del servidor */

#define ptrBuferSO1 ((pointer_t)MK_FP(segBuferSO1, 0x0000))   

word_t segBuferSeguro ( bool_t Ok ) ;                 /* seguro con el DMA */

extern word_t segBuferSO1 ;    /* bufer para leer de forma segura sectores */
                                    /* con el BIOS (ver error 0x09 int 13) */

int opSectorDB ( dword_t sectorLogico, int db, pointer_t dir, byte_t cmd ) ;

typedef struct {                        /* peticion read/write al servidor */
//  int       df ;                       /* descriptor del fichero abierto */ /*  2 bytes */
    int       db ;                      /* dispositivo de bloques (unidad) */ /*  2 bytes */
    pointer_t dir ;                                /* direccion de memoria */ /*  4 bytes */
	word_t    nbytes ;                                  /* numero de bytes */ /*  2 bytes */
    byte_t    cmd ;                                             /* comando */ /*  1 byte  */
} peticionDB_t ;                                                  /* total */ /*  9 bytes */    

/* cache */
			
#define CONCACHE TRUE
//#define CONCACHE FALSE
			
#if (CONCACHE)
	
#define numBloquesCache 16 

typedef struct {
	bool_t    ocupado ;                            /* bloque libre/ocupado */
	int       db ;                               /* dispositivo de bloques */
    dword_t   sectorLogico ;                   /* sector logico almacenado */
    pointer_t dir ;                                /* bloques de 512 bytes */
} descBloqueCache_t ;

extern descBloqueCache_t descBloqueCache [ numBloquesCache ] ;

void inicCacheDB ( void ) ;

int leerDeCache ( dword_t sectorLogico,  int db, pointer_t dir ) ;

void meterEnCacheDB ( dword_t sectorLogico,  int db, pointer_t dir ) ;

int sacarDeCacheDB ( dword_t sectorLogico,  int db ) ; 
 
#endif									
									
#endif /* DB_H */