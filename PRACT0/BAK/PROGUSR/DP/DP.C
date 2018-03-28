/* ----------------------------------------------------------------------- */
/*                                   dp.c                                  */
/* ----------------------------------------------------------------------- */
/*            comando para ver los descriptores de los procesos            */
/*                    version extendida del comando ps                     */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\ll_s_so1.h>    /* biblioteca de llamadas al sistema SO1 */
#include <so1pub.h\stdio.h>                    /* printf, getchar, putchar */

#include <so1pub.h\strings.h>                           /* strcpy, iguales */
#include <so1pub.h\scanner.h>
#include <so1pub.h\ctype.h>                                     /* tolower */

#include <so1pub.h\ptrc2c.h>                                   /* ptrC2c_t */

void formato ( void )
{
    printf(
        ""                                                               "\n"
        ""                                                               "\n"
        " formato : DP ( px [ -p | -f ] | -a | -h ) "                    "\n"                     
    ) ;
}

void help ( void )
{
    formato() ;
    printf(
        ""                                                               "\n"
        " muestra los campos del descriptor de proceso"                  "\n"
        ""                                                               "\n"
        " opciones (px = indice del descriptor del proceso):"            "\n"
        ""                                                               "\n"
        "   px    : muestra todos los campos del proceso px"             "\n"
        "   px -p : muestra los principales campos de px"                "\n"
        "   px -f : muestra su tabla de ficheros abiertos"               "\n"
        "      -a : muestra todos los procesos"                          "\n"
        "      -h : muestra este help"                                   "\n"
    ) ;
}

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

void eNibble ( word_t w )
{
    int i, masc ;
    masc = 1 << 4 ;
    for ( i = 0 ; i < 4 ; i++ )
    {
        masc = masc >> 1 ;
        if ((masc & w) != 0) putchar('1') ;
        else putchar('0') ;
    }
}

