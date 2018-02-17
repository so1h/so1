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
	?debug	S "pbrfat16.c"
	?debug	C E9C509484A0A70627266617431362E63
	?debug	C E9149D714820583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C636172736374726C2E68
	?debug	C E9118B164B1D583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C7469706F732E68
	?debug	C E9B900484A1F583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C61736D70726F632E68
	?debug	C E9D67C7B4B2B583A5C534F5C5052414354305C5F70627266617431+
	?debug	C 365C5F62706266617431365C62706266617431362E+
	?debug	C 68
	?debug	C E9118B164B1D583A5C534F5C5052414354305C736F317075622E68+
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
 	_DATA	 ENDS 
 	_TEXT	 SEGMENT BYTE PUBLIC 'CODE' 
 	ASSUME	 CS:_TEXT,DS:DGROUP 
 	_startBin	 PROC NEAR 
 	jmp	 short _bootCode 
 	nop	 
 	org	 0003EH 
 	;	 
 	_startBin	 ENDP 
 	_TEXT	 ENDS 
 	public	 _startBin 
 	_DATA	 SEGMENT WORD PUBLIC 'DATA' 
_DATA	ends
_TEXT	segment byte public 'CODE'
	assume	cs:_TEXT,ds:DGROUP
_bootCode	proc	near
	?debug	C E8010A70627266617431362E63C509484A
	?debug	C E80420583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C636172736374726C2E68149D7148
	?debug	C E801
	?debug	C E8021D583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C7469706F732E68118B164B
	?debug	C E801
	?debug	C E8051F583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C61736D70726F632E68B900484A
	?debug	C E801
	?debug	C E8032B583A5C534F5C5052414354305C5F706272+
	?debug	C 66617431365C5F62706266617431365C62706266+
	?debug	C 617431362E68D67C7B4B
	?debug	C E8061D583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C7469706F732E68118B164B
	?debug	C E803
	?debug	C E801
   ;	
   ;	void bootCode ( void ) {                  /* recibe el cotrol desde el BPB */
   ;	
   ;	
   ;	  asm {
   ;	//  cli ;        /* inhibimos las interrupciones (se suponen ya inhibidas) */
   ;	    mov sp,0x9000 ;              /* establecemos la pila SS:SP = 9000:9000 */
   ;	
	mov	 sp,09000H 
   ;	
   ;	    mov ss,sp ;                                         /* por simplicidad */
   ;	
	mov	 ss,sp 
   ;	
   ;	//  mov bp,sp ;                                       /* SS:BP = 9000:9000 */
   ;	//  sti ;                   /* permitimos las interrupciones (innecesario) */
   ;	
   ;	#if (CARGA7C00)
   ;	    mov ax,7c00h SHR 4 ;                                            /* 7C0 */
   ;	
	mov	 ax,7c00h SHR 4 
   ;	
   ;	#else              /* funciona con carga del sector en cualquier paragrafo */
   ;	                     /* lo que es util para depurar PBRFAT16.EXE con el TD */
   ;	             /* Se presupone IP alineado a paragrafo: IP & 0x000F = 0x0000 */
   ;	    call $+3 ;                         /* apilamos la dirección de retorno */
   ;	    pop bx ;                           /* BX = IP de pop bx                */
   ;	    sub bx,$-1 ;                       /* BX = IP donde se cargo el sector */
   ;	    shr bx,4 ;
   ;	    mov ax,cs ;
   ;	    add ax,bx ;            /* AX:0000 = direccion donde se cargo el sector */
   ;	#endif
   ;	    mov ds,ax ;
   ;	
	mov	 ds,ax 
   ;	
   ;	    push ax ;
   ;	
	push	 ax 
   ;	
   ;	#if defined(JWASM) || defined(TASM32)
   ;	    push OFFSET main ;                                /* retornamos a main */
   ;	
	push	 OFFSET _main 
   ;	
   ;	#else
   ;	    mov ax,OFFSET main ;
   ;	    push ax ;
   ;	#endif
   ;	    retf ;                                 /* retorno lejano (a 07C0:main) */
   ;	
	retf	 
   ;	
   ;	  }                                             /* estableciendo CS a 07C0 */
   ;	}
   ;	
	ret	
_bootCode	endp
	assume	cs:_TEXT,ds:DGROUP
