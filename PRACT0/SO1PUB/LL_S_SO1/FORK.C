#include <so1pub.h\ll_s_so1.h>

pid_t fork ( void ) {
#if (FALSE)
  pid_t hpid ;
  if (dfGP == -1) dfGP = open("GP", O_RDONLY) ;
  read(dfGP, (pointer_t)&hpid, 2) ;
  return(hpid) ;
#else
    asm
    {
        mov ax,FORK ;
        int nVIntSO1 ;
    }
    return(_AX) ;
#endif
}
