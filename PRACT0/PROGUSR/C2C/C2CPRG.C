/* ----------------------------------------------------------------------- */
/*                                  cola.c                                 */
/* ----------------------------------------------------------------------- */
/*                                                                         */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\ll_s_so1.h>    /* biblioteca de llamadas al sistema SO1 */
#include <so1pub.h\escribir.h>

#include <so1pub.h\strings.h>                                   /* iguales */
#include <so1pub.h\scanner.h>

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

#define ePtr(ptr)                                                            \
  escribirPtr(ptr)

#define eC(pindx)                                                            \
  eS("\n descProceso[") ;                                                    \
  eD(pindx, 1) ;                                                             \
  eS("].") ;

descProceso_t descProceso [ maxProcesos ] ;

descFichero_t descFichero [ dfsMax ] ;

descRecurso_t descRecurso [ maxRecursos ] ;

e2PFR_t e2PFR ;

c2c_t c2cPFR [ numColasPFR ] ;

char strCola [ ] [ 26 ] =
{
    "c2cPFR[DPLibres   ]",
    "c2cPFR[DPOcupados ]",
    "c2cPFR[DFLibres   ]",
    "c2cPFR[DFOcupados ]",
    "c2cPFR[DRLibres   ]",
    "c2cPFR[DROcupados ]",
    "c2cPFR[PPreparados]",
    "c2cPFR[PUrgentes  ]",
    "c2cPFR[POrdenados ]",
    "descProceso[0].c2cHijos",
    "descRecurso[0].c2cFichRec"
} ;

ptrC2c_t ptrC2c [ 11 ] ;

