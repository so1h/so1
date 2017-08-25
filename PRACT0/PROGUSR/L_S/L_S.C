/* ----------------------------------------------------------------------- */
/*                                   l_s.c                                 */
/* ----------------------------------------------------------------------- */
/*                       programa l_s para el sistema SO1                  */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\ll_s_so1.h>    /* biblioteca de llamadas al sistema SO1 */
#include <so1pub.h\escribir.h>

#include <so1pub.h\carsctrl.h>                                  /* CR, BEL */
#include <so1pub.h\strings.h>
#include <so1pub.h\scanner.h>
#include <so1pub.h\caracter.h>
#include <so1pub.h\msdos.h>
#include <so1pub.h\bios_0.h>

#include <so1pub.h\cmos.h>             /* leerCmos, CMOS_FLOPPY_DRIVE_TYPE */
                                                /* CFD_144MB, CFD_NO_DRIVE */
void formato ( void ) {
  escribirStr("\n\n") ;
  escribirStr(" formato: l_s [ unidad sector numero fichero | -h ] ") ;
  escribirLn() ;
}

void help ( void ) {
  escribirLn() ;
  escribirLn() ;
  escribirStr(" formato 1: l_s ") ;
                           escribirStr("muestra el formato del comando. \n\n") ;
  escribirStr(" formato 2: l_s -h ") ;
                                        escribirStr("muestra este help. \n\n") ;
  escribirStr(" formato 3: l_s unidad sector nun fichero                \n\n") ;
  escribirStr("   lee de la unidad (0->A:, 1->B:) y a partir del sector \n") ;
  escribirStr("   (0..2879) especificado, tantos sectores como indique  \n") ;
  escribirStr("   numero (1..2880), sin sobrepasar el ultimo sector del \n") ;
  escribirStr("   disquete (2879), y escribe de forma consecutiva en el \n") ;
  escribirStr("   fichero el contenido de todos los sectores leidos.    \n\n") ;
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

#pragma option -w-par            /* omitir warning parameter is never used */

void procesarOperacion ( word_t unidad, word_t sector, word_t numero, int df ) {
  int i, j ;
  char car ;
  word_t prc ;
  word_t escritos ;
  int codigo = 0 ;
  int error = 0 ;
  word_t sectorActual ;

  escribirStr("\n sector = ") ;

  sectorActual = sector ;
  for ( i = 0 ; i < numero ; i++ ) {
    escribirDec(sectorActual, 4) ;
    prc = porcentaje(i, numero) ;
    escribirPorcentaje(prc) ;
    escribirDec(prc, 6) ;
    escribirCar('%') ;

    for (j = 0 ; j < 64 ; j++ ) escribirCar('\b') ;
    if (error) break ;
    sectorActual++ ;
  }
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
  case 2 : escribirStr("\n\n error al escribir en el fichero (escritos ") ;
           escribirDec(escritos, 1) ;
           escribirStr(" bytes de los 512 previstos) ") ;
           break ;
  case 3: ;
  }

  escribirLn() ;

  closeDOS(df) ;
}

#pragma option -wpar           /* permitir warning parameter is never used */

