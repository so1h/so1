/* ----------------------------------------------------------------------- */
/*                               ejecutar.c                                */
/* ----------------------------------------------------------------------- */
/*            busqueda, carga y ejecucion de comandos externos             */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\tipos.h>  /* pid_t, pindx_t, word_t, dword_t, pointer_t */ 
//                                                /* MK_FP, FP_SEG, FP_OFF */
#include <so1pub.h\def_proc.h>                 /* descProceso, desplFinish */
#include <so1pub.h\scanner.h>                                /* tamComando */
#include <so1pub.h\msdos.h>                  /* lseekMSDOS, SEEK_SET_MSDOS */
#include <so1pub.h\strings.h>                   /* strcpy, strncpy, strcmp */
#include <so1pub.h\bios_0.h>/*                  print(Dec/Hex/Int/Str)BIOS */
#include <so1pub.h\memory.h>                          /* memcpy, memcpy_fd */
#include <so1pub.h\ptrc2c.h>       /* ptrC2c_t, (des)encolarPC2c, inicPC2c */ 
#include <so1.h\gm.h>                  /* k_buscarBloque, k_devolverBloque */
#include <so1.h\procesos.h>       /* indProcesoActual, descProceso, c2cPFR */
//                                    /* DPOcupados, DPLibres, maxProcesos */
#include <so1.h\ajustes.h>                                     /* desplCab */
#include <so1.h\sf.h>

//#include <so1.h\so1dbg.h>                            /* puntoDeParadaSO1 */

pid_t kk_fork ( void ) 
{
    word_t segmento ;
    pindx_t pindx ;
    word_t SS_NuevoProceso ;
	trama_t far * tramaNuevoProceso ;
    word_t tam = ptrDPActual->tam ;
	descProceso_t * ptrDPPindx ;

//  if (c2cPFR[DPOcupados].numElem >= (maxProcesos-1))    /* ya comprobado */
//      return(-1) ;

    if ((segmento = k_buscarBloque(tam)) == 0x0000)        /* nuevo bloque */
        return(-2) ;

	/* copia del espacio de direccionamiento (codigo, datos	y pila) */
		
    memcpy_fd(
	    MK_FP(segmento, 0x0000), 
	    MK_FP(ptrDPActual->CSProc, 0x0000), 
		16UL*(dword_t)tam
	) ;
	
	/* nuevo descriptor de proceso */
	
    pindx = desencolarPC2c((ptrC2c_t)&c2cPFR[DPLibres]) ;
    encolarPC2c(pindx, (ptrC2c_t)&c2cPFR[DPOcupados]) ;

	ptrDPPindx = (descProceso_t *)&descProceso[pindx] ; 
	
	/* copia del contenido del antiguo descriptor de proceso */
	
    memcpy(
	    ptrDPPindx, 
		ptrDPActual,
		sizeof(descProceso_t)
	) ;

	/* calculo de la direccion de la trama del nuevo proceso */
	
    SS_NuevoProceso = 
	
	    segmento + (FP_SEG(tramaProceso) - ptrDPActual->CSProc) ;
	
	tramaNuevoProceso = 
	
	    MK_FP(SS_NuevoProceso, FP_OFF(tramaProceso)) ;
	
    ptrDPPindx->trama = tramaNuevoProceso ;

    /* retoque de algunos registros del nuevo proceso, ya que la mayoria   */
	/* tienen ya el valor correcto debido a la copia del espacio de        */
	/* direccionamiento de pila                                            */
	
    ptrDPPindx->CSProc = segmento ;
	
    tramaNuevoProceso->DS = SS_NuevoProceso ;
    tramaNuevoProceso->ES = SS_NuevoProceso ;
//  tramaNuevoProceso->DI = tramaProceso->DI ;
//  tramaNuevoProceso->SI = tramaProceso->SI ;
//  tramaNuevoProceso->BP = tramaProceso->BP ;
//  tramaNuevoProceso->SP = tramaProceso->SP ;
//  tramaNuevoProceso->BX = tramaProceso->BX ;
//  tramaNuevoProceso->DX = tramaProceso->DX ;
//  tramaNuevoProceso->CX = tramaProceso->CX ;
//  tramaNuevoProceso->AX = tramaProceso->AX ;
//  tramaNuevoProceso->IP = tramaProceso->IP ;
    tramaNuevoProceso->CS = segmento ;
//  tramaNuevoProceso->Flags = tramaProceso->Flags ;

    registrarEnPOrdenados(pindx) ;

    ptrDPPindx->pid = nuevoPid() ;
    ptrDPPindx->hpindx = -1 ;
    inicPC2c(&ptrDPPindx->c2cHijos, &e2PFR.e2Hijos, maxProcesos + pindx, TRUE) ;
    ptrDPPindx->ppindx = indProcesoActual ;
    encolarPC2c(pindx, (ptrC2c_t)&ptrDPActual->c2cHijos) ;

    return(pindx) ;
}

