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
	?debug	S "..\so1\matrix.c"
	?debug	C E9B7AE61490F2E2E5C736F315C6D61747269782E63
	?debug	C E94BBE6A491B583A5C534F5C5052414354305C736F312E685C6D61+
	?debug	C 747269782E68
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
_TEXT	segment byte public 'CODE'
	assume	cs:_TEXT,ds:DGROUP
_matrix	proc	near
	?debug	C E8010F2E2E5C736F315C6D61747269782E63B7AE+
	?debug	C 6149
	?debug	C E8021B583A5C534F5C5052414354305C736F312E+
	?debug	C 685C6D61747269782E684BBE6A49
	?debug	C E801
   ;	
   ;	void matrix ( void )
   ;	
   ;	
   ;	{
   ;	    asm
   ;	    {
   ;	        dw NUM_MATRICULA dup(NUM_MATRICULA) ;
   ;	
	dw	 00123H dup(00123H) 
   ;	
   ;	    }
   ;	}
   ;	
	ret	
_matrix	endp
	?debug	C E9
	?debug	C FA00000000
_TEXT	ends
_DATA	segment word public 'DATA'
s@	label	byte
_DATA	ends
_TEXT	segment byte public 'CODE'
_TEXT	ends
	public	_matrix
	end
