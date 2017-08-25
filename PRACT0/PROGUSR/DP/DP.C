/* ----------------------------------------------------------------------- */
/*                                   dp.c                                  */
/* ----------------------------------------------------------------------- */
/*                         descriptores de proceso                         */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\ll_s_so1.h>    /* biblioteca de llamadas al sistema SO1 */
#include <so1pub.h\escribir.h>

#include <so1pub.h\strings.h>                                   /* iguales */
#include <so1pub.h\scanner.h>
#include <so1pub.h\caracter.h>                                /* minuscula */

#include <so1pub.h\ptrc2c.h>                                   /* ptrC2c_t */

#define eS(str)                                                              \
  escribirStr(str)

#define eSH(str, ancho, bool)                                                \
  escribirStrHasta(str, ancho, bool)

#define eH(num, ancho)                                                       \
  escribirHex(num, ancho)

#define eLH(num, ancho)                                                      \
  escribirLHex(num, ancho)

#define eD(num, ancho)                                                       \
  escribirDec(num, ancho)

#define eLD(num, ancho)                                                      \
  escribirLDec(num, ancho)

#define eI(num, ancho)                                                       \
  escribirInt(num, ancho)

#define eP(ptr)                                                              \
  escribirPtr((pointer_t)ptr)

#define eC(pindx)                                                            \
  eS("\n descProceso[") ;                                                    \
  eD(pindx, 1) ;                                                             \
  eS("].") ;

void ePC2c ( ptrC2c_t ptrC2c )
{
    int i, j, numElem, primero, cabecera ;
    numElem = ptrC2c->numElem ;
    primero = ptrC2c->primero ;
    cabecera = ptrC2c->cabecera ;
    eI(numElem,1 ) ;
    eS(" ") ;
    eI(primero,1 ) ;
    eS(" ") ;
    eI(cabecera,1 ) ;
    eS(" ") ;
    eS("[ ") ;
    i = primero ;
    for ( j = 0 ; j < numElem ; j++ )
    {
        eI(i, 1) ;
        /* eD(descProceso[i].pid, 1) ; */
        escribirCar(' ') ;
        i = ptrC2c->e[i].sig ;
    }
    eS("] ") ;
}

