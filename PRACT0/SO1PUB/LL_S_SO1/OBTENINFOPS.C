#include <so1pub.h\ll_s_so1.h>

void obtenInfoPS ( descProceso_t far * descProceso,
                   e2PFR_t       far * e2PFR,
                   c2c_t         far * c2cPFR )
{
    bloquePFR_t bPFR ;

    bloquePFR_t far * ptrPFR = (bloquePFR_t far *)&bPFR ;

    bPFR.descProceso = descProceso ;
    bPFR.e2PFR = e2PFR ;
    bPFR.c2cPFR = c2cPFR ;

    asm
    {
        les bx,ptrPFR ;
        mov ax,OBTENINFOPS ;
        int nVIntSO1 ;
    }
}