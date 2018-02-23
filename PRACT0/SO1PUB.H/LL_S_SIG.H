/* ----------------------------------------------------------------------- */
/*                               ll_s_sig.h                                */
/* ----------------------------------------------------------------------- */
/*   biblioteca de funciones de interfaz de las llamadas al sistema SO1    */
/* ----------------------------------------------------------------------- */

#ifndef LL_S_SIG_H
#define LL_S_SIG_H

#define SIGHUP    1               /* hangup                                 */
#define SIGINT    2               /* interrupt (DEL)                        */  //
#define SIGQUIT   3               /* quit (ASCII FS)                        */ 
#define SIGILL    4               /* illegal instruction                    */
#define SIGTRAP   5               /* trace trap (not reset when caught)     */
#define SIGABRT   6               /* IOT instruction                        */
#define SIGBUS    7               /* bus error                              */
#define SIGFPE    8               /* floating point exception               */
#define SIGKILL   9               /* kill (cannot be caught or ignored)     */  //
#define SIGUSR1  10               /* user defined signal # 1                */  //
#define SIGSEGV  11               /* segmentation violation                 */
#define SIGUSR2  12               /* user defined signal # 2                */  //
#define SIGPIPE  13               /* write on a pipe with no one to read it */
#define SIGALRM  14               /* alarm clock                            */  //
#define SIGTERM  15               /* software termination signal from kill  */  //
#define SIGEMT   16               /* EMT instruction                        */
#define SIGCHLD  17               /* child process terminated or stopped    */  //

typedef void ( * __sighandler_t ) ( int ) ;

void SIG_IGN ( int sig ) ;           /* no se hace nada al recibir la señal */

void SIG_DFL ( int sig )  ;     /* se mata al proceso tras recibir la señal */

extern __sighandler_t __sighandler [ ] ;

typedef dword_t sigset_t ; /* ??????????? */

struct sigaction {
    __sighandler_t sa_handler ; /* SIG_DFL, SIG_IGN, or pointer to function */
    sigset_t sa_mask ;              /* signals to be blocked during handler */
    int sa_flags ;                                         /* special flags */
} ;

int kill_nueva ( pid_t pid, int sig ) ;

int sigaction ( int sig, const struct sigaction * act, struct sigaction * oact ) ;

unsigned int alarm ( unsigned int seconds ) ;

void pause ( void ) ;

#endif /* LL_S_SIG_H */







