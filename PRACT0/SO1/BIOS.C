/* ----------------------------------------------------------------------- */
/*                                 bios.c                                  */
/* ----------------------------------------------------------------------- */
/*                                                                         */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\pantalla.h>
#include <so1pub.h\pic.h>                       /* valorIMR, establecerIMR */
#include <so1.h\ajustes.h>
#include <so1.h\procesos.h>
#include <so1.h\plot.h>
#include <so1.h\bios.h>                           /* cmd_get_params, CSH_t */

//                                                      /* Disco (int 13h) */

int getDriveParams ( byte_t unidadBIOS, CSH_t * CSH ) 
{
    int error = 0 ;
	word_t CSH_cs ;
	byte_t CSH_h ;
    asm 
	{
		mov ah,cmd_get_params ;
		mov dl,unidadBIOS ;
		int 13h ;		
		jnc getOk ;
        mov error,ah ;
		jmp getEnd ;
getOk:		
		mov CSH_cs,cx ;
		mov CSH_h,dh ;
	}
	CSH->cs = CSH_cs ;
	CSH->h = CSH_h ;
getEnd:	
	return(error) ;
}

int resetControllerBIOS ( void ) 
{
    int error = 0 ;
    asm 
	{
        mov ah,cmd_reset ;                        /* reset del controlador */
        int 13h ;                                    /* BIOS: E/S de disco */
        jnc resetOk ;
        mov error,ah ;  /* el flag de acarreo C se activa en caso de error */
    }
resetOk:
    return(error) ;
}

int opSectorCSH ( CSH_t * CSH, byte_t unidadBIOS, pointer_t dir, byte_t cmd ) ;

int leerSectorCSH ( CSH_t * CSH, byte_t unidadBIOS, pointer_t dir ) {
  return(opSectorCSH(CSH, unidadBIOS, dir, cmd_read_sector)) ;
}

int escribirSectorCSH ( CSH_t * CSH, byte_t unidadBIOS, pointer_t dir ) {
  return(opSectorCSH(CSH, unidadBIOS, dir, cmd_write_sector)) ;
}

int opSectorLBA ( dword_t sectorLogico, byte_t unidadBIOS, pointer_t dir, byte_t cmd ) ;

int leerSectorLBA ( dword_t sectorLogico, byte_t unidadBIOS, pointer_t dir ) {
  return(opSectorLBA(sectorLogico, unidadBIOS, dir, cmd_read_sector)) ;
}

int escribirSectorLBA ( dword_t sectorLogico, byte_t unidadBIOS, pointer_t dir ) {
  return(opSectorLBA(sectorLogico, unidadBIOS, dir, cmd_write_sector)) ;
}

int opSectorCSH ( CSH_t * CSH, byte_t unidadBIOS, pointer_t dir, byte_t cmd ) {

  byte_t CHS_h = CSH->h ;                                      /* hhhhhhhh */
  word_t CHS_cs = CSH->cs ;                            /* ccccccccCCssssss */
                                        /* el sector fisico no puede ser 0 */
  byte_t error = 0 ;

//word_t IMR ;
//IMR = valorIMR() ;
//establecerIMR(~((1 << IRQ_TIMER) |
//                (1 << IRQ_DISQUETE))) ;
//asm pushf ;
//asm { @: jmp @ ; }

  asm {
    les bx,dir ;                   /* pone en ES:BX la direccion del bufer */
    mov dl,unidadBIOS ;
    mov dh,CHS_h ;
    mov cx,CHS_cs ;
    mov al,1 ;
    mov ah,cmd ;                         /* lectura/escritura de un sector */
    int 13h ;                                        /* BIOS: E/S de disco */
    jnc lecturaOk ;
    mov error,ah ;               /* codigo de error retornado por la INT13 */

lecturaOk:
  }
  
//asm popf ;
//establecerIMR(IMR) ;

  return((int)error) ;
}

typedef struct {
  word_t  tam ;                         /* tamaño del bloque de parametros */
  word_t  numSectores ;                       /* numero de sectores a leer */
  pointer_t dir ;                         /* bufer segmento:desplazamiento */
  dword_t sectorLogicoL ;             /* primeros 32 bits del sectorLogico */
  dword_t sectorLogicoH ;              /* ultimos 32 bits del sectorLogico */
} bloqueDeParametros_t ;

bloqueDeParametros_t bloqueDeParametros ; /* ==> opSectorLBA no reentrante */

int opSectorLBA ( dword_t sectorLogico, byte_t unidadBIOS, pointer_t dir, byte_t cmd ) {

//word_t inicioBDP = (word_t)&bloqueDeParametros ;
//word_t finBDP = (word_t)&bloqueDeParametros + sizeof(bloqueDeParametros_t) ;
  byte_t error = 0 ;
  byte_t cmdExt = 0x40 + cmd ;

  bloqueDeParametros.tam = sizeof(bloqueDeParametros_t) ;
  bloqueDeParametros.numSectores = 1 ;
  bloqueDeParametros.dir = dir ;
  bloqueDeParametros.sectorLogicoL = sectorLogico ;
  bloqueDeParametros.sectorLogicoH = 0x00000000L ;

//asm { @: jmp @ ; }
  asm {
//  mov si,inicioBDP ;
    mov si,OFFSET bloqueDeParametros ;
//  mov di,finBDP ;
    mov di,si ;
    add di,16 ;
    mov dl,unidadBIOS ;
    mov al,1 ;
    mov ah,cmdExt ;     /* 42H -> EXTENDED READ                            */
    int 13h ;           /* http://hdebruijn.soo.dto.tudelft.nl/newpage/interupt/out-0700.htm#0651 */
    jc errorAlLeer ;    /* el flag de acarreo C se activa en caso de error */
    mov error,ah ;               /* codigo de error retornado por la INT13 */
  }
errorAlLeer:
  return((int)error) ;
}

