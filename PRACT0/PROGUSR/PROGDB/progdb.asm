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
	?debug	S "progdb.c"
	?debug	C E9F4098C4C0870726F6764622E63
	?debug	C E9B91A6B4C20583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C6C6C5F735F736F312E68
	?debug	C E9AA16704C1D583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C7469706F732E68
	?debug	C E9A188884C20583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C6465665F70726F632E68
	?debug	C E9AA16704C1D583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C7469706F732E68
	?debug	C E95A19774C1D583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C636F6E73742E68
	?debug	C E95CBE5A4A1D583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C66636E746C2E68
	?debug	C E9AA16704C1D583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C7469706F732E68
	?debug	C E914A571481F583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C6465665F74726D2E68
	?debug	C E9AA16704C1D583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C7469706F732E68
	?debug	C E914A571481F583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C7363616E6E65722E68
	?debug	C E9AA16704C1D583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C7469706F732E68
	?debug	C E9478E7B4C1B583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C6332632E68
	?debug	C E9AA16704C1D583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C7469706F732E68
	?debug	C E926AE664C1B583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C6363622E68
	?debug	C E9AA16704C1D583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C7469706F732E68
	?debug	C E9FB0EFC461F583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C6465665F6D656D2E68
	?debug	C E9AA16704C1D583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C7469706F732E68
	?debug	C E928AA764C1E583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C6465665F73662E68
	?debug	C E9AA16704C1D583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C7469706F732E68
	?debug	C E90879694C1F583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C6465665F7261742E68
	?debug	C E9AA16704C1D583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C7469706F732E68
	?debug	C E9F60B6A4C20583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C6465665F70616E742E68
	?debug	C E9AA16704C1D583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C7469706F732E68
	?debug	C E94513664C20583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C62696F73646174612E68
	?debug	C E9AA16704C1D583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C7469706F732E68
	?debug	C E95CBE5A4A1D583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C66636E746C2E68
	?debug	C E94498774C20583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C6C6C5F735F6578632E68
	?debug	C E9AA16704C1D583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C7469706F732E68
	?debug	C E9A188884C20583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C6465665F70726F632E68
	?debug	C E936097B4C20583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C6C6C5F735F7369672E68
	?debug	C E9AA16704C1D583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C7469706F732E68
	?debug	C E9496E8A4C20583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C6C6C5F735F6D736A2E68
	?debug	C E9AA16704C1D583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C7469706F732E68
	?debug	C E90608724C1D583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C737464696F2E68
	?debug	C E93482854C1D583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C63747970652E68
	?debug	C E9C7785B4C1F583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C737472696E67732E68
	?debug	C E9AA16704C1D583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C7469706F732E68
	?debug	C E914A571481F583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C7363616E6E65722E68
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
_main	proc	near
	?debug	C E8010870726F6764622E63F4098C4C
	?debug	C E81020583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C6C6C5F735F736F312E68B91A6B4C
	?debug	C E8021D583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C7469706F732E68AA16704C
	?debug	C E810
	?debug	C E80820583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C6465665F70726F632E68A188884C
	?debug	C E8151D583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C7469706F732E68AA16704C
	?debug	C E808
	?debug	C E8161D583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C636F6E73742E685A19774C
	?debug	C E808
	?debug	C E8031D583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C66636E746C2E685CBE5A4A
	?debug	C E8171D583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C7469706F732E68AA16704C
	?debug	C E803
	?debug	C E808
	?debug	C E8041F583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C6465665F74726D2E6814A57148
	?debug	C E8181D583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C7469706F732E68AA16704C
	?debug	C E804
	?debug	C E808
	?debug	C E8051F583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C7363616E6E65722E6814A57148
	?debug	C E8191D583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C7469706F732E68AA16704C
	?debug	C E805
	?debug	C E808
	?debug	C E8061B583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C6332632E68478E7B4C
	?debug	C E81A1D583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C7469706F732E68AA16704C
	?debug	C E806
	?debug	C E808
	?debug	C E8071B583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C6363622E6826AE664C
	?debug	C E81B1D583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C7469706F732E68AA16704C
	?debug	C E807
	?debug	C E808
	?debug	C E810
	?debug	C E8091F583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C6465665F6D656D2E68FB0EFC46
	?debug	C E81C1D583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C7469706F732E68AA16704C
	?debug	C E809
	?debug	C E810
	?debug	C E80A1E583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C6465665F73662E6828AA764C
	?debug	C E81D1D583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C7469706F732E68AA16704C
	?debug	C E80A
	?debug	C E810
	?debug	C E80D1F583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C6465665F7261742E680879694C
	?debug	C E81E1D583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C7469706F732E68AA16704C
	?debug	C E80D
	?debug	C E80C20583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C6465665F70616E742E68F60B6A4C
	?debug	C E81F1D583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C7469706F732E68AA16704C
	?debug	C E80C
	?debug	C E80B20583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C62696F73646174612E684513664C
	?debug	C E8201D583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C7469706F732E68AA16704C
	?debug	C E80B
	?debug	C E80C
	?debug	C E80D
	?debug	C E810
	?debug	C E8211D583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C66636E746C2E685CBE5A4A
	?debug	C E810
	?debug	C E80E20583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C6C6C5F735F6578632E684498774C
	?debug	C E8221D583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C7469706F732E68AA16704C
	?debug	C E80E
	?debug	C E82320583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C6465665F70726F632E68A188884C
	?debug	C E80E
	?debug	C E810
	?debug	C E80F20583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C6C6C5F735F7369672E6836097B4C
	?debug	C E8241D583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C7469706F732E68AA16704C
	?debug	C E80F
	?debug	C E810
	?debug	C E801
	?debug	C E81120583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C6C6C5F735F6D736A2E68496E8A4C
	?debug	C E8251D583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C7469706F732E68AA16704C
	?debug	C E811
	?debug	C E801
	?debug	C E8121D583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C737464696F2E680608724C
	?debug	C E801
	?debug	C E8131D583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C63747970652E683482854C
	?debug	C E801
	?debug	C E8141F583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C737472696E67732E68C7785B4C
	?debug	C E8261D583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C7469706F732E68AA16704C
	?debug	C E814
	?debug	C E801
	?debug	C E8271F583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C7363616E6E65722E6814A57148
	?debug	C E801
   ;	
   ;	int main ( int argc, char * argv [ ] ) 
   ;	
	enter	24,0
   ;	
   ;	{
   ;		int df ;
   ;		word_t n ;
   ;	    int nbytes ;
   ;		int i, j, k ;
   ;		dword_t tam ;
   ;		dword_t pos ;
   ;		dword_t posDespues ;
   ;		
   ;		if (argc < 2) return(0) ;
   ;	
	cmp	word ptr [bp+4],2
	jge	short @1@3
	jmp	@1@30
@1@3:
   ;	
   ;		
   ;		df = open(argv[1], O_RDONLY) ; 
   ;	
	push	1
	push	ds
	mov	bx,word ptr [bp+6]
	push	word ptr [bx+2]
	call	near ptr _open
	add	sp,6
	mov	word ptr [bp-2],ax
   ;	
   ;		printf("\n\n open(\"%s\", O_RDONLY) = %i\n", argv[1], df) ;
   ;	
	push	word ptr [bp-2]
	mov	bx,word ptr [bp+6]
	push	word ptr [bx+2]
	push	ds
	push	offset DGROUP:s@
	call	near ptr _printf
	add	sp,8
   ;	
   ;		
   ;		if (df > 0) 
   ;	
	cmp	word ptr [bp-2],0
	jg	@@0
	jmp	@1@30
@@0:
   ;	
   ;		{
   ;	        k = 0 ;                              /* comando es una variable global */
   ;	
	mov	word ptr [bp-12],0
   ;	
   ;	        for ( i = 2 ; i < argc ; i ++ )
   ;	
	mov	word ptr [bp-8],2
	jmp	short @1@11
@1@5:
   ;	
   ;	        {
   ;	            for ( j = 0 ; argv[i][j] != '\0' ; j++ )
   ;	
	mov	word ptr [bp-10],0
	jmp	short @1@8
@1@6:
   ;	
   ;	                comando[0][k++] = toupper(argv[i][j]) ;
   ;	
	mov	bx,word ptr [bp-8]
	add	bx,bx
	add	bx,word ptr [bp+6]
	mov	bx,word ptr [bx]
	add	bx,word ptr [bp-10]
	mov	al,byte ptr [bx]
	cbw	
	push	ax
	call	near ptr _toupper
	pop	cx
	mov	bx,word ptr [bp-12]
	mov	byte ptr DGROUP:_comando[bx],al
	inc	word ptr [bp-12]
	inc	word ptr [bp-10]
@1@8:
	mov	bx,word ptr [bp-8]
	add	bx,bx
	add	bx,word ptr [bp+6]
	mov	bx,word ptr [bx]
	add	bx,word ptr [bp-10]
	cmp	byte ptr [bx],0
	jne	short @1@6
   ;	
   ;	            comando[0][k++] = ' ' ;
   ;	
	mov	bx,word ptr [bp-12]
	mov	byte ptr DGROUP:_comando[bx],32
	inc	word ptr [bp-12]
	inc	word ptr [bp-8]
@1@11:
	mov	ax,word ptr [bp-8]
	cmp	ax,word ptr [bp+4]
	jl	short @1@5
   ;	
   ;	        }
   ;	        comando[0][k--] = '\0' ;		
   ;	
	mov	bx,word ptr [bp-12]
	mov	byte ptr DGROUP:_comando[bx],0
	dec	word ptr [bp-12]
   ;	
   ;	        inicScanner() ;
   ;	
	call	near ptr _inicScanner
   ;	
   ;	        saltarBlancos() ;
   ;	
	call	near ptr _saltarBlancos
   ;	
   ;	        numDec() ;
   ;	
	call	near ptr _numDec
   ;	
   ;			if ((simb == s_numero) || (simb == s_numeroLargo)) 
   ;	
	cmp	word ptr DGROUP:_simb,19
	je	short @1@14
	cmp	word ptr DGROUP:_simb,20
	je	@@1
	jmp	@1@30
@@1:
@1@14:
   ;	
   ;			{
   ;	    		if (simb == s_numero) pos = num ;
   ;	
	cmp	word ptr DGROUP:_simb,19
	jne	short @1@16
	mov	ax,word ptr DGROUP:_num
	mov	word ptr [bp-18],0
	mov	word ptr [bp-20],ax
	jmp	short @1@19
@1@16:
   ;	
   ;	    		else if (simb == s_numeroLargo) pos = numLargo ;
   ;	
	cmp	word ptr DGROUP:_simb,20
	jne	short @1@18
	mov	dx,word ptr DGROUP:_numLargo+2
	mov	ax,word ptr DGROUP:_numLargo
	mov	word ptr [bp-18],dx
	mov	word ptr [bp-20],ax
	jmp	short @1@19
@1@18:
   ;	
   ;		    	else return(0) ;
   ;	
	jmp	@1@30
@1@19:
   ;	
   ;	            saltarBlancos() ;
   ;	
	call	near ptr _saltarBlancos
   ;	
   ;	            numDec() ;
   ;	
	call	near ptr _numDec
   ;	
   ;	    		if (simb != s_numero) return(0) ; 
   ;	
	cmp	word ptr DGROUP:_simb,19
	je	short @1@21
	jmp	@1@30
@1@21:
   ;	
   ;	            n = num ;			
   ;	
	mov	ax,word ptr DGROUP:_num
	mov	word ptr [bp-4],ax
   ;	
   ;				if (n > tamBufer) return(0) ;
   ;	
	cmp	word ptr [bp-4],5120
	jbe	short @1@23
	jmp	@1@30
@1@23:
   ;	
   ;				printf("\n pos = %li\n", pos) ;
   ;	
	push	word ptr [bp-18]
	push	word ptr [bp-20]
	push	ds
	push	offset DGROUP:s@+30
	call	near ptr _printf
	add	sp,8
   ;	
   ;	   	        posDespues = lseek(df, pos, SEEK_SET) ;
   ;	
	push	0
	push	word ptr [bp-18]
	push	word ptr [bp-20]
	push	word ptr [bp-2]
	call	near ptr _lseek
	add	sp,8
	mov	word ptr [bp-22],dx
	mov	word ptr [bp-24],ax
   ;	
   ;	         	printf("\n lseek(%i, %li, SEEK_SET) = %li\n", df, pos, posDespues) ;
   ;	
	push	word ptr [bp-22]
	push	word ptr [bp-24]
	push	word ptr [bp-18]
	push	word ptr [bp-20]
	push	word ptr [bp-2]
	push	ds
	push	offset DGROUP:s@+43
	call	near ptr _printf
	add	sp,14
   ;	
   ;	    	    nbytes = read(df, (pointer_t)&bufer, n) ;
   ;	
	push	word ptr [bp-4]
	push	ds
	push	offset DGROUP:_bufer
	push	word ptr [bp-2]
	call	near ptr _read
	add	sp,8
	mov	word ptr [bp-6],ax
   ;	
   ;				posDespues = lseek(df, 0, SEEK_CUR) ;
   ;	
	push	1
	push	0
	push	0
	push	word ptr [bp-2]
	call	near ptr _lseek
	add	sp,8
	mov	word ptr [bp-22],dx
	mov	word ptr [bp-24],ax
   ;	
   ;				printf("\n posDespues = %li\n", posDespues) ;
   ;	
	push	word ptr [bp-22]
	push	word ptr [bp-24]
	push	ds
	push	offset DGROUP:s@+77
	call	near ptr _printf
	add	sp,8
   ;	
   ;	   	        tam = lseek(df, 0, SEEK_END) ;
   ;	
	push	2
	push	0
	push	0
	push	word ptr [bp-2]
	call	near ptr _lseek
	add	sp,8
	mov	word ptr [bp-14],dx
	mov	word ptr [bp-16],ax
   ;	
   ;		        printf(
   ;	
   ;	
   ;				    "\n read(%i, &bufer, %i) = %i   pos = %li tam = %li\n", 
   ;				    df, n, nbytes, posDespues, tam
   ;				) ;
   ;	
	push	word ptr [bp-14]
	push	word ptr [bp-16]
	push	word ptr [bp-22]
	push	word ptr [bp-24]
	push	word ptr [bp-6]
	push	word ptr [bp-4]
	push	word ptr [bp-2]
	push	ds
	push	offset DGROUP:s@+97
	call	near ptr _printf
	add	sp,18
   ;	
   ;		
   ;	            for ( i = 0 ; i < nbytes ; i++ ) 
   ;	
	mov	word ptr [bp-8],0
	jmp	short @1@28
@1@24:
   ;	
   ;		        {
   ;	                if ( i % 16 == 0) printf("\n %06lX: ", pos + i ) ;	   
   ;	
	mov	ax,word ptr [bp-8]
	mov	bx,16
	cwd	
	idiv	bx
	or	dx,dx
	jne	short @1@26
	mov	ax,word ptr [bp-8]
	cwd	
	add	ax,word ptr [bp-20]
	adc	dx,word ptr [bp-18]
	push	dx
	push	ax
	push	ds
	push	offset DGROUP:s@+148
	call	near ptr _printf
	add	sp,8
@1@26:
   ;	
   ;	                printf("%02X ", bufer[i]) ;	
   ;	
	mov	bx,word ptr [bp-8]
	mov	al,byte ptr DGROUP:_bufer[bx]
	mov	ah,0
	push	ax
	push	ds
	push	offset DGROUP:s@+158
	call	near ptr _printf
	add	sp,6
	inc	word ptr [bp-8]
@1@28:
	mov	ax,word ptr [bp-8]
	cmp	ax,word ptr [bp-6]
	jl	short @1@24
   ;	
   ;		        }
   ;		     	printf("\n") ;
   ;	
	push	ds
	push	offset DGROUP:s@+164
	call	near ptr _printf
	add	sp,4
@1@30:
   ;	
   ;			}
   ;	    }	
   ;	#if (FALSE)	
   ;		aio_read(df, bufer, 512) ;
   ;		close(df) ;
   ;	
   ;		printf("\n leer tecla ... ") ;
   ;		getchar() ;
   ;		receive(1, &respuesta) ;
   ;		printf("\n\n respuesta = %s\n", respuesta.info) ;
   ;	#endif 
   ;	
   ;		return(0) ;
   ;	
	xor	ax,ax
   ;	
   ;	}
   ;	
	leave	
	ret	
