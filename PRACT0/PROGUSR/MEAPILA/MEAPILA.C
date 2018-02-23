/* ----------------------------------------------------------------------- */
/*                                meapila.c                                */
/* ----------------------------------------------------------------------- */
/*   programa para rellenar el area no utilizada de la pila de un proceso  */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\comundrv.h>                               /* dirDescSO1 */
#include <so1pub.h\ll_s_so1.h>    /* biblioteca de llamadas al sistema SO1 */
#include <so1pub.h\stdio.h>                    /* printf, getchar, putchar */

#include <so1pub.h\strings.h>                                   /* iguales */
#include <so1pub.h\scanner.h>
#include <so1pub.h\caracter.h>                                /* minuscula */

#pragma option -w-use /* (comundrv.h) warning: 'x' declared but never used */

void formato ( void )
{
    printf(
        ""                                                               "\n"
        ""                                                               "\n"
        " formato: MEAPILA ( pid ( ( [ valor ] [ -c ] ) | -i ) | -h )"   "\n"
    ) ;
}

void help ( void )
{
    formato() ;
    printf(
        ""                                                               "\n"
        "   MEAPILA pid [valor]"                                         "\n"
        ""                                                               "\n"
        "     rellena el area no utilizada de la pila del proceso"       "\n"
        "     pid con el valor indicado (en hexadecimal). Si no se"      "\n"
        "     indica ningun valor, se utiliza el valor 20 (' ')."        "\n"
        ""                                                               "\n"
        "   MEAPILA pid [valor] -c"                                      "\n"
        ""                                                               "\n"
        "     calcula el mayor numero de bytes consecutivos con el"      "\n"
        "     valor indicado que hay en el espacio de pila que no"       "\n"
        "     esta siendo utilizado actualmente por el proceso con"      "\n"
        "     el pid indicado. Por defecto valor = 20 (' ')."            "\n"
        ""                                                               "\n"
        "   MEAPILA pid -i"                                              "\n"
        ""                                                               "\n"
        "     indica el numero de bytes que hay en el espacio de"        "\n"
        "     la pila que no esta siendo utilizado actualmente por"      "\n"
        "     el proceso con el pid indicado."                           "\n"
        ""                                                               "\n"
        "   MEAPILA -h"                                                  "\n"
        ""                                                               "\n"
        "     muestra esta informacion por la salida estandar."
    ) ;
	
    getchar() ;
    printf(
        ""                                                               "\n"
        ""                                                               "\n"
        "   autor: Pedro Pablo Lopez Rodriguez. (C) 2016-2018."          "\n"
    ) ;
}

void mostrarError ( char * mensaje, int error )
{
    putchar('\a') ;
    formato() ;
    printf("\n error: %s\n", mensaje) ;
    exit(error) ;
}

int meapila ( pid_t pid, byte_t valor )
{
    pointer_t ptr ;
    int numBytesLibres ;
    int i, j ;
/*
    printf("\n\n pid = %i", pid) ;
    printf("\n valor = %i\n", valor) ;
*/
    asm cli
    obtenInfoSO1(dirDescSO1) ;               /* obtenemos los datos de SO1 */
    i = 0 ;
    for ( i = 0 ; i < maxProcesos ; i++ )
    {
/*
        printf("\n i = %i", i) ;
        printf(" ptrDescProceso[i].pid = %i", ptrDescProceso[i].pid) ;
*/
        if (ptrDescProceso[i].pid == pid)
        {
            if (ptrDescProceso[i].estado == bloqueado)
            {
                ptr =

                    MK_FP(ptrDescProceso[i].trama->DS, ptrDescProceso[i].desplPila) ;

                numBytesLibres =

                    off((pointer_t)ptrDescProceso[i].trama) - ptrDescProceso[i].desplPila ;

                for ( j = 0 ; j < numBytesLibres ; j++ )
                {
                    *ptr++ = valor ;
                }

                asm sti

                printf(" Ok ") ;
                return(0) ;
            }
            else
            {
                asm sti
                printf(
				    "\n\n error: el proceso con pid = %i no esta bloqueado \n", 
					pid
				) ;
                return(-1) ;
            }
        }
    }
    asm sti
    printf("\n\n error: no existe ningun proceso con pid = %i\n", pid) ;
    return(-2) ;
}

