/* ----------------------------------------------------------------------- */
/*                                consola.h                                */
/* ----------------------------------------------------------------------- */
/*       declaraciones necesarias para la consola (teclado+pantalla)       */
/*                            [con multiscreen]                            */
/* ----------------------------------------------------------------------- */

#ifndef CONSOLA_H
#define CONSOLA_H

#include <so1pub.h\def_tecl.h>                                /* teclado_t */
#include <so1pub.h\def_pant.h>                               /* pantalla_t */

typedef struct
{
    pantalla_t pantalla ;
    teclado_t  teclado ;
    byte_t     F ;                       /* fila de la posicion del cursor */
    byte_t     C ;                    /* columna de la posicion del cursor */
//  byte_t     maxF ;                    /* ultima (mayor) fila modificada */
} descConsola_t ;

/* ----------------------------------------------------------------------- */
/*                    codigos de exploracion del teclado                   */
/* ----------------------------------------------------------------------- */

/* Original PX/XT Keyboard */

/* Al pulsarse la tecla el codigo de exploracion (scan code) es un numero  */
/* en binario del tipo 0XXXXXXX. Al soltarse, el codigo de exploracion es  */
/* el mismo, pero con un 1 en el bit de mayor peso: 1XXXXXXX.              */

/* tomados del TecHelp 6.0 (corresponden al teclado US) */

#define CS_Nulo   0x00
#define CS_Esc    0x01 /* Esc         */
#define CS_Adm    0x02 /* 1 !         */
#define CS_Arroba 0x03 /* 2 @         */
#define CS_Alm    0x04 /* 3 #         */
#define CS_Dolar  0x05 /* 4 $         */
#define CS_X100   0x06 /* 5 %         */
#define CS_Gorro  0x07 /* 6 ^         */
#define CS_Amp    0x08 /* 7 &         */
#define CS_Por    0x09 /* 8 *         */
#define CS_ParIz  0x0A /* 9 (         */
#define CS_ParDe  0x0B /* 0 )         */
#define CS_Menos  0x0C /* - _         */
#define CS_Mas    0x0D /* + =         */
#define CS_Bak    0x0E /* BakSpace    */
#define CS_Tab    0x0F /* Tab         */
#define CS_Q      0x10 /* Q           */
#define CS_W      0x11 /* W           */

#define CS_E      0x12 /* E           */
#define CS_R      0x13 /* R           */
#define CS_T      0x14 /* T           */
#define CS_Y      0x15 /* Y           */
#define CS_U      0x16 /* U           */
#define CS_I      0x17 /* I           */
#define CS_O      0x18 /* O           */
#define CS_P      0x19 /* P           */
#define CS_CorIz  0x1A /* [ {         */
#define CS_CorDe  0x1B /* ] }         */
#define CS_Enter  0x1C /* Enter       */
#define CS_Ctrl   0x1D /* Ctrl        */
#define CS_A      0x1E /* A           */
#define CS_S      0x1F /* S           */
#define CS_D      0x20 /* D           */
#define CS_F      0x21 /* F           */
#define CS_G      0x22 /* G           */

#define CS_H      0x23 /* H           */
#define CS_J      0x24 /* J           */
#define CS_K      0x25 /* K           */
#define CS_L      0x26 /* L           */
#define CS_2Ptos  0x27 /* ; :         */
#define CS_Quote  0x28 /* " '         */
#define CS_Onda   0x29 /* ` ~         */
#define CS_ShIz   0x2A /* Shift Iz    */
#define CS_BSlash 0x2B /* \ |         */
#define CS_Z      0x2C /* Z           */
#define CS_X      0x2D /* X           */
#define CS_C      0x2E /* C           */
#define CS_V      0x2F /* V           */
#define CS_B      0x30 /* B           */
#define CS_N      0x31 /* N           */
#define CS_M      0x32 /* M           */
#define CS_Coma   0x33 /* , <         */

#define CS_Pto    0x34 /* . >         */
#define CS_Slash  0x35 /* / ?         */
#define CS_DhDe   0x36 /* Shift De    */
#define CS_PtrSc  0x37 /* * PrtSc     */
#define CS_Alt    0x38 /* Alt         */
#define CS_Space  0x39 /* Space       */
#define CS_CLock  0x3A /* Caps Lock   */
#define CS_F1     0x3B /* F1          */
#define CS_F2     0x3C /* F2          */
#define CS_F3     0x3D /* F3          */
#define CS_F4     0x3E /* F4          */
#define CS_F5     0x3F /* F5          */
#define CS_F6     0x40 /* F6          */
#define CS_F7     0x41 /* F7          */
#define CS_F8     0x42 /* F8          */
#define CS_F9     0x43 /* F9          */
#define CS_F10    0x44 /* F10         */

#define CS_NLock  0x45 /* Num Lock    */ /* Teclado numerico */
#define CS_SLock  0x46 /* Scroll Lock */
#define CS_Home   0x47 /* Home   [7]  */
#define CS_FlUp   0x48 /* FlUp   [8]  */
#define CS_PgUp   0x49 /* PgUp   [9]  */
#define CS_KMenos 0x4A /* KeyPad -    */
#define CS_FlIz   0x4B /* FlIz   [4]  */
#define CS_K5     0x4C /*        [5]  */
#define CS_FlDe   0x4D /* FlDe   [6]  */
#define CS_KMas   0x4E /* KeyPad +    */
#define CS_End    0x4F /* End    [1]  */
#define CS_FlDn   0x50 /* FlDn   [2]  */
#define CS_PgDn   0x51 /* PgDn   [3]  */
#define CS_Ins    0x52 /* Ins    [0]  */
#define CS_Del    0x53 /* Del    [.]  */
/* INT 15H 85H */
#define CS_SysReq 0x54 /* SysReq      */         /* Teclado AT (84 teclas) */
/* pero no reportado por el BIOS */

