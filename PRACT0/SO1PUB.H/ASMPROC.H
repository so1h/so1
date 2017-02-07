/* ----------------------------------------------------------------------- */
/*                                 asmProc.h                               */
/* ----------------------------------------------------------------------- */
/*      definicion de las macros: asmProc, beginAsmProc y endAsmProc       */
/* ----------------------------------------------------------------------- */

/* ----------------------------------------------------------------------- */
/* Gracias a las macros asmProc, beginAsmProc y endAsmProc podemos         */
/* declarar funciones (sin parametros) conteniendo codigo (o datos) en     */
/* ensamblador en las que el compilador no impone la generacion de una     */
/* ultima intruccion de retorno. La macro asmProc muestra como se utilizan */
/* las dos macros beginAsmProc y endAsmProc: basta comenzar poniendo       */
/* beginAsmProc(asmProcName) { para luego ir añadiendo instrucciones o     */
/* directivas del ensamblador, para al final terminar con la invocacion de */
/* la macro } endAsmProc(asmProcName).                                     */
/*                                                                         */
/* El codigo (o los datos) de la funcion van al segmento de codigo.        */
/*                                                                         */
/* ----------------------------------------------------------------------- */
/* Estas macros solo se han probado de momento con el ensamblador JWASM y  */
/* TASM32, funcionando sin problemas.                                      */
/* ----------------------------------------------------------------------- */

#ifndef ASMPROC_H
#define ASMPROC_H

#define beginAsmProc( asmProcName ) \
  void asmProcName ( void ) ; \
  asm \
  { \
    _DATA ENDS ; \
    _TEXT SEGMENT BYTE PUBLIC 'CODE' ; \
    ASSUME CS:_TEXT,DS:DGROUP ; \
    _ ## asmProcName PROC NEAR ; \
  } \
  asm

#define endAsmProc( asmProcName ) \
  asm \
  { \
    _ ## asmProcName ENDP ; \
    _TEXT ENDS ; \
    public _ ## asmProcName ; \
    _DATA SEGMENT WORD PUBLIC 'DATA' ; \
  } ;

/* en asmProc, asmProCode no puede contener comas */
/* ya que es un parametro de la macro.            */

#define asmProc( asmProcName, asmProcCode ) \
  beginAsmProc( asmProcName ) { \
    asmProcCode ; \
  } ; \
  endAsmProc( asmProcName ) ;

#define asmSymbol( asmSymbolName ) asmProc( asmSymbolName, ; ) ;

#endif /* ASMPROC_H */