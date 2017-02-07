/* ----------------------------------------------------------------------- */
/*                               ejecutar.h                                */
/* ----------------------------------------------------------------------- */
/*            busqueda, carga y ejecucion de comandos externos             */
/* ----------------------------------------------------------------------- */

#ifndef EJECUTAR_H
#define EJECUTAR_H

pid_t kk_fork ( void ) ;

pindx_t preEjecutar ( const char far * nombre,
                      const char far * comando,
                            pindx_t    pindx ) ;

#define preEjecutarMacro(nombre, comando, pindx)                              \
  preEjecutar((char far *)nombre, (char far *)comando, pindx) ;

#endif /* EJECUTAR_H */
