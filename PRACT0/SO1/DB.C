/* ----------------------------------------------------------------------- */
/*                                   db.c                                  */
/* ----------------------------------------------------------------------- */
/*                         dispositivos de bloques                         */
/* ----------------------------------------------------------------------- */

/* Driver de los dispositivos de bloques gestionados mediante la int 13    */
/* del BIOS. El driver esta pensado para formar parte de la imagen del     */
/* nucleo (SO1.BIN o SO1.EXE) aunque podria modificarse para ejecutarse    */
/* como un comando (DB.BIN) si se resuleve el problema de como cargar el   */
/* driver desde el disco sin un driver de disco previo que cargue DB.BIN.  */
/*                                                                         */
/* Hay muchas versiones posibles:                                          */
/*                                                                         */
/* 1: se llama a la int 13 directamente desde readDB y writeDB (lo que     */
/*    tiene el problema de que la int 13 permitiria las interrupciones     */
/*    desde dentro de esas funciones.                                      */
/* 2: se crea un proceso/thread capaz de acceder al mismo espacio de       */
/*    direccionamiento que el nucleo, para que llame a la int 13           */
/*    con las interrupciones permitidas. Este proceso sera un proceso      */
/*    servidor (el unico que llama a la int 13, garantizandose con ello    */
/*    la exclusion mutua).                                                 */
/*    En el caso de querer tener un driver comando DB.BIN, el espacio de   */
/*    direccionamiento seria diferente por lo que habria que recurrir a    */
/*    acceder a descSO1 mediante la llamada al sistema obtenInfoSO1.       */
/*                                                                         */
/*    Una vez que se tiene un proceso/thread servidor las versiones se     */
/*    corresponderian con las diferentes maneras de comunicar las          */
/*    peticiones de lectura/escritura de sectores al servidor.             */
/*    Para el bloqueo podria utilizarse el recurso rec_db. Para el envio   */
/*    del mensaje podria utilizarse una estructura peticionDB_t. Bastaria  */
/*    con una sola declarada en DB.C, lo que obligaria a acceder en        */
/*    exclusion mutua bloqueando los procesos en una cola que podria       */
/*    llamarse bloqueadosEsperandoHacerPeticionDB. Tras ser desbloqueados  */
/*    por el servidor los procesos irian cumplimentando sus peticiones de  */
/*    sectores tras lo cual desbloquearian al servidor y se bloquearian    */
/*    en una cola bloqueadosEsperandoServicioPeticionDB.                   */
/*                                                                         */
/*    En ese caso la comunicacion seria sincrona. Podria mejorarse si      */
/*    tenemos un array de peticiones, una por cada proceso, o si tenemos   */
/*    una cola de peticiones. Tambien se mejoraria si la peticion se hace  */
/*    a traves de los registros del proceso (tramaProceso->BX, ...) justo  */
/*    antes de que el proceso se bloquee.                                  */
/*                                                                         */
/*    En lo que respecta al servidor, si se encolan las peticiones podria  */
/*    llevarse a cabo cierta planificacion de las mismas en funcion de     */
/*    algun criterio.                                                      */

#include <so1pub.h\ll_s_exc.h>                                   /* thread */
#include <so1pub.h\ll_s_msj.h>                             /* receive, ANY */

#include <so1.h\k_msj.h>                                         /* k_send */

#include <so1pub.h\bios_0.h>                 /* print(Car/Dec/Int/Str)BIOS */
#include <so1pub.h\strings.h>                            /* strcpy, strlen */
#include <so1pub.h\def_sf.h>                              /* mbr_t, boot_t */
#include <so1pub.h\memory.h>                                     /* memcpy */
#include <so1pub.h\cmos.h>             /* leerCmos, CMOS_FLOPPY_DRIVE_TYPE */
//                              /* CMOS_DISK_DATA, CFD_144MB, CFD_NO_DRIVE */
#include <so1.h\ajustsp.h>                                       /* SP0_DB */
#include <so1.h\bios.h>                                   /* leerSectorLBA */
#include <so1.h\recursos.h>                         /* crearRec, crearFich */
#include <so1.h\procesos.h>                                 /* descProceso */
#include <so1.h\blockpr.h>                        /* bloquearProcesoActual */
#include <so1.h\ejecutar.h>                                   /* kk_thread */
#include <so1.h\gm.h>                  /* k_buscarBloque, k_devolverBloque */
#include <so1.h\db.h>                            /* d_bloque_t, peticionDB */

