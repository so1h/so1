/* ----------------------------------------------------------------------- */
/*                               escribir.c                                */
/* ----------------------------------------------------------------------- */
/*                                                                         */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\ll_s_so1.h>
#include <so1pub.h\printgen.h>
#include <so1pub.h\put.h>                                        /* putCar */
#include <so1pub.h\escribir.h>
#include <so1pub.h\biosdata.h>                              /* ptrBiosData */

int escribirCar ( char car ) {
    return(putCar(STDOUT, car)) ;
}

int escribirLn ( void ) {
    return(printGenLn(escribirCar)) ;
}

int escribirStr ( const char * str ) {
    return(printGenStr(str, escribirCar)) ;
}

int escribirStrHasta ( const char * str, word_t n, bool_t lleno ) {
    return(printGenStrHasta(str, n, lleno, escribirCar)) ;
}

int escribirDec ( word_t num, word_t l ) {
    return(printGenDec(num, l, escribirCar)) ;
}

int escribirLDec ( dword_t num, word_t l ) {
    return(printGenLDec(num, l, escribirCar));
}

int escribirInt ( int num, word_t l ) {
    return(printGenInt(num, l, escribirCar)) ;
}

int escribirLInt ( long int num, word_t l ) {
    return(printGenLInt(num, l, escribirCar)) ;
}

int escribirHex ( word_t num, word_t l ) {
    return(printGenHex(num, l, escribirCar)) ;
}

int escribirLHex ( dword_t num, word_t l ) {
    return(printGenLHex(num, l, escribirCar)) ;
}

int escribirBin ( word_t num, word_t l ) {
    return(printGenBin(num, l, escribirCar)) ;
}

int escribirLBin ( dword_t num, word_t l ) {
    return(printGenLBin(num, l, escribirCar)) ;
}

int escribirPtr ( pointer_t ptr ) {
    return(printGenPtr(ptr, escribirCar)) ;
}

int escribirCursor ( byte_t F, byte_t C ) {
    return(lseek(STDOUT, F*80+C, SEEK_SET)) ;
}

int leerCursor ( byte_t * F, byte_t * C ) {
    int res ;
	if ((res = lseek(STDOUT, 0, SEEK_CUR)) < 0) return(-1) ;
    *F = res/80 ;
	*C = res%80 ;
	return(res) ;
}

int establecerLineas ( int lineas ) {
	return(ioctl(STDOUT, 0x0003, lineas)) ;
}	

int numeroDeLineas ( void ) {
	return(ptrBiosArea->VIDEO_lastrow + 1) ;
}