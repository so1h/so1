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
		for ( i = 0 ; i < maxParticiones ; i++ )
            ptrDUFisica->ulindx[i] = -1 ;                        /* ulindx */
		if (d_bloque[db].tipoUnidad == 0x00)
            ptrDUFisica->db = -1 ;
        else 
        {				
            ptrDUFisica->db = db ;                                   /* db */
            ptrDUFisica->unidadBIOS = d_bloque[db].unidadBIOS ;   /* uBIOS */
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
    }
	
#if (MOSTRARUNIDADES)	
    printLnBIOS() ;	
#endif	
	
	db = 0 ;
	letraUnidad = 'C' ;
    for ( ulindx = 0 ; ulindx < maxUL ; ulindx++ )     /* descUnidadLogica */
    {
		ptrDULogica = (descUnidadLogica_t *)&descUnidadLogica[ulindx] ;
		if (d_bloque[db].tipoUnidad == 0x00)
    		ptrDULogica->db = -1 ;
		else 
		{
    		ptrDULogica->db = db ;
            ptrDULogica->ufindx = (ulindx < 2) ? ulindx : 2 + (ulindx - 2)/4 ;
            ptrDULogica->indParticion = (ulindx < 6) ? (ulindx - 2) : (ulindx - 6) ;
            ptrDULogica->letra = (ulindx < 2) ? 'A' + ulindx : letraUnidad++ ;
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
				printStrBIOS(" [") ;
				for ( k = 0 ; k < ptrDUFisica->contUL ; k++ )
				{
					printCarBIOS(' ') ;
     	            printIntBIOS(ptrDUFisica->ulindx[k], 1) ; 
				}	
				printStrBIOS(" ]") ;
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
		db = ptrDUFisica->db ;  
        if (db < 0) continue ; 
		if (ptrDUFisica->unidadBIOS == unidadBIOS) 
		{
            unidadFisicaArranque = ufindx ;	       /* unidadFisicaArranque */
			if (ufindx < 2)                                    /* disquete */
			    unidadLogicaArranque = ufindx ;
        }			
		if (ufindx >= 2)                                     /* disco duro */
		{                               /* leemos el sector 0 de la unidad */

    		err = opSectorDB(0, db, (pointer_t)mbr, cmd_read_sector) ;

			if (err) 
			{
                printStrBIOS("\n error: no funciona la unidad de disco ") ;
                printStrBIOS(d_bloque[db].nombre) ;
                printLnBIOS() ;
				leerTeclaBIOS() ;
				ptrDUFisica->db = -1 ;               /* se anula la unidad */
				continue ;
			}
            else if ((mbr->signatura[0] != 0x55) || 
			         (mbr->signatura[1] != 0xAA))
            {
                printStrBIOS("\n error: MBR sin signatura (0x55, 0xAA) en ") ;
                printStrBIOS(d_bloque[db].nombre) ;
                printLnBIOS() ;
				leerTeclaBIOS() ;
				ptrDUFisica->db = -1 ;               /* se anula la unidad */
				continue ;
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
				printStrBIOS(" [") ;
				for ( k = 0 ; k < ptrDUFisica->contUL ; k++ )
				{
					printCarBIOS(' ') ;
     	            printIntBIOS(ptrDUFisica->ulindx[k], 1) ; 
				}	
				printStrBIOS(" ]") ;
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
                }
				else if (unidadFisicaArranque == ufindx) 
				{	
			        ulindx = ptrDUFisica->ulindx[j] ;
                    unidadLogicaArranque = ulindx ;				
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
		
    for ( ulindx = 0 ; ulindx < maxUL ; ulindx++ )     /* descUnidadLogica */
    {
		ptrDULogica = (descUnidadLogica_t *)&descUnidadLogica[ulindx] ;
		db = ptrDULogica->db ;
		if (db >= 0) 
		{
			ufindx = ptrDULogica->ufindx ;
			ptrDUFisica = (descUnidadFisica_t *)&descUnidadFisica[ufindx] ;
			ptrDParticion = (descParticion_t *)&ptrDUFisica->descParticion ;
            err = opSectorDB(d_bloque[db].primerSector, db, (pointer_t)boot, cmd_read_sector) ;
            if (err) 
			{
                printStrBIOS("\n aviso: no puede leerse el PBR en ") ;
                printStrBIOS(d_bloque[db].nombre) ;
				if (db < 2) 
                    printStrBIOS(" (disquete posiblemente no insertado) ") ;
                printLnBIOS() ;
				ptrDULogica->db = -1 ;		
				leerTeclaBIOS() ;
            }				
            else if ((boot->signatura[0] != 0x55) || 
			    (boot->signatura[1] != 0xAA))
            {
                printStrBIOS("\n aviso: PBR sin signatura (0x55, 0xAA) en ") ;
                printStrBIOS(d_bloque[db].nombre) ;
                printLnBIOS() ;
				ptrDULogica->db = -1 ;		
				leerTeclaBIOS() ;
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
			}
			else if ((ulindx == unidadLogicaArranque) && 
			         (boot->unidad != ptrDUFisica->unidadBIOS))
            {
                printStrBIOS("\n unidad de arranque distinta de la indicada en el sector de arranque: 0x") ;
                printHexBIOS(boot->unidad, 2) ;   
                leerTeclaBIOS() ;
            }    					 
            else if (((ulindx < 2) && 
			          ((!strncmp(boot->SF, "FAT12   ", 8)) ||
					   (!strncmp(boot->SF, "FAT16   ", 8)))) ||
					 (((ulindx >= 2) &&  
				      ((!strncmp(boot->SF, "FAT12   ", 8) && 
			           (ptrDParticion[ptrDULogica->indParticion].tipo == 0x01)) ||
                       (!strncmp(boot->SF, "FAT16   ", 8) && 
					   (ptrDParticion[ptrDULogica->indParticion].tipo == 0x04))))))
            {
				memcpy(&ptrDULogica->BPB, &boot->BPB, sizeof(BPB_t)) ;
				ptrDULogica->letra = (db < 2) ? 'A' + db : letraUnidad++ ;
            }
			else 
			{
                printStrBIOS("\n tipo de sistema de ficheros no soportado en ") ;
                printStrBIOS(d_bloque[db].nombre) ;
                printLnBIOS() ;
				ptrDULogica->db = -1 ;		
                leerTeclaBIOS() ;
			}
		}		
    }	
#if (MOSTRARUNIDADES)	
    printLnBIOS() ;	
#endif	
	return(0) ;
}