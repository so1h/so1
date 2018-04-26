/* ----------------------------------------------------------------------- */
/*                                 units.c                                 */
/* ----------------------------------------------------------------------- */
/*                                                                         */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\def_sf.h>  /* maxUL, maxUF, descUnidad(Fisica/Logica)_t */
#include <so1pub.h\ll_s_exc.h>                  /* open, lseek, read close */
#include <so1pub.h\bios_0.h>                 /* print(Car/Dec/Int/Str)BIOS */
#include <so1pub.h\memory.h>                                     /* memcpy */
#include <so1pub.h\strings.h>                /* strlcpy, strncmp, strncmpu */
#include <so1.h\bios.h>                                 /* cmd_read_sector */
#include <so1.h\db.h>                                   /* dbMax, d_bloque */

/* ----------------- descriptores de unidades logicas -------------------- */

int unidadLogicaArranque = -1 ;        

int unidadFisicaArranque = -1 ;       

descUnidadLogica_t descUnidadLogica [ maxUL ] ;

descUnidadFisica_t descUnidadFisica [ maxUF ] ;

#define MOSTRARUNIDADES TRUE
//#define MOSTRARUNIDADES FALSE

void inicUnits ( void )            /* inicializa las unidades con d_bloque */
{
    int ufindx ;
    int ulindx ;
	int df ;
	int db ;
	int i, k ; 
    int contUL ;
    char letraUnidad ;
	descUnidadFisica_t * ptrDUFisica ;
	descUnidadLogica_t * ptrDULogica ;

#if (MOSTRARUNIDADES)	
    printStrBIOS("\n") ;	
#endif	

    for ( ufindx = 0 ; ufindx < maxUF ; ufindx++ )     /* descUnidadFisica */
    {
		ptrDUFisica = (descUnidadFisica_t *)&descUnidadFisica[ufindx] ;
		db = (ufindx % 2) + (ufindx / 2)*(2 + 4*(ufindx % 2)) ; 

        ptrDUFisica->contUL = 0 ;                                /* contUL */
		if (d_bloque[db].tipoUnidad == 0x00)
        {		
            ptrDUFisica->db = -1 ;
		    ptrDUFisica->unidadBIOS = -1 ;
			for ( i = 0 ; i < maxParticiones ; i++ )
                ptrDUFisica->descParticion[i].tipo = 0x00 ;  /* dParticion */
		}
        else 
        {
            ptrDUFisica->db = db ;                                   /* db */
		    ptrDUFisica->unidadBIOS = d_bloque[db].unidadBIOS ;   /* uBIOS */
			if (ufindx >= 2)                             /* no es disquete */
            {
//				df = open(d_bloque[db].nombre, O_RDONLY) ;
//				lseek(df, tamMBR-4*sizeof(descParticion_t)-2, SEEK_SET) ;
//				read(df, &ptrDUFisica->descParticion, 4*sizeof(descParticion_t)) ;
//				close(df) ;
            }
#if (MOSTRARUNIDADES)		
	        printStrBIOS("\n ufindx = ") ;   
     		printIntBIOS(ufindx, 1) ;		    		
	        printStrBIOS(" db = ") ; 
    		printIntBIOS(ptrDUFisica->db, 1) ;		
	        printStrBIOS(" (") ; 
    		printStrBIOS(d_bloque[ptrDUFisica->db].nombre) ;				
	        printStrBIOS(") uBIOS = ") ; 
	        printHexBIOS(ptrDUFisica->unidadBIOS, 2) ; 
			if (ufindx >= 2) 
			{
	            printStrBIOS(" contUL = ") ; 
	            printIntBIOS(ptrDUFisica->contUL, 1) ; 
			}
#endif 			
        }			
		for ( i = 0 ; i < maxParticiones ; i++ )
            ptrDUFisica->ulindx[i] = -1 ;                        /* ulindx */
    }
	
#if (MOSTRARUNIDADES)	
    printStrBIOS("\n") ;	
#endif	
	
	db = 0 ;
	letraUnidad = 'C' ;
    for ( ulindx = 0 ; ulindx < maxUL ; ulindx++ )     /* descUnidadLogica */
    {
		ptrDULogica = (descUnidadLogica_t *)&descUnidadLogica[ulindx] ;
		if (d_bloque[db].tipoUnidad == 0x00)
		{
    		ptrDULogica->db = -1 ;
            ptrDULogica->ufindx = -1 ;
            ptrDULogica->indParticion = -1 ;
            ptrDULogica->letra = '$' ; 
		}
		else 
		{
    		ptrDULogica->db = db ;
            ptrDULogica->ufindx = (ulindx < 2) ? ulindx : 2 + (ulindx - 2)/4 ;
            ptrDULogica->indParticion = (ulindx < 6) ? (ulindx - 2) : (ulindx - 6) ;
            ptrDULogica->letra = (ulindx < 2) ? 'A' + ulindx : letraUnidad++ ;
//			df = open(d_bloque[db].nombre, O_RDONLY) ;
//			lseek(df, bloque[db].primerSector*512, SEEK_SET) ;
//			read(df, (pointer_t)&ptrDULogica->BPB, sizeof(BPB_t)) ;
//			close(df) ;
			if (ulindx >= 2) 
			{	
		        ufindx = ptrDULogica->ufindx ;
             	ptrDUFisica = (descUnidadFisica_t *)&descUnidadFisica[ufindx] ;
		        contUL = ptrDUFisica->contUL ;
	    		ptrDUFisica->ulindx[contUL++] = ulindx ;
				ptrDUFisica->contUL = contUL ;
			}
#if (MOSTRARUNIDADES)		
	        printStrBIOS("\n ulindx = ") ;   
     		printIntBIOS(ulindx, 1) ;		    		
	        printStrBIOS(" db = ") ; 
    		printIntBIOS(ptrDULogica->db, 1) ;		
	        printStrBIOS(" ufindx = ") ; 
	        printIntBIOS(ptrDULogica->ufindx, 1) ; 
			if (ulindx >= 2) 
			{
	            printStrBIOS(" indPart = ") ; 
	            printIntBIOS(ptrDULogica->indParticion, 1) ; 		
            }				
	        printStrBIOS(" letra = \'") ; 
	        printCarBIOS(ptrDULogica->letra) ; 			
	        printStrBIOS("\'") ; 
	        printStrBIOS(" (") ; 
    		printStrBIOS(d_bloque[db].nombre) ;				
	        printStrBIOS(")") ;
#endif 						
		}
		db++ ; 
		if ((db == 2) || (db == 7)) db++ ;
    }	
	
#if (MOSTRARUNIDADES)	
    printStrBIOS("\n") ;	
    for ( ufindx = 0 ; ufindx < maxUF ; ufindx++ )     /* descUnidadFisica */
    {
		ptrDUFisica = (descUnidadFisica_t *)&descUnidadFisica[ufindx] ;
		if (ptrDUFisica->db >= 0) 
		{
	        printStrBIOS("\n ufindx = ") ;   
     		printIntBIOS(ufindx, 1) ;		    		
	        printStrBIOS(" db = ") ; 
    		printIntBIOS(ptrDUFisica->db, 1) ;		
	        printStrBIOS(" (") ; 
    		printStrBIOS(d_bloque[ptrDUFisica->db].nombre) ;				
	        printStrBIOS(") uBIOS = ") ; 
	        printHexBIOS(ptrDUFisica->unidadBIOS, 2) ; 
			if (ufindx >= 2) 
			{
	            printStrBIOS(" contUL = ") ; 
	            printIntBIOS(ptrDUFisica->contUL, 1) ; 
				for ( k = 0 ; k < ptrDUFisica->contUL ; k++ )
				{
					printCarBIOS(' ') ;
     	            printIntBIOS(ptrDUFisica->ulindx[k], 1) ; 
				}	

			}
		}
    }
    printStrBIOS("\n") ;	
#endif		
}

