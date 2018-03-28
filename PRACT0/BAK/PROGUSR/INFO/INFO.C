/* ----------------------------------------------------------------------- */
/*                                  info.c                                 */
/* ----------------------------------------------------------------------- */
/*                       Un primer sistema operativo                       */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\ll_s_so1.h>    /* biblioteca de llamadas al sistema SO1 */
#include <so1pub.h\stdio.h>                    /* printf, getchar, putchar */
 
#include <so1pub.h\strings.h>
#include <so1pub.h\biosdata.h>
#include <so1pub.h\msdos.h>
#include <so1pub.h\puertos.h>

descProceso_t descProceso [ maxProcesos ] ;

e2PFR_t e2PFR ;

c2c_t c2cPFR [ numColasPFR ] ;

ptrBloque_t listaLibres ;

word_t tamBlqMax ;

word_t numPuertosCom = 0 ;

typedef word_t far * ptrComAdr_t ;

ptrComAdr_t ptrComAdr = (ptrComAdr_t)MK_FP(0x0000, 0x0400) ;

word_t numPuertosPar = 0 ;

typedef word_t far * ptrParAdr_t ;

ptrParAdr_t ptrParAdr = (ptrParAdr_t)MK_FP(0x0000, 0x0408) ;

info_t informacion ;

char strTipo [6][13] =
{
    "",
    "bus mouse",
    "raton serie",
    "raton inPort",
    "raton PS/2",
    "raton HP"
} ;

/* ----------------------------------------------------------------------- */
/* funcion memBIOS (<so1\memoria.c>)                                       */
/* ----------------------------------------------------------------------- */

/* tambien en BIOS AREA 0000:0413 */
word_t memBIOS ( void )    /* memoria reportada por el BIOS (en Kilobytes) */
{
    word_t numKBytes ;                            /* 1 Kilobyte = 1024 bytes */
    asm int 12h                                       /* BIOS: memoria total */
    asm mov numKBytes,ax
    return(numKBytes) ;
}

/* ----------------------------------------------------------------------- */
/* funcion valorIMR (<so1\pic.c>)                                          */
/* ----------------------------------------------------------------------- */

word_t valorIMR ( void )
{
    word_t res ;
    asm in al,PORT_PIC2_IMR
    asm mov ah,al
    asm in al,PORT_PIC1_IMR
    asm mov res,ax
    return(res) ;
}

/* ----------------------------------------------------------------------- */

void info ( void )
{

    word_t CS_SO1 ;
    word_t DS_SO1 ;
    word_t BSS_SO1 ;
    word_t version ;
    word_t menor, mayor ;
//  word_t tipo, irq ;
//  word_t numBotones ;
    dword_t nVueltasRetardo ;
    word_t ticsPorRodaja ;
    int dfTimer ;
    word_t i ;

    CS_SO1 = descProceso[0].CSProc ;
    DS_SO1 = seg((pointer_t)descProceso[0].trama) ;            /* DS == SS */
    BSS_SO1 = descProceso[0].desplBSS ;

	printf(
	    ""                                                               "\n"
		""                                                               "\n"
	) ;
    dfTimer = open("TIMER", O_RDONLY) ;
    if (dfTimer < 0)
        printf(" Rodaja de cpu no establecida ") ;
    else
    {
        lseek(dfTimer, 10, SEEK_SET) ;
        aio_read(dfTimer, (pointer_t)&ticsPorRodaja, 2) ;
        close(dfTimer) ;

        printf(" Rodaja de cpu = %i tic", ticsPorRodaja) ;
        if (ticsPorRodaja > 1) putchar('s') ;
        printf(" (%i milisegundos) ", 55*ticsPorRodaja) ;
    }
    printf("\n") ;

    if (informacion.modoSO1 == modoSO1_Bin)
    {
        printf(
		    ""                                                           "\n"
			" Arranque desde el BIOS memoria total = %i KB"              "\n"
			""                                                           "\n", 
			memBIOS()
		) ;
/*      printf("%i", ptrBiosArea->basemem_K) ; */
    }
    else
    {
        version = versionMSDOS() ;
        mayor = version & 0x00FF ;
        menor = version >> 8 ;
        if ((mayor == 7) && (menor == 10))
            printf("\n Arranque bajo Windows 98") ;
        else if (valorMSDOS("OS")[0] == 'W')
            printf("\n Arranque bajo Windows XP/7/8/10") ;
        else
            printf("\n Arranque bajo MSDOS %i.%02i", mayor, menor) ;
        printf(
		    " memoria total = %i KB\n\n", 
			(0xA000-descProceso[0].trama->DS)/64
		) ;
    }

    printf(
	    " CS = %04X DS = %04X BSS = DS:%04X a DS:%04X\n\n", 
	    CS_SO1, DS_SO1, BSS_SO1, descProceso[0].desplPila
	) ;

    printf(
	    " Pilas: consola => DS:%04X sistema => DS:%04X", 
	    informacion.SP0, informacion.SP0_So1
	) ;

    for ( i = 0 ; i < 4 ; i++ )
        if (ptrComAdr[i] != 0)
            numPuertosCom++ ;

    if (numPuertosCom > 0)
    {
        printf("\n\n numero de puertos serie %i:", numPuertosCom) ;
        for ( i = 0 ; i < numPuertosCom ; i++ )
            printf(" COM%i = %03X", i+1, ptrComAdr[i]) ;
    }

    for ( i = 0 ; i < 3 ; i++ )
        if (ptrParAdr[i] != 0)
            numPuertosPar++ ;

    if (numPuertosPar > 0)
    {
        printf("\n\n numero de puertos paralelos %i:", numPuertosPar) ;
        for ( i = 0 ; i < numPuertosPar ; i++ )
            printf(" LPT%i = %03X", i+1, ptrParAdr[i]) ;
    }

    printf("\n\n") ;
    nVueltasRetardo = retardoActivo(1) ;
    if (nVueltasRetardo != (dword_t)-1)
        printf(" retardoActivo = %li", nVueltasRetardo) ;

    printf(" IMR Inicial = %04X", informacion.IMR) ;
    if (informacion.IMR != valorIMR())
        printf(" ==> %04X", valorIMR()) ;
    printf(" =") ;
    for ( i = 0 ; i < 16 ; i++ )
    {
        if ((i % 4) == 0) putchar(' ') ;
        putchar('0'+ ((valorIMR() & (0x8000 >> i)) != 0)) ;
    }

    printf("\n") ;

/*
    resetRatonBIOS(&numBotones) ;
       if (!hayRatonBIOS())
           printf("\n raton: no hay driver bios int33h") ;
       else 
	   {
           versionRatonBIOS(&mayor, &menor, &tipo, &irq) ;
           printf("\n raton: driver v%i.%02i %s", mayor, menor, strTipo[tipo]) ;
           if (irq > 0) 
               printf(" IRQ%i", irq) ;
       }
       escribirLn() ;
*/
}

int main ( void )
{
    obtenInfoMEM((descProceso_t far *)&descProceso,
                 (e2PFR_t far *)&e2PFR,
                 (c2c_t far *)&c2cPFR,
                 (ptrBloque_t *)&listaLibres,
                 (word_t *)&tamBlqMax) ;
    obtenInfoINFO((info_t far *)&informacion) ;

    info() ;

//	getchar() ;
	return(0) ;
}

