/* ----------------------------------------------------------------------- */
/*                                scanner.h                                */
/* ----------------------------------------------------------------------- */
/*  funciones para el analisis de los comandos del interprete de comandos  */
/* ----------------------------------------------------------------------- */

#ifndef SCANNER_H
#define SCANNER_H

#include <so1pub.h\tipos.h>

#define tamComando 80

#define nMaxComandos 10                            /* historia de comandos */

typedef enum {
  s_halt, s_reboot, s_shutdn, s_exit,            /* deben ser los primeros */
  s_cls, s_ver, s_dir, s_ptos, s_ret,
  s_host, s_nohost, s_cd, s_id, s_su, s_type, s_ctty,
  s_echo, s_traza, s_ident, s_numero, s_numeroLargo,
  s_mayor, s_menor, s_pipe, s_ampersand, s_status,
  s_fin, s_nulo
} simb_t ;

/* variables globales */

extern char comando [nMaxComandos][tamComando] ;   /* historia de comandos */
                                                          /* cola circular */
extern int inCmd ;                   /* indice de la primera entrada libre */
extern int outCmd ;                /* indice de la primera entrada ocupada */
extern int nComandos ;                    /* numero de comandos en la cola */

typedef struct {
  char str [9] ;
  int largo ;
  simb_t simb ;
} regSimbCmd_t ;                   /* informacion de un simbolo de comando */

extern regSimbCmd_t simbCmd [ ] ;     /* tabla de símbolos de los comandos */

extern int numCmds ;

                                                         /* comando[inCmd] */
extern char car ;            /* ultimo caracter leido en el comando actual */

extern word_t pos ;                /* posision de car en el comando actual */

extern simb_t simb ;          /* ultimo simbolo leido en el comando actual */

extern word_t num ;                       /* valor del ultimo numero leido */

extern dword_t numLargo ;                 /* valor del ultimo numero leido */

extern char str [ tamComando ] ;                    /* ultimo string leido */


void inicScanner ( void ) ;             /* prepara el analisis del comando */

void obtenCar ( void ) ;       /* obtiene el siguiente caracter a analizar */

void obtenStr ( void ) ;                    /* obtiene el siguiente string */

void saltarBlancos ( void ) ;           /* saltamos los siguientes blancos */

void numDec ( void ) ;                /* analizar siguiente numero decimal */

void numHex ( void ) ;            /* analizar siguiente numero hexadecimal */

void identificador ( void ) ;          /* analizar siguiente identificador */

void obtenSimb ( void ) ;                  /* obtener el siguiente símbolo */


#endif /* SCANNER_H */
