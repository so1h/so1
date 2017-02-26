/* ----------------------------------------------------------------------- */
/*                                   gm.h                                  */
/* ----------------------------------------------------------------------- */
/*   Gestor de Memoria: Particiones variables (Lista de bloques libres)    */
/* ----------------------------------------------------------------------- */

#ifndef GM_H
#define GM_H

#include <so1pub.h\tipos.h>                                      /* word_t */
#include <so1pub.h\def_mem.h>                               /* ptrBloque_t */

#define tamParagrafo 16          /* un paragrafo son 16 bytes consecutivos */
                             /* comenzando en una direccion multiplo de 16 */

extern word_t tamBloqueMax ;   /* mayor bloque de memoria libre disponible */

extern ptrBloque_t listaLibres ;

word_t k_buscarBloque ( word_t tam ) ;

bool_t k_devolverBloque ( word_t segmento, word_t tam ) ;

extern rindx_t rec_gm ;

void inicGM ( void ) ;

#endif /* GM_H */

