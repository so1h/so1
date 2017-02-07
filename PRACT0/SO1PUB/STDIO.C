/* ----------------------------------------------------------------------- */
/*                                  stdio.c                                */
/* ----------------------------------------------------------------------- */
/*                                                                         */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\ll_s_so1.h>    /* biblioteca de llamadas al sistema SO1 */
#include <so1pub.h\escribir.h>
#include <so1pub.h\stdargs>

/*-----------------------------------------------------------------------*

__vprinter is a table-driven design, for speed and flexibility. There are
two tables.  The first table classifies all 7-bit ASCII chars and then the
second table is the switch table which points to the function blocks which
handle the various classes of characters.

All characters with the 8th bit set are currently classed as don't cares,
which is the class of character also used for normal alphabetics.  All
characters less than ' ' (0x20) are also classed as don't cares.

*------------------------------------------------------------------------*/

typedef
enum
{
    _si,    /* sign fill +/-        */
    _af,    /* alternate form       */
    _ar,    /* format (width or precision) by argument */
    _lj,    /* left justify         */

    _pr,    /* precision            */
    _nu,    /* numeral              */
    _lo,    /* long                 */
    _ld,    /* long double          */
    _sh,    /* short                */
    _fz,    /* fill zeros           */

    _de,    /* decimal              */
    _oc,    /* octal                */
    _un,    /* unsigned decimal     */
    _he,    /* hexadecimal          */

    _pt,    /* pointer              */
    _fl,    /* float                */
    _ch,    /* char                 */
    _st,    /* string               */

    _ns,    /* number sent          */
    _zz,    /* terminator           */
    _dc,    /* don't care           */
    _pc,    /* percent              */

    _ne,    /* near pointer         */
    _fa,    /* far pointer          */
} characterClass;

/*  Here is the table of classes, indexed by character. */

static unsigned char printCtype [96] =
{
    /*       SP   !   "   #   $   %   &   '   (   )   *   +   ,   -   .   /  */
    _si,_dc,_dc,_af,_dc,_pc,_dc,_dc,_dc,_dc,_ar,_si,_dc,_lj,_pr,_dc,

    /*        0   1   2   3   4   5   6   7   8   9   :   ;   <   =   >   ?  */
    _fz,_nu,_nu,_nu,_nu,_nu,_nu,_nu,_nu,_nu,_dc,_dc,_dc,_dc,_dc,_dc,

    /*        _@   A   B   C   D   E   F   G   H   I   J   K   L   M   N   O  */
    _dc,_dc,_dc,_dc,_dc,_fl,_fa,_fl,_sh,_dc,_dc,_dc,_ld,_dc,_ne,_dc,

    /*        P   Q   R   S   T   U   V   W   X   Y   Z   [   \   ]   ^   _  */
    _dc,_dc,_dc,_dc,_dc,_dc,_dc,_dc,_he,_dc,_dc,_dc,_dc,_dc,_dc,_dc,

    /*        `   a   b   c   d   e   f   g   h   i   j   k   l   m   n   o  */
    _dc,_dc,_dc,_ch,_de,_fl,_fl,_fl,_sh,_de,_dc,_dc,_lo,_dc,_ns,_oc,

    /*        p   q   r   s   t   u   v   w   x   y   z   {   |   }   ~ DEL  */
    _pt,_dc,_dc,_st,_dc,_un,_dc,_dc,_he,_dc,_dc,_dc,_dc,_dc,_dc,_dc,
};


