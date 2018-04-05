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
	?debug	S "mbr.c"
	?debug	C E9170B554C056D62722E63
	?debug	C E9AA16704C1D583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C7469706F732E68
	?debug	C E9B908484A1F583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C61736D70726F632E68
	?debug	C E9A07E7B4B20583A5C534F5C5052414354305C5F6D62725C6D6272+
	?debug	C 5F33326D625C6D62722E68
	?debug	C E9AA16704C1D583A5C534F5C5052414354305C736F317075622E68+
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
_TEXT	segment byte public 'CODE'
	assume	cs:_TEXT,ds:DGROUP
_startBin	proc	near
	?debug	C E801056D62722E63170B554C
	?debug	C E8021D583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C7469706F732E68AA16704C
	?debug	C E801
	?debug	C E8041F583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C61736D70726F632E68B908484A
	?debug	C E801
	?debug	C E80320583A5C534F5C5052414354305C5F6D6272+
	?debug	C 5C6D62725F33326D625C6D62722E68A07E7B4B
	?debug	C E8051D583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E682F7469706F732E68AA16704C
	?debug	C E803
	?debug	C E801
   ;	
   ;	void startBin ( void ) {            /* punto de entrada del programa (MBR) */
   ;	
   ;	
   ;	  asm {
   ;	//  cli ;             /* inhibimos las interrupciones (ya estan inhibidas) */
   ;	    mov ax,dirLinealCargaMBR SHR 4 ;                          /* CS = 0000 */
   ;	
	mov	 ax,07C00H SHR 4 
   ;	
   ;	    mov ds,ax ;                                               /* DS = 07C0 */
   ;	
	mov	 ds,ax 
   ;	
   ;	    mov ax,0x9000
   ;	
	mov	 ax,09000H
   ;	
   ;	    mov ss,ax ;                                               /* SS = 9000 */
   ;	
	mov	 ss,ax 
   ;	
   ;	    mov sp,ax ;              /* establecemos la pila SS:SP = 0x9000:0x9000 */
   ;	
	mov	 sp,ax 
   ;	
   ;	  }
   ;	  saltarA(ds, OFFSET main) ;      /* se cede el control a main (07C0:main) */
   ;	
 	push	 ds 
 	push	 OFFSET _main 
 	retf	 
   ;	
   ;	}
   ;	
	ret	
_startBin	endp
	assume	cs:_TEXT,ds:DGROUP
_printCar	proc	near
   ;	
   ;	void printCar ( char car ) {  asm {
   ;	
	push	bp
	mov	bp,sp
   ;	
   ;	    mov al,car ;     /* car -> caracter a escribir                         */
   ;	
	mov	 al,[bp+4] 
   ;	
   ;	    mov bl,0x07 ;    /* 0x07 -> atributo normal                            */
   ;	
	mov	 bl,007H 
   ;	
   ;	    mov ah,0x0e ;    /* 0x0e -> escribir caracter                          */
   ;	
	mov	 ah,00eH 
   ;	
   ;	    int 0x10 ;       /* 0x10 -> servicios BIOS de manejo de la pantalla    */
   ;	
	int	 010H 
   ;	
   ;	  }
   ;	}
   ;	
	pop	bp
	ret	