void main ( int argc, char * argv [ ] ) {

  int i ;
  int df ;
  char carAux ;
  word_t unidad ;
  word_t sector ;
  word_t numero ;
  word_t tipoDeUnidad ;

  if (!hayMSDOS()) {
    escribirCar(BEL) ;                                         /* error -1 */
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
    error(-2, " falta indicar sector, numero y fichero ") ;    /* error -2 */
  }

  if (argc == 3)                                               /* error -3 */
    error(-3, " falta indicar numero y fichero ") ;

  if (argc == 4)                                               /* error -4 */
    error(-4, " falta indicar el fichero de destino ") ;

  if (argc > 5) {
    escribirCar(BEL) ;                                         /* error -5 */
    formato() ;
    escribirStr("\n error -5: sobran los argumentos: ") ;
    for ( i = 5 ; i < argc ; i++ ) {
      escribirStr(argv[i]) ;
      escribirCar(' ') ;
    }
    escribirLn() ;
    exit(-5) ;
  }

  copiarStr(argv[1], comando[0]) ;
  inicScanner() ;
  obtenSimb() ;
  if (simb != s_numero) {
    escribirCar(BEL) ;                                         /* error -6 */
    formato() ;
    escribirStr("\n error -6: unidad debe ser un numero y no \"") ;
    escribirStr(argv[1]) ;
    escribirStr("\"\n") ;
    exit(-6) ;
  }
  else if (num > 1) {
    escribirCar(BEL) ;                                         /* error -7 */
    formato() ;
    escribirStr("\n error -7: unidad (") ;
    escribirDec(num, 1) ;
    escribirStr(") debe ser 0 (A:) o 1 (B:) \n") ;
    exit(-7) ;
  }
  else unidad = num  ;

  tipoDeUnidad = ((leerCmos(CMOS_FLOPPY_DRIVE_TYPE) >> 4*(1-unidad)) & 0x0F) ;

  /* escribirStr("\n tipoDeUnidad = ") ; escribirHex(tipoDeUnidad, 2) ; */

  if (tipoDeUnidad != CFD_144MB) {
    escribirCar(BEL) ;                                  /* errores -8 y -9 */
    formato() ;
    escribirStr("\n error -") ;
    escribirCar('8'+ (tipoDeUnidad != 0)) ;
    escribirStr(": la unidad ") ;
    escribirDec(unidad, 1) ;
    escribirStr(" (") ;
    escribirCar('A' + unidad) ;
    if (tipoDeUnidad == CFD_NO_DRIVE) {
      escribirStr(":) no esta instalada \n") ; exit(-8) ;
    }
    else {
      escribirStr(": no es de alta desnsidad (1,44 M) \n") ; exit(-9) ;
    }
  }

  copiarStr(argv[2], comando[0]) ;
  inicScanner() ;
  obtenSimb() ;
  if (simb != s_numero) {
    escribirCar(BEL) ;                                        /* error -10 */
    formato() ;
    escribirStr("\n error -10: sector debe ser un numero y no \"") ;
    escribirStr(argv[2]) ;
    escribirStr("\"\n") ;
    exit(-10) ;
  }
  else if (num > 2879) {
    escribirCar(BEL) ;                                        /* error -11 */
    formato() ;
    escribirStr("\n error -11: sector (") ;
    escribirDec(num, 1) ;
    escribirStr(") debe ser <= 2879 \n") ;
    exit(-11) ;
  }
  else sector = num  ;

  copiarStr(argv[3], comando[0]) ;
  inicScanner() ;
  obtenSimb() ;
  if (simb != s_numero) {
    escribirCar(BEL) ;                                        /* error -12 */
    formato() ;
    escribirStr("\n error -12: numero debe ser un numero y no \"") ;
    escribirStr(argv[3]) ;
    escribirStr("\"\n") ;
    exit(-12) ;
  }
  else if (num == 0)
    error(-13, " numero debe ser > 0 ") ;                     /* error -13 */
  else if ((sector + num - 1) > 2879) {
    escribirCar(BEL) ;                                        /* error -14 */
    formato() ;
    escribirStr("\n error -14: sector + numero - 1 (") ;
    escribirDec(sector + num - 1, 1) ;
    escribirStr(") debe ser <= 2879 \n") ;
    exit(-14) ;
  }
  else numero = num  ;

  if ((df = openDOS(argv[4], O_RDONLY_MSDOS)) >= 0) {
    closeDOS(df) ;
    escribirCar(BEL) ;
    escribirStr("\n\n el fichero ") ;
    escribirStr(argv[4]) ;
    escribirStr(" ya existe ¨ desea sobreescribirlo ? (S/[N]) ") ;
    while (((carAux = mayuscula(leer(STDIN))) != 'S') &&
           (carAux != 'N') &&
           (carAux != CR))
      escribirCar(BEL) ;
    escribirCar(mayuscula(carAux)) ;
    if ((carAux == 'N') || (carAux == CR)) {
      escribirLn() ;
      return ;
    }
  }

  if ((df = createDOS(argv[4], FA_ARCH)) < 0) {
    escribirCar(BEL) ;
    escribirStr("\n\n el fichero ") ;
    escribirStr(argv[4]) ;
    escribirStr(" no puede crearse/truncarse \n") ;
    exit(-15) ;
  }

  closeDOS(df) ;               /* createDOS deja el fichero abierto O_RDWR */

  if ((df = openDOS(argv[4], O_WRONLY_MSDOS)) < 0) {
    escribirCar(BEL) ;                                        /* error -16 */
    escribirStr("\n\n el fichero ") ;
    escribirStr(argv[4]) ;
    escribirStr(" no se pudo abrir para escritura \n") ;
    exit(-16) ;
  }

  escribirStr("\n\n se ha solicitado la operacion: ") ;
  escribirStr(argv[0]) ;
  escribirCar(' ') ;
  escribirDec(unidad, 1) ;
  escribirCar(' ') ;
  escribirDec(sector, 1) ;
  escribirCar(' ') ;
  escribirDec(numero, 1) ;
  escribirCar(' ') ;
  escribirStr(argv[4]) ;
  escribirStr(" (df = ") ;
  escribirDec(df, 1) ;
  escribirStr(") \n") ;

  procesarOperacion(unidad, sector, numero, df) ;

}

