#include <so1pub.h\ll_s_so1.h>

void obtenInfoFAB ( descriptor_de_fichero_t far * df )
{
    asm
    {
        les bx,df ;
        mov ax,OBTENINFOFAB ;
        int nVIntSO1 ;
    }
}