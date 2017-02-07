/* ----------------------------------------------------------------------- */
/*                                  msdos.h                                */
/* ----------------------------------------------------------------------- */
/*                                                                         */
/* ----------------------------------------------------------------------- */

#ifndef MSDOS_H
#define MSDOS_H

#include <so1pub.h\tipos.h>

void finProgDOS ( void ) ;

struct ffblk {
  char     ff_reserved [21] ;
  char     ff_attrib ;
  unsigned ff_ftime ;
  unsigned ff_fdate ;
  long     ff_fsize ;
  char     ff_name [13] ;
} ;

#define WILDCARDS 0x01
#define EXTENSION 0x02
#define FILENAME  0x04
#define DIRECTORY 0x08
#define DRIVE     0x10

#define MAXPATH   80
#define MAXDRIVE   3
#define MAXDIR    66
#define MAXFILE    9
#define MAXEXT     5

#define FA_RDONLY       0x01                        /* Read only attribute */
#define FA_HIDDEN       0x02                        /* Hidden file         */
#define FA_SYSTEM       0x04                        /* System file         */
#define FA_LABEL        0x08                        /* Volume label        */
#define FA_DIREC        0x10                        /* Directory           */
#define FA_ARCH         0x20                        /* Archive             */

#define nVIntErrCritico 0x24

#define SEEK_SET_MSDOS   0  /* lseek se posiciona desde el principio       */
#define SEEK_CUR_MSDOS   1  /* lseek se posiciona desde la posision actual */
#define SEEK_END_MSDOS   2  /* lseek se posiciona desde el final           */

#define O_RDONLY_MSDOS   0                 /* modos de apertura en openDOS */
#define O_WRONLY_MSDOS   1
#define O_RDWR_MSDOS     2

extern rti_t Old_VI_ErrCritico ;   /* antiguo vector de int. error critico */

void far rti_errCritico ( ) ;


bool_t hayMSDOS ( void ) ;

bool_t hayDOSBox ( void ) ;

bool_t hayWindowsNT ( void ) ;

word_t segPSP ( void ) ;

word_t versionMSDOS ( void ) ;

pointer_t entornoMSDOS ( void ) ;

pointer_t valorMSDOS ( char * str ) ;

int argcMSDOS ( void ) ;

bool_t argvMSDOS ( word_t n, char * str ) ;



int openDOS ( pointer_t nombre, byte_t modo ) ;

int extendedOpenDOS ( pointer_t    nombre,                   /* MSDOS 4.0+ */
                      word_t       modo,               /* 0df00000ishr0r/w */
                      word_t       atr,                        /* 00advshr */
                      word_t far * accion,             /* 0x01, 0x11, 0x12 */
                      word_t far * error ) ;

int closeDOS ( int df ) ;

int commitFileDOS ( int df ) ;

int createDOS ( pointer_t nombre, word_t atributo ) ;    /* FA_RDONLY, ... */

int readDOS ( int df, char * buf, int n ) ;

int readDOSFar ( int df, char far * buf, int n ) ;

int writeDOS ( int df, char * buf, int n ) ;

int writeDOSFar ( int df, char far * buf, int n ) ;

int lseekDOS ( int df, dword_t * pos, int whence ) ;


int getdiskDOS ( void ) ;

int setdiskDOS ( int drive ) ;

int getcurdirDOS ( int drive, char * direc ) ;

int chdirDOS ( const char * path ) ;

int findfirstDOS ( const char * pathname,
                   struct ffblk * ffblk,
                   int attrib ) ;

int findnextDOS ( struct ffblk * ffblk ) ;

#endif /* MSDOS_H */




