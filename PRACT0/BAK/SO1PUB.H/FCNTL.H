/* ----------------------------------------------------------------------- */
/*                                 fcntl.h                                 */
/* ----------------------------------------------------------------------- */
/*                     flag values accessible to open                      */
/* ----------------------------------------------------------------------- */

#ifndef FCNTL_H
#define FCNTL_H

#include <so1pub.h\tipos.h>

typedef word_t modoAp_t ;

#define O_RDONLY    0x0001
#define O_WRONLY    0x0002
#define O_RDWR      0x0004

#define O_CREAT     0x0100  /* crea y abre el fichero */
#define O_TRUNC     0x0200  /* abre con truncamiento */
#define O_EXCL      0x0400  /* apertura exclusiva */
#define O_APPEND    0x0800  /* al final del fichero */

/* en un fichero en modo de adicion solo puede escribirse por su final */

#define O_NONBLOCK  0x1000  /* no bloqueante */
#define O_TEXT      0x4000  /* CR-LF traduccion */
#define O_BINARY    0x8000  /* sin traduccion */

#define O_DENYALL   0x0010
#define O_DENYWRITE 0x0020
#define O_DENYREAD  0x0030
#define O_DENYNONE  0x0040
#define O_NOINHERIT 0x0080

#define F_DUPFD          0
#define F_GETFD          1
#define F_SETFD          2
#define F_GETFL          3
#define F_SETFL          4
#define F_GETLK          5
#define F_SETLK          6
#define F_SETLKW         7


#endif /* FCNTL_H */