pid_t kk_thread ( void * (* funcion) (void * arg), word_t SP0, void * arg ) 
{
    pindx_t pindx ;
    word_t SS_NuevoProceso ;
	trama_t far * tramaNuevoProceso ;
	word_t far * ptrWord ;
	cabecera_t far * cabecera ;
	descProceso_t * ptrDPPindx ;

//  if (c2cPFR[DPOcupados].numElem >= (maxProcesos-1))    /* ya comprobado */
//      return(-1) ;

	/* nuevo descriptor de thread (proceso) */
	
    pindx = desencolarPC2c((ptrC2c_t)&c2cPFR[DPLibres]) ;
    encolarPC2c(pindx, (ptrC2c_t)&c2cPFR[DPOcupados]) ;

	ptrDPPindx = (descProceso_t *)&descProceso[pindx] ; 
	
	/* copia del contenido del antiguo descriptor de proceso */
	
    memcpy(
	    ptrDPPindx, 
		ptrDPActual,
		sizeof(descProceso_t)
	) ;

	/* calculo de la direccion de la trama del nuevo proceso */
	
    SS_NuevoProceso = FP_SEG(tramaProceso) ;
	tramaNuevoProceso = MK_FP(
        SS_NuevoProceso, SP0 - sizeof(trama_t) - 5*sizeof(word_t)
    ) ;

    ptrDPPindx->trama = tramaNuevoProceso ;

    /* registros del nuevo proceso */
	
    tramaNuevoProceso->DS = tramaProceso->DS ;
    tramaNuevoProceso->ES = tramaProceso->ES ;
    tramaNuevoProceso->DI = 0x0000 ;
    tramaNuevoProceso->SI = 0x0000 ;
    tramaNuevoProceso->BP = SP0 - 5*sizeof(word_t) ;
    tramaNuevoProceso->SP = SP0 - 6 - 5*sizeof(word_t) ;
    tramaNuevoProceso->BX = 0x0000 ;
    tramaNuevoProceso->DX = 0x0000 ;
    tramaNuevoProceso->CX = 0x0000 ;
    tramaNuevoProceso->AX = 0x0000 ;
    tramaNuevoProceso->IP = (word_t)funcion ;
    tramaNuevoProceso->CS = tramaProceso->CS ;
    tramaNuevoProceso->Flags = (tramaProceso->Flags & 0xF000) | 0x0202 ;

	ptrWord = MK_FP(SS_NuevoProceso, SP0) ;
	
	cabecera = (cabecera_t far *)MK_FP(tramaProceso->CS, desplCab()) ;
	
	*(--ptrWord) = 0x0000 ;   /* apilo 0x0000: ver finish y (macro) atexit */
	*(--ptrWord) = 0x0000 ;                        /* apilo parametro argv */
	*(--ptrWord) = 0x0000 ;                        /* apilo parametro argc */
	*(--ptrWord) = (word_t)arg ;                /* parametro de la funcion */
	*(--ptrWord) = cabecera->desplFinish ;  /* apilo dir. de ret. a finish */
	
//  registrarEnPOrdenados(pindx) ;
	
    ptrDPPindx->pid = nuevoPid() ;
    ptrDPPindx->hpindx = -1 ;
    inicPC2c(&ptrDPPindx->c2cHijos, &e2PFR.e2Hijos, maxProcesos + pindx, TRUE) ;
    ptrDPPindx->ppindx = indProcesoActual ;
//  encolarPC2c(pindx, (ptrC2c_t)&ptrDPActual->c2cHijos) ;  /* hijo thread */

    return(pindx) ;
}

/* pindx == -1 si se quiere crear un nuevo proceso (descProceso nuevo, pid */
/*             nuevo y bloque de memoria nuevo)                            */
/* pindx != -1 si se quiere mantener el mismo descProceso y el mismo pid   */
/*             pero siendo el bloque de memoria nuevo.                     */

