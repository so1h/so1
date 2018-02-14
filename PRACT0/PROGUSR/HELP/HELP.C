/* ----------------------------------------------------------------------- */
/*                                 help.c                                  */
/* ----------------------------------------------------------------------- */
/*                     programa help para el sistema SO1                   */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\stdio.h>                             /* printf, getchar */

void help ( void )
{
    printf(
        ""                                                                                "\n"
        ""                                                                                "\n"
        "         comandos (in/ex)ternos del shell de SO1:"                               "\n"
        ""                                                                                "\n"
        "   AYUDA CLS CLEAR HELP ID SU INFO PS TIME !"                                    "\n"
        ""                                                                                "\n"
        "   SHUTDOWN [ timeout ]                ""   REBOOT   [ timeout ]                ""\n"
        "   STATUS   [ on | off ]               ""   CTTY     ( CONx | COMx | -h )       ""\n"
        "   DATE     [ -n | -f | -h ]           ""   EXIT     [ status ]                 ""\n"
        "   HALT     [ timeout ]                ""   KILL     [ pid | -a ]               ""\n"
        "   PLOT     [ off ]                    ""   PTOS     [ car | -v ]               ""\n"
//      "   TERM     [ COMx | ... ] [-l] [-n]   "                                         "\n"
        "   TYPE     fichero [lineas]           ""   DFS      [ dfs | -a | -h ]          ""\n"
        "   DMP      SSSS:DDDD [ bytes ]        ""   DIR/LS   [ patron ] [ /w ]          ""\n"
        "   MEM      [ -l | -h ]                ""   RET      [ retardoSO1 ]             ""\n"
        "   VER      [ -c ]                     ""   DR       [ rindx | -a | -h ]        ""\n"
        "   DP       pindx [ -p | -f ] | -a | -h""   SH       [ CONx | COMx ]            ""\n"
    ) ;
}

int main ( void )
{
    help() ;
//	getchar() ;
	return(0) ;
}

