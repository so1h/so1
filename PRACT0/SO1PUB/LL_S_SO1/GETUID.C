#include <so1pub.h\ll_s_so1.h>

uid_t getuid ( void )
{
    asm
    {
        mov ax,GETUID ;
        int nVIntSO1 ;
    }
    return(_AX) ;
}