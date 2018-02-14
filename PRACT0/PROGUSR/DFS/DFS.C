/* ----------------------------------------------------------------------- */
/*                                  dfs.c                                  */
/* ----------------------------------------------------------------------- */
/*             muestra los descriptores de fichero del sistema             */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\ll_s_so1.h>    /* biblioteca de llamadas al sistema SO1 */
#include <so1pub.h\stdio.h>                    /* printf, getchar, putchar */ 
#include <so1pub.h\strings.h>                                   /* iguales */
#include <so1pub.h\scanner.h>                    /* inicScanner, obtenSimb */

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

	printf(
        ""                                                               "\n"
		" descFichero[%d].nombre    = \"%s\""                            "\n"
		" descFichero[%d].tipo      = %*s (%i)"                          "\n"
		" descFichero[%d].rindx     = %i"                                "\n"
		" descFichero[%d].menor     = %i"                                "\n"
		" descFichero[%d].shareMode = %04X"                              "\n"
		" descFichero[%d].contAp_L  = %i"                                "\n"
		" descFichero[%d].contAp_E  = %i"                                "\n",
	    dfs, descFichero[dfs].nombre, 
		dfs, lMaxStrTipo-1, strTipo[tipo], tipo,
		dfs, descFichero[dfs].rindx, 
		dfs, descFichero[dfs].menor,
		dfs, descFichero[dfs].shareMode,
		dfs, descFichero[dfs].contAp_L,
		dfs, descFichero[dfs].contAp_E
	) ;
}

void mostrarFicheros ( void )
{
    int dfs ;
    tipoFichero_t tipo ;
	
	printf(
        ""                                                               "\n "
        ""                                                               "\n"
		" dfs nombre       tipo          rindx menor shareM cAp_L cAp_E ""\n"
        " --- ------------ ------------- ----- ----- ------ ----- ----- "
	) ;
    
	for ( dfs = 0 ; dfs < dfsMax ; dfs++ )
    {
        tipo = descFichero[dfs].tipo ;
        if (tipo != flibre)
        {
            printf(
			    "\n"
				" %3d %-12s %*s %5i %5i %4X %5i %5i ",
			    dfs, 
				descFichero[dfs].nombre, 
//				13, strTipo[tipo],  
				lMaxStrTipo-1, strTipo[tipo],  
			    descFichero[dfs].rindx, 
				descFichero[dfs].menor, 
				descFichero[dfs].shareMode, 
				descFichero[dfs].contAp_L, 
				descFichero[dfs].contAp_E
			) ;			
        }
    }
    printf("\n") ;
}

void formato ( void )
{
//  escribirStrIntenso(" formato: DFS [ dfs | -a | -h ] ") ;
    printf(" formato: DFS [ dfs | -a | -h ] ") ;
}

void help ( void )
{
    printf(
	    ""                                                               "\n"
		""                                                               "\n"
        " formato : DFS [ dfs | -a | -h ]"                               "\n"
		""                                                               "\n"
        " muestra los campos del descriptor de fichero dfs"              "\n"
		""                                                               "\n"
        " opciones: (por defecto -a)"                                    "\n"
		""                                                               "\n"
        "     dfs : muestra solo ese fichero en detalle"                 "\n"
        "      -a : muestra todos los ficheros"                          "\n"
        "      -h : muestra este help"                                   "\n"
	) ;
}

int main ( int argc, char * argv [ ] )
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
                putchar('\a') ;
				printf(
				    "\n"
				    "\n"
				) ;
                formato() ;
				printf(
				    "\n"
				    "\n"
                    " error: dfs debe ser un numero y no \"%s\"\n", argv[1] 
				) ;
            }
            else
            {
                dfs = (dfs_t)num ;
                if ((0 <= dfs) && (dfs < dfsMax))
                    if (descFichero[dfs].tipo != flibre)
                    {
                        printf("\n") ;
                        mostrarDescFichero(dfs) ;
                    }
                    else
                    {
//                      escribirStrIntenso(" descriptor de fichero no utilizado ") ;
                        printf(" descriptor de fichero no utilizado ") ;
                    }
                else
                {
//                  escribirStrIntenso(" dfs erroneo ") ;
                    printf(" dfs erroneo ") ;
                }
            }
        }
    else if (argc == 1)
        mostrarFicheros() ;
    else
        formato() ;
//	getchar() ;
	return(0) ;
}

