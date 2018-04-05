	.286
	ifndef	??version
?debug	macro
	endm
publicdll macro	name
	public	name
	endm
$comm	macro	name,dist,size,count
	comm	dist name:BYTE:count*size
	endm
	else
$comm	macro	name,dist,size,count
	comm	dist name[size]:BYTE:count
	endm
	endif
	?debug	V 301h
	?debug	S "bpbfat12.c"
	?debug	C E9E6A64C4C0A62706266617431322E63
	?debug	C E988837B492F583A5C534F5C5052414354305C5F7062725C706272+
	?debug	C 66617431325C5F62706266617431325C6270626661+
	?debug	C 7431322E68
	?debug	C E9AA16704C1D583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C7469706F732E68
_TEXT	segment byte public 'CODE'
_TEXT	ends
DGROUP	group	_DATA,_BSS
	assume	cs:_TEXT,ds:DGROUP
_DATA	segment word public 'DATA'
d@	label	byte
d@w	label	word
_DATA	ends
_BSS	segment word public 'BSS'
b@	label	byte
b@w	label	word
_BSS	ends
_DATA	segment word public 'DATA'
	assume	 CS:_DATA
	_startBPB	 proc near
	_startBPB	 endp
	public	 _startBPB
	_instJMP	: jmp short _bootCode 
	_instNOP	: nop                         
	_OEM	                db 'BPB FD  ' 
	_bytesPorSector	     dw 512 
	_sectoresPorCluster	 db 1 
	_sectoresReservados	 dw 1 
	_numeroDeFATs	       db 2 
	_entradasDirRaiz	    dw 224 
	_sectores16	         dw 2880 
	_tipoDeMedio	        db 0F0H 
	_sectoresPorFAT	     dw 9 
	_sectoresPorPista	   dw 18 
	_cabezas	            dw 2 
	_primerSector	       dd 0 
	_sectores	           dd 000000000H 
	_unidad	             db 000H 
	_relleno	            db 000H 
	_signatura	          db 029H 
	_numeroDeSerie	      dd 000000000H 
	_etiqueta	           db 'ETIQUETA   ' 
	_SF	                 db 'FAT12   ' 
	public	 _OEM 
	public	 _bytesPorSector 
	public	 _sectoresPorCluster
	public	 _sectoresReservados
	public	 _numeroDeFATs
	public	 _entradasDirRaiz
	public	 _sectores16
	public	 _tipoDeMedio
	public	 _sectoresPorFAT
	public	 _sectoresPorPista
	public	 _cabezas
	public	 _primerSector
	public	 _sectores
	public	 _unidad
	public	 _relleno
	public	 _signatura
	public	 _numeroDeSerie
	public	 _etiqueta
	public	 _SF
	?debug	C E9
	?debug	C FA00000000
s@	label	byte
_DATA	ends
_TEXT	segment byte public 'CODE'
_TEXT	ends
	extrn	_bootCode:near
	end
