/* ----------------------------------------------------------------------- */
/*                                   db.c                                  */
/* ----------------------------------------------------------------------- */
/*                         dispositivos de bloques                         */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\comundrv.h>       /* segDatos, ptrIndProcesoActual, ... */
#include <so1pub.h\bios_0.h>
#include <so1pub.h\msdos.h>
#include <so1pub.h\strings.h>
#include <so1.h\ajustsp.h>
#include <so1.h\ajustes.h>
#include <so1.h\recursos.h>

#include <so1pub.h\def_sf.h>

//#include <so1.h\procesos.h>

#include <so1.h\gm.h>                  /* k_buscarBloque, k_devolverBloque */

#include <so1.h\db.h>

#include <so1.h\bios.h>                                   /* leerSectorLBA */
#include <so1pub.h\copia.h>                                       /* copia */
#include <so1pub.h\cmos.h>             /* leerCmos, CMOS_FLOPPY_DRIVE_TYPE */
//                              /* CMOS_DISK_DATA, CFD_144MB, CFD_NO_DRIVE */

rindx_t rec_db ;

dfs_t dfs_db ;

#pragma warn -par

static int far openDB ( int dfs, modoAp_t modo ) 
{
    return(0) ;
}

static int far releaseDB ( int dfs ) 
{
    return(0) ;
}

static int far readDB ( int dfs, pointer_t dir, word_t nbytes ) 
{
    word_t DS_DB = *((word_t far *)pointer(_CS, (word_t)segDatos)) ;
    pindx_t indProcesoActual ;
    modoAp_t modoAp ;
    word_t nbytes0 ;
    int df ;
    int db ;
    int i = 0 ;

    asm push ds
    asm mov ds,DS_DB	
	
    indProcesoActual = *ptrIndProcesoActual ;
    df = (*ptrTramaProceso)->BX ;
    db = ptrDescFichero[dfs].menor ;
	
#if (FALSE)	
	
d_bloque[db].unidadBIOS
	
    modoAp = ptrDescProceso[indProcesoActual].tfa[df].modoAp ;

    nbytes0 = nbytes ;
    if (nbytes <= teclado->ncar)
    {
        while (nbytes > 0)
        {
//    car = sacar(teclado) ;
//    car = tablaSP[sacar(teclado)] ;
            car = tablaDeConversion[sacar(teclado)] ;
            if ((car == '\r') && (modoAp & O_TEXT)) car = '\n' ;
            dir[i++] = car ;
            nbytes-- ;
        }
        asm pop ds                /* 2013-07-09: faltaba esto (sino CRASH) */
        return(nbytes0) ;
    }
    else
    {
        while (teclado->ncar > 0)
        {
//    car = sacar(teclado) ;
//    car = tablaSP[sacar(teclado)] ;
            car = tablaDeConversion[sacar(teclado)] ;
            if ((car == '\r') && (modoAp & O_TEXT)) car = '\n' ;
            dir[i++] = car ;
            nbytes-- ;
        }
        nbytesProceso[indProcesoActual] = nbytes ;
        dirProceso[indProcesoActual] = (pointer_t)&dir[i] ;
        encolarPC2c(indProcesoActual, &(teclado->bloqueados)) ;
        bloquearProcesoActual(rec_consola) ;      /* no se retorna de aqui */
    }
#endif 
    asm pop ds
    return(-1) ;
}

static int far aio_readDB ( int dfs, pointer_t dir, word_t nbytes ) 
{
#if (0) 
    word_t far * ptrWord = (word_t far *)dir ;
    word_t segmento ;
    word_t tam ;
    switch (nbytes) 
	{
        case 2 : *ptrWord = tamBloqueMax ; return(nbytes) ;
        case 4 : segmento = ptrWord[0] ;
                 tam = ptrWord[1] ;
                 if (k_devolverBloque(segmento, tam))
                     return(nbytes) ;
    }
#endif  
    return(-1) ;
}

static int far writeDB ( int dfs, pointer_t dir, word_t nbytes ) 
{
  return(0) ;
}

static int far aio_writeDB ( int dfs, pointer_t dir, word_t nbytes ) 
{
  return(writeDB(dfs, dir, nbytes)) ;
}

static long int far lseekDB ( int dfs, long int pos, word_t whence ) 
{
  return((long int)-1) ;
}

static int far fcntlDB ( int dfs, word_t cmd, word_t arg ) 
{
  return(0) ;
}

static int far ioctlDB ( int dfs, word_t request, word_t arg ) 
{
  return(0) ;
}

/* ----------------------------------------------------------------------- */

#define paragrafosPorSector 512/16                                   /* 32 */

bool_t posibleErr9EnOpBIOS ( word_t seg ) 
{
	if ((seg & 0xF000) != ((seg + (512/16)) & 0xF000)) return(TRUE) ;
	else return(FALSE) ;
}

