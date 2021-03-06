/* ----------------------------------------------------------------------- */
/*                                   s0.c                                  */
/* ----------------------------------------------------------------------- */
/*                               sistema cero                              */
/* ----------------------------------------------------------------------- */

/* Este modulo implementa los primeros pasos a ejecutar en SO1 de manera   */
/* que se tenga una idea de la maquina/emulador que se esta utilizando.    */
/* Estos pasos son: mirarLoQueHay y mostrarLoQueHay.                       */ 

#include <so1pub.h\ll_s_exc.h>                                     /* exit */
#include <so1pub.h\escribir.h>     /* escribirStr, escribirInt, escribirLn */
#include <so1pub.h\biosdata.h>                             /* ptrFechaBios */
#include <so1pub.h\msdos.h>                    /* finProgDOS, hayWindowsNT */
#include <so1pub.h\debug.h>                                      /* assert */
#include <so1.h\bios.h>                   /* print(Ln/Str/Hex/Dec/Ptr)BIOS */
#include <so1.h\main.h>                   /* miraLoQueHay, mostrarLoQueHay */
#include <so1.h\ajustes.h>                                      /* modoSO1 */
#include <so1.h\so1dbg.h>                               /* esperarScancode */
#include <so1.h\s0.h>                    /* mirarLoQueHay, MostrarLoQueHay */
         /* hayDOS, hayNT, hayQemu, hayBochs, hayDBox, hayNTVDM, hayFake86 */
                                                               /* hayMsdos */
#define CON_PROCESO_INICIAL TRUE

#if (!CON_PROCESO_INICIAL)
#include <so1pub.h\interpre.h>                      /* interpretarComandos */
#include <so1pub.h\saludos.h>                             /* mostrarSaludo */
#endif

word_t fechaCompactada ( pointer_t ptrFecha ) {
    word_t mes, dia, anio ;
    if ((ptrFecha[2] != '/') || (ptrFecha[5] != '/')) return(0x0000) ;
    mes  = 10*(ptrFecha[0] - '0') + (ptrFecha[1] - '0') ;   /* mes:  4 bits */ 
    if ((mes == 0) || (mes > 12)) return(0x0000) ;
    dia  = 10*(ptrFecha[3] - '0') + (ptrFecha[4] - '0') ;   /* dia:  5 bits */ 
    if ((dia == 0) || (dia > 31)) return(0x0000) ;
    anio = 10*(ptrFecha[6] - '0') + (ptrFecha[7] - '0') ;   /* año: 7 bits */ 
    return((anio << 9) | (mes << 5) | dia) ; 
}

void mirarLoQueHay ( word_t * loQueHay ) {

    *loQueHay = 0x0000 ;                         /* de partida no hay nada */

    switch (modoSO1()) 
	{
    case modoSO1_Cmd :           /* SO1 ejecutado como programa de usuario */
#if (CON_PROCESO_INICIAL)
        escribirStr(
            "\a\n"
            "\n"
            " En esta version no puede ejecutarse SO1 como comando \n"
        ) ;
        exit(-1) ;                          /* fin del programa de usuario */
#else
        escribirLn() ;
        escribirLn() ;
        mostrarSaludo() ;
        exit(interpretarComandos()) ;       /* fin del programa de usuario */
#endif
    case modoSO1_Bin :                                   /* so1.bin (boot) */
    case modoSO1_Bs  :     /* so1.bin cargado como BOOT SO1.BIN (SYSLINUX) */
        break ;
    case modoSO1_Com :                                          /* so1.com */
    case modoSO1_Exe :                                          /* so1.exe */
        *loQueHay |= hayDOS ;
        if (hayWindowsNT())
            *loQueHay |= hayNT ;
        break ;
    default :                                                     /* error */
        assert(FALSE, "\a\n SO1() ERROR: modoSO1() = ");
        printHexBIOS(modoSO1(), 4) ;
        esperarScancode() ;
        finProgDOS (1) ;                                        /* int 21h */
        rebootBIOS() ;                                          /* int 19h */
        rebootLegacy() ;                                /* callf ffff:0000 */
    }

//  switch (0L)  
    switch (fechaCompactada(ptrFechaBios))  
	{
	case 50077L : *loQueHay |= hay86Box  ; break ;           /* "12/29/97" */ 	
    case 50903L : *loQueHay |= hayQemu   ; break ;           /* "06/23/99" */
    case  5706L :                                            /* "02/10/11" */ /* Bochs 2.6.7 */ 
    case  7578L :                                            /* "12/26/14" */ /* Bochs 2.6.8 */
    case  8784L : *loQueHay |= hayBochs  ; break ;           /* "02/16/17" */ /* Bochs 2.6.9 */ 
    case 47137L : *loQueHay |= hayDBox   ; break ;           /* "01/01/92" */
    case 48867L : *loQueHay |= hayNTVDM  ; break ;           /* "07/03/95" */
    case  6306L : *loQueHay |= hayFake86 ; break ;           /* "05/02/12" */
    case 50721L : *loQueHay |= hayVDos   ; break ;           /* "01/01/99" */
    case  3158L : *loQueHay |= hayMsdos  ; break ;           /* "02/22/06" */        
    default  : ;
#if (FALSE)
//#if (TRUE)
        if ((modoSO1() == modoSO1_Bin) || (modoSO1() == modoSO1_Exe)) 
	    {
            int i ;
            printLnBIOS() ;
            printStrBIOS("\a\n so1(): fecha del BIOS = ") ;
            for ( i = 0 ; i < 8 ; i++ )
                printCarBIOS(ptrFechaBios[i]) ;
            printStrBIOS("\n\n fecha compactada = ") ;
            printLDecBIOS(fechaCompactada(ptrFechaBios), 1) ;
            printStrBIOS(" ==> Maquina (virtual) inexplorada\n"
                         "\n"
                         " presione una tecla para continuar ") ;
            esperarScancode() ;
			while(TRUE) ;
        }
#endif
    }
}