/*---------------------------------------------------------------------*

Name            __vprinter - sends formatted output

Usage           int   pascal __vprinter (putnF  *putter,
                                         const char   *formP,
                                         void _ss *argP)

Prototype in    _printf.h

Description     The list of arguments *argP is combined with literal text in
                the format string *formP according to format specifications
                inside the format string.

                The supplied procedure *putter is used to generate the output.
                It is required to take the string S, which has been
                constructed by __vprinter, and copy it to the destination
                outP.  The destination may be a string, a FILE, or whatever
                other class of construct the caller requires.  It is possible
                for several calls to be made to *putter since the buffer S
                is of limited size.

                *putter is required to preserve  SI, DI.

                The only purpose of the outP argument is to be passed through
                to putter.

                The object at *argP is a record of unknown structure, which
                structure is interpreted with the aid of the format string.
                Each field of the structure may be an integer, long, double,
                or string (char *).  Chars may appear but occupy integer-sized
                cells.  Floats, character arrays, and structures may not
                appear.

Return value    The result of the function is a count of the characters sent to
                *outP.

                There is no error indication.  When an incorrect conversion
                spec is encountered __vprinter copies the format as a literal
                (since it is assumed that alignment with the argument list has
                been lost), beginning with the '%' which introduced the bad
                format.

                If the destination outP is of limited size, for example a
                string or a full disk, __vprinter does not know.  Overflowing
                the destination causes undefined results.  In some cases
                *putter is able to handle overflows safely, but that is not
                the concern of __vprinter.

                The syntax of the format string is:

                format ::=      ([literal] ['%' conversion ])* ;

                conversion ::=  '%' | [flag]* [width] ['.' precision]
                                      ['l'] type ;

                flag ::=        '-' | '+' | ' ' | '#' | '0' ;

                width ::=       '*' | number ;
                precision ::=   '.' ('*' | number) ;

                type ::=        'd'|'i'|'o'|'u'|'x'|'n'|'X'|'f'|'e'|'E'|
                                'g'|'G'|'c'|'s'|'p'|'N'|'F'

*---------------------------------------------------------------------*/

#define __CVTMAX__ 48 /* tomado de VPRINTER.C */

#define Ssize 80

typedef
enum
{
    flagStage, fillzStage, wideStage, dotStage, precStage,
    ellStage, typeStage,
} syntaxStages;

typedef
enum
{
    altFormatBit = 1,       /* the '#' flag                 */
    leftJustBit  = 2,       /* the '-' flag                 */
    notZeroBit   = 4,       /* 0 (octal) or 0x (hex) prefix */
    fillZerosBit = 8,       /* zero fill width              */
    isLongBit    = 16,      /* long-type argument           */
    farPtrBit    = 32,      /* far pointers                 */
    alt0xBit     = 64,      /* '#' confirmed for %x format  */
    floatBit     = 128,     /* float arg 4 bytes not 8!     */
    LongDoubleBit= 256      /* signal a long double argument*/
} flagBits;

flagBits flagSet;

unsigned aP;
char     fc;                    /* format char, from fmt string */
char     isSigned;              /* chooses signed/unsigned ints */
int      width;
int      precision;
char     plusSign;
int      leadZ;
unsigned abandonP;              /* posn of bad syntax in fmt str*/
char     tempStr [__CVTMAX__];  /* longest _realcvt or longtoa string*/
unsigned totalSent = 0;         /* characters sent to putter    */
unsigned Scount = Ssize;        /* free space remaining in S    */
char     S [Ssize];             /* temp working result buffer   */
int      eof_error = 0;         /* flag, TRUE is EOF error      */

#if 0   /* the remaining variables are held entirely in registers       */

char    hexCase;                /* upper/lower Hex alphabet     */
long    tempL;
syntaxStages    stage;
-- CH
char            c;
char            *cP;
int             *iP;
#endif

/*
General outline of the method:

First the string is scanned, and conversion specifications detected.

The preliminary fields of a conversion (flags, width, precision, long)
are detected and noted.

The argument is fetched and converted, under the optional guidance of
the values of the preliminary fields.  With the sole exception of the
's' conversion, the converted strings are first placed in the tempStr
buffer.

The contents of the tempStr (or the argument string for 's') are copied
to the output, following the guidance of the preliminary fields in
matters such as zero fill, field width, and justification.
*/

#define fmt_flag_cero        0x01
#define fmt_flag_almohadilla 0x02
#define fmt_flag_mas         0x04
#define fmt_flag_menos       0x08
#define fmt_flag_espacio     0x10