/* comprobarUnits comprueba signaturas, MBRs, BPBs y tipos de particion    */
/* inicializando unidadFisicaArranque y unidadLogicaArranque               */    

int comprobarUnits ( byte_t unidadBIOS )
{
    int ufindx ; 
    int ulindx ;
	int df ;
	int db ;
	int i, j, k ; 
	int err ;
    int contUL ;
	byte_t tipo ;
    char letraUnidad ;
    mbr_t far * mbr ;
    boot_t far * boot ;
	descUnidadFisica_t * ptrDUFisica ;
	descUnidadLogica_t * ptrDULogica ;	
	descParticion_t * ptrDParticion ;

    mbr = (mbr_t far *)ptrBuferSO1 ;
    boot = (boot_t far *)ptrBuferSO1 ;
	
    for ( ufindx = 0 ; ufindx < maxUF ; ufindx++ )     /* descUnidadFisica */
    {
   		ptrDUFisica = (descUnidadFisica_t *)&descUnidadFisica[ufindx] ;
		if (ptrDUFisica->unidadBIOS == unidadBIOS) 
		{
            unidadFisicaArranque = ufindx ;	       /* unidadFisicaArranque */
			if (ufindx < 2)                                    /* disquete */
			    unidadLogicaArranque = ufindx ;
        }			
		db = ptrDUFisica->db ; 
		if ((ufindx >= 2) && (db >= 0))                      /* disco duro */
		{                               /* leemos el sector 0 de la unidad */
//			df = open(d_bloque[db].nombre, O_RDONLY) ;
//			lseek(df, 0, SEEK_SET) ;
//			read(df, (pointer_t)mbr, 512) ;                         /* MBR */
//			close(df) ;
            err = opSectorDB(0, db, ptrBuferSO1, cmd_read_sector) ;

            if ((mbr->signatura[0] != 0x55) || (mbr->signatura[1] != 0xAA))
            {
                printStrBIOS("\n error: MBR sin signatura (0x55, 0xAA) en ") ;
                printStrBIOS(d_bloque[db].nombre) ;
                printLnBIOS() ;
				leerTeclaBIOS() ;
                rebootBIOS() ; 
            }   		
			else 
			{
				ptrDParticion = (descParticion_t *)&ptrDUFisica->descParticion ;
				memcpy(
			        ptrDParticion, 
					mbr->descParticion,
					maxParticiones*sizeof(descParticion_t) 
                ) ;			
#if (MOSTRARUNIDADES)		
	            printStrBIOS("\n ufindx = ") ;   
         		printIntBIOS(ufindx, 1) ;		    		
	            printStrBIOS(" db = ") ; 
        		printIntBIOS(ptrDUFisica->db, 1) ;		
	            printStrBIOS(" (") ; 
        		printStrBIOS(d_bloque[ptrDUFisica->db].nombre) ;				
	            printStrBIOS(") uBIOS = ") ; 
	            printHexBIOS(ptrDUFisica->unidadBIOS, 2) ; 
	            printStrBIOS(" contUL = ") ; 
	            printIntBIOS(ptrDUFisica->contUL, 1) ; 
				for ( k = 0 ; k < ptrDUFisica->contUL ; k++ )
				{
					printCarBIOS(' ') ;
     	            printIntBIOS(ptrDUFisica->ulindx[k], 1) ; 
				}	
#endif 			
                j = 0 ;
                for ( i = 0 ; i < maxParticiones ; i++ )
                {
                    tipo = ptrDParticion[i].tipo ;
                    if (tipo != 0x00) 
					{   
						if (ptrDParticion[i].activa == 0x80)
                            break ;
						j++ ;
					}
                }
                if (i >= maxParticiones)
                {
                    printStrBIOS("\n aviso: sin particion activa en ") ;
                    printStrBIOS(d_bloque[db].nombre) ;
                    printLnBIOS() ;
			        leerTeclaBIOS() ;
//                  rebootBIOS() ; 
                }
				else if (ptrDUFisica->unidadBIOS == unidadBIOS) 
				{	
			        ulindx = ptrDUFisica->ulindx[j] ;
                    unidadLogicaArranque = ulindx ;				
                }
				
                if ((tipo != 0x01) && (tipo != 0x04)) 
                {
                    printStrBIOS("\n aviso: particion de tipo no soportado 0x") ;
                    printHexBIOS(tipo, 2) ;
                    printStrBIOS(" presente en ") ;
                    printStrBIOS(d_bloque[db].nombre) ;
            		if (ptrDUFisica->unidadBIOS == unidadBIOS) 
					{
                        printStrBIOS("\n error: particion de arranque de tipo no soportado 0x") ;
                        printHexBIOS(tipo, 2) ;
						return(-1) ;
					}
                    leerTeclaBIOS() ;
//                  rebootBIOS() ;
                }
            }				
		}
	}
	
#if (MOSTRARUNIDADES)		
	printLnBIOS() ;
    printStrBIOS("\n unidad fisica de arranque = ") ;   
    printIntBIOS(unidadFisicaArranque, 1) ;
    printStrBIOS("\n unidad logica de arranque = ") ;   
    printIntBIOS(unidadLogicaArranque, 1) ;
#endif
    if (unidadFisicaArranque == -1)
	{
        printStrBIOS("\n aviso: no se encuentra la unidad fisica de arranque ") ;   
        leerTeclaBIOS() ;	
		return(-1) ;
	}
    if (unidadLogicaArranque == -1)
	{
        printStrBIOS("\n aviso: no se encuentra la unidad logica de arranque ") ;  
        leerTeclaBIOS() ;	
		return(-1) ;
    }		
		
leerTeclaBIOS() ;	
return(0) ;	
	
    for ( ulindx = 0 ; ulindx < maxUL ; ulindx++ )     /* descUnidadLogica */
    {
		ptrDULogica = (descUnidadLogica_t *)&descUnidadLogica[ulindx] ;
		db = ptrDULogica->db ;
		if (db > 0) 
		{
//			df = open(d_bloque[db].nombre, O_RDONLY) ;
//			lseek(df, (db < 2) ? 0 : d_bloque[db].primerSector*512, SEEK_SET) ;
//			read(df, (pointer_t)boot, 512) ;                   /* PBR/BOOT */
//			close(df) ;
            err = opSectorDB(d_bloque[db].primerSector, db, ptrBuferSO1, cmd_read_sector) ;
            if ((boot->signatura[0] != 0x55) || (boot->signatura[1] != 0xAA))
            {
                printStrBIOS("\n aviso: PBR sin signatura (0x55, 0xAA) en ") ;
                printStrBIOS(d_bloque[db].nombre) ;
                printLnBIOS() ;
				ptrDULogica->db = -1 ;		
				leerTeclaBIOS() ;
                rebootBIOS() ;
            }
			else if ((boot->instJMP[0]   != 0xEB) || 
			         (boot->instJMP[1]   <  0x3C) || 
					 (boot->instNOP      != 0x90) ||
                     (boot->signaturaExt != 0x29))       
            {
                printStrBIOS("\n error: PBR con BPB incorrecto en ") ;
                printStrBIOS(d_bloque[db].nombre) ;
                printLnBIOS() ;
				ptrDULogica->db = -1 ;		
				leerTeclaBIOS() ;
                rebootBIOS() ;
			}
            else if ((strncmp(boot->SF, "FAT12   ", 8) && (tipo == 0x01)) ||
                     (strncmp(boot->SF, "FAT16   ", 8) && (tipo == 0x04)))
            {
                printStrBIOS("\n el tipo de sistema de ficheros no corresponde a \"") ;
                for ( i = 0 ; i < 8 ; i++ ) printCarBIOS(boot->SF[i]) ;
                printStrBIOS("\" en ") ;
                printStrBIOS(d_bloque[db].nombre) ;
                printLnBIOS() ;
				ptrDULogica->db = -1 ;		
                leerTeclaBIOS() ;
                rebootBIOS() ;
            }
			else {
				memcpy(&ptrDULogica->BPB, &boot->BPB, sizeof(BPB_t)) ;
				ptrDULogica->letra = (db < 2) ? 'A' + db : letraUnidad++ ;
			}
		}		
    }
	
leerTeclaBIOS() ;	
	return(0) ;
}