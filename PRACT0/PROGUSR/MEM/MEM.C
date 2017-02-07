/* ----------------------------------------------------------------------- */
/*                                  mem.c                                  */
/* ----------------------------------------------------------------------- */
/*                                                                         */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\ll_s_so1.h>    /* biblioteca de llamadas al sistema SO1 */
#include <so1pub.h\escribir.h>

#include <so1pub.h\strings.h>

descProceso_t descProceso[maxProcesos] ;

e2PFR_t e2PFR ;

c2c_t c2cPFR [ numColasPFR ] ;

ptrBloque_t listaLibres ;

word_t tamBlqMax ;

void mostrarParticion ( pindx_t pindx )
{
    escribirHex(descProceso[pindx].CSProc, 4) ;
    escribirStr(":0000 ") ;
    escribirHex(descProceso[pindx].tam, 4) ;
    escribirStr(" Ps ocupada ") ;
    escribirDec(pindx, 3) ;
    escribirDec(descProceso[pindx].pid, 4) ;
    escribirStr(" ") ;
    escribirStrHasta(descProceso[pindx].programa, 12, TRUE) ;
    escribirStr(" ") ;
    escribirStrHasta(descProceso[pindx].comando, 25, TRUE) ;
}

void mostrarHueco ( ptrBloque_t ptrBloque )
{
    escribirHex(seg((pointer_t)ptrBloque), 4) ;
    escribirStr(":") ;
    escribirHex(off((pointer_t)ptrBloque), 4) ;
    escribirStr(" ") ;
    escribirHex(ptrBloque->tam, 4) ;
    escribirStr(" Ps") ;
    escribirStr(" libre ") ;
}

void mostrarOtroBloqueOcupado( word_t segmento, word_t tam )
{
    escribirHex(segmento, 4) ;
    escribirStr(":") ;
    escribirHex(0x0000, 4) ;
    escribirStr(" ") ;
    escribirHex(tam, 4) ;
    escribirStr(" Ps") ;
    escribirStr(" ocupado ") ;
}

void formato ( void )
{
    escribirStr(
        " formato: MEM [ -l | -h ] "
    ) ;
}

void help ( void )
{
    escribirStr(
        "\n"
        "\n"
        " formato: MEM [ -l | -h ]                   \n"
        "\n"
        " muestra el estado de la memoria gestionada \n"
        " como una lista de bloques libres ordenados \n"
        " por direccion (segmento:0000). opciones:   \n"
        "\n"
        "   -l : muestra solo los bloques libres     \n"
        "   -h : muestra este help                   \n"
    ) ;
}

void mem ( bool_t mostrarTodos )
{

    int i = 0 ;
    pindx_t pindx = 0 ;
    ptrBloque_t ptrBloque ;
    word_t tam ;
    word_t segmento ;

    obtenInfoMEM((descProceso_t far *)&descProceso,
                 (e2PFR_t far *)&e2PFR,
                 (c2c_t far *)&c2cPFR,
                 (ptrBloque_t *)&listaLibres,
                 (word_t *)&tamBlqMax) ;

    ptrBloque = (ptrBloque_t)pointer(listaLibres->sig, 0x0000) ;
    tam = descProceso[pindx].tam ;
    segmento = descProceso[pindx].CSProc ;

    escribirStr(" (mayor bloque libre: ") ;
    escribirHex(tamBlqMax, 4) ;
    escribirStr(" Ps = ") ;
    escribirDec(tamBlqMax/64, 1) ;
    escribirStr(" KB)") ;

    escribirStr(
        "\n"
        "\n"
        " part inicio    tam     estado  ind pid programa     comando\n"
        " ---- --------- ------- ------- --- --- ------------ ------------------------"
    ) ;

    if (!mostrarTodos)
    {
        while (seg((pointer_t)ptrBloque) != seg((pointer_t)listaLibres))
        {
            escribirStr("\n ") ;
            escribirDec(i++, 4) ;
            escribirStr(" ") ;
            mostrarHueco(ptrBloque) ;
            ptrBloque = (ptrBloque_t)pointer(ptrBloque->sig, 0x0000) ;
        }
        escribirLn() ;
        return ;
    }

    escribirStr("\n ") ;
    escribirDec(i++, 4) ;
    escribirStr(" ") ;
    mostrarParticion(pindx) ;
    pindx = c2cPFR[POrdenados].e[pindx].sig ;

    while ((segmento + tam) != seg((pointer_t)listaLibres))
    {
        escribirStr("\n ") ;
        escribirDec(i++, 4) ;
        escribirStr(" ") ;
        if ((segmento + tam) == seg((pointer_t)ptrBloque))
        {
            mostrarHueco(ptrBloque) ;
            segmento = seg((pointer_t)ptrBloque) ;
            tam = ptrBloque->tam ;
            ptrBloque = (bloque_t far *)pointer(ptrBloque->sig, 0x0000) ;
        }
        else if ((segmento + tam) == descProceso[pindx].CSProc)
        {
            mostrarParticion(pindx) ;
            segmento = descProceso[pindx].CSProc ;
            tam = descProceso[pindx].tam ;
            pindx = c2cPFR[POrdenados].e[pindx].sig ;
        }
        else if ((seg((pointer_t)ptrBloque) < descProceso[pindx].CSProc) ||
                 ((pindx == 0) && (ptrBloque != listaLibres)))
        {
            mostrarOtroBloqueOcupado(
                segmento+tam,
                seg((pointer_t)ptrBloque)-(segmento+tam)
            ) ;
            segmento = segmento+tam ;
            tam = seg((pointer_t)ptrBloque)-segmento ;
        }
        else if (pindx != 0)
        {
            mostrarOtroBloqueOcupado(
                segmento+tam,
                descProceso[pindx].CSProc-(segmento+tam)
            ) ;
            segmento = segmento+tam ;
            tam = descProceso[pindx].CSProc-segmento ;
        }
        else
        {
            mostrarOtroBloqueOcupado(
                segmento+tam,
                seg((pointer_t)listaLibres)-(segmento+tam)
            ) ;
            break ;
        }
    }

    escribirLn() ;

}

void main ( int argc, char * argv [ ] )
{
    if (argc == 1) mem(TRUE) ;
    else if (argc == 2)
        if (iguales(argv[1], "-h")) help() ;
        else if (iguales(argv[1], "-l")) mem(FALSE) ;
        else formato() ;
    else formato() ;
}
