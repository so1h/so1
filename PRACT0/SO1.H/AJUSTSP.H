/* ----------------------------------------------------------------------- */
/*                                 ajustsp.h                               */
/* ----------------------------------------------------------------------- */
/*                       Ajuste del tamaño de la pila                      */
/* ----------------------------------------------------------------------- */

#ifndef AJUSTSP_H
#define AJUSTSP_H

#define SP0_SO1   0x5FFE        /* puntero de pila inicial del sistema SO1 */ /* 20 */
//#define SP0_SO1   0x4FFE        /* puntero de pila inicial del sistema SO1 */ /* 20 */
//#define SP0_SO1   0x3FFE      /* puntero de pila inicial del sistema SO1 */ /* 16 */
//                                  /* para atender traps e interrupciones */

#define SP0_SF    0x4FFE   
//#define SP0_SF    0x3FFE   

#define SP0_DB    0x3FFE   
//#define SP0_DB    0x3BFE   

//#define SP0_GP    0x67FE                   /* puntero de pila inicial GP */

//#define SP0_SF    0x5FFE                   /* puntero de pila inicial SF */

#define SPInicial 0x37FE                /* puntero de pila del proceso SO1 */
//                          /* maxProcesos = 20 */ /* finBSS = 3570 < 3FFE */ /* 20 */
//#define SPInicial 0x33FE                /* puntero de pila del proceso SO1 */
//                          /* maxProcesos = 20 */ /* finBSS = 2FCA < 33FE */ /* 20 */
//#define SPInicial 0x2FFE              /* puntero de pila del proceso SO1 */ 
//                          /* maxProcesos = 16 */ /* finBSS = 2C18 < 2FFE */ /* 16 */

#endif /* AJUSTSP_H */