void eNibble ( word_t w )
{
    int i, masc ;
    masc = 1 << 4 ;
    for ( i = 0 ; i < 4 ; i++ )
    {
        masc = masc >> 1 ;
        if ((masc & w) != 0) eS("1") ;
        else eS("0") ;
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
//  return(_AX) ;
#endif
}

#define longMaxStrEstado 13

char strEstado[ ][13] = { "libre",
                          "preparado",
                          "ejecutandose",
                          "bloqueado"
                        } ;

char carRecurso [ ] =
{
    '!',
    'H', /* rec_hijo            (rindx_t)(- 1) */
    'Z', /* rec_zombie          (rindx_t)(- 2) */
    'D', /* rec_desinstalacion  (rindx_t)(- 3) */
    'S', /* rec_semaforo        (rindx_t)(- 4) */
    'B', /* rec_buzon           (rindx_t)(- 5) */
    'C', /* rec_serie           (rindx_t)(- 6) */
    'R', /* rec_raton           (rindx_t)(- 7) */
    'r', /* rec_ratonRaw        (rindx_t)(- 8) */
    'A', /* rec_tecladoRaton    (rindx_t)(- 9) */
    'a', /* rec_tecladoRawRaton (rindx_t)(-10) */
} ;

char strRecurso [ ] [20] =
{
    "!",
    "rec_hijo",              /* (rindx_t)(- 1) */
    "rec_zombie",            /* (rindx_t)(- 2) */
    "rec_desinstalacion",    /* (rindx_t)(- 3) */
    "rec_semaforo",          /* (rindx_t)(- 3) */
    "rec_buzon",             /* (rindx_t)(- 4) */
    "rec_serie",             /* (rindx_t)(- 5) */
    "rec_raton",             /* (rindx_t)(- 6) */
    "rec_ratonRaw",          /* (rindx_t)(- 7) */
    "rec_tecladoRaton",      /* (rindx_t)(- 8) */
    "rec_tecladoRawRaton"    /* (rindx_t)(- 9) */
} ;

#define tamCarRec (sizeof(carRecurso)/sizeof(char))

descProceso_t descProceso [ maxProcesos ] ;

descFichero_t descFichero [ dfsMax ] ;

descRecurso_t descRecurso [ maxRecursos ] ;

e2PFR_t e2PFR ;

c2c_t c2cPFR [ numColasPFR ] ;

info_t info ;

ptrBloque_t listaLibres ;

word_t tamBlqMax ;

void mostrarProcesos ( void )
{

//char car ;
    int i, j ;
    int ind ;
    word_t flags ;
    rindx_t esperandoPor ;

    escribirStr(" cola de preparados: ") ;
    ePC2c((ptrC2c_t)&c2cPFR[PPreparados]) ;
    escribirStr(
        "\n"
        "\n"
        " pindx pid comando       estado   rec CS   IP   DS   FBSS SP    NnpODITSZ A P C\n"
        " ----- --- ------------- ------------ ---- ---- ---- ---- ---- ----------------"
    ) ;
    for ( i = 0 ; i < maxProcesos ; i++ )
    {
        if (descProceso[i].estado != libre)
        {
            escribirStr("\n ") ;
            escribirDec(i, 5) ;
            escribirCar(' ') ;
            escribirDec(descProceso[i].pid, 3) ;
            escribirCar(' ') ;
            escribirStrHasta(descProceso[i].comando, 13, TRUE) ;
            escribirCar(' ') ;
            if (descProceso[i].estado == bloqueado)
            {
                escribirStr("bloqueado") ;
                esperandoPor = descProceso[i].esperandoPor ;
                escribirCar('(') ;
                ind = (-1)*esperandoPor ;
                if (ind > 0)
                    if (ind < tamCarRec) escribirCar(carRecurso[ind]) ;
                    else escribirCar('#') ;
                else
                    escribirDec(esperandoPor, 1) ;
                escribirCar(')') ;
            }
            else
                escribirStrHasta(strEstado[descProceso[i].estado], longMaxStrEstado-1, TRUE) ;
            if ((descProceso[i].estado == bloqueado) && (descProceso[i].esperandoPor == rec_zombie))
                continue ;

            if (descProceso[i].pid != getpid())
            {
                escribirCar(' ') ;
                escribirHex(descProceso[i].trama->CS, 4) ;
                escribirCar(' ') ;
                escribirHex(descProceso[i].trama->IP, 4) ;
                escribirCar(' ') ;
                escribirHex(descProceso[i].trama->DS, 4) ;
                escribirCar(' ') ;
                escribirHex(descProceso[i].desplPila, 4) ;
                escribirCar(' ') ;
                escribirHex(off((pointer_t)descProceso[i].trama), 4) ;
                escribirCar(' ') ;
                flags = descProceso[i].trama->Flags ;
                for ( j = 0 ; j < 16 ; j++)
                {
                    escribirDec(flags/0x8000, 1) ;
                    flags = flags << 1 ;
                }
            }
            else
            {
                escribirCar(' ') ;
                escribirHex(_CS, 4) ;
                escribirCar(' ') ;
                escribirHex(miIP(), 4) ;
                escribirCar(' ') ;
                escribirHex(_DS, 4) ;
                escribirCar(' ') ;
                escribirHex(descProceso[i].desplPila, 4) ;
                escribirCar(' ') ;
                escribirHex(_SP, 4) ;
                escribirCar(' ') ;
                asm pushf
                asm pop ax
                asm mov flags,ax
                for ( j = 0 ; j < 16 ; j++)
                {
                    escribirDec(flags/0x8000, 1) ;
                    flags = flags << 1 ;
                }
            }
/*
            escribirCar(' ') ;
            escribirDec(descProceso[i].teclado->ncar, 2) ;

            escribirCar(' ') ;
            if (descProceso[i].teclado->ncar > 0)
            {
                escribirStr("\n           bufer del teclado = \"") ;
                for ( j = descProceso[i].teclado->out ;
                      j != descProceso[i].teclado->in ;
                      j = ((j + 1) % tamBTeclado) )
                {
                    car = descProceso[i].teclado->bufer[j] ;
                    if ((' ' <= car) && (car <= '~')) escribirCar(car) ;
                    else escribirCar('.') ;
                }
                escribirCar('\"') ;
            }
*/
        }
    }

    escribirLn() ;

}

void mostrarCamposPrincipales ( pindx_t ind )
{

    word_t Flgs ;
    word_t regIP ;
    estado_t estado ;
    rindx_t esperandoPor ;

    eC(ind) ;
    eS("pid          = ") ;
    eI(descProceso[ind].pid, 1) ;
    eS(" (pindx = ") ;
    eI(ind, 1) ;
    eS(")") ;
    eC(ind) ;
    eS("ppindx       = ") ;
    eI(descProceso[ind].ppindx, 1) ;
    eC(ind) ;
    eS("uid          = ") ;
    eI(descProceso[ind].uid, 1) ;
    eC(ind) ;
    eS("gid          = ") ;
    eI(descProceso[ind].gid, 1) ;
    estado = descProceso[ind].estado ;
    eC(ind) ;
    eS("estado       = ") ;
    eS(strEstado[estado]) ;
    eS(" (") ;
    eI(estado, 1) ;
    eS(")") ;
    if (estado == 3 /* bloqueado */)
    {
        esperandoPor = descProceso[ind].esperandoPor ;
        eC(ind) ;
        eS("esperandoPor = ") ;
        if (esperandoPor >= 0)
        {
            eS("\"") ;
            eS(descRecurso[esperandoPor].nombre) ;
            eS("\"") ;
        }
        else
            eS(strRecurso[-esperandoPor]) ;
        eS(" (") ;
        eI(esperandoPor, 1) ;
        eS(")") ;
        switch (esperandoPor)
        {
        case rec_hijo   :
            eC(ind) ;
            eS("hpindx       = ") ;
            eI(descProceso[ind].hpindx, 1) ;
            break ;
        case rec_zombie :
            eC(ind) ;
            eS("noStatus     = ") ;
            if (descProceso[ind].noStatus) eS("TRUE") ;
            else eS("FALSE") ;
            eC(ind) ;
            eS("status       = ") ;
            eI(descProceso[ind].status, 1) ;
            break ;
        default :
            ;
        }
    }
    eC(ind) ;
    eS("c2cHijos     = ") ;
    ePC2c((ptrC2c_t)&descProceso[ind].c2cHijos) ;
//  eC(ind) ; eS("  numElem    = ") ; eD(descProceso[ind].c2cHijos.numElem, 1) ;
//  eC(ind) ; eS("  primero    = ") ; eD(descProceso[ind].c2cHijos.primero, 1) ;
//  eC(ind) ; eS("  cabecera   = ") ; eD(descProceso[ind].c2cHijos.cabecera, 1) ;
    eC(ind) ;
    eS("tamFichero   = ") ;
    eLH(descProceso[ind].tamFichero, 4) ;
    eS(" = ") ;
    eLD(descProceso[ind].tamFichero, 1) ;
    if ((info.modoSO1 == modoSO1_Exe) && (ind == 0))
        eS(" (+ cabecera EXE) ") ;

    eC(ind) ;
    eS("programa     = ") ;
    eS("\"") ;
    eS(descProceso[ind].programa) ;
    eS("\"") ;
    eC(ind) ;
    eS("comando      = ") ;
    eS("\"") ;
    eS(descProceso[ind].comando) ;
    eS("\"") ;

    eC(ind) ;
    eS("tamCodigo    = ") ;
    eH(descProceso[ind].tamCodigo, 1) ;
    eC(ind) ;
    eS("desplBSS     = ") ;
    eH(descProceso[ind].desplBSS, 4) ;
    eC(ind) ;
    eS("desplPila    = ") ;
    eH(descProceso[ind].desplPila, 4) ;

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

    eC(ind) ;
    eS("trama        = ") ;
    eP((pointer_t)descProceso[ind].trama) ;
    eS("       Flags:  0Nnp  ODIT  SZ0A  0P1C ") ;
    eC(ind) ;
    eS("CSProc       = ") ;
    eH(descProceso[ind].CSProc, 4) ;
    eS("                    ----  ----  ----  ---- ") ;
    eC(ind) ;
    eS("tam          = ") ;
    eH(descProceso[ind].tam, 4) ;
    eS(" Ps ") ;
    Flgs = descProceso[ind].trama->Flags ;
    eS("                ") ;
    eNibble((Flgs >> 12) & 0x000F) ;
    eS("  ") ;
    eNibble((Flgs >> 8) & 0x000F) ;
    eS("  ") ;
    eNibble((Flgs >> 4) & 0x000F) ;
    eS("  ") ;
    eNibble(Flgs & 0x000F) ;
    eS("\n ") ;
    eS("\n  DS    ES    DI    SI    BP    SP    BX    DX    CX    AX    IP    CS   Flgs ") ;
    eS("\n ----  ----  ----  ----  ----  ----  ----  ----  ----  ----  ----  ----  ---- ") ;
    eS("\n ") ;
    eH(descProceso[ind].trama->DS, 4) ;
    eS("  ") ;
    eH(descProceso[ind].trama->ES, 4) ;
    eS("  ") ;
    eH(descProceso[ind].trama->DI, 4) ;
    eS("  ") ;
    eH(descProceso[ind].trama->SI, 4) ;
    eS("  ") ;
    eH(descProceso[ind].trama->BP, 4) ;
    eS("  ") ;
    eH(descProceso[ind].trama->SP, 4) ;
    eS("  ") ;
    eH(descProceso[ind].trama->BX, 4) ;
    eS("  ") ;
    eH(descProceso[ind].trama->DX, 4) ;
    eS("  ") ;
    eH(descProceso[ind].trama->CX, 4) ;
    eS("  ") ;
    eH(descProceso[ind].trama->AX, 4) ;
    eS("  ") ;
    eH(descProceso[ind].trama->IP, 4) ;
    eS("  ") ;
    eH(descProceso[ind].trama->CS, 4) ;
    eS("  ") ;
    eH(descProceso[ind].trama->Flags, 4) ;

    if (descProceso[ind].estado == ejecutandose)   /* desapilamos 26 bytes */
    {
        asm add sp,26
    }

}

void mostrarTablaDeFicherosAbiertos ( pindx_t ind )
{

    int df ;
    int dfs ;

    eS("\n ") ;
    eC(ind) ;
    eS("nfa          = ") ;
    eD(descProceso[ind].nfa, 1) ;
    for ( df = 0 ; df < dfMax ; df++ )
        if (descProceso[ind].tfa[df].dfs >= 0)
        {
            eC(ind) ;
            eS("tfa[") ;
            eD(df, 1) ;
            dfs = descProceso[ind].tfa[df].dfs ;
            eS("]       = ( dfs: ") ;
            eI(dfs, 2) ;
            eS(" modoAp: ") ;
            eH(descProceso[ind].tfa[df].modoAp, 4) ;
            eS(" pos: ") ;
            eLD(descProceso[ind].tfa[df].pos, 5) ;
            eS(" \"") ;
            eS(descFichero[dfs].nombre) ;
            eS("\" ) ") ;
        }

}

void mostrarDescProceso ( pindx_t pindx, char opcion )
{

    if ((opcion == ' ') || (opcion == 'p'))
    {
        eS("\n") ;
        mostrarCamposPrincipales(pindx) ;
        if (opcion == ' ')
            leer(STDIN) ; /* while (leerListo(STDIN) == (char)0) ; */
        else if (opcion == 'p')
        {
            eS("\n ") ;
            return ;
        }
    }

    if ((opcion == ' ') || (opcion == 'f'))
    {
        mostrarTablaDeFicherosAbiertos(pindx) ;
        if (opcion == 'f')
        {
            eS("\n ") ;
            return ;
        }
    }
    eS("\n ") ;
}

void formato ( void )
{
    escribirStr(
        "\n"
        "\n"
        " formato : DP ( px [ -p | -f ] | -a | -h ) \n"
    ) ;
}

void help ( void )
{
    formato() ;
    escribirStr(
        "\n"
        " muestra los campos del descriptor de proceso        \n"
        "\n"
        " opciones:                                           \n"
        "\n"
        "   px    : muestra todos los campos del proceso px   \n"
        "   px -p : muestra los principales campos de px      \n"
        "   px -f : muestra su tabla de ficheros abiertos     \n"
        "      -a : muestra todos los procesos                \n"
        "      -h : muestra este help                         \n"
    ) ;
}

void main ( int argc, char * argv [ ] )
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
        return ;
    }
    else if (argc > 3)
    {
        escribirCar('\a') ;
        formato() ;
        exit(-1) ;
    }
    if ((argc == 2) &&
            (argv[1][0] == '-') &&
            (argv[1][2] == (char)0))
    {
        opcion = minuscula(argv[1][1]) ;
        if (opcion == 'h')
        {
            help() ;
            return ;
        }
        else if (opcion == 'a')
        {
            mostrarProcesos() ;
            return ;
        }
    }
    copiarStr(argv[1], comando[0]) ;
    inicScanner() ;
    obtenSimb() ;
    if (simb != s_numero)
    {
        escribirCar('\a') ;
        formato() ;
        escribirStr("\n error: pindx debe ser un numero y no \"") ;
        escribirStr(argv[1]) ;
        escribirStr("\"\n") ;
        exit(-2) ;
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
                    opcion = minuscula(argv[2][1]) ;
                    if ((opcion == 'p') ||
                            (opcion == 'f'))
                        mostrarDescProceso(pindx, opcion) ;
                    else
                    {
                        escribirCar('\a') ;
//                      escribirStrIntenso(" opcion \"") ;
                        escribirStr(" opcion \"") ;
//                      escribirStrIntenso(argv[2]) ;
                        escribirStr(argv[2]) ;
//                      escribirStrIntenso("\" incorrecta ") ;
                        escribirStr("\" incorrecta ") ;
                        exit(-3) ;
                    }
                }
                else
                {
                    escribirCar('\a') ;
                    formato() ;
                    exit(-4) ;
                }
            }
            else
            {
                escribirCar('\a') ;
//              escribirStrIntenso(" descriptor no utilizado ") ;
                escribirStr(" descriptor no utilizado ") ;
                exit(-5) ;
            }
        else
        {
            escribirCar('\a') ;
//          escribirStrIntenso(" pindx erroneo ") ;
            escribirStr(" pindx erroneo ") ;
            exit(-6) ;
        }
    }
}

