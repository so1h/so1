/* ----------------------------------------------------------------------- */
/*                                 test_sf.c                               */
/* ----------------------------------------------------------------------- */
/*          programa para probar el funcionamiento del recurso SF          */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\stdio.h>                             /* printf, getchar */
#include <so1pub.h\ll_s_so1.h>                       /* O_WRONLY, O_RDONLY */

int main ( int argc, char * argv [ ] )
{
	char bufer [512] ;
	int i ;
	int df ;
	int err ;
	int nbytes ;
    if (argc == 1) return(1) ;
	df = open(argv[1], O_WRONLY | O_CREAT) ;
    printf("\n\n open(\"%s\", O_WRONLY | O_CREAT) = %i", argv[1], df) ;
	getchar() ;
	nbytes = write(df, (pointer_t)&"un gato\ndos gatos\ntres gatos\n", 29) ;
    printf("\n\n write(%i, &\"un gato\\ndos gatos\\ntres gatos\\n\", 29) = %i", 
	       df, nbytes) ;
	getchar() ;
	err = close(df) ;
    printf("\n\n close(%i) = %i", df, err) ;
	getchar() ;
	df = open(argv[1], O_RDONLY) ;
    printf("\n\n open(\"%s\", O_RDONLY) = %i", argv[1], df) ;
	getchar() ;
	nbytes = read(df, bufer, 29) ;
    printf("\n\n read(%i, &bufer, 29) = %i", df, nbytes) ;
	printf("\n\n bufer = \"") ; 
	for ( i = 0 ; i < nbytes ; i++ )
		printf("\%c", bufer[i]) ;
	printf("\"") ; 
	getchar() ;
	err = close(df) ;
    printf("\n\n close(%i) = %i", df, err) ;
	getchar() ;
    printf("\n") ;
	return(0) ;
}