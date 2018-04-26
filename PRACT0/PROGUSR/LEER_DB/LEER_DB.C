/* ------------------------------------------------------------------------*/
/*                                 leer_db.c                               */
/* ----------------------------------------------------------------------- */
/*  comando para leer de un dispositivo de bloques reconocido por el BIOS  */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\ll_s_so1.h>    /* biblioteca de llamadas al sistema SO1 */
#include <so1pub.h\stdio.h>                    /* printf, getchar, putchar */

#include <so1pub.h\ctype.h>                                     /* tolower */
#include <so1pub.h\strings.h>                           /* strcpy, strcmpu */
#include <so1pub.h\scanner.h>                    /* inicScanner, obtenSimb */

#include <so1.h\db.h>                                        /* d_bloque_t */

#define min( x, y ) ((x <= y) ? x : y )
#define max( x, y ) ((x >= y) ? x : y )

int formato ( void ) {
    printf(
        ""                                                               "\n"
        ""                                                               "\n"
        " formato: leer_db [ fichero pos n | -l | -h ] "                 "\n"
    ) ;
    return(0) ;
}

int help ( void ) {
    formato() ;
    printf(
        ""                                                               "\n"
        " leer_db abre para lectura el fichero indicado mostrando  "     "\n"
        " por pantalla en hezadecimal los n bytes contenidos en el "     "\n"
        " fichero, a partir de la posicion pos. La posicion puede  "     "\n"
        " especificarse con un factor con * y un desplazamiento    "     "\n"
        " con + (todo sin espacios) "                                    "\n"
        ""                                                               "\n"
        " ejemplos: leer_db FDA 143 200                            "     "\n"
        "           leer_db FDA 33*512 200                         "     "\n"
        "           leer_db FDA 19*512+52 200                      "     "\n"
        ""                                                               "\n"
        " leer_db esta pensando para probar el funcionamiento del  "     "\n"
		" driver de dispositivos de bloques (DB.C). Los nombres de "     "\n"
        " estos dispositivos: FDA, FDB, HDA, HDA1, HDA2, HDB1, ... "     "\n"
        ""                                                               "\n"
        " opciones: "                                                    "\n"
		"   -l : lista los dispositivos de bloques                 "     "\n"
		"        reconocidos por el BIOS                           "     "\n"
		"   -h : muestra este help "               
    ) ;
	getchar() ;
    printf(
        ""                                                               "\n"
        ""                                                               "\n"
        " autor: Pedro Pablo Lopez Rodriguez (C) 2018              "     "\n"
    ) ;
    return(0) ;
}

#define tamBufer 512L

byte_t bufer [ tamBufer ] ;

char listaDB [ ] [5] = { 
    "FDA", "FDB", 
	"HDA", "HDA1", "HDA2", "HDA3", "HDA4",
    "HDB", "HDB1", "HDB2", "HDB3", "HDB4"
} ;

#define numMaxDB sizeof(listaDB)/sizeof(listaDB[0]) 

void escribirDB ( d_bloque_t * d_bloque ) 
{
	dword_t daux = d_bloque->numSectores ;
    printf(
        "\n     %-6s %02X %02X %3d %3d %2d %3d %6ld %8ld %3ld,%02ld MB ", 
	    d_bloque->nombre, 
	    d_bloque->unidadBIOS, 
		d_bloque->tipoUnidad, 
		d_bloque->bytesPorSector, 
		d_bloque->sectoresPorPista, 
		d_bloque->cabezas, 
		d_bloque->cilindros, 
		d_bloque->primerSector, 
		d_bloque->numSectores,                                 /* sectores */
		daux/(2*1024),                                            /* bytes */
		((100*daux)/(2*1024))%100
    ) ;				
}

int listarDispositivosDB ( void ) 
{
	int df ; 
	int res ;
    int i ;	
	int cont = 0 ;
	d_bloque_t d_bloque ;
	for ( i = 0 ; i < numMaxDB ; i++ ) 
	{
        df = open(listaDB[i], O_RDONLY) ;		
		if (df < 0) continue ;
		d_bloque.nombre[0] = '\0' ;
    	res = ioctl(df, 1, (word_t)&d_bloque) ;
		close(df) ;
        if (res != 0) continue ;
		if (((d_bloque.nombre[0] != 'F') && (d_bloque.nombre[0] != 'H')) ||
		    (d_bloque.nombre[1] != 'D')) 
            continue ;			
		    
		if (cont == 0) 
		{
			printf(			
			    ""                                                          "\n"
			    ""                                                          "\n"
			    " dispositivos de bloques detectados por el BIOS: "         "\n"
			    ""                                                          "\n"
			    "     nombre id ti b/s s/p hd cls primer numSects    MB    ""\n"
                "     ------ -- -- --- --- -- --- ------ -------- ---------"        
			) ;
		}
		escribirDB(&d_bloque) ;
		cont++ ;
	}		
	if (cont == 0) 
	    printf("\n\n no hay dispositivos de bloques reconocidos por el BIOS") ;
	putchar('\n') ;
	return(0) ;
}

