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

#define ptrBuferSO1 ((pointer_t)MK_FP(segBuferSO1, 0x0000))     

extern word_t segBuferSO1 ;    /* bufer para leer de forma segura sectores */
                                    /* con el BIOS (ver error 0x09 int 13) */
int opSectorDB ( dword_t sectorLogico, int db, pointer_t dir, byte_t cmd ) ;
									
#endif /* DB_H */