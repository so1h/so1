/* ----------------------------------------------------------------------- */
/*                                colores.h                                */
/* ----------------------------------------------------------------------- */
/*                                                                         */
/* ----------------------------------------------------------------------- */

#ifndef COLORES_H
#define COLORES_H

typedef enum {
  negro,                                                /* colores oscuros */
  azul,
  verde,
  ciano,
  rojo,
  magenta,
  marron,
  grisClaro,
  grisOscuro,                                            /* colores claros */
  azulClaro,
  verdeClaro,
  cianoClaro,
  rojoClaro,
  magentaClaro,
  amarillo,
  blanco,

  especial                               /* valor especial de tipo color_t */

} color_t ;

/* NOTA: los caracteres pueden ser de cualquier color de los anteriores,   */
/* pero como fondo solo pueden utilizarse colores oscuros.                 */

#define bitParpadeo 0x80                                /* bit de parpadeo */

               /* tabla de combinaciones de colores de caracter y de fondo */

#define maxCombinaciones 7

typedef struct {
  color_t colorCar ;
  color_t colorFondo ;
} combinacion_t ;

extern combinacion_t combinacion [ maxCombinaciones ] ;

#endif /* COLORES_H */