int main ( int argc, char * argv [ ] )
{
    int df ;
    char carAux ;
    dword_t n ;
    dword_t resto ;
    int nbytes ;
    int i, j, k ;
    dword_t tam ;
	dword_t coef ;                                          /* coeficiente */
    dword_t factor ; 
    dword_t despl ;
    dword_t pos ;
    dword_t pos_1 ;
    dword_t pos_2 ;
    dword_t posAux ;
	d_bloque_t d_bloque ;
	int res ;

    if (argc < 2) return(formato()) ;

    if (argc == 2) 
	{
		if      (!strcmpu(argv[1], "-h")) return(help()) ;
		else if (!strcmpu(argv[1], "-l")) return(listarDispositivosDB()) ;
    }
	
    if (argc != 4) return(formato()) ;

    df = open(argv[1], O_RDONLY) ;

    printf("\n\n open(\"%s\", O_RDONLY) = %i\n", argv[1], df) ;

    if (df < 0)
    {
        printf("\n no existe el fichero \"%s\"\n", argv[1]) ;
        return(-1) ;
    }

	i = 0 ;
	while ((i < numMaxDB) && (strcmpu(argv[1], listaDB[i]))) i++ ;
    if (i >= numMaxDB) 
    {
        printf("\n el fichero \"%s\" no es un dispositivo de bloques BIOS \n", argv[1]) ;
//      return(-1) ;
    }		
	else 
	{
	    res = ioctl(df, 1, (word_t)&d_bloque) ; 
	
        printf("\n ioctl(%i, 1, 0x%04X) = %i\n", df, (word_t)&d_bloque, res) ;
	
	    if (res >= 0) 
    	{
	    	escribirDB(&d_bloque) ;
	        putchar('\n') ;
	    }
	}
	
//	getchar() ;
	
    k = 0 ;                          /* comando es una variable global */
    for ( i = 2 ; i < argc ; i ++ )
    {
        for ( j = 0 ; (carAux = argv[i][j]) != '\0' ; j++ )
            comando[0][k++] = toupper(carAux) ;
        comando[0][k++] = ' ' ;
    }
    comando[0][k--] = '\0' ;

    inicScanner() ;
    saltarBlancos() ;
    numDec() ;
    if (simb == s_numeroLargo) coef = numLargo ;
    else if (simb == s_numero) coef = num ;
    else
    {
        printf(
            "\n el argumento pos tiene que ser un numero y no \"%s\"\n",
            argv[2]
        ) ;
        return(1) ;
    }
	pos = coef ;
	factor = 0L ;
    despl = 0L ;	
    if (car == '*')
    {
        obtenCar() ;
        numDec() ;
        if (simb == s_numeroLargo) factor = numLargo ;
        else if (simb == s_numero) factor = num ;
        else
        {
            printf(
                "\n factor debe ser un numero \n"
            ) ;
            return(1) ;
        }
        pos = pos*factor ;
    }
    if (car == '+')
    {
        obtenCar() ;
        numDec() ;
        if (simb == s_numeroLargo) despl = numLargo ;
        else if (simb == s_numero) despl = num ;
        else
        {
            printf(
                "\n despl debe ser un numero \n"
            ) ;
            return(1) ;
        }
        pos = pos + despl ;
    }

    printf(
        "\n pos = (coef * factor) + despl = (%ld * %ld) + %ld = %ld = 0x%06lX\n",
        coef, factor, despl, pos, pos
    ) ;
				
//  getchar() ;
		
    saltarBlancos() ;
    numDec() ;
    if (simb == s_numeroLargo) n = numLargo ;
    else if (simb == s_numero) n = num ;
    else
    {
        printf(
            "\n el argumento n tiene que ser un numero y no \"%s\"\n",
            argv[3]
        ) ;
        return(2) ;
    }

    tam = lseek(df, 0, SEEK_END) ;

    printf("\n lseek(%i, 0, SEEK_END) = %li\n", df, tam) ;

    pos_1 = lseek(df, pos, SEEK_SET) ;

    printf(
	    ""                                "\n"
		" lseek(%i, %li, SEEK_SET) = %li ""\n"
	    ""                                "\n"
		" pulse una tecla para empezar "
		, 
	    df, pos, pos_1
	) ;

    if (pos != pos_1)
    {
        printf(
            "\n el argumento pos no es una posicion valida del fichero \n"
        ) ;
        return(3) ;
    }
		
    getchar() ;
	putchar('\n') ;
		
    resto = n ;
    posAux = pos ;
    do
    {
        nbytes = read(df, (pointer_t)&bufer, min(resto, tamBufer)) ;

        if (nbytes < 0) break ;

        for ( i = 0 ; i < nbytes ; i++ )
        {
            if (i % 16 == 0) printf("\n %06lX: ", posAux + i ) ;
            printf("%02X ", bufer[i]) ;
            if ((i % 16 == 15) || (i == (nbytes-1)))
            {
     			for ( j = (i & 0x000F) + 1 ; j < 16 ; j++ ) 
                    printf("   ") ;						
                printf("  |") ;
                for ( j = 0 ; j <= (i & 0x000F) ; j++ )
                {
                    carAux = bufer[(i & 0xFFF0) + j] ;
                    putchar((' ' <= carAux) && (carAux <= '~') ? carAux : '.') ;
                }
                putchar('|') ;
            }
        }
        posAux = lseek(df, 0, SEEK_CUR) ;
        resto = resto - nbytes ;
    }
    while (resto > 0) ;

    printf("\n") ;

    pos_2 = lseek(df, 0, SEEK_CUR) ;

    printf("\n lseek(%i, 0, SEEK_CUR) = %li\n", df, pos_2) ;

	close(df) ;
	
    if (pos_2 != pos_1 + n)
    {
        printf(
            "\n solo han podido leerse %li de los %li bytes solicitados \n",
            pos_2-pos_1, n
        ) ;
        return(4) ;
    }
    return(0) ;
}