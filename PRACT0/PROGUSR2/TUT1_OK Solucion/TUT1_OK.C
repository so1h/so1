/* ----------------------------------------------------------------------- */
/*                                tut1_ok.c                                */
/* ----------------------------------------------------------------------- */
/*              programa para validar los comandos l_s y e_s               */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\ll_s_so1.h>    /* biblioteca de llamadas al sistema SO1 */
#include <so1pub.h\escribir.h>

#include <so1pub.h\strings.h>
#include <so1pub.h\scanner.h>
#include <so1pub.h\msdos.h>

void comprobarError ( char * nombre, char * comando, int errorEsperado ) {

  pid_t pid ;
  int status ;

  escribirStr("\n comprobando: ") ;
  escribirStr(comando) ;

  pid = createProcess(nombre, comando, TRUE) ;
  if (pid < 0) {
    escribirStr("\n no ha podido ejecutarse el comando: ") ;
    escribirStr(comando) ;
    escribirStr(" (pid = ") ;
    escribirInt(pid, 1) ;
    escribirStr(")\n") ;
    exit(0) ;
  }
  else {
    waitpid(pid, (int *)&status) ;
    if (status != errorEsperado) {
      escribirStr("\n error ") ;
      escribirInt(errorEsperado, 1) ;
      escribirStr(" no detectado correctamente \n") ;
      exit(0) ;
    }
  }
}

void crearFicheroDePrueba ( char * nombre ) {
  int i, j, k ;
  unsigned char car = (unsigned char) 0 ;
  char str [5] ;
  word_t pot ;
  int df ;
  escribirStr("\n creando el fichero ") ;
  escribirStr(nombre) ;
  escribirStr(" de prueba\n") ;
  df = createDOS(nombre, FA_ARCH) ;
  for (i = 0 ; i < 2880 ; i++) {
    writeDOS(df, (char *)&"Sector ", 7) ;
    pot = 1000 ;
    for (k = 0 ; k < 4 ; k++ ) {
      str[k] = '0' + ((i/pot) % 10) ;
      pot = pot/10 ;
    }
    str[4] = ' ' ;
    writeDOS(df, (char *)&str, 4) ;
    writeDOS(df, (char *)&" ", 1) ;
    for (j = 12 ; j < 512 ; j++) {
      car = 'A' + ((i + j) % 26) ;
      writeDOS(df, &car, 1) ;
    }
  }
  closeDOS(df) ;
}

void comprobarFichero ( char * nombre ) {
  int i, j ;
  unsigned char car = (unsigned char) 0 ;
  word_t pot ;
  int df ;
  char buf [10] ;
  int numSector ;
  df = openDOS(nombre, O_RDONLY_MSDOS) ;
  if (df < 0) {
    escribirStr("\n el fichero ") ;
    escribirStr(nombre) ;
    escribirStr(" no existe\n") ;
    exit(0) ;
  }
  else {
    escribirStr("\n comprobando el fichero ") ;
    escribirStr(nombre) ;
    escribirStr("\n") ;
  }
  for (i = 0 ; i < 2880 ; i++) {
    readDOS(df, (char *)&buf, 7) ;
    if (!igualesHasta((char *)&"Sector ", (char *)&buf, 7)) {
      escribirStr("\n Algo no esta bien en el sector ") ;
      escribirDec(i, 1) ;
      buf[7] = (char)0 ;
      escribirStr(" bytes 0..6 = \"") ;
      escribirStr(buf) ;
      escribirStr("\"\n\n") ;
      exit(0) ;
    }
    readDOS(df, (char *)&buf, 4) ;
    buf[4] = (char)0 ;
    copiarStr(buf, comando[0]) ;
    inicScanner() ;
    obtenSimb() ;
    if (simb != s_numero) {
      escribirStr("\n Algo no esta bien en el sector ") ;
      escribirDec(i, 1) ;
      escribirStr(" bytes 7..10 = \"") ;
      escribirStr((char *)&buf) ;
      escribirStr("\" != ") ;
      escribirDec(i, 1) ;
      escribirStr("\n\n") ;
      exit(0) ;
    }
    else if (num != i) {
      escribirStr("\n Algo no esta bien en el sector ") ;
      escribirDec(i, 1) ;
      escribirStr(" bytes 7..10 = ") ;
      escribirDec(numSector, 1) ;
      escribirStr(" != ") ;
      escribirDec(i, 1) ;
      escribirStr("\n\n") ;
      exit(0) ;
    }
    readDOS(df, (char *)&car, 1) ;
    if (car != ' ') {
      escribirStr("\n Algo no esta bien en el sector ") ;
      escribirDec(i, 1) ;
      escribirStr(" byte 11 = '") ;
      escribirCar(car) ;
      escribirStr("' != ' '\n\n") ;
      exit(0) ;
    }
    for (j = 12 ; j < 512 ; j++) {
      readDOS(df, &car, 1) ;
      if (car != ('A' + ((i + j) % 26))) {
        escribirStr("\n Algo no esta bien en el sector ") ;
        escribirDec(i, 1) ;
        escribirStr(" byte ") ;
        escribirDec(j, 1) ;
        escribirStr(" = '") ;
        escribirCar(car) ;
        escribirStr("' != '") ;
        escribirCar('A' + ((i + j) % 26)) ;
        escribirStr("'\n\n") ;
        exit(0) ;
      }
    }
  }
  closeDOS(df) ;
}

void main ( void ) {

  comprobarError("L_S", "l_s 0", -2) ;
  comprobarError("L_S", "l_s 0 0", -3) ;
  comprobarError("L_S", "l_s 0 0 1", -4) ;
  comprobarError("L_S", "l_s 0 0 1 fichero otro1 otro2", -5) ;
  comprobarError("L_S", "l_s otracosa 0 1 fichero", -6) ;
  comprobarError("L_S", "l_s 2 0 1 fichero", -7) ;
  comprobarError("L_S", "l_s 0 otracosa 1 fichero", -10) ;
  comprobarError("L_S", "l_s 0 2880 1 fichero", -11) ;
  comprobarError("L_S", "l_s 0 0 otracosa fichero", -12) ;
  comprobarError("L_S", "l_s 0 0 0 fichero", -13) ;
  comprobarError("L_S", "l_s 0 2879 2 fichero", -14) ;

  comprobarError("E_S", "e_s fichero", -2) ;
  comprobarError("E_S", "e_s fichero 0", -3) ;
  comprobarError("E_S", "e_s fichero 0 1 otro1 otro2", -4) ;
  comprobarError("E_S", "e_s fichero otracosa 1", -5) ;
  comprobarError("E_S", "e_s fichero 2 1", -6) ;
  comprobarError("E_S", "e_s fichero 0 otracosa", -9) ;
  comprobarError("E_S", "e_s fichero 0 2880", -10) ;

  crearFicheroDePrueba("origen.bin") ;
  comprobarError("E_S", "e_s origen.bin 0 0", 0) ;
  comprobarError("L_S", "l_s 0 0 2880 destino.bin", 0) ;
  comprobarFichero("destino.bin") ;

  escribirStr("\n Ok\n") ;
  exit(0) ;

}

