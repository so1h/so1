/* ----------------------------------------------------------------------- */
/*                               manej_02.c                                */
/* ----------------------------------------------------------------------- */
/*       manejadores de las llamadas al sistema de soporte de drivers      */
/* ----------------------------------------------------------------------- */

/* ----------------------------------------------------------------------- */
/* AX: 0200H ==> obtenInfoSO1                                              */
/* AX: 0201H ==> crearRecurso                                              */
/* AX: 0202H ==> crearFichero                                              */
/* AX: 0203H ==> esperarDesinstalacion                                     */
/* AX: 0204H ==> destruirRecurso                                           */
/* AX: 0205H ==> encolarCcbRecurso                                         */
/* AX: 0206H ==> eliminarCcbRecurso                                        */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\strings.h>                                    /* strcmp */
#include <so1pub.h\memory.h>                                     /* memcpy */
#include <so1pub.h\ll_s_so1.h>
#include <so1pub.h\ccb.h>
#include <so1pub.h\bios_0.h>
#include <so1.h\ajustes.h>
#include <so1.h\ajustsp.h>
#include <so1.h\interrup.h>
#include <so1.h\procesos.h>
#include <so1.h\blockpr.h>
#include <so1.h\recursos.h>
#include <so1.h\gm.h>

void cargarDescSO1 ( descSO1_t far * descSO1 ) {

  descSO1->ptrIndProcesoActual      = (pindx_t far *)&indProcesoActual ;
  descSO1->ptrTramaProceso          = (trama_t far * far *)&tramaProceso ;
  descSO1->ptrActivarAlEpilogo      = (bool_t far *)&activarAlEpilogo1 ;
  descSO1->ptrDescProceso           = (descProceso_t far *)&descProceso ;
  descSO1->tamDescProceso           = sizeof(descProcesoExt_t) ;
  descSO1->ptrDescFichero           = (descFichero_t far *)&descFichero ;
  descSO1->ptrDescRecurso           = (descRecurso_t far *)&descRecurso ;
  descSO1->ptrC2cPFR                = (c2c_t far *)&c2cPFR ;
  descSO1->ptrNivelActivacionSO1    = (int far *)&nivelActivacionSO1 ;
  descSO1->ptrEnHalt                = (bool_t far *)&enHalt ;
  descSO1->ptrHayTic                = (bool_t far *)&hayTic ;
  descSO1->ptrCcbAlEpilogo          = (ccb_t far *)&ccbAlEpilogo ;
  descSO1->ptrSS_Proceso            = (word_t far *)&SS_Proceso ;
  descSO1->ptrSP_Proceso            = (word_t far *)&SP_Proceso ;
  descSO1->SP0_So1                  = SP0_SO1 ;
  descSO1->ptrContRodajas           = (dword_t far *)&contRodajas ;
  descSO1->ptrContTicsRodaja        = (int far *)&contTicsRodaja ;
  descSO1->ptrVIOrg                 = (rti_t far *)&VIOrg ;
  descSO1->sigProceso               =
    (sigProceso_t)pointer(_CS, (word_t)sigProceso) ;
  descSO1->activarProceso           =
    (activarProceso_t)pointer(_CS, (word_t)activarProceso) ;
  descSO1->buscarNuevoProcesoActual =
    (buscarNuevoProcesoActual_t)pointer(_CS, (word_t)buscarNuevoProcesoActual) ;
  descSO1->bloquearProcesoActual    =
    (bloquearProcesoActual_t)pointer(_CS, (word_t)bloquearProcesoActual) ;
}

