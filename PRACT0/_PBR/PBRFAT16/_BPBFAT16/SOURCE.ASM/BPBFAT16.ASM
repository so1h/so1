;/***************************************************************************/
;/*                               bpbfat16.asm                              */
;/***************************************************************************/
;/*             Bloque de parametros del BIOS para un disquete              */
;/***************************************************************************/

extrn _bootCode

.model TINY

.data

  assume CS:_DATA
_startBPB proc near
; /* aqui no hay nada de nada */
_startBPB endp
public _startBPB

_instJMP: jmp short inicio ;   /* instruccion JMP (0xEB, 0x3C)             */
_instNOP: nop              ;   /* instruccion NOP (0x90) de relleno        */

C_OEM                EQU 'BPB HD  '      ;/* OEM, 8 caracteres             */
C_bytesPorSector     EQU 512             ;/* bytes por sector              */
C_sectoresPorCluster EQU 1               ;/* sectores por cluster          */
C_sectoresReservados EQU 1               ;/* sectores reservados           */
C_numeroDeFATs       EQU 2               ;/* numero de FATs                */
C_entradasDirRaiz    EQU 512             ;/* entradas del directorio raiz  */
C_sectores16         EQU 65473           ;/* n. de sectores en total (16b) */
C_tipoDeMedio        EQU 0F8h            ;/* descriptor de medio           */
C_sectoresPorFAT     EQU 254             ;/* sectores pot FAT              */
C_sectoresPorPista   EQU 63              ;/* sectores por pista            */
C_cabezas            EQU 255             ;/* cabezas                       */
C_primerSector       EQU 63              ;/* primer sector de la particion */
C_sectores           EQU 00000000h       ;/* n. de sectores en total (32)  */
C_unidad             EQU 80h             ;/* unidad (0 -> A:, 0x80 -> C:)  */
C_relleno            EQU 00h             ;/* byte que no se usa            */
C_signatura          EQU 29h             ;/* extension de la signatura     */
C_numeroDeSerie      EQU 00000000h       ;/* numero de serie               */
C_etiqueta           EQU 'ETIQUETA   '   ;/* etiqueta de volumen (11 car.) */
C_SF                 EQU 'FAT16   '      ;/* tipo de sistema de ficheros   */


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

inicio: ;

  call _bootCode ;                        /* codigo del sector de arranque */

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

END