_printCar	proc	near
   ;	
   ;	void printCar ( char car ) {
   ;	
	push	bp
	mov	bp,sp
   ;	
   ;	  asm {
   ;	    mov al,car           /* car -> caracter a escribir                     */
   ;	
	mov	 al,[bp+4]           
   ;	
   ;	    mov ah,0eH           /* 0eH -> escribir caracter                       */
   ;	
	mov	 ah,0eH           
   ;	
   ;	    int 10H              /* 10H -> servicios BIOS de manejo de la pantalla */
   ;	
	int	 10H              
   ;	
   ;	  }
   ;	}
   ;	
	pop	bp
	ret	
_printCar	endp
	assume	cs:_TEXT,ds:DGROUP
_printStr	proc	near
   ;	
   ;	void printStr ( char * str ) {
   ;	
	enter	2,0
	jmp	short @3@5
@3@2:
   ;	
   ;	  char car ;
   ;	  while ((car = *str++) != (char)0) {
   ;	    if (car == '\n') printCar('\r') ;
   ;	
	cmp	byte ptr [bp-1],10
	jne	short @3@4
	push	13
	call	near ptr _printCar
	pop	cx
@3@4:
   ;	
   ;	    printCar(car) ;
   ;	
	mov	al,byte ptr [bp-1]
	push	ax
	call	near ptr _printCar
	pop	cx
@3@5:
	mov	bx,word ptr [bp+4]
	inc	word ptr [bp+4]
	mov	al,byte ptr [bx]
	mov	byte ptr [bp-1],al
	or	al,al
	jne	short @3@2
   ;	
   ;	  }
   ;	}
   ;	
	leave	
	ret	
_printStr	endp
	assume	cs:_TEXT,ds:DGROUP
_leerTecla	proc	near
   ;	
   ;	char leerTecla ( void ) {
   ;	
   ;	
   ;	//char car ;
   ;	  asm {
   ;	    mov ah,00H                  /* 00H -> leer siguiente tecla pulsada     */
   ;	
	mov	 ah,00H                  
   ;	
   ;	    int 16H                     /* 16H -> servicios BIOS de teclado        */
   ;	
	int	 16H                     
   ;	
   ;	//  mov car,al                  /* El caracter ascii se nos devuelve en AL */
   ;	  }
   ;	//return(car) ;
   ;	  return(_AL) ;
   ;	
   ;	
   ;	}
   ;	
	ret	
_leerTecla	endp
	assume	cs:_TEXT,ds:DGROUP
_bloqueDeParametros	proc	near
   ;	
   ;	void bloqueDeParametros ( void ) {
   ;	
   ;	
   ;	  asm {
   ;	//  dw tamBDP ;
   ;	    dw 16 ;                                                      /* tamBDP */
   ;	
	dw	 16 
   ;	
   ;	    dw 1 ;
   ;	
	dw	 1 
   ;	
   ;	    dw 0x0000, 0x07C0 ;
   ;	
	dw	 00000H, 007C0H 
   ;	
   ;	    dd 0x00000000, 0x00000000 ;
   ;	
	dd	 000000000H, 000000000H 
   ;	
   ;	  }
   ;	}
   ;	
	ret	
_bloqueDeParametros	endp
	assume	cs:_TEXT,ds:DGROUP
