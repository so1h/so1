/* ----------------------------------------------------------------------- */
/*                               carsctrl.h                                */
/* ----------------------------------------------------------------------- */
/*                                                                         */
/* ----------------------------------------------------------------------- */

#ifndef CARSCTRL_H
#define CARSCTRL_H

/* Caracteres de control ASCII */

#define NUL   (char) 0  /* null (end string)    ^@ */                         /* '\000' */
#define SOH   (char) 1  /* start of heading     ^A */                         /* '\001' */
#define STX   (char) 2  /* start of text        ^B */
#define ETX   (char) 3  /* end of text          ^C */
#define EOT   (char) 4  /* end of transmission  ^D */
#define ENQ   (char) 5  /* enquiry              ^E */
#define ACK   (char) 6  /* acknowledge          ^F */
#define BEL   (char) 7  /* bell                 ^G */       /* pitido beep */ /* '\a'   */
#define BS    (char) 8  /* back space           ^H */        /* back space */ /* '\b'   */
#define HT    (char) 9  /* TAB horizontal tab   ^I */                         /* '\t'   */
#define LF    (char)10  /* line feed            ^J */   /* avance de linea */ /* '\n'   */
#define VT    (char)11  /* vertical tab         ^K */                         /* '\v'   */
#define FF    (char)12  /* form feed            ^L */  /* avance de pagina */ /* '\f'   */
#define CR    (char)13  /* carriage return      ^M */  /* retorno de carro */ /* '\r'   */
#define SO    (char)14  /* shift out            ^N */
#define SI_   (char)15  /* shift in             ^O */

#define DLE   (char)16  /* data line escape     ^P */
#define DC1   (char)17  /* dev ctrl 1 (X-ON)    ^Q */
#define DC2   (char)18  /* device ctrl 2        ^R */
#define DC3   (char)19  /* dev ctrl 3 (X-OFF)   ^S */
#define DC4   (char)20  /* device ctrl 4        ^T */
#define NAK   (char)21  /* negative acknowledge ^U */
#define SYN   (char)22  /* syncronous idle      ^V */
#define ETB   (char)23  /* end transmit block   ^W */
#define CAN   (char)24  /* cancel               ^X */
#define EM    (char)25  /* end of medium        ^Y */
#define SUB   (char)26  /* substitute           ^Z */
#define ESC   (char)27  /* escape               ^[ */
#define FSp   (char)28  /* file separator       ^\ */
#define GSp   (char)29  /* group separator      ^] */
#define RS    (char)30  /* record separator     ^^ */
#define US    (char)31  /* unit separator       ^_ */

#endif /* CARSCTRL_H */

