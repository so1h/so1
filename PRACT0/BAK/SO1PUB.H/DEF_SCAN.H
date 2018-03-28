/* ----------------------------------------------------------------------- */
/*                                def_scan.h                               */
/* ----------------------------------------------------------------------- */
/*                                                                         */
/* ----------------------------------------------------------------------- */

#ifndef DEF_SCAN_H
#define DEF_SCAN_H

#include <so1pub.h\tipos.h>

                                         /* codigos scan de algunas teclas */

#define CBrk       (byte_t)70                               /* tecla CBrk  */
#define Esc        (byte_t) 1


#define Ctrl       (byte_t)29   /* 0x1d */         /* tecla Ctrl izquierda */
#define ShIz       (byte_t)42   /* 0x2a */                  /* tecla Shift */
#define ShDe       (byte_t)54   /* 0x36 */
#define PrtSc      (byte_t)55   /* 0x37 */
#define Alt        (byte_t)56   /* 0x38 */          /* tecla Alt izquierda */
#define CapsLock   (byte_t)58   /* 0x3a */

#define Ord        (byte_t)41   /* 0x29 */         /* codigo scan de "ºª\" */

#define F1         (byte_t)59
#define F2         (byte_t)60
#define F3         (byte_t)61
#define F4         (byte_t)62
#define F5         (byte_t)63
#define F6         (byte_t)64
#define F7         (byte_t)65
#define F8         (byte_t)66
#define F9         (byte_t)67
#define F10        (byte_t)68
#define F11        (byte_t)87
#define F12        (byte_t)88

#define NumLock    (byte_t)69   /* 0x45 */
#define ScrollLock (byte_t)70   /* 0x46 */
#define Home       (byte_t)71   /* 0x47 */
#define FlUp       (byte_t)72   /* 0x48 */       /* tecla flecha arriba    */
#define PgUp       (byte_t)73   /* 0x49 */
#define FlIz       (byte_t)75   /* 0x4b */       /* tecla flecha izquierda */
#define FlDe       (byte_t)77   /* 0x4d */       /* tecla flecha derecha   */
#define End        (byte_t)79   /* 0x4f */
#define FlDn       (byte_t)80   /* 0x50 */       /* tecla flecha abajo     */
#define PgDn       (byte_t)81   /* 0x51 */
#define Ins        (byte_t)82   /* 0x52 */
#define Del        (byte_t)83   /* 0x53 */

#define E0       (byte_t)0xE0
#define E1       (byte_t)0xE1

#define CtrlTab   (byte_t)148   /* 0x94 */       /* Ctrl-Tab */
#define ShiftTab  (byte_t) 15   /* 0x0F */       /* Shift-Tab */

#endif /* DEF_SCAN_H */
