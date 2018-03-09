/* ----------------------------------------------------------------------- */
/*                               interrup.c                                */
/* ----------------------------------------------------------------------- */
/*                  rutinas de tratamiento de interrupcion                 */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\tipos.h>                /* rti_t, isrt_t, MK_FP, FP_OFF */
#include <so1pub.h\const.h>                                    /* nVIntMax */
#include <so1pub.h\pic.h>      /* ptrTVI, umask_pic1, umask_pic2, IRQ_PIC2 */
#include <so1.h\ajustsp.h>                                      /* SP0_SO1 */
#include <so1.h\blockpr.h>           /* prologo_rti_sin_PUSHA, epilogo_rti */
#include <so1.h\interrup.h>

rti_t VIOrg [ nVIntMax ] ;          /* vectores de interrupcion originales */

rindx_t recVInt [ nVIntMax ] ;             /* recurso que posee ese vector */

void isrNula (void ) 
{
}

isr_t isr [ nVIntMax ] ;             /* rutina de servicio de interrupcion */

#if defined(TASM32) || defined(IDE) || defined(BASM502)

#define RTI(i, j) \
  asm pusha ;                                                                \
  asm mov bl, 0 ## i ## j ## h ;                             /* bl = nVInt */\
  /* asm jmp fin ; */           /* tam 2 o 3 bytes dependiendo del destino */\
  asm db 0xE9, 0x00, 0x00 ;                 /* 3 bytes */ /* EB 00 2 bytes */

#else

#define RTI(i, j) \
  asm pusha ;                                                                \
  asm mov bl, 0 ## i ## j ## h ;                             /* bl = nVInt */\
  asm db 0xE9 ;                                    /* jmp tratamientoComun */\
  asm dw (OFFSET tratamientoComun) - \
           ((OFFSET rti_00) + ((0x ## i ## j + 1)* tamCodigoRTI)) ;

#endif

#define tamCodigoRTI 6           /* numero de bytes generados en RTI(i, j) */

#define RTIF(i) \
  RTI(i, 0) ; RTI(i, 1) ; RTI(i, 2) ; RTI(i, 3) ; \
  RTI(i, 4) ; RTI(i, 5) ; RTI(i, 6) ; RTI(i, 7) ; \
  RTI(i, 8) ; RTI(i, 9) ; RTI(i, a) ; RTI(i, b) ; \
  RTI(i, c) ; RTI(i, d) ; RTI(i, e) ; RTI(i, f) ;

#pragma option-O-              /* no optimizar los saltos (near --> short) */

void far rti_00 ( void ) 
{
    RTIF(0) ; RTIF(1) ; RTIF(2) ; RTIF(3) ;
    RTIF(4) ; RTIF(5) ; RTIF(6) ; RTIF(7) ;
#if (nVIntMax == 256)
    RTIF(8) ; RTIF(9) ; RTIF(a) ; RTIF(b) ;
    RTIF(c) ; RTIF(d) ; RTIF(e) ; RTIF(f) ;
#endif

//fin:

    prologo_rti_sin_PUSHA() ;                               /* nVInt en BL */

    isr[_BL]() ;     /* llamada a la rutina de servicio de la interrupcion */
                                       /* (se apila la dir. de ret. CS:IP) */
    epilogo_rti() ;

}

#pragma option-O

#if defined(JWASM)

void tratamientoComun ( void ) {                    /* opcion -r- (SI, DI) */

    prologo_rti_sin_PUSHA() ;                                  /* AH en BL */

    isr[_BL]() ;     /* llamada a la rutina de servicio de la interrupcion */
                                       /* (se apila la dir. de ret. CS:IP) */
    epilogo_rti() ;

}

#endif

void inicTVI ( void ) {

  int nVInt ;

#if defined(TASM32) || defined(IDE) || defined(BASM502)
    struct linea {   /* para resolver el problema de la tabla rti_00 (ide) */
        byte_t relleno [ 4 ] ;
        word_t despl ;
    } far * ptrLinea ; 
    int i ;
#endif

  /* salvamos la tabla de vectores original */

    for (nVInt = 0 ; nVInt < nVIntMax ; nVInt++ ) {
        VIOrg[nVInt] = ptrTVI[nVInt] ;
        recVInt[nVInt] = -1 ;
    }

#if defined(TASM32) || defined(IDE) || defined(BASM502)

                     /* para resolver el problema de la tabla rti_00 (ide) */

    ptrLinea = MK_FP(_CS, FP_OFF(rti_00)) ;
    for ( i = 0 ; i < nVIntMax ; i++ )
        ptrLinea[i].despl = (nVIntMax-(i+1))*tamCodigoRTI ;

#endif

}

void far redirigirInt ( byte_t nVInt, isr_t isrx ) 
{	
    ptrTVI[nVInt] = MK_FP(CS_SO1, (FP_OFF(rti_00)) + nVInt*tamCodigoRTI) ;
    isr[nVInt] = isrx ;
}

void far redirigirIntHardware ( byte_t irq, isr_t isr ) 
{
    byte_t nVInt ;
    if (irq < 8) {
        unmask_pic1(1 << irq);
        nVInt = 0x08 + irq;
    } else {
        unmask_pic1(1 << IRQ_PIC2);
        unmask_pic2(1 << (irq - 8));
        nVInt = 0x70 + irq - 8;
    }
    redirigirInt(nVInt, isr) ;
}

void restablecerInt ( int nVInt )
{
    ptrTVI[nVInt] = VIOrg[nVInt] ;
    isr[nVInt] = MK_FP(CS_SO1, FP_OFF(isrNula)) ;
}