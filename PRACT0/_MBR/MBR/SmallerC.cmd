rem smlrcc -S -SI Y:\SO\PRACT0 -flat16 tpart.c
rem smlrcc -S -I Y:\SO\PRACT0 -flat16 tpart.c
smlrcc -S -I .. -dosh tpart.c
nasm tpart.asm -o tpart.bin
