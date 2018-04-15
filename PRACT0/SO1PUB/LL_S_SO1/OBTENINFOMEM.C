#include <so1pub.h\ll_s_so1.h>

void obtenInfoMEM ( descProceso_t far * descProceso,
                    e2PFR_t       far * e2PFR,
                    c2c_t         far * c2cPFR,
                    ptrBloque_t       * listaLibres,
                    word_t            * tamBlqMax )
{
    bloquePFR_t bPFR ;
    word_t segmento ;
    word_t tam ;

    bloquePFR_t far * ptrPFR = (bloquePFR_t far *)&bPFR ;

    bPFR.descProceso = descProceso ;
    bPFR.e2PFR = e2PFR ;
    bPFR.c2cPFR = c2cPFR ;

    asm
    {
        les bx,ptrPFR ;
        mov ax,OBTENINFOMEM ;
        int nVIntSO1 ;

        mov segmento,ax ;
        mov tam,dx ;
    }
    *listaLibres = (ptrBloque_t)MK_FP(segmento, 0x0000) ;
    *tamBlqMax = tam ;
}