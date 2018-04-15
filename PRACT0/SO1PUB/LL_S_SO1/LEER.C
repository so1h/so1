#include <so1pub.h\ll_s_so1.h>

char leer ( int df )
{
  char car ;
  int n = read(df, (pointer_t)&car, 1) ;
  if (n < 1) car = '\0' ;
  return(car) ;
}