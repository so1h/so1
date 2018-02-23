/* ----------------------------------------------------------------------- */
/*                                  dmp.c                                  */
/* ----------------------------------------------------------------------- */
/*      comando que permite ver el contenido de la memoria principal       */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\tipos.h>                                      /* incPtr */
#include <so1pub.h\ll_s_so1.h>    /* biblioteca de llamadas al sistema SO1 */
#include <so1pub.h\stdio.h>                    /* printf, getchar, putchar */ 

#include <so1pub.h\caracter.h>                                /* mayuscula */
#include <so1pub.h\strings.h>                                   /* iguales */
#include <so1pub.h\scanner.h>                    /* inicScanner, obtenSimb */ 
#include <so1pub.h\msdos.h>                                /* versionMSDOS */

typedef enum
{
    sp_desconocido, sp_bios, sp_msdos, sp_windows98, sp_windowsNT
} sistemaPrevio_t ;

sistemaPrevio_t sistemaPrevio ( void )
{
    word_t menor, mayor ;
    word_t version ;
    info_t info ;

    obtenInfoINFO((info_t far *)&info) ;
    if (info.modoSO1 == modoSO1_Bin) return(sp_bios) ;
    else
    {
        version = versionMSDOS() ;
        mayor = version & 0x00FF ;
        menor = version >> 8 ;
        if ((mayor == 7) && (menor == 10)) return(sp_windows98) ;
        else if (valorMSDOS("OS")[0] == 'W') return(sp_windowsNT) ;
        else return(sp_msdos) ;
    }
//  return(sp_desconocido) ;
}

void escribirLinea ( pointer_t ptr, int comienzo, int final )
{

    unsigned char car ;                           /* para evitar problemas */
    int j ;

	printf(" %04X:%04X  ", seg(ptr), off(ptr)) ;
		
    for (j = 0 ; j < 16 ; j++)
        if ((comienzo <= j) && (j <= final))
			printf("%02X ", ptr[j]) ;  
        else
            printf("   ") ;
		
    putchar(' ') ;

    for (j = 0 ; j < 16 ; j++)		
        if ((comienzo <= j) && (j <= final))
		{
            car = ptr[j] ;
            if ((' ' <= car) && (car <= '~')) 
                putchar(car) ;   
            else 
				putchar('.') ;
        }
        else
            putchar(' ') ;
}

typedef union
{
    char c ;
    unsigned int i ;
    long unsigned int l ;
    char str [16] ;
} comodin_t ;

#define ultimaDirLineal ((((dword_t)0xFFFF) << 4) + 0xFFFF)  /* 0x0010FFEF */

dword_t dirLineal ( pointer_t ptr )
{
    return((((dword_t)seg(ptr)) << 4) + off(ptr)) ;
}

void volcar ( pointer_t ptr, dword_t nBytes )
{
    comodin_t far * ptrbufer = (comodin_t far *)ptr ;
    pointer_t primeraLinea = (pointer_t)((dword_t)ptr & 0xFFFFFFF0L) ;
    sistemaPrevio_t sistema = sistemaPrevio() ;
    int comienzo ;
    int final ;
    dword_t i ;

    if (nBytes == 0) return ;

	printf("\n\n %04X:%04X  (int)      ->     ", seg(ptr), off(ptr)) ;
	
    if ((off(ptr) == 0xFFFF) && (sistema == sp_windowsNT))   /* idem BOCHS */
        printf("error de direccionamiento (NTVDM)\n") ;
    else
        printf(
		    "%04XH = %11u (decimal)\n", 
		    ptrbufer->i,               /* da error si off(ptrbufer) = FFFF */
            ptrbufer->i                /* da error si off(ptrbufer) = FFFF */
		) ;
    
    printf("\n %04X:%04X  (long int) -> ", seg(ptr), off(ptr)) ;
		
    if (((off(ptr) & 0xFFFD) == 0xFFFD) && (sistema == sp_windowsNT)) /* idem en BOCHS */
        printf("    error de direccionamiento (NTVDM)\n\n") ;
    else
		printf(
		    "%08lXH = %11lu (decimal)\n\n", 
		    ptrbufer->l,           /* da error si off(ptrbufer) = FFFF o FFFD */
            ptrbufer->l            /* da error si off(ptrbufer) = FFFF o FFFD */
		) ;

    if (ultimaDirLineal < (dirLineal(ptr) + nBytes - 1))
        nBytes = nBytes - ((dirLineal(ptr) + nBytes - 1) - ultimaDirLineal) ;

    comienzo = off(ptr) & 0x000F ;

    final = ((word_t)(dirLineal(ptr) + nBytes - 1)) & 0x000F ;

    if ((dirLineal(ptr) & 0xFFFFFFF0L) ==               /* una unica linea */
        ((dirLineal(ptr) + nBytes - 1) & 0xFFFFFFF0L))
    {
        escribirLinea(primeraLinea, comienzo, final) ;
        printf("\n") ;
        return ;
    }
    escribirLinea(primeraLinea, comienzo, 16) ;
    incPtr((pointer_t far *)&primeraLinea, 16) ;
    printf("\n") ;

    for ( i = 0 ; i < (nBytes-(16-comienzo)-1)/16 ; i++ )
    {
        escribirLinea(primeraLinea, 0, 15) ;
        incPtr((pointer_t far *)&primeraLinea, 16) ;
        printf("\n") ;
        if (leerAsciiListo(STDIN) != (char)0)
        {
            printf("\n volcado interrumpido por el usuario \n") ;
            return ;
        }
    }

    if (((nBytes-(16-comienzo)-1) % 16) != 0)
    {
        escribirLinea(primeraLinea, 0, final) ;
        printf("\n") ;
    }
}

