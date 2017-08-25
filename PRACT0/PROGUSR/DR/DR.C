/* ----------------------------------------------------------------------- */
/*                                   dr.c                                  */
/* ----------------------------------------------------------------------- */
/*                         descriptores de recurso                         */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\ll_s_so1.h>    /* biblioteca de llamadas al sistema SO1 */
#include <so1pub.h\escribir.h>

#include <so1pub.h\strings.h>                                   /* iguales */
#include <so1pub.h\scanner.h>
#include <so1pub.h\carsctrl.h>                                      /* BEL */

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

#define eC(rindx)                                                            \
  eS("\n descRecurso[") ;                                                    \
  eD(rindx, 1) ;                                                             \
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

void eCcb ( ccb_t ccb )
{
    int i, j ;
    eI(ccb->num, 1) ;
    eS(" ") ;
    eI(ccb->in, 1) ;
    eS(" ") ;
    eI(ccb->out, 1) ;
    eS(" ") ;
    eI(ccb->max, 1) ;
    eS(" ") ;
    eP(ccb->arg) ;
    eS(" ") ;
    eS("[ ") ;
    j = ccb->in ;
    for ( i = 0 ; i < ccb->num ; i ++ )
    {
        eP(ccb->callBack[j]) ;
        eS(" ") ;
        j = (j + 1) % ccb->max ;
    }
    eS("] ") ;
}

char strTipoRecurso [ ] [ 13 ] =
{
    "rLibre",
    "rDCaracteres",
    "rDBloques",
    "rTuberia",
    "rGP",
    "rGM",
    "rSF",
    "rOtro"
} ;

descProceso_t descProceso [ maxProcesos ] ;

descFichero_t descFichero [ dfsMax ] ;

descRecurso_t descRecurso [ maxRecursos ] ;

e2PFR_t e2PFR ;

c2c_t c2cPFR [ numColasPFR ] ;

void mostrarDescRecurso ( rindx_t rindx )
{

    tipoRecurso_t tipo = descRecurso[rindx].tipo ;
    byte_t numVI = descRecurso[rindx].numVI ;
    int i ;

    eC(rindx) ;
    eS("nombre     = ") ;
    eS("\"") ;
    eS(descRecurso[rindx].nombre) ;
    eS("\"") ;
    eC(rindx) ;
    eS("tipo       = ") ;
    eS(strTipoRecurso[tipo]) ;
    eS(" (") ;
    eI(tipo, 1) ;
    eS(")") ;
    eC(rindx) ;
    eS("ccb        = ") ;
    eCcb(descRecurso[rindx].ccb) ;
    eC(rindx) ;
    eS("pindx      = ") ;
    eI(descRecurso[rindx].pindx, 1) ;
    eC(rindx) ;
    eS("c2cFichRec = ") ;
    ePC2c(&descRecurso[rindx].c2cFichRec) ;
    eC(rindx) ;
    eS("numVI      = ") ;
    eI(numVI, 1) ;
    for ( i = 0 ; i < numVI ; i++ )
    {
        eC(rindx) ;
        eS("nVInt[") ;
        eI(i, 1) ;
        eS("]   = ") ;
        eH(descRecurso[rindx].nVInt[0], 2) ;
        eC(rindx) ;
        eS("irq[") ;
        eI(i, 1) ;
        eS("]     = ") ;
        eI(descRecurso[rindx].irq[0], 1) ;
        eC(rindx) ;
        eS("isr[") ;
        eI(i, 1) ;
        eS("]     = ") ;
        eP(descRecurso[rindx].isr[0]) ;
    }
    eS("\n ") ;
    eC(rindx) ;
    eS("open       = ") ;
    eP(descRecurso[rindx].open) ;
    eC(rindx) ;
    eS("release    = ") ;
    eP(descRecurso[rindx].release) ;
    eC(rindx) ;
    eS("read       = ") ;
    eP(descRecurso[rindx].read) ;
    eC(rindx) ;
    eS("aio_read   = ") ;
    eP(descRecurso[rindx].aio_read) ;
    eC(rindx) ;
    eS("write      = ") ;
    eP(descRecurso[rindx].write) ;
    eC(rindx) ;
    eS("aio_write  = ") ;
    eP(descRecurso[rindx].aio_write) ;
    eC(rindx) ;
    eS("lseek      = ") ;
    eP(descRecurso[rindx].lseek) ;
    eC(rindx) ;
    eS("fcntl      = ") ;
    eP(descRecurso[rindx].fcntl) ;
    eC(rindx) ;
    eS("ioctl      = ") ;
    eP(descRecurso[rindx].ioctl) ;
    eS("\n ") ;
    eC(rindx) ;
    eS("eliminar   = ") ;
    eP(descRecurso[rindx].eliminar) ;
    eS("\n ") ;

}