#include <so1pub.h\ll_s_u2s.h>                 /* user2system, system2user */

rindx_t rec_db ;                         /* recurso dispositivo de bloques */

dfs_t dfs_db ;                        /* descriptor de fichero del sistema */

pindx_t pindxSrvDB ;                                 /* pindx del servidor */

peticionDB_t peticionDB ;



dobleEnlace_t ePeticionDB [ maxProcesos + 1 ] = { { 0, 0 } } ;

c2c_t bloqueadosEsperandoHacerPeticionDB = { 0, 0, 0, NULL } ;

d_bloque_t d_bloque [ ] ;

#pragma warn -par

static int far openDB ( int dfs, modoAp_t modo )
{
    return(0) ;
}

static int far releaseDB ( int dfs )
{
    return(0) ;
}

static int far opDB ( int dfs, pointer_t dir, word_t nbytes, byte_t cmd ) 
{
    int df ;
    int db ;
    modoAp_t modoAp ;
    int err ;
    dword_t pos1 ;
    dword_t pos2 ;
    word_t cont ;
    dword_t s ;
    int despl ;
    int despl_a ;
    int despl_b ;
    dword_t sectorLogico1 ;
    dword_t sectorLogico2 ;
    int despl1 ;
    int despl2 ;
    dword_t primerSector ;
    dword_t ultimoSector ;

    df = tramaProceso->BX ;
    db = descFichero[dfs].menor ;

    primerSector = d_bloque[db].primerSector ;
    ultimoSector = primerSector + d_bloque[db].numSectores - 1 ;

    pos1 = descProceso[indProcesoActual].tfa[df].pos ;
    pos2 = pos1 + nbytes - 1 ;

    sectorLogico1 = primerSector + (pos1 >> 9) ;
    if (sectorLogico1 > ultimoSector) return(0) ;
    despl1 = pos1 & (512 - 1) ;

    sectorLogico2 = primerSector + (pos2 >> 9) ;
    if (sectorLogico2 <= ultimoSector)
        despl2 = pos2 & (512 - 1) ;
    else
    {
        sectorLogico2 = ultimoSector ;
        despl2 = 511 ;
    }

    cont = 0 ;
    for ( s = sectorLogico1 ; s <= sectorLogico2 ; s++ )
    {
#if (TRUE)
        err = opSectorDB(s, db, ptrBuferSO1, cmd) ;
#else
        /* preguntar si el servidor esta bloqueado (receive) */
	
        if ((descProceso[pindxSrvDB].estado == bloqueado) &&
            (descProceso[pindxSrvDB].esperandoPor == rec_db))
		{
            peticionDB.pindxOrg = indProcesoActual ;
			peticionDB.tipo = 0 ;
            peticionDB.sectorLogico = s ;         /* mejor dir, nbytes y cmd */         
            peticionDB.db = db ;
            peticionDB.dir = ptrBuferSO1 ;
            peticionDB.cmd = cmd ;
			k_send(pindxSrvDB, &peticionDB) ;
		}				
        bloquearProcesoActual(rec_db) ;

////////////////////////////////
		
    	if ((descProceso[pindxSrvDB].estado != bloqueado) ||
            (descProceso[pindxSrvDB].esperandoPor != rec_db))
        {
            /* bloquearse en otro caso en rec_db */
            encolarPC2c(indProcesoActual, &bloqueadosEsperandoHacerPeticionDB) ;
            bloquearProcesoActual(rec_db) ;
        }
        /* aqui sabemos que el servidor esta bloqueado */
        /* hacer peticion */
        peticionDB.pindx = indProcesoActual ;
        peticionDB.sectorLogico = s ;
        peticionDB.db = db ;
        peticionDB.dir = ptrBuferSO1 ;
        peticionDB.cmd = cmd ;
        /* desbloquear al servidor */
        descProceso[pindxSrvDB].estado = preparado ;
        encolarPC2c(pindxSrvDB, &c2cPFR[PPreparados]) ;
        /* bloquearse a la espera en rec_db */
        bloquearProcesoActual(rec_db) ;
#endif
        if (err != 0) break ;
        if (s == sectorLogico1) despl_a = despl1 ; else despl_a = 0 ;
        if (s < sectorLogico2) despl_b = 511 ; else despl_b = despl2 ;
        for ( despl = despl_a ; despl <= despl_b ; despl++ )
            *dir++ = ptrBuferSO1[despl] ;
        cont = cont + despl_b - despl_a + 1 ;
    }	
//  descProceso[indProcesoActual].tfa[df].pos += cont ; /* se hace despues */
    return(cont) ;
}

