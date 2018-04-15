#include <so1pub.h\ll_s_so1.h>

int __handlerReturn ( int errorAX )
{
    if (errorAX == 0)
        return(_AX) ;
    else
        return(errorAX) ;
}