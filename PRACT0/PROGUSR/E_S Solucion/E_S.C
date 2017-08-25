/* ----------------------------------------------------------------------- */
/*                                  e_s.c                                  */
/* ----------------------------------------------------------------------- */
/*                      programa e_s para el sistema SO1                   */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\ll_s_so1.h>    /* biblioteca de llamadas al sistema SO1 */
#include <so1pub.h\escribir.h>

#include <so1pub.h\carsctrl.h>                             /* ESC, BEL, CR */
#include <so1pub.h\caracter.h>
#include <so1pub.h\strings.h>
#include <so1pub.h\scanner.h>
#include <so1pub.h\msdos.h>
#include <so1pub.h\bios_0.h>

#include <so1pub.h\cmos.h>             /* leerCmos, CMOS_FLOPPY_DRIVE_TYPE */
                                                /* CFD_144MB, CFD_NO_DRIVE */
#include <so1.h\bios.h>                            /* geom, leerSectorBIOS */

void formato ( void ) {
  escribirStr("\n\n") ;
/* escribirStrIntenso(" formato: e_s [ fichero unidad sector | -h ] ") ; */
  escribirStr(" formato: e_s [ fichero unidad sector | -h ] ") ;
  escribirLn() ;
}

void help ( void ) {
  escribirLn() ;
  escribirLn() ;
/* escribirStrIntenso(" formato 1: e_s ") ; */
  escribirStr(" formato 1: e_s ") ;
                           escribirStr("muestra el formato del comando. \n\n") ;
/* escribirStrIntenso(" formato 2: e_s -h ") ; */
  escribirStr(" formato 2: e_s -h ") ;
                                        escribirStr("muestra este help. \n\n") ;
/* escribirStrIntenso(" formato 3: e_s fichero unidad sector            \n\n") ; */
  escribirStr(" formato 3: e_s fichero unidad sector                    \n\n") ;
  escribirStr("   lee todos los bytes del fichero indicado a la vez que \n") ;
  escribirStr("   rellena con esos bytes los sectores de la unidad (0-> \n") ;
  escribirStr("   A:, 1->B:) comenzando desde el sector especificado (0 \n") ;
  escribirStr("   ..2879) hasta que se acaben los bytes del fichero.    \n\n") ;
  escribirStr(" autor: Pedro Pablo Lopez Rodriguez (S.O. 2014)          \n") ;
}

void error ( int codigo, char * mensaje ) {
  escribirCar(BEL) ;
  formato() ;
  escribirStr("\n error ") ;
  escribirInt(codigo, 1) ;
  escribirCar(':') ;
  escribirStr(mensaje) ;
  escribirLn() ;
  exit(codigo) ;
}

word_t porcentaje ( int i, word_t numero ) {
  dword_t producto ;
  producto = ((dword_t)i)*100L ; ;
  return((word_t)(producto/((dword_t)numero))) ;
}

void escribirPorcentaje ( word_t prc ) {                  /* 54 posiciones */
  word_t prcAux ;
  int j ;
  escribirCar(' ') ;
  escribirCar((char)179) ;                              /* marco izquierdo */
  prcAux = prc ;
  for ( j = 0 ; j < (prc/2) ; j ++ ) escribirCar((char)219) ;
  if ((prc % 2) == 1) {
    escribirCar((char)221) ;
    prcAux++ ;
  }
  for ( j = prcAux/2 ; j < 50 ; j ++ ) escribirCar(' ') ;
  escribirCar((char)179) ;                                /* marco derecho */

}

char * errStr0X [ ] = {
  "00H: ningun error en la ultima operacion ",
  "01H: comando incorrecto: peticion al controlador invalida ",
  "02H: marca de direccion incorrecta ",
  "03H: proteccion contra escritura: disquete protegido contra escritura ",
  "04H: ID de sector erronea o no encontrada ",
  "05H: error desconocido ",
  "06H: linea de cambio de disquete activa ",
  "07H: error desconocido ",
  "08H: fallo del DMA ",
  "09H: DMA overrun: intento de escribir atravesando una frontera de 64K ",
  "0AH: error desconocido ",
  "0BH: error desconocido ",
  "0CH: tipo de medio no disponible "
} ;

char * errStrX0 [ ] = {
  "10H: CRC incorrecto: el CRC no coincide con el de los datos ",
  "20H: fallo del controlador de disquete ",
  "40H: posicionamiento incorrecto: no se encuentra la pista solicitada ",
  "80H: time out ",
} ;