void mostrarRecursos ( void )
{
    int rindx ;
    int i ;
    eS("\n ") ;
    eS("\n rindx nombre       tp pindx   open    V0 irq0   isr 0   V1 irq1   isr 1   ") ;
    eS("\n ----- ------------ -- ----- --------- -- ---- --------- -- ---- --------- ") ;
    for ( rindx = 0 ; rindx < maxRecursos ; rindx++ )
        if (descRecurso[rindx].pindx >= 0)
        {
            eS("\n ") ;
            eD(rindx, 5) ;
            eS(" ") ;
            eSH(descRecurso[rindx].nombre, 12, TRUE) ;
            eI(descRecurso[rindx].tipo, 3) ;
            eI(descRecurso[rindx].pindx, 6) ;
            eS(" ") ;
            eP(descRecurso[rindx].open) ;
            eS(" ") ;
            for ( i = 0 ; i < descRecurso[rindx].numVI ; i++ )
            {
                eH(descRecurso[rindx].nVInt[i], 2) ;
                eS(" ") ;
                eI(descRecurso[rindx].irq[i], 4) ;
                eS(" ") ;
                eP(descRecurso[rindx].isr[i]) ;
                eS(" ") ;
            }
        }
    eS("\n ") ;
}

void formato ( void )
{
//  escribirStrIntenso(" formato: DR [ rindx | -a | -h ] ") ;
    escribirStr(" formato: DR [ rindx | -a | -h ] ") ;
}

void help ( void )
{
    escribirLn() ;
    escribirLn() ;
    escribirStr(" formato : DR [ rindx | -a | -h ]              \n\n") ;
    escribirStr(" muestra los campos del descriptor de recurso  \n") ;
    escribirStr(" rindx (descRecurso[rindx])                    \n\n") ;
    escribirStr(" opciones: (por defecto -a)                    \n\n") ;
    escribirStr("   rindx : muestra solo ese recurso en detalle \n") ;
    escribirStr("      -a : muestra todos los recursos          \n") ;
    escribirStr("      -h : muestra este help                   \n") ;
}

void main ( int argc, char * argv [ ] )
{

    rindx_t rindx ;
    obtenInfoPFR ((descProceso_t far *)&descProceso,
                  (descFichero_t far *)&descFichero,
                  (descRecurso_t far *)&descRecurso,
                  (e2PFR_t far *)&e2PFR,
                  (c2c_t far *)&c2cPFR
                 ) ;
    if (argc == 2)
        if (iguales(argv[1], "-h") || iguales(argv[1], "-H"))
            help() ;
        else if (iguales(argv[1], "-a") || iguales(argv[1], "-A"))
            mostrarRecursos() ;
        else
        {
            copiarStr(argv[1], comando[0]) ;
            inicScanner() ;
            obtenSimb() ;
            if (simb != s_numero)
            {
                escribirCar(BEL) ;
                formato() ;
                escribirStr("\n error: rindx debe ser un numero y no \"") ;
                escribirStr(argv[1]) ;
                escribirStr("\"\n") ;
            }
            else
            {
                rindx = (rindx_t)num ;
                if ((0 <= rindx) && (rindx < maxRecursos))
                    if (descRecurso[rindx].tipo != rLibre)
                    {
                        escribirLn() ;
                        mostrarDescRecurso(rindx) ;
                    }
                    else
                    {
//                      escribirStrIntenso(" descriptor de recurso no utilizado ") ;
                        escribirStr(" descriptor de recurso no utilizado ") ;
                    }
                else
                {
//                  escribirStrIntenso(" rindx erroneo ") ;
                    escribirStr(" rindx erroneo ") ;
                }
            }
        }
    else if (argc == 1)
        mostrarRecursos() ;
    else
        formato() ;
}