static int far readDB ( int dfs, pointer_t dir, word_t nbytes )
{
	return(opDB(dfs, dir, nbytes, cmd_read_sector)) ;
}

static int far aio_readDB ( int dfs, pointer_t dir, word_t nbytes )
{
    mensaje_0_t peticion ;

    strcpy(peticion.info, "peticion a aio_readDB") ;
//  while(TRUE) ;
//  k_send(1, &peticion) ;
//  k_send(pindxSrvDB, &peticion) ;
    k_notify(pindxSrvDB) ;
    return(0) ;
}

static int far writeDB ( int dfs, pointer_t dir, word_t nbytes )
{
    return(opDB(dfs, dir, nbytes, cmd_write_sector)) ; 
}

static int far aio_writeDB ( int dfs, pointer_t dir, word_t nbytes )
{
    return(0) ;
}

static long far lseekDB ( int dfs, long pos, word_t whence )
{
	int db ;
    int df ;
	long posActual ;
    long tam ;
    
    db = descFichero[dfs].menor ;
	df = tramaProceso->BX ;
	posActual = descProceso[indProcesoActual].tfa[df].pos ;
    tam = d_bloque[db].numSectores*d_bloque[db].bytesPorSector ;
	
	switch (whence)
	{
	case SEEK_SET : posActual = pos       ; break ;
	case SEEK_CUR : // while(TRUE) ;
	                posActual += pos      ; break ;
	case SEEK_END : posActual = tam + pos ; break ;	
	default       : posActual = -1L ; 
	}
	if ((0 <= posActual) && (posActual <= tam))  
	    descProceso[indProcesoActual].tfa[df].pos = posActual ;
    return(posActual) ;
}

static int far fcntlDB ( int dfs, word_t cmd, word_t arg )
{
  return(0) ;
}

static int far ioctlDB ( int dfs, word_t request, word_t arg )
{
  return(0) ;
}

static int far eliminarDB ( pindx_t pindx )
{
  return(0) ;
}

/* ----------------------------------------------------------------------- */

#define paragrafosPorSector 512/16                          /* 32 = 0x0020 */

bool_t posibleErr9EnOpBIOS ( word_t seg )
{
    if ((seg & 0xF000) != ((seg + ((512/16)-1)) & 0xF000)) return(TRUE) ;
    else return(FALSE) ;
}

word_t segBuferSO1 = 0xFFFF ;  /* bufer para leer de forma segura sectores */
                                   /* con el BIOS (ver error 0x09 int 13h) */
                /* el sector no debe atravesar una frontera de 64 KB (DMA) */

/* Funcion para encontrar un bufer seguro de cara a leer/escribir con la   */
/* interrupcion int 13h. Hay que asegurar que el bufer no atraviesa        */
/* ninguna frontera de 64 Bytes (0x10000, 0x20000, .... , 0xF0000) ya que  */
/* en ese caso el bus de direcciones del DMA da problemas. La funcion      */
/* solicita al gestor de memoria un bloque de 1024 bytes (dos sectores).   */
/* Si los primeros 512 bytes no atraviesan ninguna frontera de 64 KB nos   */
/* quedamos con ese bufer devolviendo los segundos 512 bytes al gestor.    */
/* Si los primeros 512 bytes si atraviesan una frontera de 64 KB tomamos   */
/* como bufer los 512 bytes que comienzan en una direccion multiplo de     */
/* 512 bytes, devolviendo de los 1024 bytes solicitados los bytes          */
/* restantes tras el bufer.                                                */