void procesarOperacion ( word_t unidad, word_t sector, word_t numero, dword_t tam, int df ) {
  int i, j ;
  char car ;
  char buf [512] ;
  word_t prc ;
  word_t leidos ;
  word_t nBytes = 512 ;
  dword_t porLeer = tam ;
  int codigo ;
  int error = 0 ;
  word_t sectorActual ;

  geom[unidad].sectores = 18 ;
  geom[unidad].cabezas = 2 ;
  geom[unidad].cilindros = 80 ;

  escribirStr("\n sector = ") ;

  sectorActual = sector ;
  for ( i = 0 ; i < numero ; i++ ) {
    escribirDec(sectorActual, 4) ;
    prc = porcentaje(i, numero) ;
    escribirPorcentaje(prc) ;
    escribirDec(prc, 6) ;
    escribirCar('%') ;

    if (porLeer > (dword_t)512) porLeer = porLeer - (dword_t)512 ;
    else nBytes = (word_t)porLeer ;

    if ((leidos = readDOSFar(df, (char far *)&buf, nBytes)) < nBytes) {
      error = 2 ;
    }
    else {
      codigo = escribirSectorBIOS(sectorActual, (byte_t)unidad, (pointer_t)&buf) ;
      if (codigo) error = 1 ;
      else if (((car = leerAsciiListo(STDIN)) == ESC) || (mayuscula(car) == 'Q'))
        error = 3 ;
    }
    for (j = 0 ; j < 64 ; j++ ) escribirCar('\b') ;
    if (error) break ;
    sectorActual++ ;
  }
  if (!error) sectorActual-- ;
  escribirDec(sectorActual, 4) ;
  prc = porcentaje(i, numero) ;
  escribirPorcentaje(prc) ;

  switch (error) {
  case 0 :
  case 1 :
  case 2 :
  case 3 : escribirStr("   ") ;
           escribirDec(prc, 3) ;
           break ;
  }

  escribirCar('%') ;
  switch (error) {
  case 0 : escribirStr(" Ok ") ; break ;
  case 1 : escribirStr(" eL ") ; break ;
  case 2 : escribirStr(" eE ") ; break ;
  case 3 : escribirStr(" Ab ") ; break ;
  }

  switch (error) {
  case 0 : break ;
  case 1 : escribirStr("\n\n error ") ;
           escribirHex(codigo, 2) ;
           escribirStr(" al leer el sector ") ;
           escribirDec(sectorActual, 1) ;
           escribirStr("\n\n error ") ;
           if (codigo <= 0x0C)
             escribirStr(errStr0X[codigo]) ;
           else switch (codigo) {
             case 0x10 : escribirStr(errStrX0[0]) ; break ;
             case 0x20 : escribirStr(errStrX0[1]) ; break ;
             case 0x40 : escribirStr(errStrX0[2]) ; break ;
             case 0x80 : escribirStr(errStrX0[3]) ; break ;
             default   : escribirHex(codigo, 2) ;
                         escribirStr((char *)&errStr0X[0x05][2]) ;
           }
           break ;
  case 2 : escribirStr("\n\n error al leer del fichero (leidos ") ;
           escribirDec(leidos, 1) ;
           escribirStr(" bytes de los 512 previstos) ") ;
           break ;
  case 3: ;
  }

  escribirLn() ;

  closeDOS(df) ;

}