int espacioLibre ( pid_t pid )
{
    int numBytesLibres ;
    int i ;

    asm cli
    obtenInfoSO1(dirDescSO1) ;                 /* obtenemos los datos de SO1 */
    i = 0 ;
    for ( i = 0 ; i < maxProcesos ; i++ )
    {
        if (ptrDescProceso[i].pid == pid)
        {
            if (ptrDescProceso[i].estado == bloqueado)
            {
                numBytesLibres =

                    off((pointer_t)ptrDescProceso[i].trama) - ptrDescProceso[i].desplPila ;

                asm sti

                printf(
				    ""                                                                   "\n"
					""                                                                   "\n"
					" informacion sobre la pila del proceso con pid = %i (SS = DS = %04X) \n"
					""                                                                   "\n"
					"   desplData SS:0000"                                               "\n"
                    ""                                                                   "\n"
					"   desplBSS  SS:%04X"                                               "\n"
                    ""                                                                   "\n"
					"   desplPila SS:%04X <----   | espacio libre"                       "\n"
					"                             |"                                     "\n"
					"   desplCima SS:%04X <---- ^"                                       "\n"
                    "                           |"                                       "\n"
					"   desplBase SS:%04X <---- |"                                       "\n"
                    ""                                                                   "\n"
					" espacio libre en la pila:"                                         "\n"
                    ""                                                                   "\n"
					"   desde SS:%04X hasta SS:%04X inclusive (%04X = %i bytes libres)"  "\n",
                    pid, 
					ptrDescProceso[i].trama->DS, 
					ptrDescProceso[i].desplBSS, 
					ptrDescProceso[i].desplPila, 
					off((pointer_t)ptrDescProceso[i].trama), 
					(ptrDescProceso[i].tam - (ptrDescProceso[i].tamCodigo >> 4)) << 4,
                    ptrDescProceso[i].desplPila, 
                    off((pointer_t)ptrDescProceso[i].trama) - 1, 
					numBytesLibres, 
                    numBytesLibres
				) ;
                return(0) ;
            }
            else
            {
                asm sti
                printf(
				    "\n\n error: el proceso con pid = %i no esta bloqueado \n", 
					pid
				) ;
                return(-1) ;
            }
        }
    }
    asm sti
    printf("\n\n error: no existe ningun proceso con pid = %i\n") ;
    return(-2) ;
}

int espacioNoUtilizado ( pid_t pid, byte_t valor )
{
    int numBytesLibres ;
    word_t contador = 0 ;
    pointer_t ptr ;
    int i, j ;

    asm cli
    obtenInfoSO1(dirDescSO1) ;               /* obtenemos los datos de SO1 */
    i = 0 ;
    for ( i = 0 ; i < maxProcesos ; i++ )
    {
        if (ptrDescProceso[i].pid == pid)
        {
            if (ptrDescProceso[i].estado == bloqueado)
            {
                ptr =

                    MK_FP(ptrDescProceso[i].trama->DS, ptrDescProceso[i].desplPila) ;

                numBytesLibres =

                    off((pointer_t)ptrDescProceso[i].trama) - ptrDescProceso[i].desplPila ;

                for ( j = 0 ; j < numBytesLibres ; j++ )
                {
                    if (*ptr++ == valor)
                    {
                        contador++ ;
                    }
                    else
                    {
                        break ;
                    }
                }

                asm sti

                printf(
				    ""                                                                     "\n"
				    ""                                                                     "\n"
					" pila del proceso con pid = %i (SS = DS = %04X)"                      "\n"
                    ""                                                                     "\n"
					""                                                                     "\n"
					"   numero de bytes libres        = %04X = %i bytes (SS:%04X .. SS:%04X)\n"
                    ""                                                                     "\n"
					"   numero de bytes no utilizados = %04X = %i bytes (SS:%04X .. SS:%04X)\n",
                    pid, 
                    ptrDescProceso[i].trama->DS, 
                    numBytesLibres, 
                    numBytesLibres, 
                    ptrDescProceso[i].desplPila, 
                    ptrDescProceso[i].desplPila + numBytesLibres - 1, 
                    contador, 
                    contador, 
                    ptrDescProceso[i].desplPila, 
                    ptrDescProceso[i].desplPila + contador - 1
				) ;

                return(0) ;
            }
            else
            {
                asm sti
                printf("\n\n error: el proceso con pid = %i no esta bloqueado \n", pid) ;
                return(-1) ;
            }
        }
    }
    asm sti
    printf("\n\n error: no existe ningun proceso con pid = %i\n", pid) ;
    return(-1) ;
}

