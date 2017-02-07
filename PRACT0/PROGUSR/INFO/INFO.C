/* ----------------------------------------------------------------------- */
/*                                  info.c                                 */
/* ----------------------------------------------------------------------- */
/*                       Un primer sistema operativo                       */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\ll_s_so1.h>    /* biblioteca de llamadas al sistema SO1 */
#include <so1pub.h\escribir.h>

#include <so1pub.h\strings.h>
#include <so1pub.h\biosdata.h>
#include <so1pub.h\msdos.h>
#include <so1pub.h\puertos.h>

descProceso_t descProceso [ maxProcesos ] ;

e2PFR_t e2PFR ;

c2c_t c2cPFR [ numColasPFR ] ;

ptrBloque_t listaLibres ;

word_t tamBlqMax ;

typedef word_t far * ptrComAdr_t ;

ptrComAdr_t ptrComAdr = (ptrComAdr_t)MK_FP(0x0000, 0x0400) ;

word_t numPuertosCom = 0 ;

typedef word_t far * ptrParAdr_t ;

ptrParAdr_t ptrParAdr = (ptrParAdr_t)MK_FP(0x0000, 0x0408) ;

word_t numPuertosPar = 0 ;

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
//word_t tipo, irq ;
//word_t numBotones ;
    dword_t nVueltasRetardo ;
    word_t ticsPorRodaja ;
    int dfTimer ;
    word_t i ;

    CS_SO1 = descProceso[0].CSProc ;
    DS_SO1 = seg((pointer_t)descProceso[0].trama) ;               /* DS == SS */
    BSS_SO1 = descProceso[0].desplBSS ;

    escribirLn() ;
    escribirLn() ;
    dfTimer = open("TIMER", O_RDONLY) ;
    if (dfTimer < 0)
        escribirStr(" Rodaja de cpu no establecida ") ;
    else
    {
        lseek(dfTimer, 10, SEEK_SET) ;
        aio_read(dfTimer, (pointer_t)&ticsPorRodaja, 2) ;
        close(dfTimer) ;
        escribirStr(" Rodaja de cpu = ") ;
        escribirDec(ticsPorRodaja, 1) ;
        escribirStr(" tic") ;
        if (ticsPorRodaja > 1) escribirCar('s') ;
        escribirStr(" (") ;
        escribirDec(55*ticsPorRodaja, 1) ;
        escribirStr(" milisegundos) ") ;
    }
    escribirLn() ;

    if (informacion.modoSO1 == modoSO1_Bin)
    {
        escribirStr("\n Arranque desde el BIOS") ;
        escribirStr(" memoria total = ") ;
        escribirDec(memBIOS(), 1) ;
        /*  escribirDec(ptrBiosArea->basemem_K, 1) ; */
        escribirStr(" KB\n\n") ;
    }
    else
    {
        version = versionMSDOS() ;
        mayor = version & 0x00FF ;
        menor = version >> 8 ;
        if ((mayor == 7) && (menor == 10))
            escribirStr("\n Arranque bajo Windows 98") ;
        else if (valorMSDOS("OS")[0] == 'W')
            escribirStr("\n Arranque bajo Windows XP") ;
        else
        {
            escribirStr("\n Arranque bajo MSDOS ") ;
            escribirDec(mayor, 1) ;
            escribirCar('.') ;
            escribirDec(menor/10, 1) ;
            escribirDec(menor%10, 1) ;
        }
        escribirStr(" memoria total = ") ;
        escribirDec((0xA000-descProceso[0].trama->DS)/64, 1) ;
        escribirStr(" KB\n\n") ;
    }

    escribirStr(" CS = ") ;
    escribirHex(CS_SO1, 4) ;
    escribirStr(" DS = ") ;
    escribirHex(DS_SO1, 4) ;
    escribirStr(" BSS = DS:") ;
    escribirHex(BSS_SO1, 4) ;
    escribirStr(" a DS:") ;
    escribirHex(descProceso[0].desplPila, 4) ;

    escribirLn() ;
    escribirLn() ;
    escribirStr(" Pilas: consola => DS:") ;
    escribirHex(informacion.SP0, 4) ;
    escribirStr(" sistema => DS:") ;
    escribirHex(informacion.SP0_So1, 4) ;

    for ( i = 0 ; i < 4 ; i++ )
        if (ptrComAdr[i] != 0)
            numPuertosCom++ ;

    if (numPuertosCom > 0)
    {
        escribirStr("\n\n numero de puertos serie ") ;
        escribirDec(numPuertosCom, 1) ;
        escribirCar(':') ;
        for ( i = 0 ; i < numPuertosCom ; i++ )
        {
            escribirStr(" COM") ;
            escribirDec(i+1, 1) ;
            escribirStr(" = ") ;
            escribirHex(ptrComAdr[i], 3) ;
        }
    }

    for ( i = 0 ; i < 3 ; i++ )
        if (ptrParAdr[i] != 0)
            numPuertosPar++ ;

    if (numPuertosPar > 0)
    {
        escribirStr("\n\n numero de puertos paralelos ") ;
        escribirDec(numPuertosPar, 1) ;
        escribirCar(':') ;
        for ( i = 0 ; i < numPuertosPar ; i++ )
        {
            escribirStr(" LPT") ;
            escribirDec(i+1, 1) ;
            escribirStr(" = ") ;
            escribirHex(ptrParAdr[i], 3) ;
        }
    }

    escribirStr("\n\n") ;
    nVueltasRetardo = retardoActivo(1) ;
    if (nVueltasRetardo != (dword_t)-1)
    {
        escribirStr(" retardoActivo = ") ;
        escribirLDec(nVueltasRetardo, 1) ;
    }

    escribirStr(" IMR Inicial = ") ;
    escribirHex(informacion.IMR, 4) ;
    if (informacion.IMR != valorIMR())
    {
        escribirStr(" ==> ") ;
        escribirHex(valorIMR(), 4) ;
    }
    escribirStr(" =") ;
    for ( i = 0 ; i < 16 ; i++ )
    {
        if ((i % 4) == 0) escribirCar(' ') ;
        escribirCar('0'+ ((valorIMR() & (0x8000 >> i)) != 0)) ;
    }

    escribirLn() ;

    /*
      resetRatonBIOS(&numBotones) ;
      if (!hayRatonBIOS())
        escribirStr("\n raton: no hay driver bios int33h") ;
      else {
        versionRatonBIOS(&mayor, &menor, &tipo, &irq) ;
        escribirStr("\n raton: driver v") ;
        escribirDec(mayor, 1) ;
        escribirCar('.') ;
        escribirDec(menor/10, 1) ;
        escribirDec(menor % 10, 1) ;
        escribirCar(' ') ;
        escribirStr(strTipo[tipo]) ;
        if (irq > 0) {
          escribirStr(" IRQ") ;
          escribirDec(irq, 1) ;
        }
      }
      escribirLn() ;
    */
}

void main ( void )
{
    obtenInfoMEM((descProceso_t far *)&descProceso,
                 (e2PFR_t far *)&e2PFR,
                 (c2c_t far *)&c2cPFR,
                 (ptrBloque_t *)&listaLibres,
                 (word_t *)&tamBlqMax) ;
    obtenInfoINFO((info_t far *)&informacion) ;
    info() ;
}