extern void so1_manejador_02 ( ) {                     /* ah = 8 ; int SO1 */

  switch (tramaProceso->AL) {

  case 0x00 : {                                                  /* 0x0200 */
    descSO1_t far * descSO1 ;                              /* obtenInfoSO1 */
    descSO1 = (descSO1_t far *)pointer(tramaProceso->ES, tramaProceso->BX) ;
    cargarDescSO1(descSO1) ;
    break ;
  }
  case 0x01 : {                                                  /* 0x0201 */
    descRecurso_t far * dR ;                               /* crearRecurso */
    rindx_t rindx ;
    dR = (descRecurso_t far *)pointer(tramaProceso->ES, tramaProceso->BX) ;
    rindx = crearRec(dR) ;
    tramaProceso->AX = rindx ;
    break ;
  }
  case 0x02 : {                                                  /* 0x0202 */
    char far * nombre ;                                    /* crearFichero */
    rindx_t rindx ;
    word_t menor ;
    tipoFichero_t tipo ;
    nombre = (char far *)pointer(tramaProceso->ES, tramaProceso->BX) ;
    rindx = tramaProceso->CX ;
    menor = tramaProceso->DX ;
    tipo = tramaProceso->SI ;
    tramaProceso->AX = crearFich(nombre, rindx, menor, tipo) ;
    break ;
  }                                                              /* 0x0203 */
  case 0x03 : {                                   /* esperarDesinstalacion */
    word_t tamDATA       = tramaProceso->BX ;              /* BX = tamDATA */
	word_t finCodeDriver = tramaProceso->CX ;        /* CX = finCodeDriver */
	word_t finishDriver  = tramaProceso->DX ;         /* DX = finishDriver */
    if (tamDATA != 0x0000)                            /* tamDATA != 0x0000 */
    {
    	word_t tamPs ;                                       /* paragrafos */
	    word_t tamPilaPs ;                                   /* paragrafos */
	    word_t segBloqueLibre ;
		word_t nuevoSegDatosSR = finCodeDriver >> 4 ;
		word_t nuevoDS = tramaProceso->CS + nuevoSegDatosSR ; 
		trama_t far * nuevaTramaProceso ;
     	
		tamDATA       = (tamDATA + 0x000F) & 0xFFF0 ;             /* bytes */
	    finCodeDriver = (finCodeDriver + 0x000F) & 0xFFF0 ;
		tamPs = descProceso[indProcesoActual].tam ;
//		tamPilaPs = tramaProceso->CS + tamPs 
//		          - (tramaProceso->DS + (off((pointer_t)tramaProceso) >> 4)) ;
        tamPilaPs = (sizeof(trama_t) + 0x000F) >> 4 ;  /* minimo minimorum */
        segBloqueLibre = tramaProceso->DS + (tamDATA >> 4) + tamPilaPs ;		
        nuevoSegDatosSR = finCodeDriver >> 4 ;

		memcpy(          /* copia de los primeros tamDATA bytes de la DATA */
		    MK_FP(tramaProceso->CS, finCodeDriver),             /* destino */
		    MK_FP(tramaProceso->DS, 0x0000),        /* comienzo de la DATA */
            tamDATA
		) ;               
	
        nuevaTramaProceso = MK_FP(
		    nuevoDS, 
			tamDATA + (off((pointer_t)tramaProceso) & 0x000F)
		) ;			   

		memcpy(                               /* copia de la trama de pila */
	       nuevaTramaProceso, 
		   tramaProceso, 
           (tamPilaPs << 4) + (off((pointer_t)tramaProceso) & 0x000F) 
		) ;  

		descProceso[indProcesoActual].tam = segBloqueLibre - tramaProceso->CS ;
		descProceso[indProcesoActual].desplBSS = off((pointer_t)nuevaTramaProceso) ; 
		descProceso[indProcesoActual].desplPila = off((pointer_t)nuevaTramaProceso) ;
		
        ((cabecera_t far *)MK_FP(tramaProceso->CS, 0x0000))->segDatosSR =
            nuevoSegDatosSR ;	
        ((cabecera_t far *)MK_FP(tramaProceso->CS, 0x0000))->finData =
            descProceso[indProcesoActual].desplBSS ;
        ((cabecera_t far *)MK_FP(tramaProceso->CS, 0x0000))->finBSS =
            descProceso[indProcesoActual].desplPila ;	

        k_devolverBloque(segBloqueLibre, tramaProceso->CS + tamPs - segBloqueLibre) ; 

		tramaProceso = nuevaTramaProceso ;
		
        tramaProceso->DS = nuevoDS ;
		tramaProceso->ES = nuevoDS ;
		tramaProceso->AX = 0x0000 ;
		tramaProceso->BX = 0x0000 ;
		tramaProceso->CX = 0x0000 ;
		tramaProceso->DX = 0x0000 ;
		tramaProceso->SI = 0x0000 ;
		tramaProceso->DI = 0x0000 ;
		tramaProceso->SP = off((pointer_t)tramaProceso) + 10*(sizeof(word_t)) ;
		tramaProceso->BP = 0x0000 ; 
		tramaProceso->IP = (word_t)finishDriver ;		
	}
    bloquearProcesoActual(rec_desinstalacion) ;  /* rec_desinstalacion */   
    break ;
  }                                                              /* 0x0204 */
  case 0x04 : {                                         /* destruirRecurso */    
    tramaProceso->AX =
      destruirRec((char far *)pointer(tramaProceso->ES, tramaProceso->BX)) ;
    break ;
  }                                                              /* 0x0205 */
  case 0x05 : ;                                       /* encolarCcbRecurso */
  case 0x06 : {                                      /* eliminarCcbRecurso */
    callBack_t cb ;
    rindx_t rindx ;
    bool_t enc = FALSE ;
    int res ;
    rindx = c2cPFR[DROcupados].primero ;                  /* ver si existe */
    while (rindx != c2cPFR[DROcupados].cabecera) {
      if (!strcmp(descRecurso[rindx].nombre,
                  (char far *)pointer(tramaProceso->CX, tramaProceso->BX))) {
        enc = TRUE ;
        break ;                                          /* sale del bucle */
      }
      rindx = c2cPFR[DROcupados].e[rindx].sig ;
    }
    if (!enc) res = -1 ;                           /* el recurso no existe */
    else {
      cb = (callBack_t)pointer(tramaProceso->ES, tramaProceso->DX) ;
      switch (tramaProceso->AL) {
      case 0x05 :
        res = encolarCcb(cb, descRecurso[rindx].ccb) ;   /* 0, -1, ..., -3 */
        break ;
      case 0x06 :
        res = eliminarCcb(cb, descRecurso[rindx].ccb) ;  /* 0, -1, ..., -4 */
        break ;
      }
      if (res < 0) res-- ;
    }
    tramaProceso->AX = res ;
    break ;
  }
  default : ;
  }

}

