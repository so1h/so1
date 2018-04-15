#include <so1pub.h\ll_s_so1.h>

void activarTraza ( word_t numInstr, pid_t pid )
{
    asm
    {
        mov bx,numInstr ;
        mov dx,pid ;
        mov ax,ACTIVARTRAZA ;
        int nVIntSO1 ;
    }
}