void main ( int argc, char * argv [ ] ) {

  int i ;
  int df ;
  char carAux ;
  word_t unidad ;
  word_t sector ;
  word_t numero ;
  word_t tipoDeUnidad ;
  dword_t tam ;
  dword_t pos ;

  if (!hayMSDOS()) {
    escribirCar(BEL) ;                                            /* error -1 */
    escribirStr("\n\n error -1: este programa requiere MSDOS \n") ;
    exit(-1) ;
  }

  if (argc == 1) {
    formato() ;
    return ;
  }

  if (argc == 2) {
    if ((argv[1][0] == '-') &&
        (mayuscula(argv[1][1]) == 'H') &&
        (argv[1][2] == (char)0)) {
      help() ; return ;
    }
    error(-2, " falta indicar unidad y sector ") ;             /* error -2 */
  }

  if (argc == 3)                                               /* error -3 */
    error(-3, " falta indicar sector ") ;

  if (argc > 4) {
    escribirCar(BEL) ;                                         /* error -4 */
    formato() ;
    escribirStr("\n error -4: sobran los argumentos: ") ;
    for ( i = 4 ; i < argc ; i++ ) {
      escribirStr(argv[i]) ;
      escribirCar(' ') ;
    }
    escribirLn() ;
    exit(-4) ;
  }

  copiarStr(argv[2], comando[0]) ;
  inicScanner() ;
  obtenSimb() ;
  if (simb != s_numero) {
    escribirCar(BEL) ;                                         /* error -5 */
    formato() ;
    escribirStr("\n error -5: unidad debe ser un numero y no \"") ;
    escribirStr(argv[2]) ;
    escribirStr("\"\n") ;
    exit(-5) ;
  }
  else if (num > 1) {
    escribirCar(BEL) ;                                         /* error -6 */
    formato() ;
    escribirStr("\n error -6: unidad (") ;
    escribirDec(num, 1) ;
    escribirStr(") debe ser 0 (A:) o 1 (B:) \n") ;
    exit(-6) ;
  }
  else unidad = num  ;

  tipoDeUnidad = ((leerCmos(CMOS_FLOPPY_DRIVE_TYPE) >> 4*(1-unidad)) & 0x0F) ;

  /* escribirStr("\n tipoDeUnidad = ") ; escribirHex(tipoDeUnidad, 2) ; */

  if (tipoDeUnidad != CFD_144MB) {
    escribirCar(BEL) ;                                  /* errores -7 y -8 */
    formato() ;
    escribirStr("\n error -") ;
    escribirCar('7'+ (tipoDeUnidad != 0)) ;
    escribirStr(": la unidad ") ;
    escribirDec(unidad, 1) ;
    escribirStr(" (") ;
    escribirCar('A' + unidad) ;
    if (tipoDeUnidad == CFD_NO_DRIVE) {
      escribirStr(":) no esta instalada \n") ; exit(-7) ;
    }
    else {
      escribirStr(": no es de alta desnsidad (1,44 M) \n") ; exit(-8) ;
    }
  }

  copiarStr(argv[3], comando[0]) ;
  inicScanner() ;
  obtenSimb() ;
  if (simb != s_numero) {
    escribirCar(BEL) ;                                         /* error -9 */
    formato() ;
    escribirStr("\n error -9: sector debe ser un numero y no \"") ;
    escribirStr(argv[3]) ;
    escribirStr("\"\n") ;
    exit(-9) ;
  }
  else if (num > 2879) {
    escribirCar(BEL) ;                                        /* error -10 */
    formato() ;
    escribirStr("\n error -10: sector (") ;
    escribirDec(num, 1) ;
    escribirStr(") debe ser <= 2879 \n") ;
    exit(-10) ;
  }
  else sector = num  ;

  if ((df = openDOS(argv[1], O_RDONLY_MSDOS)) < 0) {
    escribirCar(BEL) ;                                        /* error -11 */
    formato() ;
    escribirStr("\n error -11: el fichero ") ;
    escribirStr(argv[1]) ;
    escribirStr(" no existe \n") ;
    exit(-11) ;
  }

  pos = (dword_t)0 ;

  lseekDOS(df, (dword_t *)&pos, SEEK_END) ;

  tam = pos ;

  if (tam == (dword_t)0) {
    closeDOS(df) ;
    error(-12, " el fichero esta vacio ") ;                   /* error -12 */
  }

  if (tam > ((dword_t)2880*(dword_t)512)) {
    closeDOS(df) ;
    escribirCar(BEL) ;                                        /* error -13 */
    formato() ;
    escribirStr("\n error -13: el fichero no cabe en el disquete (") ;
    escribirLDec(tam, 1) ;
    escribirStr(" > ") ;
    escribirLDec((dword_t)2880*(dword_t)512, 1) ;
    escribirStr(") \n") ;
    exit(-13) ;
  }

  numero = (word_t)((tam+511)/512) ;

  if ((sector + numero) > 2880) {
    closeDOS(df) ;
    escribirCar(BEL) ;                                        /* error -14 */
    formato() ;
    escribirStr("\n error -14: el ultimo sector rellenado (") ;
    escribirDec(sector + numero - 1, 1) ;
    escribirStr(") seria > 2879 \n") ;
    exit(-13) ;
  }

  escribirStr("\n\n se ha solicitado la operacion: ") ;
  escribirStr(argv[0]) ;
  escribirCar(' ') ;
  escribirStr(argv[1]) ;
  escribirCar(' ') ;
  escribirDec(unidad, 1) ;
  escribirCar(' ') ;
  escribirDec(sector, 1) ;
  escribirStr("\n\n (tam = ") ;
  escribirLDec(tam, 1) ;
  escribirStr(", numero de sectores a escribir = ") ;
  escribirDec(numero, 1) ;
  escribirStr(", df = ") ;
  escribirDec(df, 1) ;
  escribirStr(") \n") ;

  pos = (dword_t)0 ;

  lseekDOS(df, (dword_t *)&pos, SEEK_SET) ;

  procesarOperacion(unidad, sector, numero, tam, df) ;

}