void ponerMuescaX ( char * nombreSistema, bool_t hayEseSistema ) {
    printStrBIOS(nombreSistema) ;
    printCarBIOS('[') ;
	printCarBIOS(hayEseSistema ? 'X' : ' ') ;
    printStrBIOS("]  ") ;
}

void mostrarLoQueHay ( word_t loQueHay ) 
{
    clrScrBIOS() ;
    goToXYBIOS(1, 0) ;
    switch (modoSO1()) 
	{
    case modoSO1_Cmd : return ;
    case modoSO1_Bin : printStrBIOS(" so1.bin") ; break ;
    case modoSO1_Com : printStrBIOS(" so1.com") ; break ;
    case modoSO1_Exe : printStrBIOS(" so1.exe") ; break ;
    case modoSO1_Bs  : printStrBIOS(" so1 slx") ; break ;
    }
    printStrBIOS("   ") ;
    ponerMuescaX("MSDOS" , (bool_t)(loQueHay & hayDOS)) ;
    ponerMuescaX("WinNT" , (bool_t)(loQueHay & hayNT)) ;
    switch (loQueHay & 0xFFFC) 
	{
    case hayQemu   : ponerMuescaX("Qemu"  , TRUE) ; break ;
    case hayBochs  : ponerMuescaX("Bochs" , TRUE) ; break ;
    case hayDBox   : ponerMuescaX("DOSBox", TRUE) ; break ;
    case hayNTVDM  : ponerMuescaX("NTVDM" , TRUE) ; break ;
    case hayFake86 : ponerMuescaX("Fake86", TRUE) ; break ;
    case hayVDos   : ponerMuescaX("VDos"  , TRUE) ; break ;
    case hayMsdos  : ponerMuescaX("msdos" , TRUE) ; break ;    
    case hay86Box  : ponerMuescaX("86Box" , TRUE) ; break ;    
	default :
        printStrBIOS("\aMaquina (virtual) inexplorada ") ;
    }
    printLnBIOS() ;
}

void tirarS0 ( word_t loQueHay ) 
{
    if (loQueHay & hayDOS) {
        finProgDOS(0) ;                                         /* int 21h */
        rebootBIOS() ;                                          /* int 19h */
        rebootLegacy() ;                                /* callf ffff:0000 */
    }
    else if (!(loQueHay & (hayDBox || hayVDos))) 
	{
        rebootBIOS() ;                                          /* int 19h */
        rebootLegacy() ;                                /* callf ffff:0000 */
    }
    else               /* DOSBox con boot da algun problema al reiniciarse */
	{      
        clrScrBIOS() ;
        goToXYBIOS(11, 14) ;
        ocultaCursorBIOS() ;
        printStrBIOS(" puede apagar la maquina virtual DOSBox (Ctrl+F9) o VDos") ;
        asm 
		{
           sti ;
           hlt ;
        }
    }
}