void ePC2c ( ptrC2c_t ptrC2c, char opcion)
{
    int i, j ;
    int numElem = ptrC2c->numElem ;
    int primero = ptrC2c->primero ;
    int cabecera = ptrC2c->cabecera ;
    if (opcion == 'e')
    {
        eI(numElem,2 ) ;
        eS(" ") ;
        eI(primero,2 ) ;
        eS(" ") ;
        eI(cabecera,2 ) ;
        eS(" ") ;
    }
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

void formato ( void )
{
//  escribirStrIntenso(" formato: COLA [ num | -n | -e | -h ] ") ; */
    escribirStr(" formato: COLA [ num | -n | -e | -h ] ") ;
}

void help ( void )
{
    escribirStr(
        "\n"
        "\n"
        " formato : COLA [ num | -n | -e | -h ]                            \n"
        "      -n : muestra todas las colas sin mas detalles               \n"
        "      -e : muestra ademas numElen, primero y cabecera             \n"
        "      -h : muestra este help                                      \n"
        "     num : muestra en detalle la cola con ese numero            \n\n"
        "       0 : DPLibres    : cola de descriptores de proceso libres   \n"
        "       1 : DPOcupados  : cola de descriptores de proceso ocupados \n"
        "       2 : DFLibres    : cola de descriptores de fichero libres   \n"
        "       3 : DFOcupados  : cola de descriptores de fichero ocupados \n"
        "       4 : DRLibres    : cola de descriptores de recurso libres   \n"
        "       5 : DROcupados  : cola de descriptores de recurso ocupados \n"
        "       6 : PPreparados : cola de procesos preparados              \n"
        "       7 : PUrgentes   : cola de procesos urgentes                \n"
        "       8 : POrdenados  : cola de procesos ordenados por direccion \n"
        "       9 : c2cHijos    : cola de procesos hijos de un proceso     \n"
        "      10 : c2cFichRec  : cola de ficheros de un recurso           \n"
        "\n"
    ) ;
}

void colas ( char opcion, word_t num )
{

    pindx_t pindx ;
    rindx_t rindx ;
    int numElem ;
    int primero ;
    int cabecera ;
    int i ;

    if ((opcion == 'n') || (opcion == 'e'))
    {

        eS("\n ") ;

        eS("\n c2cPFR[DPLibres   ] = ") ;
        ePC2c(&c2cPFR[DPLibres   ], opcion) ;
        eS("\n c2cPFR[DPOcupados ] = ") ;
        ePC2c(&c2cPFR[DPOcupados ], opcion) ;
        eS("\n c2cPFR[DFLibres   ] = ") ;
        ePC2c(&c2cPFR[DFLibres   ], opcion) ;
        eS("\n c2cPFR[DFOcupados ] = ") ;
        ePC2c(&c2cPFR[DFOcupados ], opcion) ;
        eS("\n c2cPFR[DRLibres   ] = ") ;
        ePC2c(&c2cPFR[DRLibres   ], opcion) ;
        eS("\n c2cPFR[DROcupados ] = ") ;
        ePC2c(&c2cPFR[DROcupados ], opcion) ;
        eS("\n c2cPFR[PPreparados] = ") ;
        ePC2c(&c2cPFR[PPreparados], opcion) ;
        eS("\n c2cPFR[PUrgentes  ] = ") ;
        ePC2c(&c2cPFR[PUrgentes  ], opcion) ;
        eS("\n c2cPFR[POrdenados ] = ") ;
        ePC2c(&c2cPFR[POrdenados ], opcion) ;
        eS("\n c2cPFR[PDormidos  ] = ") ;
        ePC2c(&c2cPFR[PDormidos  ], opcion) ;

        if (c2cPFR[DPOcupados].numElem > 0)
            eS("\n ") ;
        for ( pindx = 0 ; pindx < maxProcesos ; pindx++ )
            if (descProceso[pindx].estado != libre)
            {
                eC(pindx) ;
                eS("c2cHijos = ") ;
                ePC2c(&descProceso[pindx].c2cHijos, opcion) ;
            }

        if (c2cPFR[DROcupados].numElem > 0)
            eS("\n ") ;
        for ( rindx = 0 ; rindx < maxRecursos ; rindx++ )
            if (descRecurso[rindx].tipo != rLibre)
            {
                eS("\n descRecurso[") ;
                eD(rindx, 1) ;
                eS("].") ;
                eS("c2cFichRec = ") ;
                ePC2c(&descRecurso[rindx].c2cFichRec, opcion) ;
            }
        eS("\n ") ;
    }
    else
    {
        eS("\n\n ") ;
        ePC2c(ptrC2c[num], 'n') ;
        eS("\n\n") ;
        numElem = ptrC2c[num]->numElem ;
        primero = ptrC2c[num]->primero ;
        cabecera = ptrC2c[num]->cabecera ;
        eS(" ") ;
        eS(strCola[num]) ;
        eS(".numElem  = ") ;
        eI(numElem,2 ) ;
        eS("\n") ;
        eS(" ") ;
        eS(strCola[num]) ;
        eS(".primero  = ") ;
        eI(primero,2 ) ;
        eS("\n") ;
        eS(" ") ;
        eS(strCola[num]) ;
        eS(".cabecera = ") ;
        eI(cabecera,2 ) ;
        eS("\n") ;
        eS(" ") ;
        eS(strCola[num]) ;
        eS(".e        = ") ;
        ePtr((pointer_t)ptrC2c[num]->e) ;
        eS("\n") ;
        eS("\n        i: ") ;
        for ( i = 0 ; i <= cabecera ; i++ )
        {
            eI(i, 2) ;
            escribirCar(' ') ;
        }
        eS("\n ---------") ;
        for ( i = 0 ; i <= cabecera ; i++ )
        {
            eS("---") ;
        }
        eS("\n e[i].sig: ") ;
        for ( i = 0 ; i <= cabecera ; i++ )
        {
            eI(ptrC2c[num]->e[i].sig, 2) ;
            escribirCar(' ') ;
        }
        eS("\n e[i].ant: ") ;
        for ( i = 0 ; i <= cabecera ; i++ )
        {
            eI(ptrC2c[num]->e[i].ant, 2) ;
            escribirCar(' ') ;
        }
        eS("\n") ;
    }
}

void main ( int argc, char * argv [ ] )
{

    obtenInfoPFR ((descProceso_t far *)&descProceso,
                  (descFichero_t far *)&descFichero,
                  (descRecurso_t far *)&descRecurso,
                  (e2PFR_t far *)&e2PFR,
                  (c2c_t far *)&c2cPFR
                 ) ;
    if (argc == 2)
        if (iguales(argv[1], "-h") || iguales(argv[1], "-H"))
            help() ;
        else if (iguales(argv[1], "-n") || iguales(argv[1], "-N"))
            colas('n', 0) ;
        else if (iguales(argv[1], "-e") || iguales(argv[1], "-E"))
            colas('e', 0) ;
        else
        {
            copiarStr(argv[1], comando[0]) ;
            inicScanner() ;
            obtenSimb() ;
            if (simb != s_numero)
            {
                escribirCar('\a') ;
                escribirStr("\n\n") ;
                formato() ;
                escribirStr("\n\n error: num debe ser un numero y no \"") ;
                escribirStr(argv[1]) ;
                escribirStr("\"\n") ;
            }
            else if (num <= 10)
            {
                ptrC2c[ 0] = &c2cPFR[DPLibres] ;
                ptrC2c[ 1] = &c2cPFR[DPOcupados] ;
                ptrC2c[ 2] = &c2cPFR[DFLibres] ;
                ptrC2c[ 3] = &c2cPFR[DFOcupados] ;
                ptrC2c[ 4] = &c2cPFR[DRLibres] ;
                ptrC2c[ 5] = &c2cPFR[DROcupados] ;
                ptrC2c[ 6] = &c2cPFR[PPreparados] ;
                ptrC2c[ 7] = &c2cPFR[PUrgentes] ;
                ptrC2c[ 8] = &c2cPFR[POrdenados] ;
                ptrC2c[ 9] = &descProceso[0].c2cHijos ;
                ptrC2c[10] = &descRecurso[0].c2cFichRec ;
                colas('f', num) ;
            }
            else
            {
                escribirCar('\a') ;
                escribirStr("\n\n") ;
                formato() ;
                escribirStr("\n\n error: num debe ser <= 10 \n") ;
            }
        }
    else if (argc == 1)
        colas('n', 0) ;
    else
        formato() ;
}

