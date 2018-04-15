#include <so1pub.h\ll_s_so1.h>

void obtenInfoINFO ( info_t far * info )
{
    asm
    {
        les bx,info ;
        mov ax,OBTENINFOINFO ;
        int nVIntSO1 ;
    }
}