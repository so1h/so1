#include <so1pub.h\ll_s_so1.h>

long lseek ( int df, long pos, word_t whence  )
{
#if (FALSE)
    asm {
        mov bx,df ;
        les dx,pos ;
        mov cx,whence ;
        mov ax,LSEEK ;
        int nVIntSO1 ;
    }
    return((((long)_BX) << 16) | _AX) ;
#else	
  dword_t res ;
  word_t res_L ;
  word_t res_H ;
  asm {
    mov bx,df ;
    les dx,pos ;
    mov cx,whence ;
    mov ax,0106h ;
    int nVIntSO1 ;
    mov res_L,ax ;
    mov res_H,bx ;
  }
  res = (((dword_t)res_H) << 16) | res_L ;
  return(res) ;
#endif
}