_leerSectorLBA	proc	near
   ;	
   ;	int leerSectorLBA ( dword_t sectorLogico,
   ;	
	push	bp
	mov	bp,sp
	push	si
	push	di
   ;	
   ;	//                  byte_t unidad,
   ;	                    pointer_t dir ) {
   ;	
   ;	//word_t inicioBDP = (word_t)&bloqueDeParametros ;
   ;	//word_t finBDP = (word_t)&bloqueDeParametros + tamBDP ;
   ;	
   ;	//bloqueDeParametros.tam = tamBDP ;       /* sizeof(bloqueDeParamnetros_t) */
   ;	//bloqueDeParametros.numSectores = 1 ;
   ;	  ((bloqueDeParametros_t *)&bloqueDeParametros)->dir = dir ;  /* cambia DL */
   ;	
	mov	dx,word ptr [bp+10]
	mov	ax,word ptr [bp+8]
	mov	word ptr cs:_bloqueDeParametros+6,dx
	mov	word ptr cs:_bloqueDeParametros+4,ax
   ;	
   ;	  ((bloqueDeParametros_t *)&bloqueDeParametros)->sectorLogicoL = sectorLogico ;
   ;	
	mov	dx,word ptr [bp+6]
	mov	ax,word ptr [bp+4]
	mov	word ptr cs:_bloqueDeParametros+10,dx
	mov	word ptr cs:_bloqueDeParametros+8,ax
   ;	
   ;	//bloqueDeParametros.sectorLogicoH = 0x00000000L ;
   ;	
   ;	  asm {
   ;	//  mov si,inicioBDP ;
   ;	    mov si,OFFSET bloqueDeParametros ;
   ;	
	mov	 si,OFFSET _bloqueDeParametros 
   ;	
   ;	//  mov di,finBDP ;
   ;	    mov di,si ;
   ;	
	mov	 di,si 
   ;	
   ;	    add di,16 ;
   ;	
	add	 di,16 
   ;	
   ;	//  mov dl,unidad ;
   ;	    mov dl,C_unidad ;
   ;	
	mov	 dl,080H 
   ;	
   ;	    mov al,1 ;
   ;	
	mov	 al,1 
   ;	
   ;	    mov ah,42h ;        /* 42H -> EXTENDED READ                            */
   ;	
	mov	 ah,42h 
   ;	
   ;	    int 13h ;           /* http://hdebruijn.soo.dto.tudelft.nl/newpage/interupt/out-0700.htm#0651 */
   ;	
	int	 13h 
   ;	
   ;	    jc errorAlLeer ;    /* el flag de acarreo C se activa en caso de error */
   ;	
	jc	short @6@11
   ;	
   ;	  }
   ;	  return(0) ;
   ;	
	xor	ax,ax
	jmp	short @6@12
@6@11:
   ;	
   ;	errorAlLeer:
   ;	  return(_AH) ;
   ;	
	mov	al,ah
	mov	ah,0
	jmp	short @6@12
@6@12:
   ;	
   ;	}
   ;	
	pop	di
	pop	si
	pop	bp
	ret	
_leerSectorLBA	endp
	assume	cs:_TEXT,ds:DGROUP
_incAddr	proc	near
   ;	
   ;	void incAddr ( address_t * p, word_t i ) {
   ;	
	push	bp
	mov	bp,sp
   ;	
   ;	  if (i > (0xFFFF - p->offset)) p->segment += 0x1000 ;
   ;	
	mov	bx,word ptr [bp+4]
	mov	ax,-1
	sub	ax,word ptr [bx]
	cmp	ax,word ptr [bp+6]
	jae	short @7@3
	mov	bx,word ptr [bp+4]
	add	word ptr [bx+2],4096
@7@3:
   ;	
   ;	  p->offset += i ;
   ;	
	mov	bx,word ptr [bp+4]
	mov	ax,word ptr [bp+6]
	add	word ptr [bx],ax
   ;	
   ;	}
   ;	
	pop	bp
	ret	
_incAddr	endp
_TEXT	ends
_DATA	segment word public 'DATA'
 	_DATA	 ENDS 
 	_TEXT	 SEGMENT BYTE PUBLIC 'CODE' 
 	ASSUME	 CS:_TEXT,DS:DGROUP 
 	_msg	 PROC NEAR 
	db	 0ah, 0dh, 'PBR P.P.L.R. (C) 2016', 0ah,  0ah,  0h 
 	_msg	 ENDP 
 	_TEXT	 ENDS 
 	public	 _msg 
 	_DATA	 SEGMENT WORD PUBLIC 'DATA' 
_DATA	ends
_TEXT	segment byte public 'CODE'
	assume	cs:_TEXT,ds:DGROUP
