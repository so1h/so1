/***************************************************************************/
/*                                bpbfat16.c                               */
/***************************************************************************/
/*            Bloque de parametros del BIOS para una particion             */
/***************************************************************************/

//#include "bpbfat12.h"
#include <_pbr\pbrfat12\_bpbfat12\bpbfat12.h>

extern void bootCode ( void ) ;

asm {
//assume NOTHING
//assume CS:_DATA, DS:_DATA
  assume CS:_DATA
_startBPB proc near
  /* aqui no hay nada de nada */
_startBPB endp
public _startBPB
}

asm {                /* usar inicio si se requiere primeros bytes EB 3C 90 */
_instJMP: jmp short bootCode ;        /* instruccion JMP (0xEB, 0x3C+)     */
//_instJMP: jmp short inicio ;        /* instruccion JMP (0xEB, 0x3C+)     */
     /* tlink enlaza mal (0x3B), pero tanto jwlink como ulink enlazan bien */
_instNOP: nop                         /* instruccion NOP (0x90) de relleno */
_OEM                db C_OEM ;
_bytesPorSector     dw C_bytesPorSector ;
_sectoresPorCluster db C_sectoresPorCluster ;
_sectoresReservados dw C_sectoresReservados ;
_numeroDeFATs       db C_numeroDeFATs ;
_entradasDirRaiz    dw C_entradasDirRaiz ;
_sectores16         dw C_sectores16 ;
_tipoDeMedio        db C_tipoDeMedio ;
_sectoresPorFAT     dw C_sectoresPorFAT ;
_sectoresPorPista   dw C_sectoresPorPista ;
_cabezas            dw C_cabezas ;
_primerSector       dd C_primerSector ;
_sectores           dd C_sectores ;
_unidad             db C_unidad ;
_relleno            db C_relleno ;
_signatura          db C_signatura ;
_numeroDeSerie      dd C_numeroDeSerie ;
_etiqueta           db C_etiqueta ;
_SF                 db C_SF ;

//inicio:
//  call bootCode ;

public _OEM ;
public _bytesPorSector ;
public _sectoresPorCluster
public _sectoresReservados
public _numeroDeFATs
public _entradasDirRaiz
public _sectores16
public _tipoDeMedio
public _sectoresPorFAT
public _sectoresPorPista
public _cabezas
public _primerSector
public _sectores
public _unidad
public _relleno
public _signatura
public _numeroDeSerie
public _etiqueta
public _SF

}
