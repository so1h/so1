#include <so1pub.h\ll_s_so1.h>

int setgid ( gid_t gid )
{
    asm
    {
        mov bx,gid ;
        mov ax,SETGID ;
        int nVIntSO1 ;
    }
    return(0) ;
}