_printCar	endp
_TEXT	ends
_DATA	segment word public 'DATA'
 	_DATA	 ENDS 
 	_TEXT	 SEGMENT BYTE PUBLIC 'CODE' 
 	ASSUME	 CS:_TEXT,DS:DGROUP 
 	_msg	 PROC NEAR 
	db	 0ah, 0dh, 'MBR P.P.L.R. (C) 2016', 0ah, 0dh, 0 
 	_msg	 ENDP 
 	_TEXT	 ENDS 
 	public	 _msg 
 	_DATA	 SEGMENT WORD PUBLIC 'DATA' 
 	_DATA	 ENDS 
 	_TEXT	 SEGMENT BYTE PUBLIC 'CODE' 
 	ASSUME	 CS:_TEXT,DS:DGROUP 
 	_msg_Load	 PROC NEAR 
	db	 0ah, 0dh, 'Loading PBR ...', 0ah, 0dh, 0 
 	_msg_Load	 ENDP 
 	_TEXT	 ENDS 
 	public	 _msg_Load 
 	_DATA	 SEGMENT WORD PUBLIC 'DATA' 
 	_DATA	 ENDS 
 	_TEXT	 SEGMENT BYTE PUBLIC 'CODE' 
 	ASSUME	 CS:_TEXT,DS:DGROUP 
 	_msg_ReadError	 PROC NEAR 
	db	 0ah, 0dh, 'E', 0ah,  0ah, 0dh, 0 
 	_msg_ReadError	 ENDP 
 	_TEXT	 ENDS 
 	public	 _msg_ReadError 
 	_DATA	 SEGMENT WORD PUBLIC 'DATA' 
 	_DATA	 ENDS 
 	_TEXT	 SEGMENT BYTE PUBLIC 'CODE' 
 	ASSUME	 CS:_TEXT,DS:DGROUP 
 	_msg_NoPartition	 PROC NEAR 
	db	 0ah, 0dh, 'No A P Found', 0ah,  0ah, 0dh, 0 
 	_msg_NoPartition	 ENDP 
 	_TEXT	 ENDS 
 	public	 _msg_NoPartition 
 	_DATA	 SEGMENT WORD PUBLIC 'DATA' 
 	_DATA	 ENDS 
 	_TEXT	 SEGMENT BYTE PUBLIC 'CODE' 
 	ASSUME	 CS:_TEXT,DS:DGROUP 
 	_msg_PartitionError	 PROC NEAR 
	db	 0ah, 0dh, 'PT Error', 0ah,  0ah, 0dh, 0 
 	_msg_PartitionError	 ENDP 
 	_TEXT	 ENDS 
 	public	 _msg_PartitionError 
 	_DATA	 SEGMENT WORD PUBLIC 'DATA' 
_DATA	ends
_TEXT	segment byte public 'CODE'
	assume	cs:_TEXT,ds:DGROUP
_printStr	proc	near
   ;	
   ;	void printStr ( char * str ) {
   ;	
	push	bp
	mov	bp,sp
	jmp	short @3@5
@3@2:
   ;	
   ;	  while ((car = *str++) != (char)0) {
   ;	    if (car == '\n')                  /* '\n' caracter de avance de linea  */
   ;	
	cmp	byte ptr DGROUP:_car,10
	jne	short @3@4
   ;	
   ;	      printCar('\r') ;                /* '\r' caracter de retorno de carro */
   ;	
	push	13
	call	near ptr _printCar
	pop	cx
@3@4:
   ;	
   ;	    printCar(car) ;
   ;	
	mov	al,byte ptr DGROUP:_car
	push	ax
	call	near ptr _printCar
	pop	cx
@3@5:
	mov	bx,word ptr [bp+4]
	inc	word ptr [bp+4]
	mov	al,byte ptr [bx]
	mov	byte ptr DGROUP:_car,al
	or	al,al
	jne	short @3@2
   ;	
   ;	  }
   ;	}
   ;	
	pop	bp
	ret	
_printStr	endp
	assume	cs:_TEXT,ds:DGROUP
