/************* prueba ****************************/

void far ratonHandler ( void ) {

  /* se llama a esta funcion tras la interrupcion y mediante una llamada   */
  /* a procedimiento lejano (CALL FAR). Por ese motivo la pila contiene lo */
  /* siguiente: Flags, CS, IP del proceso y direccion de retorno lejana.   */

  asm cli
  asm push ds                    /* segmento de datos del driver del raton */
  asm mov ds,word ptr cs:[segDatos]          /* segmento de datos DS_Raton */

#if (FALSE)                                             /* para depuracion */
  mostrarRegistros(win_so) ;
  paradaActiva() ;
#endif

  /* asm mov rH_AX,ax */                   /* evento ocurrido, ver techelp */
  asm mov rH_BX,bx                                              /* botones */
  asm mov rH_CX,cx                                                    /* X */
  asm mov rH_DX,dx                                                    /* Y */
  /* asm mov rH_SI,si */                                           /* incX */
  /* asm mov rH_DI,di */                                           /* incY */
  asm push ax
  asm push bp
  asm mov bp,sp
  asm mov ax,ss:[bp+12]
  asm mov rH_CS,ax                                       /* CS del proceso */
  asm mov ax,ss:[bp+10]                                  /* IP del proceso */
  asm mov rH_IP,ax
  asm mov ss:[bp+12],cs                                              /* CS */
/*  asm mov ax,OFFSET rti_raton_BIOS */                      /* rti_raton_BIOS */
  asm mov ax,OFFSET offset_rti_nVIntRaton                /* rti_raton_BIOS */
  asm mov ss:[bp+10],ax
  asm pop bp
  asm pop ax
  asm pop ds                     /* segmento de datos del driver del raton */
}

void far rti_raton_BIOS ( void ) {                  /* opcion -r- (SI, DI) */

  asm pushf                              /* apilamos los flags del proceso */
  asm sub sp,4                                /* sitio para CS del proceso */
                                              /* sitio para IP del proceso */
  asm push ds
  asm mov ds, word ptr cs:[OFFSET segDatos]           /* segmento de datos */
  asm push ax
  asm push bp
  asm mov bp,sp
  asm mov ax,rH_CS
  asm mov ss:[bp+8],ax
  asm mov ax,rH_IP
  asm mov ss:[bp+6],ax
  asm pop bp
  asm pop ax
  asm pop ds

  prologo_rti() ;

/*  plot('m', 0, contadorTimer0()) ; */

  procesarRaton(rH_BX, rH_CX, rH_DX)  ;

/*  plot('e', 0, contadorTimer0()) ; */

  /* eoi_pic2() ; */

  epilogo_rti() ;

}

/************* fin prueba ***********************/

