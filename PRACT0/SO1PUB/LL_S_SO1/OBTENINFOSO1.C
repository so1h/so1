#include <so1pub.h\ll_s_so1.h>

void obtenInfoSO1 ( descSO1_t far * descSO1 )
{
    asm
    {
        les bx,descSO1 ;
        mov ax,OBTENINFOSO1 ;
        int nVIntSO1 ;
    }
}