void formato ( void )
{
//  escribirStrIntenso(" formato: DMP [ SSSS:DDDD [ num ] | -h ]") ;
    printf(" formato: DMP [ SSSS:DDDD [ num ] | -h ]") ;
}

void help ( void )
{
    printf(
	    ""                                                               "\n"
	    ""                                                               "\n"
	    " formato: DMP [ SSSS:DDDD [ num ] | -h ]"                       "\n"
        ""                                                               "\n"
  	    " volcado de memoria a partir de la direccion SSSS:DDDD"         "\n"
        ""                                                               "\n"
	    " SSSS : segmento de memoria   (hexadecimal)"                    "\n"
	    " DDDD : desplazamiento        (hexadecimal)"                    "\n"
	    " num  : numero de bytes a mostrar (decimal)"                    "\n"
	    "        por defecto num = 64 bytes"                             "\n"
	) ;
}

int dmp ( void )
{

    pointer_t ptr ;
    word_t numSeg, numOff ;

    inicScanner() ;
    saltarBlancos() ;
    if ((('0' <= car) && (car <= '9')) || (('A' <= car) && (car <= 'F')))
    {
        numHex() ;
        if (simb == s_numero)
        {
            numSeg = num ;
            if (car == ':')
            {
                obtenCar() ;
                if ((('0' <= car) && (car <= '9')) ||
                    (('A' <= car) && (car <= 'F')))
                {
                    numHex() ;
                    if (simb == s_numero)
                    {
                        numOff = num ;
                        ptr = pointer(numSeg, numOff) ;
                        obtenSimb() ;
                        if ((simb == s_numero) || (simb == s_numeroLargo))
                        {
                            volcar(ptr, numLargo) ;
                            return(0) ;
                        }
                        else if (simb == s_fin)
                        {
                            volcar(ptr, 64) ;
                            return(0) ;
                        }
                    }
                    else if (simb == s_numeroLargo)
                    {
                        printf("\n\n") ;
                        formato() ;
                        printf(
						    "\n"
							"\n"
							" el numero de desplazamiento (off = %08lX) debe ser <= FFFF \n",
							numLargo
						) ;
                        return(2) ;
                    }
                }
            }
        }
        else if (simb == s_numeroLargo)
        {
            printf("\n\n") ;
            formato() ;
            printf(
			    "\n"
				"\n"
				" el numero de segmento (seg = %08lX) debe ser <= FFFF \n",
				numLargo
			) ;
            return(3) ;
        }
    }
    formato() ;
	return(5) ;
}

int main ( int argc, char * argv [ ] )
{

    int i, j, k ;
    int error ; 
	
    if (argc <= 1)                              /* al menos dos argumentos */
    {
        formato() ;
        return(1) ;
    }
	
    if ((argc == 2) && (iguales(argv[1], "-h") || iguales(argv[1], "-H")))
    {
        help() ;
        return(0) ;
    }
	
    k = 0 ;                              /* comando es una variable global */
    for ( i = 1 ; i < argc ; i ++ )
    {
        for ( j = 0 ; argv[i][j] != '\0' ; j++ )
            comando[0][k++] = mayuscula(argv[i][j]) ;
        comando[0][k++] = ' ' ;
    }
    comando[0][k--] = '\0' ;

    error = dmp() ;

//  getchar() ;
    return(error) ;	

}

