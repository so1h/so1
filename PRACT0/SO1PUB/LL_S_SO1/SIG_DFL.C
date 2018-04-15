#include <so1pub.h\ll_s_so1.h>

#pragma option -w-par

void SIG_DFL ( int sig )        /* se mata al proceso tras recibir la señal */
{
    exit(0) ;           /* en el estado de terminacion indicar la señal sig */
}