word_t segBuferSO1 ;           /* bufer para leer de forma segura sectores */
                                    /* con el BIOS (ver error 0x09 int 13) */
word_t segBuferSeguro ( void )                        /* seguro con el DMA */
{
    word_t segB ;
    word_t segBufer ;                     /* 512 bytes = 512/16 paragrafos */
    segB = k_buscarBloque(512/16) ;        /* primer segmento tras la pila */
    segBufer = segB ;
    if ((segB % (512/16)) != 0)                    /* multiplo de (512/16) */
    {
        segBufer = ((segB & 0xFFE0) + 512/16) ;
        k_devolverBloque(segB, 512/16) ;                             /* GM */
        k_buscarBloque(segBufer + 512/16 - segB) ;                   /* GM */
    }
    return(segBufer) ;
}

typedef struct {
    char nombre [ 5 ] ;
	byte_t unidadBIOS ;
	byte_t tipoUnidad ;
    word_t bytesPorSector ;
    byte_t sectoresPorPista ;
    byte_t cabezas ;
	byte_t cilindros ;
	dword_t primerSector ;
	dword_t numSectores ;
} d_bloque_t ;

d_bloque_t d_bloque [ ] = {
    "FDA",  0x00, 0x00,  512,   18,   2,   80, 0x00000000, 0x00000000, /*  0 */
    "FDB",  0x01, 0x00,  512,   18,   2,   80, 0x00000000, 0x00000000, /*  1 */  
    "HDA",  0x80, 0x00,  512,   63,  16, 0x00, 0x00000000, 0x00000000, /*  2 */
    "HDA1", 0x80, 0x00,  512,   63,  16, 0x00, 0x00000000, 0x00000000, /*  3 */
    "HDA2", 0x80, 0x00,  512,   63,  16, 0x00, 0x00000000, 0x00000000, /*  4 */
    "HDA3", 0x80, 0x00,  512,   63,  16, 0x00, 0x00000000, 0x00000000, /*  5 */ 
    "HDA4", 0x80, 0x00,  512,   63,  16, 0x00, 0x00000000, 0x00000000, /*  6 */
    "HDB",  0x81, 0x00,  512,   63,  16, 0x00, 0x00000000, 0x00000000, /*  7 */
    "HDB1", 0x81, 0x00,  512,   63,  16, 0x00, 0x00000000, 0x00000000, /*  8 */
    "HDB2", 0x81, 0x00,  512,   63,  16, 0x00, 0x00000000, 0x00000000, /*  9 */
    "HDB3", 0x81, 0x00,  512,   63,  16, 0x00, 0x00000000, 0x00000000, /* 10 */
    "HDB4",	0x81, 0x00,  512,   63,  16, 0x00, 0x00000000, 0x00000000  /* 11 */
} ;

#define dbMax (sizeof(d_bloque)/sizeof(d_bloque_t)) 

#pragma warn +par

#define maxCbDB 0

static descCcb_t descCcbDB = { 0, 0, 0, maxCbDB, NULL } ;

