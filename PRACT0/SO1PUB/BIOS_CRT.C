/* ----------------------------------------------------------------------- */
/*                                bios_crt.c                               */
/* ----------------------------------------------------------------------- */
/*                                                                         */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\tipos.h>                                      /* byte_t */
#include <so1pub.h\bios_0.h>                     /* readXYBIOS, goToXYBIOS */
#include <so1pub.h\bios_crt.h>                          /* tipoAdaptador_t */
#include <so1pub.h\biosdata.h>                              /* ptrBiosArea */

/* Ver proyecto Public Domain Curses (pdcurses) en sourceforge             */

byte_t numFilas ;
byte_t numColumnas ;

pantalla_t far * ptrPant ;

tipoAdaptador_t tipoAdaptador ;

static bool_t enColor ;
static bool_t videoDirecto ;
bool_t redimensionable ;
static bool_t adaptadorNoFiable ;

byte_t fontSize ;
byte_t visibilidad = 1 ;             /* 0 invisible, 2 grande, por defecto */

static byte_t cursorOriginalF ;
static byte_t cursorOriginalC ;
static byte_t lineaOriginal1 ;
static byte_t lineaOriginal2 ;

static byte_t numFilasOriginal ;
static byte_t numColumnasOriginal ;

static byte_t fontSizeOriginal ;
static byte_t VIDEO_mode ;                 /* si = 0x07 => enColor == TRUE */
static word_t VIDEO_pagesize ;
static word_t VIDEO_pageoff ;

/* static pantalla_t far * copiaPantallaOriginal ; */

static word_t segMemVideo ;           /* 0xB000 (mono) o 0xB800 (en color) */

void inicBiosCrt ( int filas, int columnas, bool_t redim ) {

  numFilasOriginal = ptrBiosArea->VIDEO_lastrow + 1 ;
  numColumnasOriginal = ptrBiosArea->VIDEO_width ;
  fontSizeOriginal = ptrBiosArea->VIDEO_hchar ;
  readXYBIOS(&cursorOriginalF, &cursorOriginalC, &lineaOriginal1, &lineaOriginal2) ;
  tipoAdaptador = tipoDeAdaptador() ;            /* inicializa segMemVideo */

  ptrPant = (pantalla_t far *)pointer(segMemVideo, 0x0000) ;
/*  copiaPantallaOriginal = (pantalla_t far *)&ptrPant->t[maxFilas][0] ; */
/*  copiarPantalla(ptrPant, copiaPantallaOriginal, numFilasOriginal) ; */
/*  copiarPantalla(ptrPant, copiaPantallaOriginal, maxFilas) ; */

  redimensionable = TRUE ;               /* ¡deberia ser TRUE y no lo era! */

  redimensionar(filas, columnas) ;
//redimensionar(50, 80) ;
//redimensionar(25, 80) ;
  /* redimensionar(25, 80) ; */ /* setFontSize(_FONT16) ; */
  /* redimensionar(28, 80) ; */ /* setFontSize(_FONT15) ; */
  /* redimensionar(50, 80) ; */ /* setFontSize(_FONT08) ; */
  /* set25x80() ; */

  numFilas = ptrBiosArea->VIDEO_lastrow + 1 ;
  numColumnas = (byte_t)ptrBiosArea->VIDEO_width ;

  redimensionable = (numFilas > 25) || redim ;

  VIDEO_mode = ptrBiosArea->VIDEO_mode ;
  VIDEO_pagesize = ptrBiosArea->VIDEO_pagesize ;
  VIDEO_pageoff = ptrBiosArea->VIDEO_pageoff ;

}

void finBiosCrt ( void ) {
  redimensionar(numFilasOriginal, numColumnasOriginal) ;
/*  copiarPantalla(copiaPantallaOriginal, ptrPant, numFilasOriginal) ; */
/*  copiarPantalla(copiaPantallaOriginal, ptrPant, maxFilas) ; */
/*  goToXYBIOS(cursorOriginalF, cursorOriginalC) ; */
}

fontSize_t getFontSize ( void ) {
  int res = ptrBiosArea->VIDEO_hchar ;
  /* Assume the MDS Genius is in 66 line mode. */
  if ((res == 0) && (tipoAdaptador == _MDS_GENIUS)) res = _FONT15;
  switch (tipoAdaptador) {
  case _MDA : res = 10 ; break ;  /* POINTS is not certain on MDA/Hercules */
  case _EGACOLOR :
  case _EGAMONO  :
    switch (res) {
    case _FONT08  :
    case _FONT14 : break ;
    default      : res = _FONT14 ;
    }
    break ;
  case _CGA : res = _FONT08;
  }
  return res;
}

