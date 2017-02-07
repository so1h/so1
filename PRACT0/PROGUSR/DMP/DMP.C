/* ----------------------------------------------------------------------- */
/*                                  dmp.c                                  */
/* ----------------------------------------------------------------------- */
/*      comando que permite ver el contenido de la memoria principal       */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\tipos.h>                                      /* incPtr */
#include <so1pub.h\ll_s_so1.h>    /* biblioteca de llamadas al sistema SO1 */
#include <so1pub.h\escribir.h>
#include <so1pub.h\caracter.h>                          /* dig, mayusculas */

#include <so1pub.h\strings.h>
#include <so1pub.h\scanner.h>
#include <so1pub.h\msdos.h>

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

    escribirStr(" ") ;
    escribirHex(seg(ptr), 4) ;
    escribirStr(":") ;
    escribirHex(off(ptr), 4) ;
    escribirStr("  ") ;
    for (j = 0 ; j < 16 ; j++)
    {
        if ((comienzo <= j) && (j <= final))
        {
            car = ptr[j] ;
            escribirCar(dig[car >> 4]) ;
            escribirCar(dig[car & 0x0F]) ;
            escribirCar(' ') ;
        }
        else
            escribirStr("   ") ;
    }
    escribirCar(' ') ;
    for (j = 0 ; j < 16 ; j++)
    {
        if ((comienzo <= j) && (j <= final))
        {
            car = ptr[j] ;
            if ((' ' <= car) && (car <= '~')) escribirCar(car) ;
            else escribirCar('.') ;
        }
        else
            escribirCar(' ') ;
    }
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

    escribirStr("\n\n ") ;
    escribirHex(seg(ptr), 4) ;
    escribirStr(":") ;
    escribirHex(off(ptr), 4) ;
    escribirStr("  (int)      ->     ") ;
    if ((off(ptr) == 0xFFFF) && (sistema == sp_windowsNT))  /* idem en BOCHS */
        escribirStr("error de direccionamiento (NTVDM)\n") ;
    else
    {
        escribirHex(ptrbufer->i, 4) ;  /* da error si off(ptrbufer) = FFFF */
        escribirStr("H = ") ;
        escribirDec(ptrbufer->i, 11) ; /* da error si off(ptrbufer) = FFFF */
        escribirStr(" (decimal)\n") ;
    }

    escribirStr("\n ") ;
    escribirHex(seg(ptr), 4) ;
    escribirStr(":") ;
    escribirHex(off(ptr), 4) ;
    escribirStr("  (long int) -> ") ;
    if (((off(ptr) & 0xFFFD) == 0xFFFD) && (sistema == sp_windowsNT)) /* idem en BOCHS */
        escribirStr("    error de direccionamiento (NTVDM)\n\n") ;
    else
    {
        escribirLHex(ptrbufer->l, 8) ;  /* da error si off(ptr) = FFFD o FFFF */
        escribirStr("H = ") ;
        escribirLDec(ptrbufer->l, 11) ; /* da error si off(ptr) = FFFD o FFFF */
        escribirStr(" (decimal)\n\n") ;
    }

    if (ultimaDirLineal < (dirLineal(ptr) + nBytes - 1))
        nBytes = nBytes - ((dirLineal(ptr) + nBytes - 1) - ultimaDirLineal) ;

    comienzo = off(ptr) & 0x000F ;

    final = ((word_t)(dirLineal(ptr) + nBytes - 1)) & 0x000F ;

    if ((dirLineal(ptr) & 0xFFFFFFF0L) ==               /* una unica linea */
            ((dirLineal(ptr) + nBytes - 1) & 0xFFFFFFF0L))
    {
        escribirLinea(primeraLinea, comienzo, final) ;
        escribirStr("\n") ;
        return ;
    }
    escribirLinea(primeraLinea, comienzo, 16) ;
    incPtr((pointer_t far *)&primeraLinea, 16) ;
    escribirStr("\n") ;

    for ( i = 0 ; i < (nBytes-(16-comienzo)-1)/16 ; i++ )
    {
        escribirLinea(primeraLinea, 0, 15) ;
        incPtr((pointer_t far *)&primeraLinea, 16) ;
        escribirStr("\n") ;
        if (leerAsciiListo(STDIN) != (char)0)
        {
            escribirStr("\n volcado interrumpido por el usuario \n") ;
            return ;
        }
    }

    if (((nBytes-(16-comienzo)-1) % 16) != 0)
    {
        escribirLinea(primeraLinea, 0, final) ;
        escribirStr("\n") ;
    }
}

void formato ( void )
{
//  escribirStrIntenso(" formato: DMP [ SSSS:DDDD [ num ] | -h ]") ;
    escribirStr(" formato: DMP [ SSSS:DDDD [ num ] | -h ]") ;
}

void help ( void )
{
    escribirStr("\n\n formato: DMP [ SSSS:DDDD [ num ] | -h ]\n") ;
    escribirStr("\n volcado de memoria a partir de la direccion SSSS:DDDD \n") ;
    escribirStr("\n SSSS : segmento de memoria   (hexadecimal) ") ;
    escribirStr("\n DDDD : desplazamiento        (hexadecimal) ") ;
    escribirStr("\n num  : numero de bytes a mostrar (decimal) ") ;
    escribirStr("\n        por defecto num = 64 bytes        \n") ;
}

void dmp ( void )
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
                            return ;
                        }
                        else if (simb == s_fin)
                        {
                            volcar(ptr, 64) ;
                            return ;
                        }
                    }
                    else if (simb == s_numeroLargo)
                    {
                        escribirStr("\n\n") ;
                        formato() ;
                        escribirStr("\n\n el numero de desplazamiento (off = ") ;
                        escribirLHex(numLargo, 4) ;
                        escribirStr(") debe ser <= FFFF \n") ;
                        return ;
                    }
                }
            }
        }
        else if (simb == s_numeroLargo)
        {
            escribirStr("\n\n") ;
            formato() ;
            escribirStr("\n\n el numero de segmento (seg = ") ;
            escribirLHex(numLargo, 1) ;
            escribirStr(") debe ser <= FFFF \n") ;
            return ;
        }
    }
    formato() ;
}

void main ( int argc, char * argv [ ] )
{

    int i, j, k ;

    if (argc <= 1)                              /* al menos dos argumentos */
    {
        formato() ;
        return ;
    }
    if ((argc == 2) && (iguales(argv[1], "-h") || iguales(argv[1], "-H")))
    {
        help() ;
        return ;
    }
    k = 0 ;                              /* comando es una variable global */
    for ( i = 1 ; i < argc ; i ++ )
    {
        for ( j = 0 ; argv[i][j] != (char)0 ; j++ )
            comando[0][k++] = mayuscula(argv[i][j]) ;
        comando[0][k++] = ' ' ;
    }
    comando[0][k--] = (char)0 ;

    dmp() ;

}

