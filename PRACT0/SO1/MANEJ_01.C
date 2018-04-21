/* ----------------------------------------------------------------------- */
/*                               manej_01.c                                */
/* ----------------------------------------------------------------------- */
/*          manejador de las llamadas al sistema del grupo AH = 01         */
/* ----------------------------------------------------------------------- */

/* ----------------------------------------------------------------------- */
/* AX = 0x0100 open                                                        */
/* AX = 0x0101 close                                                       */
/* AX = 0x0102 read                                                        */
/* AX = 0x0103 aio_read                                                    */
/* AX = 0x0104 write                                                       */
/* AX = 0x0105 aio_write                                                   */
/* AX = 0x0106 lseek                                                       */
/* AX = 0x0107 fcntl                                                       */
/* AX = 0x0108 ioctl                                                       */
/*                                                                         */
/* en general hay que calcular df, dfs comprobar errores y llamar a la     */
/* funcion correspondiente del driver:                                     */
/*                                                                         */
/*          read  --> descRecurso[descFichero[dfs].rindx].read             */
/*          write --> descRecurso[descFichero[dfs].rindx].write            */
/*                                                                         */
/* luego el driver que se las apañe.                                       */
/*                                                                         */
/* El driver, en principio sigue utilizando la pila del sistema operativo. */
/* Para drivers complicados puede haber un primer thread que sirve las     */
/* peticiones mientras el proceso de usuario se bloquea. Las peticiones    */
/* pueden ser encomendadas a uno o varios threads del driver.              */
/* El driver debe poder admitir que los procesos encolen tareas sencillas  */
/* (callbacks) para que se ejecuten desde la isr(s) asociada(s) al driver. */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\ll_s_so1.h>                   /* OPEN, CLOSE, READ, ... */
#include <so1pub.h\tipos.h>                                       /* MK_FP */
#include <so1pub.h\strings.h>                           /* strcmp, strncmp */
#include <so1pub.h\ptrc2c.h>                                   /* ptrC2c_t */
#include <so1.h\procesos.h>    /* descProceso, descFihero, descRecurso ... */ 
#include <so1.h\minifs.h>                                        /* rec_sf */

#include <so1pub.h\bios_0.h>                               /* printStrBIOS */

int indiceTFAS ( const char far * nombre ) 
{
    int dfs ;
	ptrC2c_t ptrC2cDFOcupados = (ptrC2c_t)&c2cPFR[DFOcupados] ;
    dfs = ptrC2cDFOcupados->primero ;                  /* ver si existe ya */
    while (dfs != ptrC2cDFOcupados->cabecera) 
	{
        if (!strcmp(descFichero[dfs].nombre, nombre)) return(dfs) ;
        dfs = ptrC2cDFOcupados->e[dfs].sig ;
    }
    return(-1) ;
}

int indiceTFA ( const char far * nombre ) 
{
    int df, dfs ;
    for ( df = 0 ; df < dfMax ; df++ ) 
        if (((dfs = ptrDPActual->tfa[df].dfs) != -1) &&
            (!strncmp(descFichero[dfs].nombre, nombre, 8)))
            return(df) ;
    return(-1) ;
}

int nuevaEntradaTFA ( void ) 
{
    int df ;
	dfa_t * ptrDfa = (dfa_t *)&ptrDPActual->tfa ; 
    for ( df = 0 ; df < dfMax ; df++ )
	{
        if (ptrDfa->dfs < 0) 
            return(df) ;
		ptrDfa++ ;
	}
    return(-1) ;
}

/* static int contOpen = 0 ; */                 /* para depurar (comentar) */