word_t segBuferSeguro ( void )                        /* seguro con el DMA */
{
    word_t segB ;
    word_t segBufer ;                     /* 512 bytes = 512/16 paragrafos */
    segB = k_buscarBloque(2*(512/16)) ;                              /* GM */
    segBufer = segB ;
//  if ((segB % (512/16)) != 0)                 /* no multiplo de (512/16) */
    if ((segB & 0xF000) != ((segB + 512/16) & 0xF000))   /* atraviesa 64KB */
        segBufer = ((segB & 0xFFE0) + 512/16) ;      /* siguiente multiplo */
    k_devolverBloque(
        segBufer + 512/16,
//      segB + 2*(512/16) - (segBufer + 512/16)
        segB + 512/16 - segBufer
    ) ;                                                              /* GM */
    return(segBufer) ;
}

d_bloque_t d_bloque [ ] = {
//  nombre  uBIOS tipoU  bPS   sPP  cab   cil    primerS       numS        i
//  ======  ====  ====   ===   ===  ===  ====  ==========  ==========    ====
    "FDA" , 0x00, 0x00,  512,   18,   2,   80, 0x00000000, 0x00000000, /*  0 */
    "FDB" , 0x01, 0x00,  512,   18,   2,   80, 0x00000000, 0x00000000, /*  1 */
    "HDA" , 0x80, 0x00,  512,   63,  16, 0x00, 0x00000000, 0x00000000, /*  2 */
    "HDA1", 0x80, 0x00,  512,   63,  16, 0x00, 0x00000000, 0x00000000, /*  3 */
    "HDA2", 0x80, 0x00,  512,   63,  16, 0x00, 0x00000000, 0x00000000, /*  4 */
    "HDA3", 0x80, 0x00,  512,   63,  16, 0x00, 0x00000000, 0x00000000, /*  5 */
    "HDA4", 0x80, 0x00,  512,   63,  16, 0x00, 0x00000000, 0x00000000, /*  6 */
    "HDB" , 0x81, 0x00,  512,   63,  16, 0x00, 0x00000000, 0x00000000, /*  7 */
    "HDB1", 0x81, 0x00,  512,   63,  16, 0x00, 0x00000000, 0x00000000, /*  8 */
    "HDB2", 0x81, 0x00,  512,   63,  16, 0x00, 0x00000000, 0x00000000, /*  9 */
    "HDB3", 0x81, 0x00,  512,   63,  16, 0x00, 0x00000000, 0x00000000, /* 10 */
    "HDB4", 0x81, 0x00,  512,   63,  16, 0x00, 0x00000000, 0x00000000  /* 11 */
} ;

#define dbMax (sizeof(d_bloque)/sizeof(d_bloque_t))

#pragma warn +par

#define maxCbDB 0

static descCcb_t descCcbDB = { 0, 0, 0, maxCbDB, NULL } ;

//void mostrarDispositivosDeBloques ( d_bloque_t far * d_bloque ) ;
void mostrarDispositivosDeBloques ( void ) ;

