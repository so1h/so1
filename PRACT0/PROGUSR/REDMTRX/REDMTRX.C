/* ----------------------------------------------------------------------- */
/*                                  redmtrx.c                              */
/* ----------------------------------------------------------------------- */
/*                        programa para el TrabajoSO_3                     */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\ll_s_so1.h>    /* biblioteca de llamadas al sistema SO1 */
#include <so1pub.h\escribir.h>
#include <so1pub.h\carsctrl.h>                                       /* FF */
#include <so1pub.h\caracter.h>                           /* dig, mayuscula */
#include <so1pub.h\strings.h>                                   /* iguales */

#include <so1pub.h\stdio.h>                               /* printf, getch */

int dfReloj ;

void formato ( void )
{
    printf(
        " formato : \n"
		"\n"
        "    REDMTRX [ cmd [ 1 | 2 | 3 ] | -m nmatr | -h ] \n"
    ) ;
}

void help ( void )
{
    printf(
        "\n"
        "\n"
        " formato : REDMTRX [ cmd [ 1 | 2 | 3 ] | -m nmatr | -h ] \n"
        "\n"
        "   crea jerarquicamente 1, 2 o 3 procesos ejecutando \n"
		"   el comando cmd y redirigidos de acuerdo con la    \n"
		"   matriz de redirecciones correspondiente al alumno \n"
		"   autor del programa segun su numero de matricula   \n"
        "\n"
        " opciones:                                           \n"
        "\n"
        "      cmd      : lo mismo que cmd 1                  \n"
        "      cmd n    : crea n procesos que ejecutan el     \n"
		"                 comando cmd con las redirecciones   \n" 
		"                 encomendadas al alumno              \n"
        "      -m nmatr : indica la matriz de redirecciones   \n"
		"                 encomendadas al alumno con nmatr    \n" 
		"                 (con formato XX9999)                \n"
        "      -h       : muestra este help                   \n"
        "\n"
        " autor: Jaime Martin Bradshaw (C) 2016         \n"
	) ;
}

#pragma option -w-par            /* omitir warning parameter is never used */

char * strREDIN [ 3 ] = { "CON4", "CON5", "TIMER" } ;

char * strREDOUT [ 3 ] = { "CON4", "CON5", "CON6" } ;

int calculaMatriz ( char * strMatr ) {
   
	int i, j, k ;  
	
	int numCifras ;
	
	int valorNumMatr ;
	
	int acum ;
	
	i = 0 ;
	while ((strMatr[i] != (char)0) && ((strMatr[i] < '0') || (strMatr[i] > '9')))
	{
		i++ ;
	}	

	if (strMatr[i] == (char) 0) 
	{
        return(-3) ;
	}	
	else 
    {
		j = 0 ;
		while (strMatr[j] != (char)0) 
		{
     	    j++ ;
		}	

		for ( k = i ; k < j ; k++ ) {
		    if (strMatr[k] != '0') break ;
		}  
		if ( k == j ) valorNumMatr = 0 ;
		else 
		{
		    numCifras = j - k ;
            if (numCifras > 3)
                return(-4) ;
			else {
			    acum = 0 ;
			    for ( i = k ; i < j ; i++ ) 
					acum = 10*acum + strMatr[i] ;
				valorNumMatr = acum ;
			}	
        }    			
		
		printf("\n") ;
		
		printf("\n"
               " El proceso hijo debe tener  \n"
		       "    como entrada estandar (df 0) \"") ;
          printf(strREDIN[valorNumMatr % 3]) ;	
	  	printf("\"\n"
		       "  y como salida  estandar (df 1) \"") ;
          printf(strREDOUT[(valorNumMatr / 3) % 3]) ;	
        printf("\"\n") ;
				       		
		printf("\n"
               " El proceso nieto debe tener  \n"
		       "    como entrada estandar (df 0) \"") ;
          printf(strREDIN[(valorNumMatr / 9) % 3]) ;	
	  	printf("\"\n"
		       "  y como salida  estandar (df 1) \"") ;
          printf(strREDOUT[(valorNumMatr / 27) % 3]) ;	
        printf("\"\n") ;
				       		
		printf("\n"
               " El proceso bisnieto debe tener  \n"
		       "    como entrada estandar (df 0) \"") ;
          printf(strREDIN[(valorNumMatr / 81) % 3]) ;	
	  	printf("\"\n"
		       "  y como salida  estandar (df 1) \"") ;
          printf(strREDOUT[(valorNumMatr / 243) % 3]) ;	
        printf("\"\n") ;
				       		
	}	

	return(0) ;
}