void so1_manejador_01 ( void )                         /* ah = 1 ; int SO1 */
{
    int df, dfs, dfs1, res ;
    rindx_t rindx ;
    char far * nombre ;
    modoAp_t modoAp ;
    pointer_t dir ;
    word_t nbytes ;
    long posNueva ;
    long pos ;
    int whence ;
    word_t cmd ;                                                  /* fcntl */
    word_t arg ;

    switch (tramaProceso->AX) 
	{
    case OPEN :                                                  /* 0x0100 */
        modoAp = tramaProceso->DX ;                                /* open */
        nombre = MK_FP(tramaProceso->ES, tramaProceso->BX) ;
        df = -1 ;                                           /* por defecto */
        if ((modoAp & (O_TEXT | O_BINARY)) == (O_TEXT | O_BINARY)) 
		{	
            tramaProceso->AX = df ;                                  /* -1 */
            break ;
        }
        if (!(modoAp & (O_TEXT | O_BINARY)))         /* por defecto O_TEXT */
            modoAp = modoAp + O_TEXT ;
        df = indiceTFA(nombre) ;
        if (df >= 0)                       /* fichero ya abierto (proceso) */
		{
//          printStrBIOS("\n fichero ya abierto \n") ;  			
            dfs = ptrDPActual->tfa[df].dfs ;
            rindx = descFichero[dfs].rindx ;
        }
        else                               /* fichero no abierto (proceso) */
		{
            dfs = indiceTFAS(nombre) ;
            if (dfs >= 0)                  /* fichero ya abierto (sistema) */
                rindx = descFichero[dfs].rindx ;
            else                           /* fichero no abierto (sistema) */
//              rindx = rec_sf ;                    /* sistema de ficheros */
            {
                tramaProceso->AX = df ;                              /* -1 */
                break ;
			}
        }
//      printStrBIOS("\n manejador_01 dfs = ") ; printIntBIOS(dfs, 1) ; 
//      printStrBIOS(" rindx = ") ; printIntBIOS(rindx, 1) ; 
//      printLnBIOS() ; 			
        if ((descRecurso[rindx].open != (open_t)NULL) &&
            ((dfs1 = descRecurso[rindx].open(dfs, modoAp)) >= 0)) 
		{
            if ((df = nuevaEntradaTFA()) >= 0) 
			{
                if (dfs1 > 0) dfs = dfs1 ;
                ptrDPActual->nfa++ ;
                ptrDPActual->tfa[df].modoAp = modoAp ;
                ptrDPActual->tfa[df].dfs = dfs ;
                ptrDPActual->tfa[df].pos = 0 ;
                if ((modoAp & 0x0007) == O_RDONLY) descFichero[dfs].contAp_L++ ;
                else if ((modoAp & 0x0007) == O_WRONLY) descFichero[dfs].contAp_E++ ;
            }
            else descRecurso[rindx].release(dfs) ;
        }
        tramaProceso->AX = df ;
        break ;

    case CLOSE :                                                 /* 0x0101 */
        df = tramaProceso->BX ;                                   /* close */
        res = -1 ;
        if ((0 <= df) && (df < dfMax)) 
		{
            dfs = ptrDPActual->tfa[df].dfs ;
            if ((0 <= dfs) && (dfs < dfsMax)) 
			{
                rindx = descFichero[dfs].rindx ;
//              printStrBIOS("\n close dfs = ") ; printDecBIOS(dfs, 1) ; 
                ptrDPActual->nfa-- ;
                modoAp = ptrDPActual->tfa[df].modoAp ;
                if ((modoAp & 0x0007) == O_RDONLY) descFichero[dfs].contAp_L-- ;
                else if ((modoAp & 0x0007) == O_WRONLY) descFichero[dfs].contAp_E-- ;
                ptrDPActual->tfa[df].dfs = -1 ;
                descRecurso[rindx].release(dfs) ;
                res = 0 ;
            }
        }
        tramaProceso->AX = res ;
        break ;

    case READ      :                             /* read      */ /* 0x0102 */
    case AIO_READ  :                             /* aio_read  */ /* 0x0103 */
    case WRITE     :                             /* write     */ /* 0x0104 */
    case AIO_WRITE :                             /* aio_write */ /* 0x0105 */
        df = tramaProceso->BX ;
        res = -1 ;
        if ((0 <= df) && (df < dfMax)) 
		{
            nbytes = tramaProceso->CX ;
            if (nbytes == 0) res = 0 ;
            else 
			{
                dfs = ptrDPActual->tfa[df].dfs ;
                if ((0 <= dfs) && (dfs < dfsMax)) 
			    {
                    rindx = descFichero[dfs].rindx ;
                    if ((0 <= rindx) && (rindx < maxRecursos))
                       modoAp = ptrDPActual->tfa[df].modoAp ;
                    dir = MK_FP(tramaProceso->ES, tramaProceso->DX) ;
                    switch (tramaProceso->AX) 
				    {
                    case READ     :                                /* read */
                    case AIO_READ :                            /* aio_read */
                        if ((modoAp & 0x0007) == O_RDONLY) 
					    {
                            if (tramaProceso->AX == READ)
                                res = descRecurso[rindx].read(dfs, dir, nbytes) ;
                            else
                                res = descRecurso[rindx].aio_read(dfs, dir, nbytes) ;
                        }
                        break ;
                    case WRITE     :                              /* write */
                    case AIO_WRITE :                          /* aio_write */
                        if ((modoAp & 0x0007) == O_WRONLY) 
						{
                            if (tramaProceso->AX == WRITE)
                                res = descRecurso[rindx].write(dfs, dir, nbytes) ;
                            else
                                res = descRecurso[rindx].aio_write(dfs, dir, nbytes) ;
                        }
                        break ;
                    }
                    if (res > 0)
                        ptrDPActual->tfa[df].pos += res ;
                }
            }
        }
        tramaProceso->AX = res ;
        break ;

    case LSEEK :                                                /* 0x0106 */
        df = tramaProceso->BX ;                                  /* lseek */
        posNueva = -1L ;
        if ((0 <= df) && (df < dfMax)) {
            dfs = ptrDPActual->tfa[df].dfs ;
            if ((0 <= dfs) && (dfs < dfsMax)) 
			{
                rindx = descFichero[dfs].rindx ;
                if ((0 <= rindx) && (rindx < maxRecursos)) 
				{
                    pos = (long)MK_FP(tramaProceso->ES, tramaProceso->DX) ;
                    whence = tramaProceso->CX ;
                    if ((SEEK_SET <= whence) && (whence <= SEEK_END)) 
					{
                        posNueva = descRecurso[rindx].lseek(dfs, pos, whence) ;
                        if (posNueva >= 0L)
                            ptrDPActual->tfa[df].pos = posNueva ;
                    }
                }
            }
        }
        tramaProceso->AX = (word_t)posNueva ;
        tramaProceso->DX = (word_t)(posNueva >> 16) ;
        break ;

    case FCNTL :                                                /* 0x0107 */
        df = tramaProceso->BX ;                                  /* fcntl */
        cmd = tramaProceso->CX ;
        arg = tramaProceso->DX ;
        res = -1 ;
        if ((0 <= df) || (df < dfMax)) 
		{
            modoAp = ptrDPActual->tfa[df].modoAp ;
            res = 0 ;

            if (cmd == F_GETFD) res = modoAp ;
            else if (cmd == F_SETFD) 
			{
                modoAp = modoAp & ~(O_APPEND | O_NONBLOCK) ;
                modoAp = modoAp | (arg & (O_APPEND | O_NONBLOCK)) ;
                ptrDPActual->tfa[df].modoAp = modoAp ;
            }
/*
            switch (cmd) 
			{
            case F_GETFD : res = modoAp ; break ;
            case F_SETFD : modoAp = modoAp & ~(O_APPEND | O_NONBLOCK) ;
                           modoAp = modoAp | (arg & (O_APPEND | O_NONBLOCK)) ;
                           descriptor[indProcesoActual].tfa[df].modoAp = modoAp ;
                           break ;
            default : ;
            }
*/
        }
        tramaProceso->AX = res ;
        break ;
		
    case IOCTL :                                                 /* 0x0108 */
        df = tramaProceso->BX ;                                   /* ioctl */
        cmd = tramaProceso->CX ;
        arg = tramaProceso->DX ;
        res = -1 ;
        if ((0 <= df) || (df < dfMax)) 
		{
            dfs = ptrDPActual->tfa[df].dfs ;
            if ((0 <= dfs) && (dfs < dfsMax)) 
			{
                rindx = descFichero[dfs].rindx ;
                if ((0 <= rindx) && (rindx < maxRecursos)) 
				{
                    res = descRecurso[rindx].ioctl(dfs, cmd, arg) ;
                }
            }
        }
        tramaProceso->AX = res ;
        break ;
    default : ;
    }
}