void * servicioDB ( void * arg ) ;
//void * servicioDB ( void * arg )
void * servicioDB ( )
{
//  esperarDesinstalacion(0) ;
//  bloquearProcesoActual(rec_db) ;        /* esperamos nueva peticion */ /* no puede hacerse */
    while (TRUE) {
//      (*(pointer_t)MK_FP(0xB800, (word_t)arg))++ ;
        (*(pointer_t)MK_FP(0xB800, 0x0000))++ ;
        yield() ; /* llamada yield todavia no */
    }

#if (FALSE)
    pindx_t pindx ;
    asm cli ;                              /* inhibimos las interrupciones */
    while (TRUE)
    {
        bloquearProcesoActual(rec_db) ;        /* esperamos nueva peticion */
        if (peticionDB.pindx < 0) break ;              /* fin del servidor */
                                                   /* servimos la peticion */
        opSectorDB(peticionDB.sectorLogico,  /* permite las interrupciones */
                   peticionDB.db,
                   peticionDB.dir,
                   peticionDB.cmd
        ) ;
                       /* desbloqueamos al proceso que se acaba de atender */
        asm cli ;                          /* inhibimos las interrupciones */
        descProceso[peticionDB.pindx].estado = preparado ;
        encolarPC2c(peticionDB.pindx, &c2cPFR[PPreparados]) ;

        if (bloqueadosEsperandoHacerPeticionDB.numElem > 0)
        {
            pindx = desencolarPC2c(&bloqueadosEsperandoHacerPeticionDB) ;
            descProceso[pindx].estado = preparado ;
            encolarPC2c(pindx, &c2cPFR[PPreparados]) ;
        }
    }
#endif
    return((void *)0) ;
}

