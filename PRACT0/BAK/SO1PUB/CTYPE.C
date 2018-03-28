/* ----------------------------------------------------------------------- */
/*                                 ctype.c                                 */
/* ----------------------------------------------------------------------- */
/*                                                                         */
/* ----------------------------------------------------------------------- */

int tolower ( int car ) 
{
    if (('A' <= car) && (car <= 'Z')) 
//      return('a' + (car - 'A')) ;
        return(car | 0x20) ;                                  /* bit 5 a 1 */
    else if (car == '¥') return('¤') ;                       /* letra enie */
    return(car) ;
}

int toupper ( int car ) 
{ 
    if (('a' <= car) && (car <= 'z')) 
//      return('A' + (car - 'a')) ;
        return(car & 0xDF) ;                                  /* bit 5 a 0 */
    else if (car == '¤') return('¥') ;                       /* letra enie */
    return(car) ;
}

int isprint ( int car ) 
{
    return((' ' < car) & (car < '~')) ;
}