/* ----------------------------------------------------------------------- */
/*                                 memory.h                                */
/* ----------------------------------------------------------------------- */
/*                  Funciones de manipulacion de memoria                   */
/* ----------------------------------------------------------------------- */

/* Copia los n bytes que comienzan en origen a partir de destino. Devuelve */
/* como resultado destino. Si hay solapamiento entre los intervalos        */
/* (origen, origen + n - 1) y (destino, destino + n -1) el resultado es    */
/* indefinido (ANSI).                                                      */   
/* Hay que asegurarse de que offset(origen)+n-1 y offset(destino)+n-1 no   */
/* son mayores de 0xFFFF.                                                  */         

#include <so1pub.h\tipos.h>                             /* word_t, dword_t */
#include <so1pub.h\memory.h> 

#if (TRUE)

void * memcpy_n ( void * destino, const void * origen, word_t n ) 
{
    asm 
	{
		mov si,origen ;
		mov di,destino ;
		mov cx,n ;
        shr cx,1 ;   /* flag de acarreo = bit 0 del valor de partida de cx */
        cld ;
        rep movsw ;
        jnc cpy_end ;
        movsb                                /* ultimo byte si n era impar */
cpy_end:
	}
    return(destino) ;		
}

#endif 

void far * memcpy ( void far * destino, const void far * origen, word_t n ) 
{
    asm 
	{
		mov dx,ds ;                                                /* save */
		lds si,origen ;
		les di,destino ;
		mov cx,n ;
        shr cx,1 ;   /* flag de acarreo = bit 0 del valor de partida de cx */
        cld ;
        rep movsw ;
        jnc cpy_end ;
        movsb                                /* ultimo byte si n era impar */
cpy_end:
        mov ds,dx ;                                             /* restore */
	}
    return(destino) ;		
}

void far * normaliza ( void far * dir ) 
{
//	while (TRUE) ;
	asm 
	{
		les si,dir ;         /* ES:SI ----> (ES + (SI >> 4)):(SI & 0x000F) */
		mov ax,es ;
		mov bx,si ;
		shr bx,4 ;
		add ax,bx ;
		mov dx,ax ;
		mov ax,si ;
		and ax,0x000F ;
		pop si
		pop bp
		ret ;
	}
//	return(MK_FP(_DX, _AX)) ; 
}

void far * memcpy_fd ( void far * destino, const void far * origen, dword_t n ) 
{
	void far * destinoAux = destino ;
	void far * origenAux = origen ;
	word_t min ;	
    do 
    {
  	    destinoAux = normaliza(destinoAux) ;
	    origenAux = normaliza(origenAux) ;
	    min = (n <= 0xFFF0) ? n : 0xFFF0 ; 
        destinoAux = memcpy(destinoAux, origenAux, min) ;
        (pointer_t)origenAux += min ;
		n = n - min ;
	}
	while (n > 0) ;
    return(destinoAux) ;		
}

void * memmove_n ( void * destino, const void * origen, word_t n ) 
{
//  asm push ds ;	                   /* para el caso de punteros lejanos */
	if (origen < destino) 
	asm 
	{
        std ;                                               /* hacia atras */
        mov ax, 1 ;			
	}	
	else 
	asm 
    {
		cld ;                                            /* hacia adelante */
		xor ax,ax ;
	}
	asm 
	{
//      lds si,origen ; 	           /* para el caso de punteros lejanos */
//      les di,origen ;                /* para el caso de punteros lejanos */
     	mov si,origen ;
		mov di, destino ;
		mov cx,n ;
        or ax, ax ;
        jz movit ;
        add si, cx ;   /* hacia atras; ajustar punteros al ultimo elemento */
        dec si ;
        add di, cx ;
        dec di
movit:
        test di, 1 ;
        jz isAligned ;
        jcxz done ;
        movsb ;
        dec cx ;
isAligned:
        sub si, ax ;                     /* compensacion al mover palabras */
        sub di, ax ;
        shr cx, 1 ;
        rep movsw ;
        jnc noOdd ;
        add si, ax ;               /* compensacion al mover el ultimo byte */
        add di, ax
        movsb ;
noOdd:
done:
        cld ;
    }		
//  asm pop ds ;	                   /* para el caso de punteros lejanos */	
	return(destino) ;
}