void inicDB ( void )
{
    int i, j, k ;
    byte_t tipoUnidad ;
    CSH_t CSH, CSHMax ;

//  dword_t numSectores ;
    dword_t primerSector ;
    word_t bytesPorSector ;
    byte_t sectoresPorPista ;
    byte_t cabezas ;
    word_t cilindros ;
    mbr_t far * mbr ;
    boot_t far * boot ;
    word_t sectoresPorCilindro ;
    int error ;
    descRecurso_t dR ;

//  pindxSrvDB = getpindx() ;
    pindxSrvDB = indProcesoActual ;            /* pindx del servidor de DB */

    dR.tipo = rDB ;
    strcpy(dR.nombre, "DB") ;
    dR.ccb = (ccb_t)&descCcbDB ;
    dR.ccb->arg = NULL ;
    dR.pindx = indProcesoActual ;                                       /* */
    dR.numVI = 0 ;

    dR.open      = (open_t     )MK_FP(_CS, FP_OFF(openDB)) ;
    dR.release   = (release_t  )MK_FP(_CS, FP_OFF(releaseDB)) ;
    dR.read      = (read_t     )MK_FP(_CS, FP_OFF(readDB)) ;
    dR.aio_read  = (aio_read_t )MK_FP(_CS, FP_OFF(aio_readDB)) ;
    dR.write     = (write_t    )MK_FP(_CS, FP_OFF(writeDB)) ;
    dR.aio_write = (aio_write_t)MK_FP(_CS, FP_OFF(aio_writeDB)) ;
    dR.lseek     = (lseek_t    )MK_FP(_CS, FP_OFF(lseekDB)) ;
    dR.fcntl     = (fcntl_t    )MK_FP(_CS, FP_OFF(fcntlDB)) ;
    dR.ioctl     = (ioctl_t    )MK_FP(_CS, FP_OFF(ioctlDB)) ;

    dR.eliminar  = (eliminar_t) MK_FP(_CS, FP_OFF(eliminarDB)) ;

    rec_db = crearRec(&dR) ;

    inicPC2c(
       &bloqueadosEsperandoHacerPeticionDB,
       &ePeticionDB,
       maxProcesos + 0,
       FALSE
    ) ;

    segBuferSO1 = segBuferSeguro() ;                /* para poder leer las */
                                            /* tablas de particiones (MBR) */
    mbr = (mbr_t far *)ptrBuferSO1 ;
    CSH.h = 0 ;                      /* sector logico 0 => sector fisico 1 */
    CSH.cs = 0x0001 ;                              /* C = 0, S = 1 y H = 0 */
//                                        /* CMOS_FLOPPY_DRIVE_TYPE = 0x10 */
//                                                /* CMOS_DISK_DATA = 0x12 */
    j = 0 ;            /* contador de unidades 0:FDA, 1:FDB, 2:HDA y 3:HDB */
    for ( i = 0 ; i < dbMax ; i++ ) {
        if (strlen(d_bloque[i].nombre) == 3) {       /* FDA, FDB, HDA, HDB */
            tipoUnidad = (
                leerCmos(CMOS_FLOPPY_DRIVE_TYPE + (j & 2)) >> (4*(1-(j % 2)))
            ) & 0x0F ;
            k = i + 1 ; /* indice de la primera particion (3:HDA1, 8:HDB1) */
            if (tipoUnidad != 0x00) {
                getDriveParams(d_bloque[i].unidadBIOS, (CSH_t *)&CSHMax) ;
                bytesPorSector = 512 ;
                cabezas = CSHMax.h + 1 ;
                sectoresPorPista = CSHMax.cs & 0x003F ;/* ccccccccCCssssss */
                cilindros = (((CSHMax.cs & 0x00C0) << 2) | (CSHMax.cs >> 8)) + 1 ;
//              numSectores = sectoresPorPista*cabezas*cilindros ;
                if (j & 2) {                              /* 2:HDA o 3:HDB */
                    error = leerSectorCSH((CSH_t *)&CSH,            /* MBR */
                                          d_bloque[i].unidadBIOS,
                                          (pointer_t)mbr) ;
                    if (error) tipoUnidad = 0x00 ;
                }
                d_bloque[i].tipoUnidad = tipoUnidad ;
                d_bloque[i].bytesPorSector = bytesPorSector ;
                d_bloque[i].cabezas = cabezas ;
                d_bloque[i].sectoresPorPista = sectoresPorPista ;
                d_bloque[i].cilindros = cilindros ;
                d_bloque[i].primerSector = 0x00000000L ;
                d_bloque[i].numSectores =
                    ((dword_t)cilindros)
                        *((dword_t)sectoresPorPista)
                        *((dword_t)cabezas) ;
            }
            j++ ;
        }
        else if ((tipoUnidad != 0x00) &&
                 (mbr->signatura[0] == 0x55) &&
                 (mbr->signatura[1] == 0xAA) &&
                 (mbr->descParticion[i-k].tipo != 0x00)) {    /* particion */
            d_bloque[i].tipoUnidad = tipoUnidad ;
            d_bloque[i].bytesPorSector = bytesPorSector ;
            d_bloque[i].cabezas = cabezas ;
            d_bloque[i].sectoresPorPista = sectoresPorPista ;
            d_bloque[i].cilindros = 0 ;
            d_bloque[i].primerSector = mbr->descParticion[i-k].primerSector ;
            d_bloque[i].numSectores = mbr->descParticion[i-k].sectores ;
        }
        if (d_bloque[i].tipoUnidad != 0x00)
            dfs_db = crearFich(d_bloque[i].nombre, rec_db, i, fedBloques) ;
    }

//  printStrBIOS("\n &d_bloque = ") ;
//  printPtrBIOS(&d_bloque) ;
//  /* mostrarDispositivosDeBloques falla con ese parametro d_bloque ???     */
//  mostrarDispositivosDeBloques((d_bloque_t far *)&d_bloque) ;
    mostrarDispositivosDeBloques() ;
}

int opSectorDB ( dword_t sectorLogico, int db, pointer_t dir, byte_t cmd )
{
    int err ;
    bool_t posibleErr9 ;
    pointer_t dirAux ;
    byte_t unidadBIOS ;
    word_t sector ;
    word_t cabeza ;
    word_t cilindro ;
    word_t sectoresPorPista ;
    word_t cabezas ;
    CSH_t CSH ;

    if (db >= dbMax) return(-1) ;
    unidadBIOS = d_bloque[db].unidadBIOS ;
    posibleErr9 = posibleErr9EnOpBIOS(FP_SEG(dir)) ;
    dirAux = dir ;
    if (posibleErr9)
    {
        if (cmd == cmd_write_sector)
            memcpy(dirAux, dir, 512) ;
        dirAux = ptrBuferSO1 ;
    }
    if (unidadBIOS < 0x80)
    {
        sectoresPorPista = d_bloque[db].sectoresPorPista ;
        cabezas = d_bloque[db].cabezas ;
        sector = (word_t)(sectorLogico % sectoresPorPista) + 1 ;
        cabeza = (word_t)((sectorLogico / sectoresPorPista) % cabezas) ;
        cilindro = (word_t)(sectorLogico / (sectoresPorPista * cabezas)) ;
        CSH.h = cabeza ;
        CSH.cs = ((cilindro & 0x00FF) << 8) | ((cilindro & 0x0300) >> 2) | (sector & 0x003F) ;
        err = opSectorCSH ((CSH_t *)&CSH, unidadBIOS, dirAux, cmd ) ;
    }
    else err = opSectorLBA(sectorLogico, unidadBIOS, dirAux, cmd) ;
	
    if ((!err) && (posibleErr9) && (cmd == cmd_read_sector))
        memcpy(dir, dirAux, 512) ;
    return(err) ;
}