void main ( int argc, char * argv [ ] )
{

    pid_t pid ;
    char opcion ;

    if (argc == 1)                                              /* MEAPILA */
    {
        formato() ;
        return ;
    }

    if (argc > 4)                                                 /* error */
    {
        mostrarError("demasiados argumentos para el comando", 1) ;
    }

    if ((argc == 2) &&
        (argv[1][0] == '-') &&
        (argv[1][2] == (char)0))
    {
        opcion = minuscula(argv[1][1]) ;
        if (opcion == 'h')                                   /* MEAPILA -h */
        {
            help() ;
            return ;
        }
        else                                                      /* error */
        {
            mostrarError("opcion != -h o faltan argumentos", 2) ;
        }
    }

    copiarStr(argv[1], comando[0]) ;
    inicScanner() ;
    obtenSimb() ;
    if (simb != s_numero)                                         /* error */
    {
        mostrarError("pid debe ser un numero", 3) ;
    }

    pid = (pid_t)num ;

    if (argc == 2)                                          /* MEAPILA pid */
    {
        exit(meapila(pid, 0x20)) ;                          /* 0x20 == ' ' */
    }

    copiarStr(argv[2], comando[0]) ;
    inicScanner() ;
    saltarBlancos() ;
    if ((('0' <= car) && (car <= '9')) || (('A' <= car) && (car <= 'F')))
    {
        numHex() ;
        if ((simb == s_numero) && ((car == ' ') || (car == (char)0)))
        {
            if (num < 256)
                if (argc == 3)                          /* MEAPILA pid num */
                    exit(meapila(pid, num)) ;
                else
                    if ((argv[3][0] == '-') && (argv[3][2] == (char)0))
                    {
                        opcion = minuscula(argv[3][1]) ;
                        if (opcion == 'c')           /* MEAPILA pid num -c */
                            exit(espacioNoUtilizado(pid, num)) ;
                        else                                      /* error */
                        {
                            mostrarError("se esperaba la opcion -c", 4) ;
                        }
                    }
            else                                                  /* error */
            {
                mostrarError("valor debe ser <= FF", 5) ;
            }
        }
        else
        {
            mostrarError("se esperaba un numero hexadecimal", 6) ;
        }
    }
    else if (car == '-')
    {
        if (argc == 3)
        {
            opcion = minuscula(argv[2][1]) ;
            if ((opcion != (char)0) && (argv[2][2] == (char)0))
            {
                if (opcion == 'c')                       /* MEAPILA pid -c */
                {
                    exit(espacioNoUtilizado(pid, 0x20)) ;
                }
                else if (opcion == 'i')                  /* MEAPILA pid -i */
                {
                    exit(espacioLibre(pid)) ;
                }
                else
                {
                    mostrarError("opcion distinta de -c y -i", 7) ;
                }
            }
            else
            {
                mostrarError("opcion incorrecta", 8) ;
            }
        }
        else
        {
            mostrarError("numero de argumentos excesivo", 9) ;
        }
    }
}

