/* ----------------------------------------------------------------------- */
/*                                 ajustsp.h                               */
/* ----------------------------------------------------------------------- */
/*                       Ajuste del tamaño de la pila                      */
/* ----------------------------------------------------------------------- */

#ifndef AJUSTSP_H
#define AJUSTSP_H

#define SP0_SO1   0x3FFE        /* puntero de pila inicial del sistema SO1 */
//#define SP0_SO1   0x6FFE      /* puntero de pila inicial del sistema SO1 */
//                                  /* para atender traps e interrupciones */

//#define SP0_GP    0x67FE                   /* puntero de pila inicial GP */

//#define SP0_SF    0x5FFE                   /* puntero de pila inicial SF */

#define SPInicial 0x2FFE                /* puntero de pila del proceso SO1 */
//#define SPInicial 0x57FE              /* puntero de pila del proceso SO1 */
//                                                 /* finBSS = 2C18 < 2FFE */

#endif /* AJUSTSP_H */