byte_t getFilas ( void ) {
  int filas = ptrBiosArea->VIDEO_lastrow + 1 ;
  if ((filas == 1) && (tipoAdaptador == _MDS_GENIUS)) filas = 66 ;
  if ((filas == 1) && (tipoAdaptador == _MDA)) filas = 25 ;
  if (filas == 1) {
    filas = 25 ;
    videoDirecto = FALSE;
  }
  switch (tipoAdaptador) {
  case _EGACOLOR :
  case _EGAMONO  :
    switch (filas) {
    case 25 :
    case 43 : break;
    default : filas = 25;
    }
    break;
  case _VGACOLOR :
  case _VGAMONO  : break;
  default : filas = 25 ; break;
  }
  return filas ;
}

int comprobarAdaptador ( tipoAdaptador_t tipoAdaptador ) {

  int fontSize = getFontSize();
  int filas = getFilas() ;
  switch (tipoAdaptador) {
  case _EGACOLOR :
  case _EGAMONO  :
    switch (filas) {
      case 25 :
      case 43 : break;
      default : adaptadorNoFiable = TRUE;
    }
    switch (fontSize) {
    case _FONT08  :
    case _FONT14 : break;
    default: adaptadorNoFiable = TRUE;
    }
    break;
  case _VGACOLOR  :
  case _VGAMONO   : break;
  case _CGA       :
  case _MDA       :
  case _MCGACOLOR :
  case _MCGAMONO  :
    switch (filas) {
    case 25 : break;
    default : adaptadorNoFiable = TRUE;
    }
    break;
  default: adaptadorNoFiable = TRUE;
  }
  if (adaptadorNoFiable) {
    redimensionable = FALSE;
    videoDirecto = FALSE;
  }

  return(tipoAdaptador) ;
}

/* revisar la funcion tipoDeAdaptador y el valor de redimensionable        */

tipoAdaptador_t tipoDeAdaptador ( void ) {  /* Determina el tipo de adapt. */

  word_t CRTC_base ;
  int res = _NONE ;
  word_t DCC ;                                 /* Display Combination Code */

  redimensionable = FALSE ;

             /* intentamos llamar a la funcion de identificacion de la VGA */

  asm mov ax,01a00h ;                    /* Query Display Combination Code */
  asm int 10h
  asm mov DCC,bx                           /* BL = ID del display activo   */
                                           /* BH = ID del display inactivo */
  if (_AL == 0x1a) {            /* AL == 0x1a si la función esta soportada */

    /* Hay BIOS de video PS/2 funcionando */

    switch ((byte_t)DCC) {                                           /* BL */
    case 0x00 : res = _NONE ; break ;
    case 0x01 : res = _MDA ; break ;
    case 0x02 : res = _CGA ; break ;
    case 0x04 : res = _EGACOLOR ; redimensionable = TRUE ; break ;
    case 0x05 : res = _EGAMONO ; break ;
    case 0x1a :                                   /* ... otro BIOS VGA ... */
    case 0x07 : res = _VGACOLOR ; redimensionable = TRUE ; break ;
    case 0x08 : res = _VGAMONO ; break ;
    case 0x0a :
    case 0x0d : res = _MCGACOLOR ; break ;
    case 0x0c : res = _MCGAMONO ; break;
    default   : res = _CGA;
    }
  }
  else {

    /* No hay BIOS VGA. Miramos si hay un BIOS EGA seleccionando una */
    /* funcion de servicio alternativa:                              */
    /*   BX == 0x0010 --> devuelve informacion EGA                   */

    asm mov ah,12h
    asm mov bx,0010h
    asm int 10h

    if ((_BL != 0x10) && (res == _NONE)) {        /* Hay un BIOS EGA */

      asm mov ah,12h
      asm mov bx,0010h
      asm int 10h

      if (_BH == 0x00)         /* configuracion del BIOS por defecto */
        res = _EGACOLOR;
      else
        res = _EGAMONO;
    }
    else if (res == _NONE) {                    /* tenemos CGA o MDA */

      asm int 11h
      switch (_AL & 0x30) {
      case 0x10 :
      case 0x20 : res = _CGA ; break;
      case 0x30 : res = _MDA ; break;
      default   : res = _NONE ;
      }
    }
  }

  CRTC_base = ptrBiosArea->CRTC_base ;
  if (CRTC_base == 0x3D4) {
    segMemVideo = 0xB800;
    switch (res) {
    case _EGAMONO : res = _EGACOLOR ; break ;
    case _VGAMONO : res = _VGACOLOR ;
    }
  }
  else if (CRTC_base == 0x3B4) {
    segMemVideo = 0xB000;
    switch (res) {
    case _EGACOLOR : res = _EGAMONO ; break;
    case _VGACOLOR : res = _VGAMONO ;
    }
  }

  enColor = (segMemVideo == 0xB800) ;

    /* Check for DESQview shadow buffer
       thanks to paganini@ax.apc.org for the GO32 fix */
/*
  asm mov ah,0xfe
  asm mov al,0x00
  asm mov es,CRTC_base
  asm mov di,CRTC_base
  asm int 10h

  regs.h.ah = 0xfe;
  regs.h.al = 0;
  regs.x.di = pdc_video_ofs;
  segs.es   = pdc_video_seg;
  int86x(0x10, &regs, &regs, &segs);
  pdc_video_seg = segs.es;
  pdc_video_ofs = regs.x.di;
*/

  printStrBIOS("DCC = 0x") ; printHexBIOS(DCC, 4) ;
  printStrBIOS(" CRTC = 0x") ; printHexBIOS(CRTC_base, 4) ;
  printStrBIOS(" segmento = 0x") ; printHexBIOS(segMemVideo, 4) ;

  return comprobarAdaptador((tipoAdaptador_t)res) ;

}

