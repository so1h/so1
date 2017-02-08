/***************************************************************************/
/*                                 bpb16.h                                 */
/***************************************************************************/
/*            Bloque de parametros del BIOS para una particion             */
/***************************************************************************/
/* Bloque de parametros del BIOS para una particion de disco duro con      */
/* formato FAT16, un unico sector reservado (el propio sector de arranque) */
/* y 512 entradas en el directorio raiz. El primer sector de la particion  */
/* corresponde al sector logico 63. Tras tomar el control en startBPB se   */
/* cede el control a una funcion externa denominada bootCode.              */
/***************************************************************************/

#ifndef BPB16_H
#define BPB16_H

#include <so1pub.h\tipos.h>                      /* byte_t, word_t, dword_t*/
                                                   /* pointer_t, address_t */

void startBPB ( void ) ;                               /* comienzo del BPB */

#define C_instJMP            {0xEB,0x3C}  /* instruccion JMP (0xEB, 0x3C+) */
#define C_instNOP            0x90         /* instruccion NOP (0x90)        */
#define C_OEM                'BPB HD  '   /* OEM, 8 caracteres             */
#define C_bytesPorSector     512          /* bytes por sector              */
#define C_sectoresPorCluster 1            /* sectores por cluster          */
#define C_sectoresReservados 1            /* sectores reservados           */
#define C_numeroDeFATs       2            /* numero de FATs                */
#define C_entradasDirRaiz    512          /* entradas del directorio raiz  */
#define C_sectores16         65473        /* n. de sectores en total (16b) */
#define C_tipoDeMedio        0xF8         /* descriptor de medio           */
#define C_sectoresPorFAT     254          /* sectores pot FAT              */
#define C_sectoresPorPista   63           /* sectores por pista            */
#define C_cabezas            255          /* cabezas                       */
#define C_primerSector       63           /* primer sector de la particion */
#define C_sectores           0x00000000   /* n. de sectores en total (32)  */
#define C_unidad             0x80         /* unidad (0 -> A:, 0x80 -> C:)  */
#define C_relleno            0x00         /* byte que no se usa            */
#define C_signatura          0x29         /* extension de la signatura     */
#define C_numeroDeSerie      0x00000000   /* numero de serie               */
#define C_etiqueta           'ETIQUETA   '/* etiqueta de volumen (11 car.) */
#define C_SF                 'FAT16   '   /* tipo de sistema de ficheros   */



#define camposBPB( xtrn ) \
xtrn byte_t  instJMP [2] ;          /* instruccion JMP (0xEB, 0x3C)        */\
xtrn byte_t  instNOP ;              /* instruccion NOP (0x90) de relleno   */\
xtrn byte_t  OEM [8] ;              /* OEM, 8 caracteres                   */\
xtrn word_t  bytesPorSector ;       /* bytes por sector                    */\
xtrn byte_t  sectoresPorCluster ;   /* sectores por cluster                */\
xtrn word_t  sectoresReservados ;   /* sectores reservados                 */\
xtrn byte_t  numeroDeFATs ;         /* numero de FATs                      */\
xtrn word_t  entradasDirRaiz ;      /* entradas del directorio raiz        */\
xtrn word_t  sectores16 ;           /* numero de sectores en total (16 bts)*/\
xtrn byte_t  tipoDeMedio ;          /* descriptor de medio                 */\
xtrn word_t  sectoresPorFAT ;       /* sectores pot FAT                    */\
xtrn word_t  sectoresPorPista ;     /* sectores por pista                  */\
xtrn word_t  cabezas ;              /* cabezas                             */\
xtrn dword_t primerSector ;         /* primer sector de la particion       */\
xtrn dword_t sectores ;             /* numero de sectores en total (32 bts)*/\
xtrn byte_t  unidad ;               /* unidad (0 -> A:)                    */\
xtrn byte_t  relleno ;              /* byte que no se usa                  */\
xtrn byte_t  signatura ;            /* extension de la signatura           */\
xtrn dword_t numeroDeSerie ;        /* numero de serie                     */\
xtrn char    etiqueta [11]  ;       /* etiqueta de volumen (11 caracteres) */\
xtrn char    SF [8] ;               /* tipo de sistema de ficheros         */

camposBPB( extern ) ;  /* xtrn = extern, static, register, auto o volatile */

typedef struct {
  camposBPB(volatile)
} bpb_t ;

#define tamBPB 0x003E

#if (sizeof(bpb_t) != tamBPB)
#error el tamanio del tipo bpb_t debe ser tamBPB bytes
#endif

#endif /* BPB16_H */