void inicDB ( void ) {            

    int i, j, k ;
	byte_t tipoUnidad ;
	byte_t cilindros ;
    mbr_t far * mbr ;
    boot_t far * boot ;
	word_t sectoresPorCilindro ;
    CSH_t CSH ;
	int error ;
	
    descRecurso_t dR ;

    dR.tipo = rDB ;
    copiarStr("DB", dR.nombre) ;
    dR.ccb = (ccb_t)&descCcbDB ;
//  dR.pindx = indProcesoActual ;
    dR.pindx = *ptrIndProcesoActual ;
    dR.numVI = 0 ;

    dR.open      = (open_t     )pointer(_CS, (word_t)openDB) ;
    dR.release   = (release_t  )pointer(_CS, (word_t)releaseDB) ;
    dR.read      = (read_t     )pointer(_CS, (word_t)readDB) ;
    dR.aio_read  = (aio_read_t )pointer(_CS, (word_t)aio_readDB) ;
    dR.write     = (write_t    )pointer(_CS, (word_t)writeDB) ;
    dR.aio_write = (aio_write_t)pointer(_CS, (word_t)aio_writeDB) ;
    dR.lseek     = (lseek_t    )pointer(_CS, (word_t)lseekDB) ;
    dR.fcntl     = (fcntl_t    )pointer(_CS, (word_t)fcntlDB) ;
    dR.ioctl     = (ioctl_t    )pointer(_CS, (word_t)ioctlDB) ;

    rec_db = crearRec(&dR) ;
	
	segBuferSO1 = segBuferSeguro() ;
	
//                                    	  /* CMOS_FLOPPY_DRIVE_TYPE = 0x10 */
//                                                /* CMOS_DISK_DATA = 0x12 */
    j = 0 ;   
    cilindros = 0 ;	
    mbr = (mbr_t far *)ptrBuferSO1 ;
    CSH.h = 0 ;                      /* sector logico 0 => sector fisico 1 */
    CSH.cs = 0x0001 ;                              /* C = 0, S = 1 y H = 0 */
	for ( i = 0 ; i < 12 ; i++ ) {
		if (d_bloque[i].nombre[3] == '\0') {
			tipoUnidad = (
			    leerCmos(CMOS_FLOPPY_DRIVE_TYPE + (j & 2)) >> (4*(1-(j % 2)))
			) & 0x0F ;
			k = i + 1 ;
			if ((j < 2) && (tipoUnidad != 0x00)) {
				error = leerSectorCSH((CSH_t *)&CSH, d_bloque[i].unidadBIOS, (pointer_t)boot) ; 
				if (error) tipoUnidad = 0x00 ;
				else if ((boot->signatura[0] != 0x55) || (boot->signatura[1] != 0xAA)) 
					tipoUnidad = 0x00 ;
				else if ((boot->instJMP[0] != 0xEB) || (boot->instJMP[1] < 0x3C) || (boot->instNOP != 0x90) ||
                         (boot->signaturaExt != 0x29))    
					tipoUnidad = 0x00 ;
                else {
					sectoresPorCilindro = 
					    (boot->BPB.cabezas)*(boot->BPB.sectoresPorPista) ;
					if (sectoresPorCilindro != 0) { 	
         			    d_bloque[i].bytesPorSector = boot->BPB.bytesPorSector ;
         			    d_bloque[i].sectoresPorPista = boot->BPB.sectoresPorPista ;
         			    d_bloque[i].cabezas = boot->BPB.cabezas ;
         			    cilindros = 
					        boot->BPB.sectores16/sectoresPorCilindro ;
         			    d_bloque[i].cilindros = cilindros ;
         			    d_bloque[i].numSectores = boot->BPB.sectores16 ;
					}  
                    else tipoUnidad = 0x00 ;					
                }
			}
			else if ((j & 2) && (tipoUnidad != 0x00)) {      /* disco duro */
	            if (tipoUnidad == 0x0F) 
					tipoUnidad = leerCmos(CMOS_DISK_DRIVE1_TYPE + (j % 2)) ;
				cilindros = leerCmos(CMOS_DISK_DRIVE1_CYL+ (i%2)) ;
				error = leerSectorCSH((CSH_t *)&CSH, d_bloque[i].unidadBIOS, (pointer_t)mbr) ; 
				if (error) tipoUnidad = 0x00 ;
				else if ((mbr->signatura[0] != 0x55) || (mbr->signatura[1] != 0xAA)) 
					tipoUnidad = 0x00 ;
				if (tipoUnidad != 0x00) 
					d_bloque[i].numSectores = 
				        ((dword_t)cilindros)
						    *((dword_t)d_bloque[i].sectoresPorPista)
						    *((dword_t)d_bloque[i].cabezas) ; 
			}
			j++ ;
        }
		else if (mbr->descParticion[i-k].tipo != 0x00) { 
			d_bloque[i].primerSector = mbr->descParticion[i-k].primerSector ;
			d_bloque[i].numSectores = mbr->descParticion[i-k].sectores ;			
		}
		else continue ;
		d_bloque[i].tipoUnidad = tipoUnidad ;
		d_bloque[i].cilindros = cilindros ;
        if (d_bloque[i].tipoUnidad != 0x00) 
			dfs_db = crearFich(d_bloque[i].nombre, rec_db, i, fedBloques) ;
	}
	
#if (TRUE)
	for ( i = 0 ; i < 12 ; i++ ) {
		if (d_bloque[i].tipoUnidad != 0x00) {
	        printStrBIOS(d_bloque[i].nombre) ;
            printCarBIOS(' ') ; 
		}
    }
#endif	

#if (TRUE) 	
	for ( i = 0 ; i < 12 ; i++ ) {
		if (d_bloque[i].tipoUnidad != 0x00) {
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
	        printDecBIOS(d_bloque[i].cilindros, 2) ;
	        printStrBIOS(" 0x") ;
	        printHexBIOS(d_bloque[i].primerSector, 8) ;
	        printStrBIOS(" 0x") ;
	        printHexBIOS(d_bloque[i].numSectores, 8) ;
		}
	}
#endif
}

#if (TRUE)

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
	posibleErr9 = posibleErr9EnOpBIOS(seg(dir)) ;
	dirAux = dir ;
	if (posibleErr9) { 
	    dirAux = ptrBuferSO1 ;
		if (cmd == cmd_write_sector) 
			copia(dir, dirAux, 512) ;
	}
	if (unidadBIOS < 0x80) {
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
		copia(dirAux, dir, 512) ;
    return(err) ;
}

#endif