int extractFileName ( char * cmd, char * fileName ) {

    int i, i0, i1, j ;

	if (cmd[0] == (char)0) 
	{
		return(-1) ;
	}
	else 
	{
		i0 = 0 ;
		while ((cmd[i0] != (char)0) && (cmd[i0] == ' ')) i0++ ;
		i1 = i0 ;
		while ((cmd[i1] != (char)0) && (cmd[i1] != '.') && (cmd[i1] != ' ')) i1++ ;
		if (i0 == i1) 
			return(-2) ;
		else if ((i1 - i0) > 8)
            return(-3) ;
        else 
        {
			if (cmd[i1] == '.') 
			{
				if ((mayuscula(cmd[i1+1]) != 'B') ||
				    (mayuscula(cmd[i1+2]) != 'I') ||
				    (mayuscula(cmd[i1+3]) != 'N') ||
				    ((cmd[i1+4] != (char)0) && (cmd[i1+4] != ' '))) 
                    return(-4) ;										
			}	

//          printf("\n\n i0 = ") ; escribirDec(i0, 1) ;			
//          printf("\n\n i1 = ") ;escribirDec(i1, 1) ;			

			j = 0 ;
			for ( i = i0 ; i < i1 ; i++ )
				fileName[j++] = mayuscula(cmd[i]) ;
			fileName[j] = (char)0 ;
/*		
	        printf(
	          "\n"
	          "\n cmd = \"") ; 
	          printf(cmd) ;
	          printf("\"") ;
	
	        printf(
              "\n"
	          "\n fileName = \"") ; 
	          printf(fileName) ;
	          printf("\"") ;
	
	        printf(
              "\n"
			  "\n") ;
*/			  
			return(0) ;
		}			
			
	}
}

int redmtrx ( char * cmd, char * str ) {
	
    int numProc ; 
	
	pid_t pid, pidHijo ;
	int status ;
	
	char fileName [ 12 ] ;
	
	if (str[0] == (char)0)
    {		
		return(-1) ;
	}
	else if (((int)(str[0]) < '0') || ((int)(str[0]) > '3'))
	{		
		return(-2) ;
	}
	else 
	{
        numProc = ((int)str[0] - (int)'0') ;        		
    }	
/*
	printf("\n\n numProc = ") ;
	escribirDec(numProc, 1) ;
	printf("\n") ;
*/	
    extractFileName(cmd, fileName) ;
	
	/* ------------------------------------------------------------------- */
	/*                                                                     */
    /*    Aquí es donde en principio debe ir el codigo del alumno          */
    /*    para conseguir que el comando REDMTRX se comporte como se	       */
    /*    solicita para la entrega del Trabajo SO 3 (REDMTRX) creando      */
	/*    los procesos hijo, nieto y bisnieto que deben ejecutar el        */
	/*    comando cmd. Por supuesto pueden declararse las variables        */
	/*    locales o globales, así como las funciones auxiliares que        */
	/*    el alumno considere convenientes.                                */
	/*                                                                     */
	/*    No olvideis poner comentarios, ya que os serviran de ayuda       */ 
	/*    en caso de que tras la entrega tenga que consultaros por         */
	/*    aspectos dudosos de la implementacion que hagais.                */
	/*                                                                     */
	/* ------------------------------------------------------------------- */

	
	if((pid=fork())==0){							/*Proceso hijo*/
		close(STDIN);
		close(STDOUT);
		close(STDERR);
		open("CON4",O_RDONLY);
		open("CON5",O_WRONLY);
		exec( fileName,cmd );
	}else{											/*Si pid != 0 es el proceso padre y comprobamos numProc*/
		if (numProc>1){
			if((pid=fork())==0){					/*Proceso hijo del padre => Hijo*/
				if((pidHijo=fork())==0){ 			/*Proceso hijo del hijo => Nieto*/
					close(STDIN);
					close(STDOUT);
					close(STDERR);
					open("CON5",O_RDONLY);
					open("CON4",O_WRONLY);
					exec( fileName,cmd );
				}else{ 								/*Si pidHijo != 0 es el proceso hijo y comprobamos numProc*/
					if(numProc>2){
						if((pid=fork())==0){		/*Proceso hijo del hijo => Nieto*/
							if((pidHijo=fork())==0){/*Proceso hijo del nieto => Bisnieto*/
								close(STDIN);
								close(STDOUT);
								close(STDERR);
								open("TIMER",O_RDONLY);
								open("CON6",O_WRONLY);
								exec( fileName,cmd );
							}else{
								waitpid(-1,(int far *)&status); /*-1 Espera por cualquier proceso hijo*/
								exit(0);
							}
						}
					}
					waitpid(-1,(int far *)&status);
					if(numProc>2) waitpid(-1,(int far *)&status) ; /*Evita que un proceso se quede zombie (Bisnieto)*/
					exit(0);
				}
			}
		}
		waitpid(-1,(int far *)&status);
		if(numProc>1) waitpid(-1,(int far *)&status) ; /*Evita que un proceso se quede zombie (Nieto)*/
		exit(0);
	}
	
	return(numProc) ;

}

void main ( int argc, char * argv [ ] )
{
    
	if (argc == 1) 
	{
		formato() ;
		exit(0) ;
	}
    else if (argc == 2) 
	{ 
		if (iguales(argv[1], "-h") || iguales(argv[1], "-H")) 
		{
            help() ;
            exit(0) ;
		}
        else 
			exit(redmtrx(argv[1], (char *)&"1")) ;	
    }
	else if (argc == 3) 
	{
		if (iguales(argv[1], "-m") || iguales(argv[1], "-M"))
            exit(calculaMatriz(argv[2])) ;
        else 
			exit(redmtrx(argv[1], argv[2])) ;
	}
    else 
	{		
        formato() ;
        exit(-1) ;
    }
}