_leerTecla	proc	near
   ;	
   ;	char leerTecla ( void ) {
   ;	
   ;	
   ;	//static char car ;               /* consume menos bytes del MBR asi (BSS) */
   ;	  asm {
   ;	    mov ah,0x00 ;        /* 0x00 -> leer siguiente tecla pulsada           */
   ;	
	mov	 ah,000H 
   ;	
   ;	    int 0x16 ;           /* 0x16 -> servicios BIOS de teclado              */
   ;	
	int	 016H 
   ;	
   ;	//  mov car,al ;         /* El caracter ascii se nos devuelve en AL        */
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
_resetController	proc	near
   ;	
   ;	int resetController ( void ) {
   ;	
   ;	
   ;	  asm {
   ;	    mov ah,0x00 ;       /* 0x00 -> reset del controlador                   */
   ;	
	mov	 ah,000H 
   ;	
   ;	    int 0x13 ;          /* BIOS: E/S de disco                              */
   ;	
	int	 013H 
   ;	
   ;	    jc resetError ;     /* el flag de acarreo C se activa en caso de error */
   ;	
	jc	short @5@4
   ;	
   ;	  }
   ;	  return(0) ;
   ;	
	xor	ax,ax
	ret	
@5@4:
   ;	
   ;	resetError:
   ;	  return(1) ;
   ;	
	mov	ax,1
   ;	
   ;	}
   ;	
	ret	
_resetController	endp
	assume	cs:_TEXT,ds:DGROUP
_leerPrimerSectorF	proc	near
   ;	
   ;	int leerPrimerSectorF ( void ) {
   ;	
   ;	
   ;	
   ;	//cabeza   = cabezaInicial ;
   ;	//sector   = sectorInicial ;
   ;	//cilindro = cilindroInicial ;
   ;	
   ;	//sc = *((word_t *)&sectorInicial) ;                    /* sector+cilindro */
   ;	                                                            /* codificados */
   ;	  asm {
   ;	    les bx,dirCarga ;              /* pone en ES:BX la direccion del bufer */
   ;	
	les	 bx,DGROUP:_dirCarga 
   ;	
   ;	//  mov dl,unidadBIOS ;           /* numero de unidad indicado por el BIOS */
   ;	    mov dh,cabezaInicial ;                                       /* cabeza */
   ;	
	mov	 dh,DGROUP:_cabezaInicial 
   ;	
   ;	//  mov cx,sc ;                            /* codificacion sector/cilindro */
   ;	    mov cx,scInicial ;                     /* codificacion sector/cilindro */
   ;	
	mov	 cx,DGROUP:_scInicial 
   ;	
   ;	    mov al,1 ;                                /* numero de sectores a leer */
   ;	
	mov	 al,1 
   ;	
   ;	    mov ah,0x02 ;                          /* 0x02 -> lectura de un sector */
   ;	
	mov	 ah,002H 
   ;	
   ;	    int 0x13 ;                                       /* BIOS: E/S de disco */
   ;	
	int	 013H 
   ;	
   ;	    jc errorAlLeer ;    /* el flag de acarreo C se activa en caso de error */
   ;	
	jc	short @6@8
   ;	
   ;	  }
   ;	  return(0) ;
   ;	
	xor	ax,ax
	ret	
@6@8:
   ;	
   ;	errorAlLeer:
   ;	  return(1) ;
   ;	
	mov	ax,1
   ;	
   ;	}
   ;	
	ret	
_leerPrimerSectorF	endp
	assume	cs:_TEXT,ds:DGROUP
_main	proc	near
   ;	
   ;	void main ( void ) {                                    /* CS = DS = 0x7C0 */
   ;	
   ;	
   ;	
   ;	  origen  = (byte_t *)0x0000 ;                        /* 0x07C0:0 = 0x7C00 */
   ;	
	mov	word ptr DGROUP:_origen,0
   ;	
   ;	  destinoLejano = (pointer_t)MK_FP(_SS,0x0000) ;     /* 0x9000:0 = 0x90000 */
   ;	
	mov	word ptr DGROUP:_destinoLejano+2,ss
	mov	word ptr DGROUP:_destinoLejano,0
   ;	
   ;	
   ;	  for ( i = 0 ; i < tamMBR ; i++ )      /* reubicacion del mbr a 9000:0000 */
   ;	
	mov	word ptr DGROUP:_i,0
	jmp	short @7@3
@7@1:
   ;	
   ;	    *destinoLejano++ = *origen++ ;
   ;	
	mov	bx,word ptr DGROUP:_origen
	mov	al,byte ptr [bx]
	les	bx,dword ptr DGROUP:_destinoLejano
	mov	byte ptr es:[bx],al
	inc	word ptr DGROUP:_origen
	inc	word ptr DGROUP:_destinoLejano
	inc	word ptr DGROUP:_i
@7@3:
	cmp	word ptr DGROUP:_i,512
	jl	short @7@1
   ;	
   ;	
   ;	  saltarA(ss, OFFSET reubicado) ;      /* Cedemos el control al sector de */
   ;	
 	push	 ss 
 	push	 OFFSET _reubicado 
 	retf	 
   ;	
   ;	                               /* arranque reubicado en el segmento 0x0060 */
   ;	}
   ;	
	ret	
_main	endp
	assume	cs:_TEXT,ds:DGROUP
_reubicado	proc	near
   ;	
   ;	void reubicado ( void ) {                 /* CS = SS = 0x9000, DS = 0x7C00 */
   ;	
   ;	
   ;	
   ;	  asm {
   ;	    push ss ;
   ;	
	push	 ss 
   ;	
   ;	    pop ds ;
   ;	
	pop	 ds 
   ;	
   ;	  }                                               /* CS = DS = SS = 0x9000 */
   ;	
   ;	  setVideoMode() ;
   ;	
 	mov	 ax, 00003H 
 	int	 010H 
   ;	
   ;	
   ;	//printStr((char *)msg) ;
   ;	  printStrMacro(msg) ;
   ;	
 	push	 offset _msg 
 	call	 near ptr _printStr 
 	pop	 cx 
   ;	
   ;	//leerTecla() ;
   ;	
   ;	//asm mov unidadBIOS,dl ;                    /* guardamos dl en unidadBIOS */
   ;	
   ;	  origenDesc = (byte_t *)&descParticion ;
   ;	
	mov	word ptr DGROUP:_origenDesc,offset DGROUP:_descParticion
   ;	
   ;	
   ;	  for ( i = 0 ; i < 4 ; i++ ) {
   ;	
	mov	word ptr DGROUP:_i,0
	jmp	@8@31
@8@8:
   ;	
   ;	
   ;	    destinoDesc = (byte_t *)&activa ;
   ;	
	mov	word ptr DGROUP:_destinoDesc,offset DGROUP:_activa
   ;	
   ;	    for ( j = 0 ; j < sizeof(descParticion_t) ; j++ )
   ;	
	mov	word ptr DGROUP:_j,0
	jmp	short @8@11
@8@9:
   ;	
   ;	      *destinoDesc++ = *origenDesc++ ;
   ;	
	mov	bx,word ptr DGROUP:_origenDesc
	mov	al,byte ptr [bx]
	mov	bx,word ptr DGROUP:_destinoDesc
	mov	byte ptr [bx],al
	inc	word ptr DGROUP:_origenDesc
	inc	word ptr DGROUP:_destinoDesc
	inc	word ptr DGROUP:_j
@8@11:
	cmp	word ptr DGROUP:_j,16
	jl	short @8@9
   ;	
   ;	
   ;	    if (activa == 0x80) {
   ;	
	cmp	byte ptr DGROUP:_activa,128
	jne	short @8@25
   ;	
   ;	//    printStr((char *)msg_Load) ;
   ;	      printStrMacro(msg_Load) ;
   ;	
 	push	 offset _msg_Load 
 	call	 near ptr _printStr 
 	pop	 cx 
   ;	
   ;	//    leerTecla() ;
   ;	      resetController() ;                /* Hacer un reset del controlador */
   ;	
	call	near ptr _resetController
   ;	
   ;	//    leerPrimerSector(unidadBIOS, MK_FP(0x0000, 0x7C00)) ;    /* leer PBR */
   ;	      dirCarga = MK_FP(0x0000, 0x7C00) ;
   ;	
	mov	word ptr DGROUP:_dirCarga+2,0
	mov	word ptr DGROUP:_dirCarga,31744
   ;	
   ;	      if (leerPrimerSector(i, _DL, dirCarga)) {
   ;	
	call	near ptr _leerPrimerSectorF
	or	ax,ax
	je	short @8@21
   ;	
   ;	//      printStr((char *)msg_ReadError) ;
   ;	        printStrMacro(msg_ReadError) ;
   ;	
 	push	 offset _msg_ReadError 
 	call	 near ptr _printStr 
 	pop	 cx 
   ;	
   ;	        goto fin ;
   ;	
	jmp	short @8@35
@8@21:
   ;	
   ;	      }
   ;	//    asm mov dl,unidadBIOS ;           /* pasamos la unidad al PBR en DL. */
   ;	                                        /* Se cede el control al sector de */
   ;	                                        /* arranque de la particion activa */
   ;	                                        /* cargado en 0000:7C00 = 0x7C00   */
   ;	//    asm xor ax,ax ;
   ;	//    asm mov ds,ax ;
   ;	//    asm mov ss,ax ;
   ;	//    asm mov sp,ax ;
   ;	
   ;	      saltarA(ax, 0x7C00) ;
   ;	
 	push	 ax 
 	push	 07C00H 
 	retf	 
   ;	
   ;	    }
   ;	
	jmp	short @8@30
@8@25:
   ;	
   ;	    else if (activa != 0x00) {
   ;	
	cmp	byte ptr DGROUP:_activa,0
	je	short @8@30
   ;	
   ;	//    printStr((char *)msg_PartitionError) ;
   ;	      printStrMacro(msg_PartitionError) ;
   ;	
 	push	 offset _msg_PartitionError 
 	call	 near ptr _printStr 
 	pop	 cx 
   ;	
   ;	      goto fin ;
   ;	
	jmp	short @8@35
@8@30:
	inc	word ptr DGROUP:_i
@8@31:
	cmp	word ptr DGROUP:_i,4
	jge	@@0
	jmp	@8@8
@@0:
   ;	
   ;	    }
   ;	  }
   ;	
   ;	//printStr((char *)msg_NoPartition) ;   /* no hay ninguna particion activa */
   ;	  printStrMacro(msg_NoPartition) ;      /* no hay ninguna particion activa */
   ;	
 	push	 offset _msg_NoPartition 
 	call	 near ptr _printStr 
 	pop	 cx 
@8@35:
   ;	
   ;	
   ;	fin: ;
   ;	  leerTecla() ;
   ;	
	call	near ptr _leerTecla
   ;	
   ;	  reboot() ;
   ;	
 	int	 019H 
   ;	
   ;	
   ;	  asm org 0x01FF ;  /* para alinear a multiplo de 16 el tamanio del codigo */
   ;	
 	org	 001FFH 
   ;	
   ;	
   ;	}
   ;	
	ret	
_reubicado	endp
_TEXT	ends
_BSS	segment word public 'BSS'
_codigo	label	byte
	db	440 dup (?)
_WinDiskSig	label	dword
	db	4 dup (?)
_relleno	label	word
	db	2 dup (?)
_descParticion	label	word
	db	64 dup (?)
_signatura	label	word
	db	2 dup (?)
_activa	label	byte
	db	1 dup (?)
_cabezaInicial	label	byte
	db	1 dup (?)
_scInicial	label	word
	db	2 dup (?)
_tipo	label	byte
	db	1 dup (?)
_cabezaFinal	label	byte
	db	1 dup (?)
_sectorFinal	label	byte
	db	1 dup (?)
_cilindroFinal	label	byte
	db	1 dup (?)
_primerSector	label	dword
	db	4 dup (?)
_sectores	label	dword
	db	4 dup (?)
_unidadBIOS	label	byte
	db	1 dup (?)
_car	label	byte
	db	1 dup (?)
_dirCarga	label	dword
	db	4 dup (?)
_origen	label	word
	db	2 dup (?)
_destinoLejano	label	dword
	db	4 dup (?)
_origenDesc	label	word
	db	2 dup (?)
_destinoDesc	label	word
	db	2 dup (?)
_i	label	word
	db	2 dup (?)
_j	label	word
	db	2 dup (?)
	?debug	C E9
	?debug	C FA00000000
_BSS	ends
_DATA	segment word public 'DATA'
s@	label	byte
_DATA	ends
_TEXT	segment byte public 'CODE'
_TEXT	ends
	public	_codigo
	public	_WinDiskSig
	public	_relleno
	public	_descParticion
	public	_signatura
	public	_activa
	public	_cabezaInicial
	public	_scInicial
	public	_tipo
	public	_cabezaFinal
	public	_sectorFinal
	public	_cilindroFinal
	public	_primerSector
	public	_sectores
	public	_unidadBIOS
	public	_main
	public	_startBin
	public	_printCar
	public	_car
	public	_printStr
	public	_leerTecla
	public	_resetController
	public	_dirCarga
	public	_leerPrimerSectorF
	public	_reubicado
	public	_origen
	public	_destinoLejano
	public	_origenDesc
	public	_destinoDesc
	public	_i
	public	_j
	end
