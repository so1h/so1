/* ----------------------------------------------------------------------- */
/*                                verargs.c                                */
/* ----------------------------------------------------------------------- */
/*                    programa verargs para el sistema SO1                 */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\ll_s_so1.h>    /* biblioteca de llamadas al sistema SO1 */
#include <so1pub.h\escribir.h>

void main ( int argc, char * argv [ ] ) {
  int i ;
  escribirStr("\n\n VER_ARGS: muestra los argumentos argc y argv de main") ;
  escribirStr("\n\n   argc = ") ;
  escribirInt(argc, 1) ;
  escribirLn() ;
  for ( i = 0 ; i < argc ; i++ ) {
    escribirStr("\n   argv[") ;
    escribirDec(i, 1) ;
    escribirStr("] = \"") ;
    escribirStr(argv[i]) ;
    escribirStr("\"") ;
  }
  escribirLn() ;
}

