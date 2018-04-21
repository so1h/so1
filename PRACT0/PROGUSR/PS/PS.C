/* ----------------------------------------------------------------------- */
/*                                   ps.c                                  */
/* ----------------------------------------------------------------------- */
/*             comando que muestra el estado de los procesos               */
/*                       (comando mas completo: dp)                        */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\ll_s_so1.h>    /* biblioteca de llamadas al sistema SO1 */
#include <so1pub.h\stdio.h>                    /* printf, getchar, putchar */
#include <so1pub.h\escribir.h>

#include <so1pub.h\ptrc2c.h>                                   /* ptrC2c_t */

#define lMaxStrEstado 13

char strEstado[ ][lMaxStrEstado] = { "libre",
                                     "preparado",
                                     "ejecutandose",
                                     "bloqueado"
                                   } ;

char carRecurso [ ] =
{
    '!',
    'H', /* rec_hijo                                        (rindx_t)(- 1) */
    'Z', /* rec_zombie                                      (rindx_t)(- 2) */
    'D', /* rec_desinstalacion                              (rindx_t)(- 3) */
    'S', /* rec_semaforo                                    (rindx_t)(- 4) */
    'B', /* rec_buzon                                       (rindx_t)(- 5) */
    'C', /* rec_serie                                       (rindx_t)(- 6) */
    'R', /* rec_raton                                       (rindx_t)(- 7) */
    'r', /* rec_ratonRaw                                    (rindx_t)(- 8) */
    'A', /* rec_tecladoRaton                                (rindx_t)(- 9) */
    'a', /* rec_tecladoRawRaton                             (rindx_t)(-10) */
} ;

#define tamCarRec (sizeof(carRecurso)/sizeof(char))

descProceso_t descProceso [ maxProcesos ] ;

e2PFR_t e2PFR ;

c2c_t c2cPFR [ numColasPFR ] ;

void ePC2c ( ptrC2c_t ptrC2c )
{
    int i, j ;
	int numElem ;
	int primero ;
//	int cabecera ;
    numElem = ptrC2c->numElem ;
    primero = ptrC2c->primero ;
//  cabecera = ptrC2c->cabecera ;
//  printf("%i %i %i ", numElem, primero, cabecera) ;
    printf("[ ") ;
    i = primero ;
    for ( j = 0 ; j < numElem ; j++ )
    {
        printf("%i ", i) ;
//      printf("%i ", descProceso[i].pid) ; 
        i = ptrC2c->e[i].sig ;
    }
    printf("] ") ;
}

void eFlags ( word_t flags ) 
{
	word_t mascara ;
    for ( mascara = 0x8000 ; mascara > 0 ; mascara >>= 1 ) 
		putchar('0' + ((flags & mascara) != 0)) ;
}	

word_t miIP ( void )
{
#if (FALSE)
    asm
    {
        push bp ;
        mov bp,sp ;
        mov bx,ss:[bp+2] ;
        pop bp ;
    }
    return(_BX) ;
#else
    asm
    {
        pop ax ;
        push ax ;
    }
    return(_AX) ;
#endif
}

void mostrarProcesos ( void )
{
    int i, j ;
    int ind ;
    word_t flags ;
    rindx_t esperandoPor ;

    printf(" cola de preparados: ") ;
    ePC2c((ptrC2c_t)&c2cPFR[PPreparados]) ;
    printf(
        "\n"
        "\n"
        " pindx pid comando       estado   rec CS   IP   DS   FBSS SP    NnpODITSZ A P C\n"
        " ----- --- ------------- ------------ ---- ---- ---- ---- ---- ----------------"
    ) ;
    for ( i = 0 ; i < maxProcesos ; i++ )
    {
        if (descProceso[i].estado != libre)
        {
            printf(
			  "\n"
			  " %5i %3i %-13.13s ", 
			  i, descProceso[i].pid, descProceso[i].comando
			) ;
            if (descProceso[i].estado == bloqueado)
            {
		       /* chapuza para que funcione en Fake86, porque Fake86 tiene */
			   /* error al acceder a las variables locales poniendo a cero */
			   /* el byte alto de las palabras. Ej: (word)(-1) ==> 255.    */
			   /* pasando a (signed char) se soluciona para valores entre  */
			   /* -128 y 127, pero fallaría en el resto de casos.          */ 
			   
#define CHAPUZA_FAKE86 int                     /* no funcionaria en Fake86 */
//#define CHAPUZA_FAKE86 signed char        /* para que funcione en Fake86 */		   
			   
                printf("bloqueado(") ;
#if (TRUE)	
                if ((CHAPUZA_FAKE86)descProceso[i].esperandoPor >= 0)    /**/
					printf("%i", descProceso[i].esperandoPor) ;
                else {
                    ind = -(CHAPUZA_FAKE86)descProceso[i].esperandoPor ; /**/
                    if (ind < tamCarRec) putchar(carRecurso[ind]) ;
                    else putchar('#') ;
				}
#else 
                esperandoPor = descProceso[i].esperandoPor ;
                ind = (-1)*esperandoPor ;
                if (ind > 0)
                    if (ind < tamCarRec) putchar(carRecurso[ind]) ;
                    else putchar('#') ;
                else
                    printf("%i", esperandoPor) ;
#endif				
                putchar(')') ;
            }
            else
				printf("%-*.*s", lMaxStrEstado-1, lMaxStrEstado-1, 
			                    strEstado[descProceso[i].estado]) ;
            if ((descProceso[i].estado == bloqueado) && (descProceso[i].esperandoPor == rec_zombie))
                continue ;

            if (descProceso[i].pid != getpid())
            {
				printf(
				    " %04X %04X %04X %04X %04X ", 
					descProceso[i].trama->CS, 
					descProceso[i].trama->IP,
					descProceso[i].trama->DS,
					descProceso[i].desplPila, 
					off((pointer_t)descProceso[i].trama)
				) ;
                eFlags(descProceso[i].trama->Flags) ;
            }
            else
            {
                printf(
				    " %04X %04X %04X %04X %04X ", 
					_CS, 
					miIP(), 
					_DS, 
					descProceso[i].desplPila, 
					_SP
				) ;
                asm pushf ;
                asm pop ax ;
                asm mov flags,ax ;
                eFlags(flags) ;
            }
        }
    }
//	getchar() ;
    printf("\n") ;
}

int main ( void )
{
    obtenInfoPS((descProceso_t far *)&descProceso, (e2PFR_t far *)&e2PFR, (c2c_t far *)&c2cPFR) ;
    mostrarProcesos() ;
	return(0) ;
}

