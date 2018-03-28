/* ----------------------------------------------------------------------- */
/*                               ll_s_msj.c                                */
/* ----------------------------------------------------------------------- */
/*   biblioteca de funciones de interfaz de las llamadas al sistema SO1    */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\tipos.h>

#define ANY (-1) 

/* comparar con MINIX3\lib\i386\rts\_ipc.s */

int send ( pindx_t pindx, void * msj ) 
{
    asm 
	{
		mov dx, pindx ;
		les bx, msj ;
		mov ax,0x0501 ;
		int nVIntSO1 ;
	}
}

int receive ( pindx_t pindx, void * msj ) 
{
    asm 
	{
		mov dx, pindx ;
		les bx, msj ;
		mov ax,0x0502 ;
		int nVIntSO1 ;
	}
}

int sendrec ( pindx_t pindx, void * msj ) 
{
    asm 
	{
		mov dx, pindx ;
		les bx, msj ;
		mov ax,0x0500 ;
		int nVIntSO1 ;
	}
}

int notify ( pindx_t pindx ) 
{
    asm 
	{
		mov dx, pindx ;
		mov ax,0x0503 ;
		int nVIntSO1 ;
	}
}

int echo ( pindx_t pindx, void * msj ) 
{
    asm 
	{
		mov dx, pindx ;
		les bx, msj ;
		mov ax,0x0504 ;		
		int nVIntSO1 ;
	}
}