/* ----------------------------------------------------------------------- */
/*                               ll_s_msj.c                                */
/* ----------------------------------------------------------------------- */
/*   biblioteca de funciones de interfaz de las llamadas al sistema SO1    */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\tipos.h>
#include <so1pub.h\ll_s_msj.h>                                /* mensaje_t */

#define ANY (-1) 

/* comparar con MINIX3\lib\i386\rts\_ipc.s */

int send ( pindx_t pindx, mensaje_t * msj ) 
{
    asm 
	{
		mov bx, pindx ;
	    mov dx, msj ;
		mov ax,SEND ;
		int nVIntSO1 ;
	}
	return(_AX) ;
}

int receive ( pindx_t pindx, mensaje_t * msj ) 
{
    asm 
	{
		mov bx, pindx ;
	    mov dx, msj ;
		mov ax,RECEIVE ;
		int nVIntSO1 ;
	}
	return(_AX) ;
}

int sendrec ( pindx_t pindx, mensaje_t * msj ) 
{
    asm 
	{
		mov bx, pindx ;
		mov dx, msj ;
		mov ax,SENDREC ;
		int nVIntSO1 ;
	}
	return(_AX) ;
}

int notify ( pindx_t pindx ) 
{
    asm 
	{
		mov bx, pindx ;
		mov ax,NOTIFY ;
		int nVIntSO1 ;
	}
	return(_AX) ;
}

int echo ( pindx_t pindx, mensaje_t * msj ) 
{
    asm 
	{
		mov bx, pindx ;
		mov dx, msj ;
		mov ax,ECHO ;		
		int nVIntSO1 ;
	}
	return(_AX) ;
}