void eFlags ( word_t flags ) 
{
	word_t mascara ;
    for ( mascara = 0x8000 ; mascara > 0 ; mascara >>= 1 ) 
	{
        if ((flags & mascara) != 0)
            putchar('1') ;
        else
            putchar('0') ;
	}
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

char strRecurso [ ] [20] =
{
    "!",
    "rec_hijo",                                          /* (rindx_t)(- 1) */
    "rec_zombie",                                        /* (rindx_t)(- 2) */
    "rec_desinstalacion",                                /* (rindx_t)(- 3) */
    "rec_semaforo",                                      /* (rindx_t)(- 3) */
    "rec_buzon",                                         /* (rindx_t)(- 4) */
    "rec_serie",                                         /* (rindx_t)(- 5) */
    "rec_raton",                                         /* (rindx_t)(- 6) */
    "rec_ratonRaw",                                      /* (rindx_t)(- 7) */
    "rec_tecladoRaton",                                  /* (rindx_t)(- 8) */
    "rec_tecladoRawRaton"                                /* (rindx_t)(- 9) */
} ;

#define tamCarRec (sizeof(carRecurso)/sizeof(char))

descProceso_t descProceso [ maxProcesos ] ;

descFichero_t descFichero [ dfsMax ] ;

descRecurso_t descRecurso [ maxRecursos ] ;

e2PFR_t e2PFR ;

c2c_t c2cPFR [ numColasPFR ] ;

info_t info ;

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

void mostrarCamposPrincipales ( pindx_t ind )
{
    word_t Flgs ;
    word_t regIP ;
    estado_t estado ;
    rindx_t esperandoPor ;

	char espacio [2] ;     /* para que no se muevan los Flags si ind >= 10 */
	
	espacio[1] = '\0' ;
	if (ind < 10) 
		espacio[0] = ' ' ;                                /* espacio = " " */
	else 
		espacio[0] = '\0' ;                                /* espacio = "" */
	
    estado = descProceso[ind].estado ;

	printf(
	    ""                                                               "\n"
        " descProceso[%i].pid          = %i  (pindx = %i)"               "\n"
        " descProceso[%i].ppindx       = %i"                             "\n"
        " descProceso[%i].uid          = %i"                             "\n"
        " descProceso[%i].gid          = %i"                             "\n"
        " descProceso[%i].estado       = %s (%i)",
        ind, descProceso[ind].pid, ind,
        ind, descProceso[ind].ppindx,
        ind, descProceso[ind].uid,
        ind, descProceso[ind].gid,
        ind, strEstado[estado], estado
    ) ;		

    if (estado == 3)                                          /* bloqueado */
    {
        esperandoPor = descProceso[ind].esperandoPor ;
		
        printf("\n descProceso[%i].esperandoPor = ", ind) ;
		
        if (esperandoPor >= 0)
            printf("\"%s\"", descRecurso[esperandoPor].nombre) ;
        else
            printf(strRecurso[-esperandoPor]) ;
        
		printf(" (%i)", esperandoPor) ;
		
        switch (esperandoPor)
        {
        case rec_hijo   :
	        printf("\n descProceso[%i].hpindx       = %i", ind, descProceso[ind].hpindx) ;
            break ;
        case rec_zombie :
	        printf("\n descProceso[%i].noStatus     = ", ind) ;
            if (descProceso[ind].noStatus) 
				printf("TRUE") ;
            else 
				printf("FALSE") ;
	        printf("\n descProceso[%i].status       = %i", ind, descProceso[ind].status) ;
            break ;
        default :
            ;
        }
    }
	
	printf("\n descProceso[%i].c2cHijos     = ", ind) ;
	ePC2c((ptrC2c_t)&descProceso[ind].c2cHijos) ;
    printf("\n descProceso[%i].tamFichero   = %04lX = %li", 
	    ind, (dword_t)descProceso[ind].tamFichero, (dword_t)descProceso[ind].tamFichero, 1
	) ;
    if ((info.modoSO1 == modoSO1_Exe) && (ind == 0))
        printf(" (+ cabecera EXE) ") ;
    
	printf("\n") ;
    printf(
	    " descProceso[%i].programa     = \"%s\""                         "\n" 
	    " descProceso[%i].comando      = \"%s\""                         "\n"
	    " descProceso[%i].tamCodigo    = %X"                             "\n"
	    " descProceso[%i].desplBSS     = %04X"                           "\n"
	    " descProceso[%i].desplPila    = %04X"                           "\n",
     	ind, descProceso[ind].programa,
        ind, descProceso[ind].comando,
		ind, descProceso[ind].tamCodigo,
		ind, descProceso[ind].desplBSS, 
		ind, descProceso[ind].desplPila
	) ;
	
    if (descProceso[ind].estado == ejecutandose)      /* apilamos 26 bytes */
    {
        regIP = miIP() ;
        asm pushf ;
        asm push cs ;
        asm push regIP ;
        asm pusha ;
        asm push es ;
        asm push ds ;
        descProceso[ind].trama = (trama_t far *)pointer(_SS, _SP) ;
    }

    printf(
	    " descProceso[%i].trama        = %04X:%04X", 
		ind, seg((pointer_t)descProceso[ind].trama), 
		off((pointer_t)descProceso[ind].trama)
	) ;
	
    printf("%s      Flags:  0Nnp  ODIT  SZ0A  0P1C \n", espacio) ;
    printf(
	    " descProceso[%i].CSProc       = %04X    %s               ----  ----  ----  ---- \n"
	    " descProceso[%i].tam          = %04X Ps %s",
        ind, descProceso[ind].CSProc, espacio,
	    ind, descProceso[ind].tam, espacio
    ) ;
		
    Flgs = descProceso[ind].trama->Flags ;
	printf("               ") ;
    eNibble((Flgs >> 12) & 0x000F) ;
    printf("  ") ;
    eNibble((Flgs >> 8) & 0x000F) ;
    printf("  ") ;
    eNibble((Flgs >> 4) & 0x000F) ;
    printf("  ") ;
    eNibble(Flgs & 0x000F) ;
    printf("\n\n") ;
	printf(
   	    "  DS    ES    DI    SI    BP    SP    BX    DX    CX    AX    IP    CS   Flgs \n"
        " ----  ----  ----  ----  ----  ----  ----  ----  ----  ----  ----  ----  ---- \n"
        " %04X  %04X  %04X  %04X  %04X  %04X  %04X  %04X  %04X  %04X  %04X  %04X  %04X \n",
        descProceso[ind].trama->DS,
		descProceso[ind].trama->ES,
		descProceso[ind].trama->DI,
		descProceso[ind].trama->SI,
		descProceso[ind].trama->BP,
		descProceso[ind].trama->SP, 
		descProceso[ind].trama->BX, 
		descProceso[ind].trama->DX,
		descProceso[ind].trama->CX,
		descProceso[ind].trama->AX, 
		descProceso[ind].trama->IP, 
		descProceso[ind].trama->CS, 
		descProceso[ind].trama->Flags
	) ;

    if (descProceso[ind].estado == ejecutandose)   /* desapilamos 26 bytes */
    {
        asm add sp,26 ;
    }
}

void mostrarTablaDeFicherosAbiertos ( pindx_t ind )
{

    int df ;
    int dfs ;

    printf("\n descProceso[%i].nfa = %i", 
	       ind, descProceso[ind].nfa) ;

	if (descProceso[ind].nfa > 0) printf("\n") ;	   
		   
    for ( df = 0 ; df < dfMax ; df++ )
        if (descProceso[ind].tfa[df].dfs >= 0)
        {
            dfs = descProceso[ind].tfa[df].dfs ;
            printf("\n descProceso[%i].tfa[%i] = ( dfs: %2i modoAp: %04X pos: %5lu \"%s\" ) ", 
	            ind, df, dfs, 
				descProceso[ind].tfa[df].modoAp, 
				descProceso[ind].tfa[df].pos, 
				descFichero[dfs].nombre
			) ;
        }

}

void mostrarDescProceso ( pindx_t pindx, char opcion )
{

    if ((opcion == ' ') || (opcion == 'p'))
    {
        printf("\n") ;
        mostrarCamposPrincipales(pindx) ;
        if (opcion == ' ')
            getchar() ;    
        else if (opcion == 'p')
            return ;
    }

    if ((opcion == ' ') || (opcion == 'f'))
    {
        if (opcion == 'f') printf("\n") ;			
        mostrarTablaDeFicherosAbiertos(pindx) ;
        if (opcion == 'f')
        {
            printf("\n ") ;
            return ;
        }
    }
    printf("\n ") ;
}

int main ( int argc, char * argv [ ] )
{

    pindx_t pindx ;
    char opcion ;
	
    obtenInfoPFR ((descProceso_t far *)&descProceso,
                  (descFichero_t far *)&descFichero,
                  (descRecurso_t far *)&descRecurso,
                  (e2PFR_t far *)&e2PFR,
                  (c2c_t far *)&c2cPFR
                 ) ;
				 
    obtenInfoINFO((info_t far *)&info) ;
	
    if (argc == 1)
    {
        formato() ;
        return(0) ;
    }
    else if (argc > 3)
    {
        putchar('\a') ;
        formato() ;
        return(1) ;
    }
    if ((argc == 2) &&
            (argv[1][0] == '-') &&
            (argv[1][2] == '\0'))
    {
        opcion = tolower(argv[1][1]) ;
        if (opcion == 'h')
        {
            help() ;
            return(0) ;
        }
        else if (opcion == 'a')
        {
            mostrarProcesos() ;
            return(0) ;
        }
    }
    strcpy(comando[0], argv[1]) ;
    inicScanner() ;
    obtenSimb() ;
    if (simb != s_numero)
    {
        putchar('\a') ;
        formato() ;
        printf("\n error: pindx debe ser un numero y no \"%s\"\n", argv[1]) ;
        return(2) ;
    }
    else
    {
        pindx = (pindx_t)num ;
        if ((0 <= pindx) && (pindx < maxProcesos))
            if (descProceso[pindx].pid >= 0)
            {
                if (argc == 2)
                    mostrarDescProceso(pindx, ' ') ;
                else if ((argc == 3) &&
                         (argv[2][0] == '-') &&
                         (argv[2][2] == (char)0))
                {
                    opcion = tolower(argv[2][1]) ;
                    if ((opcion == 'p') ||
                            (opcion == 'f'))
                        mostrarDescProceso(pindx, opcion) ;
                    else
                    {
                        putchar('\a') ;
                        printf(" opcion \"%s\" incorrecta ", argv[2]) ;
                        return(3) ;
                    }
                }
                else
                {
                    putchar('\a') ;
                    formato() ;
                    return(4) ;
                }
            }
            else
            {
                putchar('\a') ;
                printf(" descriptor no utilizado ") ;
                return(5) ;
            }
        else
        {
            putchar('\a') ;
            printf(" pindx erroneo ") ;
            return(6) ;
        }
    }
	return(0) ;
}