_main	proc	near
   ;	
   ;	void main ( void ) {                                            /* CS = DS */
   ;	
	push	si
	push	di
   ;	
   ;	
   ;	  /* La reubicacion del PBR puede hacerse realizando un bucle de copia o   */
   ;	  /* volviendo a leer el primer sector del disco en la direccion de        */
   ;	  /* de destino. En principio es mas rapido hacer la copia si la CPU es    */
   ;	  /* suficientemente rapida, pero en simuladores lentos como emu8086 es    */
   ;	  /* mucho mas rapido la lectura del disco simulada que la copia. Otra     */
   ;	  /* ventaja de la lectura del disco es que requiere menos código que la   */
   ;	  /* copia lejana, lo que es un hecho muy a tener en cuenta.               */
   ;	
   ;	#if (FALSE)
   ;	
   ;	  origen = 0x0000 ;                                           /*   DS:0000 */
   ;	
   ;	  destinoLejano = (pointer_t)MK_FP(0x9000, 0x0000) ;          /* 9000:0000 */
   ;	
   ;	  for ( i = 0 ; i < tamPBR ; i++ )   /* reubicacion del sector de arranque */
   ;	    *destinoLejano++ = *origen++ ;
   ;	
   ;	/* *destinoLejano++ = *origen++ ; es preferible a la instruccion:          */
   ;	/* destinoLejano[i] = origen[i] ; ya que requiere el uso de si (push si)   */
   ;	
   ;	#endif ;
   ;	
   ;	  macroCopia(0x0000, 0x9000, 512) ;
   ;	
 	mov	 si,00000H 
 	mov	 di,si 
 	mov	 ax,09000H 
 	mov	 es,ax 
 	mov	 cx,512 
 	rep movsb	 
   ;	
   ;	
   ;	#if (FALSE)
   ;	  leerSectorLBA(C_primerSector, /* C_unidad, */ (pointer_t)MK_FP(0x9000, 0x0000)) ;
   ;	#endif
   ;	
   ;	  asm {
   ;	    push 9000H                 /* Cedemos el control al sector de arranque */
   ;	
	push	 9000H                 
   ;	
   ;	                                                              /* reubicado */
   ;	#if defined(JWASM) || defined(TASM32)
   ;	    push OFFSET reubicado
   ;	
	push	 OFFSET _reubicado
   ;	
   ;	#else
   ;	    mov ax,OFFSET reubicado
   ;	    push ax
   ;	#endif
   ;	    retf
   ;	
	retf	
   ;	
   ;	  }
   ;	
   ;	}
   ;	
	pop	di
	pop	si
	ret	
_main	endp
	assume	cs:_TEXT,ds:DGROUP
