/* ----------------------------------------------------------------------- */
/*                                 tipos.h                                 */
/* ----------------------------------------------------------------------- */
/*                  Definiciones y tipos de datos basicos                  */
/* ----------------------------------------------------------------------- */

#ifndef TIPOS_H
#define TIPOS_H

typedef enum { FALSE, TRUE } bool_t ;               /* FALSE = 0, TRUE = 1 */
                                                     /* sizeof(bool_t) = 2 */
typedef unsigned char byte_t ;                            /* byte (8 bits) */
                                                     /* sizeof(byte_t) = 1 */
typedef unsigned int word_t ;                         /* palabra (16 bits) */

typedef unsigned long dword_t ;                 /* doble palabra (32 bits) */

typedef struct {
    dword_t dwl ;
    dword_t dwh ;
} qword_t ;                                 /* cuadruple palabra (64 bits) */

typedef byte_t far * pointer_t ;                      /* puntero (32 bits) */

#define NULL (pointer_t)0x00000000L

typedef void (* manejador_t) ( void ) ; /* manejador de llamada al sistema */

typedef void interrupt ( far * rti_t) ( void ) ; /* rutina de trat. de int */

typedef void far ( far * isr_t ) ( void ) ;  /* rutina de servicio de int. */

typedef void far ( far * handler_t ) ( void ) ;                 /* handler */

typedef int far ( * retardarProceso_t ) ( void ) ;

typedef rti_t far * ptrTVI_t ;        /* tabla de vectores de interrupcion */

typedef enum {
    modoSO1_Cmd,             /* SO1 se ejecuta como un comando de la shell */
    modoSO1_Bin,             /* SO1 se ejecuta como so1.bin (boot)         */
    modoSO1_Com,             /* SO1 se ejecuta como so1.com (MSDOS/XP)     */
    modoSO1_Exe,             /* SO1 se ejecuta como so1.exe (MSDOS/XP)     */
    modoSO1_Bs               /* SO1 se carga en 0000:7C00   (SYSLINUX)     */ /* 2015-06-21 */
} modoSO1_t ;

typedef struct {
    byte_t lo ;
    byte_t hi ;
} lh_t ;

typedef struct {
    word_t offset ;
    word_t segment ;
} address_t ;                                  /* direccion 8086 (32 bits) */

typedef union {
    pointer_t ptr ;
    address_t adr ;
} uPtrAdr_t ;                                   /* union pointer y address */

typedef int pid_t ;                            /* identificador de proceso */

typedef int uid_t ;                            /* identificador de usuario */

typedef int gid_t ;                              /* identificador de grupo */

typedef int pindx_t ;         /* indice de proceso en la tabla de procesos */

#define NIL (pindx_t)(-1)      /* fin de una lista de procesos (campo sig) */ /* obsoleto c2c_t */

typedef int df_t ;        /* indice en la tabla de descriptores de fichero */

typedef int dfs_t ;      /* indice en la tabla de descriptores del sistema */

typedef int rindx_t ;         /* indice de recurso en la tabla de recursos */

typedef struct {
    pindx_t primero ;
    pindx_t ultimo ;
} cola_t ;                                      /* cola de procesos simple */ /* obsoleto c2c_t */

typedef char far * comando_t ;

typedef comando_t far * ptrComando_t ;

/* Funciones */

pointer_t pointer ( word_t seg, word_t off ) ;

word_t seg ( pointer_t p ) ;

word_t off ( pointer_t p ) ;

void incPtr ( pointer_t far * p, word_t i ) ;

/* Macros */

#define SEG(p) (((address_t *)&(p))->segment)        /* p variable puntero */
//                        /* falla en algunos contextos (DR.C) usar FP_SEG */
#define OFF(p) ((word_t)(p))

#define LINEAL(p) ((((dword_t)SEG(p)) << 4) + (dword_t)OFF(p))


#define FP_SEG(fp)      ((word_t)((dword_t)(fp) >> 16))

#define FP_OFF(fp)      ((word_t)(fp))              /* fp variable puntero */

#define MK_FP(seg,ofs)  ((void far *) \
                           (((dword_t)(seg) << 16) | (word_t)(ofs)))

#define poke(a,b,c)     (*((word_t far*)MK_FP((a),(b))) = (word_t)(c))

#define pokeb(a,b,c)    (*((byte_t far*)MK_FP((a),(b))) = (byte_t)(c))

#define peek(a,b)       (*((word_t far*)MK_FP((a),(b))))

#define peekb(a,b)      (*((byte_t far*)MK_FP((a),(b))))

/* deficiencias de algunos ensambladores (como el BASM) */

#ifdef IDE
#define retf db 0xCB
#endif

#endif /* TIPOS_H */