/* Algunos codigos de exploracion adicionales en el teclado de 101 teclas  */

#define CS_F11    0x57 /* F11         */
#define CS_F12    0x58 /* F12         */
#define CS_AltPS  0x54 /* Alt Print S */

/* otras teclas producen una secuencia de 2, 4 o 6 codigos de exploracion  */

/* En el teclado español el Back Slash se corresponde con el 0x29 */

#define CS_ES_BSlash CS_Onda


/* ----------------------------------------------------------------------- */
/*              extended ASCII Keystroques (INT16 Fn 00H y 01H)            */
/* ----------------------------------------------------------------------- */

#define KS_F1        0x3B
#define KS_F2        0x3C
#define KS_F3        0x3D
#define KS_F4        0x3E
#define KS_F5        0x3F
#define KS_F6        0x40
#define KS_F7        0x41
#define KS_F8        0x42
#define KS_F9        0x43
#define KS_F10       0x44

#define KS_ShF1      0x54
#define KS_ShF2      0x55
#define KS_ShF3      0x56
#define KS_ShF4      0x57
#define KS_ShF5      0x58
#define KS_ShF6      0x59
#define KS_ShF7      0x5A
#define KS_ShF8      0x5B
#define KS_ShF9      0x5C
#define KS_ShF10     0x5D

#define KS_CtrlF1    0x5E
#define KS_CtrlF2    0x5F
#define KS_CtrlF3    0x60
#define KS_CtrlF4    0x61
#define KS_CtrlF5    0x62
#define KS_CtrlF6    0x63
#define KS_CtrlF7    0x64
#define KS_CtrlF8    0x65
#define KS_CtrlF9    0x66
#define KS_CtrlF10   0x67

#define KS_AltF1     0x68
#define KS_AltF2     0x69
#define KS_AltF3     0x6A
#define KS_AltF4     0x6B
#define KS_AltF5     0x6C
#define KS_AltF6     0x6D
#define KS_AltF7     0x6E
#define KS_AltF8     0x6F
#define KS_AltF9     0x70
#define KS_AltF10    0x71

#define KS_AltA      0x1E
#define KS_AltB      0x30
#define KS_AltC      0x2E
#define KS_AltD      0x20
#define KS_AltE      0x12
#define KS_AltF      0x21
#define KS_AltG      0x22
#define KS_AltH      0x23
#define KS_AltI      0x17
#define KS_AltJ      0x24
#define KS_AltK      0x25
#define KS_AltL      0x26
#define KS_AltM      0x32
#define KS_AltN      0x31
#define KS_AltO      0x18
#define KS_AltP      0x19
#define KS_AltQ      0x10
#define KS_AltR      0x13
#define KS_AltS      0x1F
#define KS_AltT      0x14
#define KS_AltU      0x16
#define KS_AltV      0x2F
#define KS_AltW      0x11
#define KS_AltX      0x2D
#define KS_AltY      0x15
#define KS_AltZ      0x2C

#define KS_Alt0      0x81
#define KS_Alt1      0x78
#define KS_Alt2      0x79
#define KS_Alt3      0x7A
#define KS_Alt4      0x7B
#define KS_Alt5      0x7C
#define KS_Alt6      0x7D
#define KS_Alt7      0x7E
#define KS_Alt8      0x7F
#define KS_Alt9      0x80
#define KS_AltMenos  0x82
#define KS_AltIgual  0x83

#define KS_NUL       0x03

#define KS_ShTab     0x0F
#define KS_Ins       0x52
#define KS_Del       0x53
#define KS_CtrlPtrSc 0x72
#define KS_FlDn      0x50
#define KS_FlIz      0x4B
#define KS_FlDe      0x4D
#define KS_FlUp      0x48
#define KS_End       0x4F
#define KS_Home      0x47
#define KS_PgDn      0x51
#define KS_PgUp      0x49
#define KS_CtrlFlIz  0x73
#define KS_CtrlFlDe  0x74
#define KS_CtrlEnd   0x75
#define KS_CtrlHome  0x77
#define KS_CtrlPgDn  0x76
#define KS_CtrlPgUp  0x84

/* Keystrokes adicionales en el teclado de 101 teclas  */

#define KS_F11       0x85
#define KS_F12       0x86
#define KS_ShF11     0x87
#define KS_ShF12     0x88
#define KS_CtrlF11   0x89
#define KS_CtrlF12   0x8A
#define KS_AltF11    0x8B
#define KS_AltF12    0x8C
#define KS_AltCorcIz 0x1A
#define KS_AltCorcDe 0x1B

#define KS_AltPtoYC  0x27
#define KS_AltApost  0x28
#define KS_AltApost2 0x29
#define KS_AltBSlash 0x2B
#define KS_AltComa   0x33
#define KS_AltPto    0x34
#define KS_AltSlash  0x35


/* faltan por añadir: TODO */

#define KS_AltKSlash 0xA4
#define KS_AltKPor   0x37
#define KS_AltKMenos 0x4A
#define KS_AltKMas   0x4E
#define KS_AltKEnter 0xA6

#endif /* CONSOLA_H */
