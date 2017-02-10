

SO1 es un pequeño sistema operativo para la arquitectura 8086.
Esta escrito en C utilizando el compilador Borland C 5.2, los
ensambladores TASM32 y JWASM, y los enlazadores TLINK y JWLINK.
Esa toolchain puede utilizarse perfectamente sobre Windows en
sus versiones de 32 y 64 bits (en el caso de TLINK gracias al
uso del MSDOS Player de Takeda Toshiya).

SO1 simplemente pretende ser una herramienta para la enseñanza
de los sistemas operativos en las primeras etapas de estudio
de la materia. La compilación (sobre Windows) y la puesta en 
marcha del sistema, tanto sobre Windows (NTVDM) como sobre una 
máquina virtual (qemu, bochs, DOSBox, ...) como sobre una
máquina física (un PC estándar) de manera nativa utilizando un
pen drive USB, son tareas que requieren escasos minutos, a  
través de una corta secuencia de pasos sencillos, lo que
convierte a SO1 en un medio ideal para iniciar al alumno en
el conocimiento del funcionamiento interno de un sistema 
operativo.     
 
                  Pedro Pablo López Rodríguez

                     -- enero de 2017 --
