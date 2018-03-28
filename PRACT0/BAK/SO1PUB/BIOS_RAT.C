/* ----------------------------------------------------------------------- */
/*                                bios_rat.c                               */
/* ----------------------------------------------------------------------- */
/*                                                                         */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\pantalla.h>
#include <so1pub.h\pic.h>
#include <so1pub.h\bios_rat.h>

#include <so1.h\ajustes.h>
#include <so1.h\procesos.h>
#include <so1.h\plot.h>

/* Raton (int 33h) */

static bool_t hayMouseBIOS ;

bool_t hayDriverRaton ( void ) 
{                                         /* 0xCF = codigo maquina de IRET */
    return(                                 
        **((pointer_t far *)(0x33*4)) != 0xCF
    ) ;
}

bool_t hayRatonBIOS ( void ) 
{
    return(hayMouseBIOS) ;
}

word_t resetRatonBIOS ( word_t * numBotones ) 
{
    word_t estado ;
    word_t num ;
    asm
	{
        mov ax,0000h ;
        int 33h ;
        mov num,bx ;
        mov estado,ax ;
	}
    *numBotones = num ;
    hayMouseBIOS = (estado == 0xFFFF) ;
    return(estado) ;
}

void mostrarCursorRatonBIOS ( void ) 
{
    asm 
	{ 
	    mov ax,0001h ;
        int 33h ;
	}
}

void ocultarCursorRatonBIOS ( void ) 
{
    asm 
	{
		mov ax,0002h ;
        int 33h ;
	}
}

void leerRatonBIOS ( word_t * W, word_t * X, word_t * Y ) 
{
    asm 
	{
		mov ax,0003h ;
        int 33h ;
	}
    *W = _BX ;                                    /* W = 00000000 00000MRL */
    *X = _CX ;
    *Y = _DX ;
}

void situarCursorRatonBIOS ( word_t X, word_t Y ) 
{
    asm 
	{
	    mov cx,X ;
        mov dx,Y ;
        mov ax,0004h ;
        int 33h ;
	}
}

void installMouseEventHandler ( handler_t handler ) 
{
    asm 
	{
		mov cx,007fh ;                       /* capturar todos los eventos */
        mov ax,000ch ;
        les dx,handler ;                         /* (mediante un CALL FAR) */
        int 33h ;
	}
}

void uninstallMouseEventHandler ( handler_t handler ) 
{
    asm 
	{   mov cx,0000h ;                        /* no capturar ningun evento */
        mov ax,000ch ;
        les dx,handler ;
        int 33h ;
    }
}

void versionRatonBIOS ( word_t * menor,
                        word_t * mayor,
                        word_t * tipo,
                        word_t * irq ) {
    asm 
    {
		mov ax,0024h ;
        int 33h ;
	}
    *menor = (_BX >> 8) ;
    *mayor = (_BX & 0x00FF) ;
    *tipo  = (_CX >> 8) ;
    *irq   = (_CX & 0x00FF) ;
}