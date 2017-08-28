/* ----------------------------------------------------------------------- */
/*                                   ps.c                                  */
/* ----------------------------------------------------------------------- */
/*             comando que muestra el estado de los procesos               */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\ll_s_so1.h>    /* biblioteca de llamadas al sistema SO1 */
#include <so1pub.h\escribir.h>

#define lMaxStrEstado 13

char strEstado[4][13] = { "libre       ",
                          "preparado   ",
                          "ejecutandose",
                          "bloqueado   "
                        } ;

char carRecurso [ ] =
{
    '!',
    'H', /* rec_hijo            (rindx_t)(- 1) */
    'Z', /* rec_zombie          (rindx_t)(- 2) */
    'D', /* rec_desinatalacion  (rindx_t)(- 3) */
    'S', /* rec_semaforo        (rindx_t)(- 4) */
    'B', /* rec_buzon           (rindx_t)(- 5) */
    'C', /* rec_serie           (rindx_t)(- 6) */
    'R', /* rec_raton           (rindx_t)(- 7) */
    'r', /* rec_ratonRaw        (rindx_t)(- 8) */
    'A', /* rec_tecladoRaton    (rindx_t)(- 9) */
    'a', /* rec_tecladoRawRaton (rindx_t)(-10) */
} ;

#define tamCarRec (sizeof(carRecurso)/sizeof(char))

descProceso_t descProceso [ maxProcesos ] ;

e2PFR_t e2PFR ;

c2c_t c2cPFR [ numColasPFR ] ;

ptrBloque_t listaLibres ;

word_t tamBlqMax ;

word_t miIP ( void )
{
    asm push bp
    asm mov bp,sp
    asm mov bx,ss:[bp+2]
    asm pop bp
    return(_BX) ;
}

void ps ( void )
{

    int i, j, numElem, ind ;
    word_t flags ;
    rindx_t esperandoPor ;

    escribirStr(" cola de preparados: [ ") ;
    numElem = c2cPFR[PPreparados].numElem ;
    i = c2cPFR[PPreparados].primero ;
    for ( j = 0 ; j < numElem ; j++ )
    {
        escribirDec(i, 1) ;
//      escribirDec(descProceso[i].pid, 1) ;
        escribirCar(' ') ;
        i = c2cPFR[PPreparados].e[i].sig ;
    }
    escribirStr("] ") ;

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
				/* no funciona en Fake86 porque Fake86 tiene un error */	
#if (TRUE)						
                escribirStr("bloqueado") ;
                escribirCar('(') ;
    			if (descProceso[i].esperandoPor >= 0)
                    escribirDec(descProceso[i].esperandoPor, 1) ;
				else {
					ind = -descProceso[i].esperandoPor ; 
                    if (ind < tamCarRec) 
						escribirCar(carRecurso[ind]) ;
                    else escribirCar('#') ;
				}	
                escribirCar(')') ;
#else			     
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
#endif				
            }
            else
                escribirStr(strEstado[descProceso[i].estado]) ;
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
        }
    }

    escribirLn() ;

}

void main ( void )
{
    obtenInfoPS((descProceso_t far *)&descProceso, (e2PFR_t far *)&e2PFR, (c2c_t far *)&c2cPFR) ;
    ps() ;
}