_reubicado	proc	near
   ;	
   ;	void reubicado ( void ) {
   ;	
   ;	
   ;	
   ;	  asm {                   /* establecemos el segmento de datos DS igual CS */
   ;	    push cs ;
   ;	
	push	 cs 
   ;	
   ;	    pop ds ;
   ;	
	pop	 ds 
   ;	
   ;	  }
   ;	
   ;	//asm mov unidadBIOS,dl ;             /* el BIOS puso el numero de la unidad */
   ;	                                    /* de la que arranca en el registro DL */
   ;	//if (unidadBIOS != bpb.unidad) {
   ;	//if (C_unidad != bpb.unidad) {
   ;	#if (TRUE)
   ;	  if (_DL != bpb.unidad) {
   ;	
	cmp	dl,byte ptr DGROUP:_bpb+36
	je	short @9@5
   ;	
   ;	    printCar('E') ;
   ;	
	push	69
	call	near ptr _printCar
	pop	cx
   ;	
   ;	    leerTecla() ;
   ;	
	call	near ptr _leerTecla
   ;	
   ;	    rebootBIOS() ;
   ;	
 	int	 19h 
@9@5:
   ;	
   ;	  }
   ;	#endif
   ;	
   ;	//resetControler() ;               /* se supone ya hecho por parte del MBR */
   ;	
   ;	              /* Cargar el S.O. a partir de la dirección 0600H escribiendo */
   ;	              /* un punto tras la lectura de cada sector                   */
   ;	
   ;	  entrada = (entrada_t far *)MK_FP(0x9000, 0x0300) ;           /* ojo: BSS */
   ;	
	mov	word ptr DGROUP:_entrada+2,-28672
	mov	word ptr DGROUP:_entrada,768
   ;	
   ;	
   ;	  ptr = (pointer_t)MK_FP(0x0060, 0x0000) ;           /* 0060:0000 = 00600H */
   ;	
	mov	word ptr DGROUP:_ptr+2,96
	mov	word ptr DGROUP:_ptr,0
   ;	
   ;	
   ;	  sectorlogico = C_primerSectorDatos ;           /* primer sector de datos */
   ;	
	mov	word ptr DGROUP:_sectorlogico,604
   ;	
   ;	
   ;	  leerSectorLBA(C_primerSectorDirRaiz, /* _DL, */ (pointer_t)entrada) ;
   ;	
	push	word ptr DGROUP:_entrada+2
	push	word ptr DGROUP:_entrada
	push	0
	push	572
	call	near ptr _leerSectorLBA
	add	sp,8
   ;	
   ;	  nsects = (entrada[0].tam.reg.med/2)+1 ;
   ;	
	les	bx,dword ptr DGROUP:_entrada
	mov	ax,word ptr es:[bx+29]
	shr	ax,1
	inc	ax
	mov	word ptr DGROUP:_nsects,ax
   ;	
   ;	
   ;	/* grldr (0.4.4)  funciona, aunque no detecta el fichero de configuracion  */
   ;	/*   menu.lst hasta que no se ejecuta el comando: root (fd0). (3 MB Ok).   */
   ;	/* grldr (0.4.5c) funciona si cuenta con al menos 35 megas de RAM.         */
   ;	/* grldr (0.4.6a) no funciona con BOOT_9 (usar bootlace).                  */
   ;	
   ;	#if (TRUE)
   ;	  printStrMacro(msg) ;
   ;	
 	push	 offset _msg 
 	call	 near ptr _printStr 
 	pop	 cx 
   ;	
   ;	#else
   ;	  printStr((char *)msg) ;
   ;	#endif
   ;	
   ;	  for ( i = 0 ; i < nsects ; i++ ) {
   ;	
	mov	word ptr DGROUP:_i,0
	jmp	short @9@12
@9@10:
   ;	
   ;	    leerSectorLBA(sectorlogico++, /* C_unidad, */ ptr) ;    /* unidad 0x80 */
   ;	
	push	word ptr DGROUP:_ptr+2
	push	word ptr DGROUP:_ptr
	mov	ax,word ptr DGROUP:_sectorlogico
	inc	word ptr DGROUP:_sectorlogico
	push	0
	push	ax
	call	near ptr _leerSectorLBA
	add	sp,8
   ;	
   ;	    printCar('.') ;
   ;	
	push	46
	call	near ptr _printCar
	pop	cx
   ;	
   ;	    incAddr((address_t *)&ptr, C_bytesPorSector) ;
   ;	
	push	512
	push	offset DGROUP:_ptr
	call	near ptr _incAddr
	add	sp,4
	inc	word ptr DGROUP:_i
@9@12:
	mov	ax,word ptr DGROUP:_i
	cmp	ax,word ptr DGROUP:_nsects
	jb	short @9@10
   ;	
   ;	  }
   ;	
   ;	  asm {
   ;	//  mov dl,unidadBIOS ;  /* Transmitimos el numero de la unidad de aranque */
   ;	    mov dl,C_unidad ;    /* Transmitimos el numero de la unidad de aranque */
   ;	
	mov	 dl,080H 
   ;	
   ;	    push 0060H ;      /* Cedemos el control al S.O. en 0060:0000 = 000600H */
   ;	
	push	 0060H 
   ;	
   ;	    push 0000H ;
   ;	
	push	 0000H 
   ;	
   ;	    retf ;
   ;	
	retf	 
   ;	
   ;	  }
   ;	
   ;	  asm org 0x1FF ;
   ;	
 	org	 01FFH 
   ;	
   ;	
   ;	}
   ;	
	ret	
_reubicado	endp
_TEXT	ends
_BSS	segment word public 'BSS'
_bpb	label	word
	db	62 dup (?)
_codigo	label	byte
	db	448 dup (?)
_signaturaPBR	label	byte
	db	2 dup (?)
_error	label	byte
	db	1 dup (?)
	db	1 dup (?)
_i	label	word
	db	2 dup (?)
_entrada	label	dword
	db	4 dup (?)
_ptr	label	dword
	db	4 dup (?)
_sectorlogico	label	word
	db	2 dup (?)
_nsects	label	word
	db	2 dup (?)
	?debug	C E9
	?debug	C FA00000000
_BSS	ends
_DATA	segment word public 'DATA'
s@	label	byte
_DATA	ends
_TEXT	segment byte public 'CODE'
_TEXT	ends
	public	_bootCode
	public	_bpb
	public	_codigo
	public	_signaturaPBR
	public	_main
	public	_printCar
	public	_printStr
	public	_leerTecla
	public	_error
	public	_bloqueDeParametros
	public	_leerSectorLBA
	public	_incAddr
	public	_reubicado
	public	_i
	public	_entrada
	public	_ptr
	public	_sectorlogico
	public	_nsects
	end
