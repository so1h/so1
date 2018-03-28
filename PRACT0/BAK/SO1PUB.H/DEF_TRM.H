/* ----------------------------------------------------------------------- */
/*                                 def_trm.h                               */
/* ----------------------------------------------------------------------- */
/*  Deficiones relativas a la trama de pila de un proceso no en ejecucion  */
/* ----------------------------------------------------------------------- */

#ifndef DEF_TRM_H
#define DEF_TRM_H

#include <so1pub.h\tipos.h>

typedef struct {
  word_t DS, ES ;                                 /* apilados por software */
  word_t DI, SI, BP, SP ;                                         /* pusha */
  word_t BX, DX, CX, AX ;                                         /* pusha */
  word_t IP, CS, Flags ;                                            /* int */
} tramaWords_t ;

typedef struct {
  word_t DS, ES ;                                 /* apilados por software */
  word_t DI, SI, BP, SP ;                                         /* pusha */
  byte_t BL, BH, DL, DH, CL, CH, AL, AH ;                         /* pusha */
  word_t IP, CS, Flags ;                                            /* int */
} tramaBytes_t ;

typedef union {
  tramaWords_t tw ;
  tramaBytes_t tb ;
} trama_t ;

#define DS tw.DS          /* para simplificar las referencias trama->tw.DS */
#define ES tw.ES                                     /* quedando trama->DS */
#define DI tw.DI
#define SI tw.SI
#define BP tw.BP
#define SP tw.SP
#define BX tw.BX
#define DX tw.DX
#define CX tw.CX
#define AX tw.AX
#define IP tw.IP
#define CS tw.CS
#define Flags tw.Flags

#define BL tb.BL          /* para simplificar las referencias trama->tb.BL */
#define BH tb.BH                                     /* quedando trama->BL */
#define DL tb.DL
#define DH tb.DH
#define CL tb.CL
#define CH tb.CH
#define AL tb.AL
#define AH tb.AH

#endif /* DEF_TRM */

