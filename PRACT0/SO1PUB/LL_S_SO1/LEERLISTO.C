#include <so1pub.h\ll_s_so1.h>

char leerListo ( int df ) {
  char car ;
  int n = aio_read(df, (pointer_t)&car, 1) ;
  if (n < 1) car = (char)0 ;
  return(car) ;
}