pindx_t preEjecutar ( const char far * nombre,
                      const char far * comando,
                            pindx_t    pindx ) 
{
    int i, n ;
    int df ;
    pindx_t pindxAux ;
    word_t tam ;
    word_t segmento ;
    dword_t tamFich ;
    dword_t posFich ;
    char nombreCompleto [ tamComando ] ;
    char nombreFormateado [ 12 ] ;
    pointer_t ptr ;
    int unidad ;
    int unidad0 ;
    int unidad1 ;
    word_t accion ;
    word_t error ;
    char nombreBackup [12] ;                                   /* v. local */
    char comandoBackup [tamComando] ;                          /* v. local */

#if (TRUE)  
    /* Fake86: No se por que motivo con el emulador Fake86 cuando se llama */
    /* con el parametro pindx = -1 (0xFFFF), resulta que se recibe 255     */
    /* (0x00FF). Para intentar subsanarlo pongo la siguiente instruccion:  */
    if (pindx >= maxProcesos) pindx = -1 ;                   /* 2017-08-25 */
#endif
  
#if (FALSE)
    printStrBIOS("\n ejecutar: pindx = ") ;
    printIntBIOS(pindx, 1) ;
    printStrBIOS(" = 0x") ;
    printHexBIOS(pindx, 4) ;
#endif 

    strncpy(nombreBackup, nombre, 12) ;
    strncpy(comandoBackup, comando, tamComando) ;

#if (FALSE)
//  if (pindx != -1) {                                             /* exec */
    printStrBIOS("\n ejecutar: pindx = ") ;
    printIntBIOS(pindx, 1) ;
    printStrBIOS(" = 0x") ;
    printHexBIOS(pindx, 4) ;
    printStrBIOS("\n ejecutar: nombre = ") ;
    printStrBIOS((char far *)nombre) ;
    printStrBIOS("\n ejecutar: comando = ") ;
    printStrBIOS((char far *)comando) ;
    printStrBIOS("\n ejecutar: nombreBackup = ") ;
    printStrBIOS((char far *)nombreBackup) ;
    printStrBIOS("\n ejecutar: comandoBackup = ") ;
    printStrBIOS((char far *)comandoBackup) ;
//}
#endif

    if (!formatearNombre(nombre, (char *)&nombreFormateado))
        return(-1) ;

#if (FALSE)
    printStrBIOS("\n ejecutar: nombre formateado = \"") ;
    printStrBIOS((char *)&nombreFormateado) ;
    printStrBIOS("\"") ;
#endif

    i = 0 ;
    while (nombre[i] != '\0') 
	{
        nombreCompleto[i] = nombre[i] ;
        i++ ;
    }
    nombreCompleto[i] = '\0' ;

    if (!strcmp(&nombreFormateado[8], "   "))     /* extension por defecto */
    {    
		strcpy(&nombreCompleto[i], ".BIN") ;
        strcpy(&nombreFormateado[8], "BIN") ;
    }

#if (FALSE)
    printStrBIOS("\n ejecutar: nombreCompleto = ") ;
    printStrBIOS(nombreCompleto) ;
    printStrBIOS("\n ejecutar: nombre formateado = \"") ;
    printStrBIOS(nombreFormateado) ;
    printStrBIOS("\"") ;
#endif

    if (strcmp(&nombreFormateado[8], "BIN"))
        return(-2) ;

    if (unidadLogicaActual >= 0) 
	{
        if ((df = abrirFichero(nombreCompleto, unidadLogicaActual)) == -1)
            return(-5) ;
    }
    else                                     /* openDOS(nombreCompleto, 0) */
    {
        unidad = getdiskDOS() ;
//      mostrarFlags() ;/* comprobar que no se permiten las interrupciones */
        if (('A' < nombreCompleto[0]) && (nombreCompleto[0] <= 'Z') && 
		    (nombreCompleto[1] == ':')) 
		{
            unidad0 = nombreCompleto[0]-'A' ;
            setdiskDOS(unidad0) ;
//          mostrarFlags() ;     /* comprobar que no se permiten las ints. */ 
            unidad1 = getdiskDOS() ;
//          mostrarFlags() ;     /* comprobar que no se permiten las ints. */ 
            setdiskDOS(unidad) ;
//          mostrarFlags() ;     /* comprobar que no se permiten las ints. */ 
            if (unidad1 != unidad0) 
                return(-4) ;
            
        }
        accion = 1 ;
        if ((df = extendedOpenDOS((pointer_t)nombreCompleto, 0x2000, 0, &accion, &error)) == -1)
            return(-5) ;
//      mostrarFlags() ;         /* comprobar que no se permiten las ints. */ 
        }

        if (unidadLogicaActual >= 0)
            tamFich = tamFichero(df) ;
        else 
		{
            posFich = (dword_t)0UL ;
            lseekDOS(df, (dword_t *)&posFich, SEEK_END_MSDOS) ;
//          mostrarFlags() ;     /* comprobar que no se permiten las ints. */ 
            tamFich = posFich ;
            posFich = (dword_t)0 ;                         /* volvemos a 0 */
            lseekDOS(df, (dword_t *)&posFich, SEEK_SET_MSDOS) ;  
//          mostrarFlags() ;     /* comprobar que no se permiten las ints. */ 
        }

#if (FALSE)
        printStrBIOS("\n tamFich = ") ;
        printLDecBIOS(tamFich, 1) ;
        printStrBIOS(" k_tamBloqueMax = ") ;
        printHexBIOS(k_tamBloqueMax, 4) ;
        printStrBIOS(" 16*k_tamBloqueMax = ") ;
        printLHexBIOS(16L*(long)k_tamBloqueMax, 8) ;
#endif

    if (pindx >= 0) {                         /* pindx == indProcesoActual */

    /* Se va a devolver el bloque ocupado por el proceso que ejecuta exec  */
    /* pudiendo ser sobreescrito al cargar el nuevo programa. Por ese      */
    /* motivo ya no va poder utilizarse el espacio de direcionamiento del  */
    /* proceso actual que ejecuta el exec, en particular cualquier         */
    /* variable del proceso.                                               */

        k_devolverBloque(descProceso[pindx].CSProc, descProceso[pindx].tam) ;
    }

    if ((tamFich > (dword_t)(16L*(long)k_tamBloqueMax)) ||
        ((tam = tamProceso(df)) == 0x0000) ||
        ((segmento = k_buscarBloque(tam)) == 0x0000)) 
	{
#if (FALSE)
        printStrBIOS("\n df = ") ;
        printDecBIOS(df, 1) ;
        printStrBIOS("\n tam = 0x") ;
        printHexBIOS(tam, 4) ;
        printStrBIOS("\n segmento = 0x") ;
        printHexBIOS(segmento, 4) ;
#endif
        if (unidadLogicaActual >= 0)
            cerrarFichero(df) ;
        else
            closeDOS(df) ;
//      mostrarFlags() ;         /* comprobar que no se permiten las ints. */ 
        return(-6) ;
    }

#if (FALSE)
    printStrBIOS("\n tam = ") ;
    printDecBIOS(tam, 1) ;
    printStrBIOS("\n segmento = ") ;
    printHexBIOS(segmento, 4) ;
#endif

    if (unidadLogicaActual >= 0)   /* pueden cargarse fichs. de mas de 64K */
	{
        if (!cargaFicheroAbierto_Ok(df, segmento)) 
		{
            k_devolverBloque(segmento, tam) ;
            cerrarFichero(df) ;
            return(-9) ;
        }
        cerrarFichero(df) ;
    }
    else 
	{
        word_t s = segmento ;
        ptr = MK_FP(s, 0x0000) ;
        posFich = (dword_t)0 ;
        lseekDOS(df, (dword_t *)&posFich, SEEK_SET_MSDOS) ;      /* whence */
        while ((n = readDOS(df, (char far *)ptr, 32*512)) > 0) {
            s = s + (32*512)/16 ;
            ptr = MK_FP(s, 0x0000) ;
        }
        closeDOS(df) ;
        if (n < 0) return(-9) ;
    }

#if (FALSE)
    printStrBIOS("\n antes de crearProceso\n") ;
    if (pindx != -1)                                               /* exec */
	{
        printStrBIOS("\n ejecutar: nombre = ") ;
        printStrBIOS((char far *)nombre) ;
        printStrBIOS("\n ejecutar: comando = ") ;
        printStrBIOS((char far *)comando) ;
//      puntoDeParadaSO1() ;                           /* puntoDeParadaSO1 */
    }
#endif

    if ((pindxAux = crearProceso(
                        segmento, tam, tamFich, nombreBackup, comandoBackup, pindx)
					) < 0) 
	{
#if (FALSE)
        printStrBIOS("\n crearProceso devuelve el error: ") ;
        printIntBIOS(pindxAux, 1) ;
     	printLnBIOS() ; 
#endif
        k_devolverBloque(segmento, tam) ;
        return(-10) ;
    }

#if (FALSE)
    printStrBIOS("\n despues del if de crearProceso") ;
#endif

    if ((pindx >= 0) && (pindxAux != pindx)) return(-11) ;

    return(pindxAux) ;
}