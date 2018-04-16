/* ----------------------------------------------------------------------- */
/*                               manej_00.c                                */
/* ----------------------------------------------------------------------- */
/*         manejador de las llamadas al sistema del grupo AX = 00xx        */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\ll_s_so1.h>                 /* CREATEPROCESS, FORK, ... */
#include <so1pub.h\tipos.h>                              /* pid_t, pindx_t */
#include <so1pub.h\ptrc2c.h>              /* (encolar/apilar/eliminar)PC2c */
#include <so1pub.h\def_proc.h>                    /* descProceso, rec_hijo */
#include <so1.h\ejecutar.h>             /* kk_fork, kk_thread, preEjecutar */
#include <so1.h\procesos.h>         /* tramaProceso, c2cPFR, matarProcIndx */
#include <so1.h\blockpr.h>          /* (bloquear/buscarNuevo)ProcesoActual */
#include <so1.h\dokill.h>                                        /* doKill */
#include <so1.h\dowait.h>                                        /* doWait */
#include <so1pub.h\bios_0.h>                               /* printStrBIOS */

void so1_manejador_00 ( void )                        /* ah = 00 ; int SO1 */
{
    int i ;
    pid_t pid ;
    pid_t hpid ;
    pindx_t pindx ;
    pindx_t hpindx ;
    bool_t noStatus ;

    switch (tramaProceso->AX) 
	{
    case CREATEPROCESS :                     /* createProcess */ /* 0x0000 */
    case FORK          :                              /* fork */ /* 0x0001 */
    case THREAD        :                            /* thread */ /* 0x000d */

        if (c2cPFR[DPOcupados].numElem >= (maxProcesos-1)) 
		{
            tramaProceso->AX = -10 ;                    /* codigo de error */
            break ;
        }

        ptrDPActual->trama = tramaProceso ;          /* salvamos el estado */

        if      (tramaProceso->AX == FORK)                         /* fork */
            pindx = kk_fork() ;                                    /* ==== */
        else if (tramaProceso->AX == THREAD)                     /* thread */
            pindx = kk_thread(                                   /* ====== */
			            (funcion_t)tramaProceso->BX,            /* funcion */
						tramaProceso->CX,                           /* SP0 */
						(void *)tramaProceso->DX                    /* arg */
					) ;
		else /* tramaProceso->AX == CREATEPROCESS */
		{                                                 /* createProcess */
            pindx = preEjecutar(                          /* ============= */ 
                MK_FP(tramaProceso->CX, tramaProceso->BX),/* CX:BX nombre  */
                MK_FP(tramaProceso->ES, tramaProceso->DX),/* ES:DX comando */
                -1                                    /* createProcess: -1 */
            ) ;
        }

        if (pindx >= 0) 
		{
             /* avisar por consola cuando quede un unico descProceso libre */

            if (c2cPFR[DPOcupados].numElem == (maxProcesos-2)) 
			{
                printStrBIOS("\n solo queda un descriptor de proceso libre ") ;

//              printStrBIOS("\n c2cPFR[DPOcupados].numElem = ") ;
//              printDecBIOS(c2cPFR[DPOcupados].numElem, 1) ;
//              printStrBIOS(" maxProcesos = ") ;
//              printDecBIOS(maxProcesos, 1) ;
//              printStrBIOS(" pindx = ") ;
//              printDecBIOS(pindx, 1) ;
            }

        /* imponemos que se ejecute a continuacion en la CPU primero el    */
        /* proceso hijo pindx y luego el proceso padre indProcesoActual.   */
        /* Para ello apilamos en la cola intermedia PUrgentes, la cual el  */
        /* planificador incorpora a la cola de preparados desapilando de   */
        /* PUrgentes y apilando en PPreparados.                            */
        /* esto es crucial en las inicializaciones (INICCODE.C).           */

            descProceso[pindx].estado = preparado ;
//          encolarPC2c(pindx, (ptrC2c_t)&c2cPFR[PUrgentes]) ;
            apilarPC2c(pindx, (ptrC2c_t)&c2cPFR[PUrgentes]) ;

            ptrDPActual->estado = preparado ;
//          encolarPC2c(indProcesoActual, (ptrC2c_t)&c2cPFR[PUrgentes]) ;
            apilarPC2c(indProcesoActual, (ptrC2c_t)&c2cPFR[PUrgentes]) ;

        /* se ejecutara pindx antes que indProcesoActual */
            if (tramaProceso->AX == FORK)                          /* fork */
                descProceso[pindx].trama->AX = 0x0000 ;         /* hijo AX */
            tramaProceso->AX = descProceso[pindx].pid ;        /* padre AX */
            buscarNuevoProcesoActual() ;  /* no se retorna de esta funcion */
        }                                 /* ============================= */
        else
           tramaProceso->AX = pindx ;                   /* codigo de error */
        break ;

    case 0x02 :                                       /* exec */ /* 0x0002 */
        pindx = preEjecutar(
            MK_FP(tramaProceso->CX, tramaProceso->BX),    /* CX:BX nombre  */
            MK_FP(tramaProceso->ES, tramaProceso->DX),    /* ES:DX comando */
            indProcesoActual                     /* exec: indProcesoActual */
        ) ;

#if (FALSE)
        printStrBIOS("\n pindx = ") ;
        printIntBIOS(pindx, 1) ;
        printStrBIOS(" pid = ") ;
        printIntBIOS(descProceso[pindx].pid, 1) ;
        printStrBIOS(" estado = ") ;
        printIntBIOS(descProceso[pindx].estado, 1) ;
        printStrBIOS("\n CSProc = 0x") ;
        printHexBIOS(descProceso[pindx].CSProc, 4) ;
        printStrBIOS("\n programa = ") ;
        printStrBIOS("\"") ;
        printStrHastaBIOS(descProceso[pindx].programa, 12, FALSE) ;
        printStrBIOS("\"") ;
        printStrBIOS("\n comando = \"") ;
        printStrBIOS((char *)&descProceso[pindx].comando) ;
        printStrBIOS("\"") ;
#endif

        if (pindx < -5)                                     /* error fatal */
		{                                   
            ptrDPActual->estado = bloqueado ;
            ptrDPActual->esperandoPor = rec_zombie ;
            eliminarProcIndx(indProcesoActual) ;
            eliminarPC2c(ptrDPActual->ppindx,
                         (ptrC2c_t)&ptrDPActual->c2cHijos) ;
            buscarNuevoProcesoActual() ;/* ¡no se retorna de esta funcion! */
        }                               /* =============================== */
        else if (pindx < 0)                                  /* error leve */
            break ;                          /* el procesoActual se retoma */
        else       /* el proceso recubierto pasa a (comienza su) ejecucion */
            activarProceso(pindx) ;       /* no se retorna de esta funcion */

                                                                 /* 0x0003 */
    case WAITPID :                                              /* waitpid */
	    {
		    pid_t hpid = tramaProceso->BX ;                /* pid del hijo */
			int far * statloc ;
			statloc = MK_FP(tramaProceso->ES, tramaProceso->DX) ;
			tramaProceso->AX = doWait(hpid, statloc) ;
			break ;
        }
                                                                 /* 0x0004 */
    case EXIT :                                                    /* exit */
        doKill(indProcesoActual) ;
        break ;
                                                                 /* 0x0005 */
    case GETPID :                                                /* getpid */
        tramaProceso->AX = ptrDPActual->pid ;
        break ;
                                                                 /* 0x0006 */
    case GETPINDX :                                            /* getpindx */
        tramaProceso->AX = indProcesoActual ;
        break ;
                                                                 /* 0x0007 */
    case GETPPID :                                              /* getppid */
        tramaProceso->AX = descProceso[ptrDPActual->ppindx].pid ;
        break ;
                                                                 /* 0x0008 */
    case GETUID :                                                /* getuid */
        tramaProceso->AX = ptrDPActual->uid ;
        break ;
                                                                 /* 0x0009 */                                                                 /* 0x0008 */
    case SETUID :                                                /* setuid */
        ptrDPActual->uid = tramaProceso->BX ;
        break ;
                                                                 /* 0x000a */
    case GETGID :                                                /* getgid */
        tramaProceso->AX = ptrDPActual->gid ;
        break ;
                                                                 /* 0x000b */
    case SETGID :                                                /* setgid */
      ptrDPActual->gid = tramaProceso->BX ;
      break ;
                                                                 /* 0x000c */
    case KILLPID :                                              /* killpid */
        pid = (pid_t)tramaProceso->DX ;
        if (pid >= 0)                              /* matar el proceso pid */
		{
            i = indice(pid) ;
            tramaProceso->AX = (word_t)((i != -1) ? doKill(i) : -2) ;
//          tramaProceso->AX = (word_t)((i != -1) ? matarProcIndx(i) : -2) ;
        }
        else                                 /* matar a todos los procesos */
		{
            for ( i = 1 ; i < maxProcesos ; i++ )
                matarProcIndx(i) ;
            tramaProceso->AX = 0 ;
            if (indProcesoActual > 0)
                buscarNuevoProcesoActual() ;
        }
        break ;
//                                                               /* 0x000d */
//  case THREAD : ;                                              /* thread */
//      /* esta implementada arriba con fork y createProcess */	

                                                                 /* 0x000e */
    case YIELD :                                                  /* yield */
        ptrDPActual->trama = tramaProceso ;
        ptrDPActual->estado = preparado ;	
        encolarPC2c(indProcesoActual, &c2cPFR[PPreparados]) ;		
		activarProceso(sigProceso()) ;
		break ;
		
    default : ;
    }
}