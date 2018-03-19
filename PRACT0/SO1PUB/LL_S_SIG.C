/* ----------------------------------------------------------------------- */
/*                               ll_s_sig.c                                */
/* ----------------------------------------------------------------------- */
/*   biblioteca de funciones de interfaz de las llamadas al sistema SO1    */
/* ----------------------------------------------------------------------- */

//#include <so1pub.h\ll_s_so1.h>
//#include <so1pub.h\ll_s_sig.h>

//#pragma option -w-argsused
#pragma option -w-par

void SIG_IGN ( int sig )             /* no se hace nada al recibir la señal */
{
}

void SIG_DFL ( int sig )        /* se mata al proceso tras recibir la señal */
{	
    exit(0) ;           /* en el estado de terminacion indicar la señal sig */
}

void __handlerReturn ( int errorAX ) 
{
	if (errorAX == 0) 
		return(_AX) ;
	else 
		return(errorAX) ;
}

__sighandler_t __sighandler [ ] = {
    SIG_DFL,  /*  1 */
    SIG_DFL,  /*  2 */
    SIG_DFL,  /*  3 */
    SIG_DFL,  /*  4 */
    SIG_DFL,  /*  5 */
    SIG_DFL,  /*  6 */
    SIG_DFL,  /*  7 */
    SIG_DFL,  /*  8 */
    SIG_DFL,  /*  9 */
    SIG_DFL,  /* 10 */
    SIG_DFL,  /* 11 */
    SIG_DFL,  /* 12 */
    SIG_DFL,  /* 13 */
    SIG_DFL,  /* 14 */
    SIG_DFL,  /* 15 */
    SIG_DFL,  /* 16 */
    SIG_DFL,  /* 17 */ 
    SIG_DFL,  /* 18 */ 
    SIG_DFL,  /* 19 */ 
    SIG_DFL   /* 20 */ 
} ; 

int kill ( pid_t pid, int sig ) 
{
    asm 
	{
		mov si,sig ;
        mov bx,pid ;
	    mov ax,0x0700 ;
        int nVIntSO1 ;
    }		
	return(_AX) ;
}

int sigaction ( int sig, const struct sigaction * act, struct sigaction * oact ) 
{
    asm 
	{
        mov si,sig ;
        mov bx,act ;
	    mov dx,oact ; 
	    mov ax,0x0701 ;
        int nVIntSO1 ;	
	}
	return(_AX) ;
}

unsigned int alarm ( unsigned int seconds ) 
{	
    asm 
	{
        mov cx,seconds ;
	    mov ax,0x0702 ;
        int nVIntSO1 ;
    }		
	return(_AX) ;
}

void pause ( void ) 
{
    asm 
	{
	    mov ax,0x0703 ;
        int nVIntSO1 ;	
	}
}

void sigsuspend ( int sig ) 
{
    asm 
	{
	    mov ax,0x0704 ;
        int nVIntSO1 ;	
	}
}