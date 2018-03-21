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
	?debug	S "startbss.c"
	?debug	C E9149D71480A73746172746273732E63
	?debug	C E9149D714820583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C73746172746273732E68
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
	_BSS	 segment word public 'BSS'
	assume	 NOTHING
	_startBSS	 proc near
	_startBSS	 endp
	public	 _startBSS
	_BSS	 ends
	?debug	C E9
	?debug	C FA00000000
s@	label	byte
_DATA	ends
_TEXT	segment byte public 'CODE'
_TEXT	ends
	end