byte_t setCursorVisibilidad ( int valorDeVisibilidad ) {
  int res = visibilidad ;
  byte_t start, end;
  /* byte_t VIDEO_mode = ptrBiosArea->VIDEO_mode ; */
  visibilidad = valorDeVisibilidad ;
  switch (visibilidad) {
    case 0  : start = 32 ; end = 0 ; break ;                  /* invisible */
    case 2  : start = 0 ; end = 7 ; break ;           /* altamente visible */
                                              /* bloque completo en altura */
    default :                                        /* visibilidad normal */
      start = lineaOriginal1 ;
      end = lineaOriginal2 ;
  }
       /* si no se establece el modo de video puede fallar en algunos BIOS */

  asm mov ah,01h
  /* asm mov al,80+VIDEO_mode */             /* para no borrar la pantalla */
  asm mov ch,start
  asm mov cl,end
  asm int 10h

  return res ;
}

void setFontSize ( fontSize_t size ) {
  if (adaptadorNoFiable) return;
  switch (tipoAdaptador) {
  case _CGA        :
  case _MDA        :
  case _MCGACOLOR  :
  case _MCGAMONO   :
  case _MDS_GENIUS : break;
  case _EGACOLOR   :
  case _EGAMONO    :
    if (redimensionable && (fontSize != size)) {
      switch (size) {
      case _FONT08:
        asm mov ax,1112h ;
        asm mov bl,00h ;
        asm int 10h
        break;
      case _FONT14:
        asm mov ax,1111h ;
        asm mov bl,00h ;
        asm int 10h
      }
    }
    break;
  case _VGACOLOR:
  case _VGAMONO:
    if (redimensionable && (fontSize != size)) {
      switch (size) {
      case _FONT08:
        asm mov ax,1112h ;
        asm mov bl,00h ;
        asm int 10h
        break;
      case _FONT14:
        asm mov ax,1111h ;
        asm mov bl,00h ;
        asm int 10h
        break;
      case _FONT16:
        asm mov ax,1114h ;
        asm mov bl,00h ;
        asm int 10h
      }
    }
  }

  setCursorVisibilidad(visibilidad) ;
  fontSize = getFontSize() ;
}

void set25x80 ( void ) {
  switch (tipoAdaptador) {
  case _CGA       :
  case _EGACOLOR  :
  case _EGAMONO   :
  case _VGACOLOR  :
  case _VGAMONO   :
  case _MCGACOLOR :
  case _MCGAMONO  : establecerModoDeVideo(0x03) ; break ;
  case _MDA       : establecerModoDeVideo(0x07) ; break ;
  }
}

#pragma warn -par

void redimensionar ( byte_t nFilas, byte_t nCols ) {
  /* SP->orig_cursor = 0x0607 ; */
  switch(tipoAdaptador) {
  case _EGACOLOR :
    if (nFilas >= 43)
      setFontSize(_FONT08) ;
    else
      set25x80() ;
    break;
  case _VGACOLOR :
    if (nFilas > 28)
      setFontSize(_FONT08) ;
    else
      if (nFilas > 25)
        setFontSize(_FONT14) ;
      else
        set25x80() ;
  }
/*  PDC_set_blink(COLORS == 8); */
}

#pragma warn +par

