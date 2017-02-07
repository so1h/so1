/* ----------------------------------------------------------------------- */
/*                                leercmd.h                                */
/* ----------------------------------------------------------------------- */
/*             lectura de comandos del interprete de comandos              */
/* ----------------------------------------------------------------------- */

#ifndef LEERCMD_H
#define LEERCMD_H

/* leeComando lee el siguiente comando en comando[inCmd] permitiendo en    */
/* parte su edicion. Los parametros autCcompletado y enmascarado sirven    */
/* para cambiar el comportamiento de leerComando para autocompletar los    */
/* comandos (con nombres de comandos internos o de ficheros ejecutables    */
/* .BIN), o tambien para enmascarar los caracteres introducidos con el     */
/* caracter * con el fin de que no sean visualizados en la pantalla.       */

void leerComando ( bool_t autoCompletado, bool_t enmascarado ) ;


#endif /* LEERCMD_H */