//void mostrarDispositivosDeBloques ( d_bloque_t far * d_bloque )
void mostrarDispositivosDeBloques ( void )
{
    int i ;
    int numUnidades = 0 ;

//  printStrBIOS("\n d_bloque = ") ;
//  printPtrBIOS(d_bloque) ;

#if (TRUE)
    numUnidades = 0 ;     /* mostrar nombre de los dispositivos encontrados */
    for ( i = 0 ; i < dbMax ; i++ ) {
        if (d_bloque[i].tipoUnidad != 0x00) {
            numUnidades++ ;
            printStrBIOS(d_bloque[i].nombre) ;
            printCarBIOS(' ') ;
        }
    }
    if (numUnidades == 0)
        k_devolverBloque(segBuferSO1, 512/16) ;
#endif

    for ( i = 0 ; i < dbMax ; i++ ) {            /* mostrar tabla d_bloque */
        if (d_bloque[i].tipoUnidad != 0x00) {
            dword_t daux ;
            printStrBIOS("\n ") ;
            printStrBIOS(d_bloque[i].nombre) ;
            if (d_bloque[i].nombre[3] == '\0') printCarBIOS(' ') ;
            printStrBIOS(" 0x") ;
            printHexBIOS(d_bloque[i].unidadBIOS, 2) ;
            printStrBIOS(" 0x") ;
            printHexBIOS(d_bloque[i].tipoUnidad, 2) ;
            printStrBIOS(" ") ;
            printDecBIOS(d_bloque[i].bytesPorSector, 3) ;
            printStrBIOS(" ") ;
            printDecBIOS(d_bloque[i].sectoresPorPista, 2) ;
            printStrBIOS(" ") ;
            printDecBIOS(d_bloque[i].cabezas, 2) ;
            printStrBIOS(" ") ;
            printDecBIOS(d_bloque[i].cilindros, 4) ;
            printStrBIOS(" 0x") ;
            printLHexBIOS(d_bloque[i].primerSector, 8) ;
            printStrBIOS(" 0x") ;
            printLHexBIOS(d_bloque[i].numSectores, 8) ;
            printStrBIOS("  ") ;
            daux = d_bloque[i].numSectores ;               /* sectores */
            printLIntBIOS(daux/(2*1024), 3) ;                 /* bytes */
            printStrBIOS(",") ;
            printLIntBIOS(((10*daux)/(2*1024))%10, 1) ;
            printLIntBIOS(((100*daux)/(2*1024))%10, 1) ;
            printStrBIOS(" MB ") ;

        }
    }
}

void * DB ( void * arg ) ;
//void * DB ( void * arg )
void * DB ( )
{
    peticionDB_t peticion ;
    mensaje_0_t respuesta ;

    inicDB() ;

    for ( ; ; )
    {
//      user2system() ;
//      bloquearProcesoActual(rec_db) ;
        receive(ANY, &peticion) ;
	
        asm sti ;		
        /* procesar peticion */
		asm cli ;
		
		/* mirar si hay mas peticiones pendientes */
		
        strcpy(respuesta.info, "respuesta de DB") ;
        send(peticion.pindxOrg, &respuesta) ;
    }
    return(0) ;
}