int printf ( const char far * fmt, ... )
{

    int i ;
    unsigned char car ;
    byte_t fmt_flags ;
    bool_t fin_fmt_flags ;
    word_t anchura ;
    word_t anchuraSolicitada ;
    word_t precision ;
    word_t precisionSolicitada ;
    char fmt_tipo ;

//byte_t pCt ;

//int cont ;

    va_list ap ;
    va_start(ap, fmt) ;
//escribirStr(fmt) ;

    i = 0 ;
//cont = 0 ;
//  while ((car = (unsigned char)fmt[i]) >= ' ') {
    while ((car = (unsigned char)fmt[i]) > (char)0)
    {
        if (car != '%')
        {
            escribirCar(car) ;
            if (car == '\n')
                escribirCar('\r') ;
            i++ ;
        }
        else                    /* se ha encontrado una descripcion de formato */
        {
            /* %s, %u, %d, %x, %X, %lu, %ld, %lx, %lX, 5x, 05x */
            fmt_flags   = 0x00 ;
            fin_fmt_flags = FALSE ;
            do
            {
                i++ ;
//      cont++ ;
                car = (unsigned char)fmt[i] ;
//      pCt = printCtype[car - ' '] ;
                switch (car)
                {
                case '-' :
                    fmt_flags = fmt_flags + fmt_flag_menos ;
                    break ;
                case '+' :
                    fmt_flags = fmt_flags + fmt_flag_mas ;
                    break ;
                case ' ' :
                    fmt_flags = fmt_flags + fmt_flag_espacio ;
                    break ;
                case '#' :
                    fmt_flags = fmt_flags + fmt_flag_almohadilla ;
                    break ;
                case '0' :
                    fmt_flags = fmt_flags + fmt_flag_cero ;
                    break ;
                default  :
                    fin_fmt_flags = TRUE ;
                    break ;
                }
            }
            while (!fin_fmt_flags) ;

            anchuraSolicitada = 0 ;                   /* valor por defecto */
            if (car == '*')
            {
                anchuraSolicitada = *((word_t *)ap) ;
                va_arg(ap, word_t) ;
                i++ ;
                car = (unsigned char)fmt[i] ;
            }
            else
            {
                while (('0' <= car) && (car <= '9'))
                {
                    anchuraSolicitada += (word_t)(car - '0') ;
                    i++ ;
                    car = fmt[i] ;
                }
            }

            precisionSolicitada = 0 ;                 /* valor por defecto */
            if (car = '.')
            {
                i++ ;
                car = (unsigned char)fmt[i] ;
                if (car == '*')
                {
                    precisionSolicitada = *((word_t *)ap) ;
                    va_arg(ap, word_t) ;
                    i++ ;
                    car = (unsigned char)fmt[i] ;
                }
                else
                {
                    while (('0' <= car) && (car <= '9'))
                    {
                        precisionSolicitada += (word_t)(car - '0') ;
                        i++ ;
                        car = fmt[i] ;
                    }
                }
            }

            if (car == 'l')
            {
                i++ ;
                car = (unsigned char)fmt[i] ;
            }

            fmt_tipo = fmt[i] ;

            if (fmt[i] == 's')
            {
                escribirStr(*((char * *)ap)) ;
                va_arg(ap, char *) ;
                i++ ;
            }
            else if (fmt[i] == 'u')
            {
                escribirDec(*((word_t *)ap), 1) ;
                va_arg(ap, word_t) ;
                i++ ;
            }
            else if (fmt[i] == 'd')
            {
                escribirInt(*((word_t *)ap), 1) ;
                va_arg(ap, word_t) ;
                i++ ;
            }
            else if (fmt[i] == 'x')
            {
                escribirStr("0x") ;
                escribirHex(*((word_t *)ap), 1) ;
                va_arg(ap, word_t) ;
                i++ ;
            }
        }
    }


#if (FALSE)
    escribirStr("\n contador = ") ;
    escribirDec(cont, 1) ;

    escribirStr("\n segundo argumento = 0x") ;
    escribirHex(*((word_t *)ap), 4) ;

    va_arg(ap, word_t) ;
    escribirStr("\n tercer argumento = 0x") ;
    escribirHex(*((word_t *)ap), 4) ;

    va_arg(ap, char *) ;
    escribirStr("\n cuarto argumento = ") ;
    escribirStr(*((char * *)ap)) ;
#endif

    va_end(ap) ;
    return(0) ;
}

char getch ( void )
{
    return(leer(STDIN)) ;
}

