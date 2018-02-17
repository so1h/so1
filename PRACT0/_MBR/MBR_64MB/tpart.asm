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
	?debug	S "tpart.c"
	?debug	C E912854C4C0774706172742E63
	?debug	C E9118B164B1D583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C7469706F732E68
	?debug	C E9A0767B4B1D583A5C534F5C5052414354305C5F6D62725C6D6272+
	?debug	C 5F325C6D62722E68
	?debug	C E9118B164B1D583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 2F7469706F732E68
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
_startBin	label	word
	db	128
	db	1
	db	1
	db	0
	db	6
	db	15
	db	63
	db	129
	db	63
	db	0
	db	0
	db	0
	db	160
	db	255
	db	1
	db	0
	db	0
	db	0
	db	0
	db	0
	db	0
	db	0
	db	0
	db	0
	db	0
	db	0
	db	0
	db	0
	db	0
	db	0
	db	0
	db	0
	db	0
	db	0
	db	0
	db	0
	db	0
	db	0
	db	0
	db	0
	db	0
	db	0
	db	0
	db	0
	db	0
	db	0
	db	0
	db	0
	db	0
	db	0
	db	0
	db	0
	db	0
	db	0
	db	0
	db	0
	db	0
	db	0
	db	0
	db	0
	db	0
	db	0
	db	0
	db	0
	?debug	C E9
	?debug	C FA00000000
s@	label	byte
_DATA	ends
_TEXT	segment byte public 'CODE'
_TEXT	ends
	public	_startBin
	end