_main	endp
_TEXT	ends
_BSS	segment word public 'BSS'
_respuesta	label	word
	db	32 dup (?)
_bufer	label	byte
	db	5120 dup (?)
	?debug	C E9
	?debug	C FA00000000
_BSS	ends
_DATA	segment word public 'DATA'
s@	label	byte
	db	10
	db	10
	db	' open("%s", O_RDONLY) = %i'
	db	10
	db	0
	db	10
	db	' pos = %li'
	db	10
	db	0
	db	10
	db	' lseek(%i, %li, SEEK_SET) = %li'
	db	10
	db	0
	db	10
	db	' posDespues = %li'
	db	10
	db	0
	db	10
	db	' read(%i, &bufer, %i) = %i   pos = %li tam = %li'
	db	10
	db	0
	db	10
	db	' %06lX: '
	db	0
	db	'%02X '
	db	0
	db	10
	db	0
_DATA	ends
_TEXT	segment byte public 'CODE'
_TEXT	ends
	extrn	_comando:byte
	extrn	_simb:word
	extrn	_num:word
	extrn	_numLargo:dword
	extrn	_inicScanner:near
	extrn	_saltarBlancos:near
	extrn	_numDec:near
	extrn	_open:near
	extrn	_lseek:near
	extrn	_read:near
	extrn	_printf:near
	extrn	_toupper:near
	public	_respuesta
	public	_bufer
	public	_main
	extrn	__setargv__:far
	end
