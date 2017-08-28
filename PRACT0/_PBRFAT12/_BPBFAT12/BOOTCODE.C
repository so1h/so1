/***************************************************************************/
/*                                bootcode.c                               */
/***************************************************************************/
/*               codigo de arranque vacio para hacer pruebas               */
/***************************************************************************/

asm
{
    assume NOTHING
    _bootCode proc near
    /* aqui no hay nada de nada */
    _bootCode endp
    public _bootCode
}
