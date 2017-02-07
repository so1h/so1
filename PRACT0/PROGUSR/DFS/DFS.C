/* ----------------------------------------------------------------------- */
/*                                  dfs.c                                  */
/* ----------------------------------------------------------------------- */
/*                   descriptores de fichero del sistema                   */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\ll_s_so1.h>    /* biblioteca de llamadas al sistema SO1 */
#include <so1pub.h\escribir.h>

#include <so1pub.h\strings.h>                                   /* iguales */
#include <so1pub.h\scanner.h>

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

#define eC(dfs)                                                              \
  eS("\n descFichero[") ;                                                    \
  eD(dfs, 1) ;                                                               \
  eS("].") ;

#define lMaxStrTipo 14

char strTipo [ ][14] = { "flibre",
                         "fRegular",
                         "fedBloques",
                         "fedCaracteres",
                         "tuberia"
                       } ;

descProceso_t descProceso [ maxProcesos ] ;

descFichero_t descFichero [ dfsMax ] ;

descRecurso_t descRecurso [ maxRecursos ] ;

e2PFR_t e2PFR ;

c2c_t c2cPFR [ numColasPFR ] ;

void mostrarDescFichero ( dfs_t dfs )
{

    tipoFichero_t tipo = descFichero[dfs].tipo ;

    eC(dfs) ;
    eS("nombre    = ") ;
    eS("\"") ;
    eS(descFichero[dfs].nombre) ;
    eS("\"") ;
    eC(dfs) ;
    eS("tipo      = ") ;
    eSH(strTipo[tipo], lMaxStrTipo-1, TRUE) ;
    eS(" (") ;
    eI(tipo, 1) ;
    eS(")") ;
    eC(dfs) ;
    eS("rindx     = ") ;
    eI(descFichero[dfs].rindx, 1) ;
    eC(dfs) ;
    eS("menor     = ") ;
    eI(descFichero[dfs].menor, 1) ;
    eC(dfs) ;
    eS("shareMode = ") ;
    eH(descFichero[dfs].shareMode, 4) ;
    eC(dfs) ;
    eS("contAp_L  = ") ;
    eI(descFichero[dfs].contAp_L, 1) ;
    eC(dfs) ;
    eS("contAp_E  = ") ;
    eI(descFichero[dfs].contAp_E, 1) ;

    eS("\n ") ;
}

void mostrarFicheros ( void )
{
    int dfs ;
    tipoFichero_t tipo ;
    eS("\n ") ;
    eS("\n dfs nombre       tipo          rindx menor shareM cAp_L cAp_E ") ;
    eS("\n --- ------------ ------------- ----- ----- ------ ----- ----- ") ;
    for ( dfs = 0 ; dfs < dfsMax ; dfs++ )
    {
        tipo = descFichero[dfs].tipo ;
        if (tipo != flibre)
        {
            eS("\n ") ;
            eD(dfs, 3) ;
            eS(" ") ;
            eSH(descFichero[dfs].nombre, 12, TRUE) ;
            eS(" ") ;
            eSH(strTipo[tipo], lMaxStrTipo-1, TRUE) ;
            eS(" ") ;
            eI(descFichero[dfs].rindx, 5) ;
            eS(" ") ;
            eI(descFichero[dfs].menor, 5) ;
            eS("  ") ;
            eH(descFichero[dfs].shareMode, 4) ;
            eS("  ") ;
            eI(descFichero[dfs].contAp_L, 5) ;
            eS(" ") ;
            eI(descFichero[dfs].contAp_E, 5) ;
            eS(" ") ;
        }
    }
    eS("\n ") ;
}

void formato ( void )
{
//  escribirStrIntenso(" formato: DFS [ dfs | -a | -h ] ") ;
    escribirStr(" formato: DFS [ dfs | -a | -h ] ") ;
}

void help ( void )
{
    escribirLn() ;
    escribirLn() ;
    escribirStr(" formato : DFS [ dfs | -a | -h ]               \n\n") ;
    escribirStr(" muestra los campos del descriptor de fichero  \n") ;
    escribirStr(" dfs (descFichero[dfs])                        \n\n") ;
    escribirStr(" opciones: (por defecto -a)                    \n\n") ;
    escribirStr("     dfs : muestra solo ese fichero en detalle \n") ;
    escribirStr("      -a : muestra todos los ficheros          \n") ;
    escribirStr("      -h : muestra este help                   \n") ;
}

void main ( int argc, char * argv [ ] )
{

    dfs_t dfs ;
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
            mostrarFicheros() ;
        else
        {
            copiarStr(argv[1], comando[0]) ;
            inicScanner() ;
            obtenSimb() ;
            if (simb != s_numero)
            {
                escribirCar('\a') ;
                formato() ;
                escribirStr("\n error: dfs debe ser un numero y no \"") ;
                escribirStr(argv[1]) ;
                escribirStr("\"\n") ;
            }
            else
            {
                dfs = (dfs_t)num ;
                if ((0 <= dfs) && (dfs < dfsMax))
                    if (descFichero[dfs].tipo != flibre)
                    {
                        escribirLn() ;
                        mostrarDescFichero(dfs) ;
                    }
                    else
                    {
//                      escribirStrIntenso(" descriptor de fichero no utilizado ") ;
                        escribirStr(" descriptor de fichero no utilizado ") ;
                    }
                else
                {
//                  escribirStrIntenso(" dfs erroneo ") ;
                    escribirStr(" dfs erroneo ") ;
                }
            }
        }
    else if (argc == 1)
        mostrarFicheros() ;
    else
        formato() ;
}

