/* ----------------------------------------------------------------------- */
/*                                 dowait.c                                */
/* ----------------------------------------------------------------------- */
/*                       codigo de la llamada wait                         */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\ll_s_so1.h>                            /* EXIT, KILLPID */
#include <so1pub.h\tipos.h>                                     /* pindx_t */
#include <so1pub.h\ptrc2c.h>                     /* (eliminar/encolar)PC2c */
#include <so1.h\procesos.h>             /* matarProcIndx, terminarProcIndx */
#include <so1.h\blockpr.h>                     /* buscarNuevoProcesoActual */

#define bloquearEsperandoHijo( hpindx )                                      \
{                                                                            \
    ptrDPActual->hpindx = hpindx ;                                           \
    bloquearProcesoActual(rec_hijo) ;                                        \
}                                                                            \

int doWait ( pid_t hpid, int far * statloc ) 
{		
    int i ;
	pindx_t hpindx ;
	pid_t pidDifunto ;
	descProceso_t * ptrDPHijo ;
    bool_t noStatus = (statloc == (int far *)NULL) ;  
			
			/* noStatus: el hijo puede morir sin devolver un status */
     		
	if ((hpid < 0) && ((hpid != -1) || noStatus))
		return(-1) ;
			
    hpindx = indice(hpid) ;
        
    if ((hpid >= 0) && (hpindx == -1))                   /* hpid no existe */
        return(-1) ;

	if (hpid == -1)                                /* buscar hijos zombies */
	{
        i = 0 ;
        while ((i < maxProcesos) &&                                
               ((descProceso[i].ppindx != indProcesoActual) ||  /* no hijo */
                (descProceso[i].estado != bloqueado) ||       /* no zombie */
                (descProceso[i].esperandoPor != rec_zombie)))  
            i++ ;
        if (i < maxProcesos) {                       /* primer hijo zombie */
			hpindx = i ;           
            ptrDPHijo = (descProceso_t *)&descProceso[hpindx] ;			
		}
        else hpindx = -1 ;
    }
	else ptrDPHijo = (descProceso_t *)&descProceso[hpindx] ;			
    if ((hpindx != -1) &&                 /* terminar de matar hijo zombie */
        (ptrDPHijo->ppindx == indProcesoActual) &&
        (ptrDPHijo->estado == bloqueado) &&
        (ptrDPHijo->esperandoPor == rec_zombie)) 
	{
        pidDifunto = ptrDPHijo->pid ;                        /* pidDifunto */
        *statloc = ptrDPHijo->status ;                           /* status */
        eliminarProcIndx(hpindx) ;
        eliminarPC2c(hpindx, (ptrC2c_t)&ptrDPActual->c2cHijos) ;
    }
    else                                         /* no hay hijo(s) zombies */
	{
        if (hpid != -1) 
		{
            if (noStatus) 
    		{
                descProceso[hpindx].noStatus = TRUE ;
                pidDifunto = 0 ;
            }
            else bloquearEsperandoHijo(hpindx) ;
        }
        else if (noStatus) 
		{
            i = 0 ;
            while (i < maxProcesos) 
		    {                                                      /* hijo */
                if (descProceso[i].ppindx == indProcesoActual) 
                    descProceso[i].noStatus = TRUE ;
                i++ ;
            }
            pidDifunto = 0 ;
        }
        else 
		{	
            i = 0 ;
            while ((i < maxProcesos) &&
                   (descProceso[i].ppindx != indProcesoActual))    /* hijo */
                i++ ;
            if (i < maxProcesos)                         /* hay algun hijo */
         	{
                ptrDPActual->hpindx = -1 ;
                bloquearProcesoActual(rec_hijo) ;
            }
            else                                           /* no hay hijos */
                pidDifunto = -1 ;                            /* pidDifunto */
        }
    }
	return(pidDifunto) ;
}