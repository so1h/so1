/* ----------------------------------------------------------------------- */
/*                                 e_clstr.c                               */
/* ----------------------------------------------------------------------- */
/*                     programa e_clstr para el sistema SO1                */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\ll_s_so1.h>    /* biblioteca de llamadas al sistema SO1 */
#include <so1pub.h\escribir.h>

#include <so1pub.h\carsctrl.h>                                  /* CR, BEL */
#include <so1pub.h\strings.h>
#include <so1pub.h\scanner.h>
#include <so1pub.h\caracter.h>
#include <so1pub.h\msdos.h>
#include <so1pub.h\bios_0.h>
#include <so1.h\bios.h>

#include <so1pub.h\cmos.h>             /* leerCmos, CMOS_FLOPPY_DRIVE_TYPE */
                                                /* CFD_144MB, CFD_NO_DRIVE */
void formato ( void ) {
  escribirStr("\n\n") ;
  escribirStr(" formato: e_clstr [ fichero unidad cluster | -h ] ") ;
  escribirLn() ;
}

void help ( void ) {
  escribirLn() ;
  escribirLn() ;
  escribirStr(" formato 1: e_clstr ") ;
  escribirStr("muestra el formato del comando. \n\n") ;
  escribirStr(" formato 2: e_clstr -h ") ;
                                        escribirStr("muestra este help.  \n\n") ;
  escribirStr(" formato 3: e_clstr fichero unidad cluster                \n\n") ;
  escribirStr("   lee todos los bytes del fichero indicado a la vez que  \n") ;
  escribirStr("   rellena con esos bytes los clusteres de la unidad (0-> \n") ;
  escribirStr("   A:, 1->B:) comenzando desde el cluster especificado (0 \n") ;
  escribirStr("   ..1439) hasta que se acaben los bytes del fichero      \n\n") ;
  escribirStr(" autor: Miguel Arber Magjistari Pulaci (S.O. 30-06-2014)  \n") ;
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
  "04H: ID de cluster erronea o no encontrada ",
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

void procesarOperacion ( word_t unidad, word_t sector, word_t numero, int df ) {
  int i, j ;
  char car ;
  word_t prc ;
  word_t leidos ;
  int codigo = 0 ;
  int error = 0 ;
  word_t sectorActual ;
  char buf[1024] ;

  geom[unidad].sectores = 18 ;
  geom[unidad].cabezas = 2 ;
  geom[unidad].cilindros = 80 ;

  escribirStr("\n cluster = ") ;

  sectorActual = sector ;
  for ( i = 0 ; i < numero ; i++ ) {
    escribirDec(sectorActual, 4) ;
    prc = porcentaje(i, numero) ;
    escribirPorcentaje(prc) ;
    escribirDec(prc, 6) ;
    escribirCar('%') ;

    if (leidos = readDOS(df, &buf, 1024) < 0) error = 1 ;
    else {
      codigo = escribirSectorBIOS(2*sectorActual, unidad, (pointer_t)&buf) ;
      if (codigo) error = 2 ;
      else {
        codigo = escribirSectorBIOS(2*sectorActual+1, unidad, (pointer_t)&buf[512]) ;
        if (codigo) error = 2 ;
        else if((car = leerAsciiListo(STDIN)) == ESC || (mayuscula(car) == 'Q')) error = 3 ;
      }
    }

    for (j = 0 ; j < 64 ; j++ ) escribirCar('\b') ;
    if (error) break ;
    sectorActual++ ;
  }
  sectorActual-- ;
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
           escribirStr(" al leer el cluster ") ;
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
           escribirDec(leidos, 1) ;
           escribirStr(" bytes de los 1024 previstos) ") ;
           break ;
  case 3: ;
  }

  escribirLn() ;

  closeDOS(df) ;

}

void main ( int argc, char * argv [ ] ) {

  int i ;
  int df ;
  word_t unidad ;
  word_t sector ;
  word_t tipoDeUnidad ;
  dword_t tam ;
  dword_t posAux ;

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
    error(-2, " falta indicar unidad y cluster ") ;    /* error -2 */
  }

  if (argc == 3)                                               /* error -3 */
    error(-3, " falta indicar cluster ") ;

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
    escribirCar(BEL) ;                                        /* error -9 */
    formato() ;
    escribirStr("\n error -9: cluster debe ser un numero y no \"") ;
    escribirStr(argv[3]) ;
    escribirStr("\"\n") ;
    exit(-9) ;
  }
  else if (num > 1439) {
    escribirCar(BEL) ;                                        /* error -10 */
    formato() ;
    escribirStr("\n error -10: cluster (") ;
    escribirDec(num, 1) ;
    escribirStr(") debe ser <= 1439 \n") ;
    exit(-10) ;
  }
  else sector = num  ;

  if ((df = openDOS(argv[1], O_RDONLY_MSDOS)) < 0) {          /* error -11 */
    escribirCar(BEL) ;
    escribirStr("\n\n error -11: el fichero ") ;
    escribirStr(argv[1]) ;
    escribirStr(" no existe \n") ;
    exit(-11) ;
  }

  tam = (dword_t)0 ;
  lseekDOS(df, (dword_t *)&tam, SEEK_END_MSDOS) ;

  if (tam == 0) {                                             /* error -12 */
    escribirCar(BEL) ;
    escribirStr("\n\n error -12: el fichero ") ;
    escribirStr(argv[1]) ;
    escribirStr(" esta vacio\n") ;
    exit(-12) ;
  }

  if ((tam + 1023)/1024 > 1440) {                             /* error -13 */
    escribirCar(BEL) ;
    formato();
    escribirStr("\n\n error -13: el fichero no cabe en el disquete (") ;
    escribirLDec(tam, 1) ;
    escribirStr(" > ");
    escribirLDec(1440*1024, 1);
    escribirStr(")\n");
    exit(-13) ;
  }

  if ((tam + 1023)/1024 + sector > 1440) {                      /* error -14 */
    escribirCar(BEL) ;
    formato();
    escribirStr("\n\n error -14: el ultimo cluster rellenado (") ;
    escribirLDec((tam + 1023)/1024 + sector -1, 1) ;
    escribirStr(") seria > 1439\n");
    exit(-14) ;
  }

  escribirStr("\n\n se ha solicitado la operacion: ") ;
  escribirStr(argv[0]) ;
  escribirCar(' ') ;
  escribirStr(argv[1]) ;
  escribirCar(' ') ;
  escribirDec(unidad, 1) ;
  escribirCar(' ') ;
  escribirDec(sector, 1) ;
  escribirStr(" \n\n") ;
  escribirStr(" (tam = ");
  escribirLDec(tam, 1);
  escribirStr(", numero de clusteres a escribir = ");
  escribirLDec((tam + 1023)/1024, 1);
  escribirStr(", df = ");
  escribirDec(df, 1);
  escribirStr(")\n");

  posAux = (dword_t)0 ;
  lseekDOS(df, (dword_t *)&posAux, SEEK_SET) ;

  procesarOperacion(unidad, sector, (tam + 1023)/1024, df) ;

}

