@rem generacion de los ficheros objeto a partir de la
@rem biblioteca estandar CS.LIB de Borland C utilizando
@rem el comando tlib.
cd CS.LIB
tlib cs.lib * h_ldiv
tlib cs.lib * h_lursh
tlib cs.lib * h_llsh
tlib cs.lib * f_lxmul
tlib cs.lib * n_lxmul
move *.obj ..\obj
cd ..