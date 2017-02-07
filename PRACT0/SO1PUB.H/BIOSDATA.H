/* ----------------------------------------------------------------------- */
/*                                biosdata.h                               */
/* ----------------------------------------------------------------------- */
/*                                                                         */
/* ----------------------------------------------------------------------- */

#ifndef BIOSDATA_H
#define BIOSDATA_H

#include <so1pub.h\tipos.h>

typedef struct {
 unsigned int HW_LPT_cnt  : 2 ;          /* number of parallel ports */
 unsigned int HWLIST_13   : 1 ;
 unsigned int HW_gameport : 1 ;          /* game port present */
 unsigned int HW_COM_cnt  : 3 ;          /* number of serial ports */
 unsigned int HWLIST_8    : 1 ;
 unsigned int HW_FDD_cnt  : 2 ;          /* number of floppy disk drives-1 */
 unsigned int HW_vidmode  : 2 ;          /* initial video mode */
                                         /* 00 EGA/VGA,   01 40x25 CGA */
                                         /* 10 80x25 CGA, 11 80x25 mono */
 unsigned int HWLIST_3    : 1 ;
 unsigned int HW_PS2      : 1 ;          /* PS/2 pointing device present */
 unsigned int HW_FPU      : 1 ;          /* math coprocessor present */
 unsigned int HW_FDD      : 1 ;          /* floppy disk drives present */
} HWLIST_t ;

/* flags del teclado 0 y 1 */

typedef struct {
  unsigned int KBD_RShift_press  : 1 ; /* bit 0 */  /* Right Shift pressed */
  unsigned int KBD_LShift_press  : 1 ; /* bit 1 */  /* Left Shift pressed */
  unsigned int KBD_Ctrl_press    : 1 ; /* ..... */  /* either Ctrl pressed */
  unsigned int KBD_Alt_press     : 1 ;              /* either Alt pressed */
  unsigned int KBD_ScrollLock    : 1 ;
  unsigned int KBD_NumLock       : 1 ;
  unsigned int KBD_CapsLock      : 1 ;
  unsigned int KBD_INSERT        : 1 ;
  unsigned int KBD_LCtrl_pressed : 1 ;              /* Left Ctrl pressed */
  unsigned int KBD_LAlt_pressed  : 1 ;              /* Left Alt pressed */
  unsigned int KBD_SysReq_press  : 1 ;
  unsigned int KBD_Pause         : 1 ;              /* Pause state active */
  unsigned int KBD_Scroll_press  : 1 ;              /* Scroll Lock pressed */
  unsigned int KBD_Num_press     : 1 ;              /* Num Lock pressed */
  unsigned int KBD_Caps_press    : 1 ;              /* Caps Lock pressed */
  unsigned int KBD_INS_press     : 1 ;
} KBDFLAGS_t ;

/* flags del teclado 2 */

typedef struct {
  unsigned int KBDSTATUS_0       : 2 ;
  unsigned int KBD_RCtrl_pressed : 1 ; /* Right Ctrl pressed */
  unsigned int KBD_RAlt_pressed  : 1 ; /* Right Alt pressed */
  unsigned int KBD_AT            : 1 ; /* 101-key enhanced keyboard present */
  unsigned int KBDSTATUS_5       : 3 ;
  unsigned int KBD_Scroll_LED    : 1 ; /* Scroll Lock LED */
  unsigned int KBD_Num_LED       : 1 ; /* Num Lock LED */
  unsigned int KBD_Caps_LED      : 1 ; /* Caps Lock LED */
} KBDSTATUS_t ;

/* definiciones de bits para los flags del teclado 0, 1 y 2 */

#define KF0_RSHIFT       (1 << 0)  /* KBDFLAGS_t */
#define KF0_LSHIFT       (1 << 1)
#define KF0_CTRLACTIVE   (1 << 2)
#define KF0_ALTACTIVE    (1 << 3)
#define KF0_SCROLLACTIVE (1 << 4)
#define KF0_NUMACTIVE    (1 << 5)
#define KF0_CAPSACTIVE   (1 << 6)

#define KF1_LCTRL        (1 << 0)
#define KF1_LALT         (1 << 1)
#define KF1_PAUSEACTIVE  (1 << 3)
#define KF1_SCROLL       (1 << 4)
#define KF1_NUM          (1 << 5)
#define KF1_CAPS         (1 << 6)

#define KF2_LAST_E1      (1 << 0)  /* KBDSTATUS_t */
#define KF2_LAST_E0      (1 << 1)
#define KF2_RCTRL        (1 << 2)
#define KF2_RALT         (1 << 3)
#define KF2_101KBD       (1 << 4)

typedef struct {
  unsigned int VCTRL_clear      : 1 ; /* high bit of video mode */
                                      /* =1 screen is not cleared */
  unsigned int VCTRL_RAM_64K    : 2 ; /* RAM on adapter-1 in 64K */
  unsigned int VCTRL_4          : 1 ;
  unsigned int VCTRL_inactive   : 1 ; /* EGA/VGA video system inactive */
  unsigned int VCTRL_wait       : 1 ; /* wait for display enable */
  unsigned int VCTRL_mono       : 1 ; /* display is mono */
  unsigned int VCTRL_notemulate : 1 ; /* =0 cursor shape treated as CGA ones */
} VIDEOCONTROL_t ;

typedef struct {
  unsigned int drive0rec         : 1 ;          /* 1 = recalibrate drive 0 */
  unsigned int drive1rec         : 1 ;          /* 1 = recalibrate drive 1 */
  unsigned int drive2rec         : 1 ;          /* 1 = recalibrate drive 2 */
  unsigned int drive3rec         : 1 ;          /* 1 = recalibrate drive 3 */
  unsigned int relleno           : 3 ;                           /* unused */
  unsigned int workingInterrupt  : 1 ;       /* 1 = working interrupt flag */
} DRECSTATUS_t ;                             /* drive recalibration status */

typedef struct {
  unsigned int drive0on          : 1 ;             /* 1 = drive 0 motor on */
  unsigned int drive1on          : 1 ;             /* 1 = drive 1 motor on */
  unsigned int drive2on          : 1 ;             /* 1 = drive 2 motor on */
  unsigned int drive3on          : 1 ;             /* 1 = drive 3 motor on */
  unsigned int relleno           : 3 ;                           /* unused */
  unsigned int writeOp           : 1 ;              /* 1 = write operation */
} DMOTORSTATUS_t ;                                /* disquete motor status */

typedef struct {
  unsigned int invalidFDcmd      : 1 ;         /* invalid disquete command */
  unsigned int adrMarkNotFound   : 1 ;  /* diskette address mark not found */
  unsigned int secNotFound       : 1 ;                 /* sector not found */
  unsigned int fdDMAError        : 1 ;               /* diskette DMA error */
  unsigned int CRCDataError      : 1 ;           /* CRC check / data error */
  unsigned int fdcFailure        : 1 ;      /* diskette controller failure */
  unsigned int seekTrackFailed   : 1 ;             /* seek to track failed */
  unsigned int fdTimeout         : 1 ;                /* diskette time-out */
} FDOPSTATUS_t ;                      /* status of last disquete operation */


typedef struct {                        /* EGA/VGA video switches */
  unsigned int VIDSW_feature   : 3 ;    /* power-on state of feature */
                                        /* connector bits 3-1 */
  unsigned int VIDSW_feature0  : 1 ;    /* =0 enhanced color display (ECD) */
  unsigned int VIDSW_display   : 4 ;    /* adapters+attached display */
                   /*      primary                 secondary               */
                   /* 00h  MDA                     EGA+40x25 color display */
                   /* 01h  MDA                     EGA+80x25 color display */
                   /* 02h  MDA                     EGA+ECD (CGA emulation) */
                   /* 03h  MDA                     EGA+ECD                 */
                   /* 04h  CGA+40x25 color display EGA+mono display        */
                   /* 05h  CGA+80x25 color display EGA+mono display        */
                   /* 06h  EGA+40x25 color display MDA                     */
                   /* 07h  EGA+80x25 color display MDA                     */
                   /* 08h  EGA+ECD (CGA emulation) MDA                     */
                   /* 09h  EGA+ECD                 MDA                     */
                   /* 0Ah  EGA+mono display        CGA+40x25 color display */
                   /* 0Ah  EGA+mono display        CGA+80x25 color display */
} VIDEOSWITCHES_t ;

typedef struct {                      /* MCGA/VGA mode set option control */
  unsigned int VGA_scan2       : 1 ;  /* =0 350-line mode when VGA_scan=0 */
                                      /* =1 200-line mode when VGA_scan=0 */
  unsigned int VGA_DCC_enabled : 1 ;  /* DCC display switching enabled */
  unsigned int VGAOPTIONS_1    : 1 ;
  unsigned int VGA_scan        : 1 ;  /* =1 400-line mode at next mode set */
                                      /* =0 350/200-line mode */
  unsigned int VGA_nopalette   : 1 ;  /* default palette loading disabled */
  unsigned int VGA_grayscale   : 1 ;  /* gray scale summing enabled */
  unsigned int VGA_active      : 1 ;  /* =1 VGA active (MCGA always 0) */
} VGAOPTIONS_t ;

typedef struct {
  pointer_t VIDEO_paramtbl ;               /* ptr to Video Parameter Table */
  pointer_t VIDEO_dynsavearea ;      /* ptr to Parameter Dynamic Save Area */
  pointer_t VIDEO_text_charset ;     /* ptr to Text Character Set Override */
  pointer_t VIDEO_graph_charset ;/* ptr to Graphics Character Set Override */
  pointer_t VIDEO_savetbl2 ;  /* [VGA] ptr to Secondary Save Pointer Table */
} VIDEOSAVETBL_t ;              /* Video Save Pointer Table [EGA/VGA/MCGA] */

/* Video Parameter Table:                                                  */
/*       VIDEOPARAM 4 dup(?)  ; modes 0-3 in 200-line CGA emulation mode   */
/*       VIDEOPARAM 11 dup(?) ; modes 4-0Eh                                */
/*       VIDEOPARAM 2 dup(?)  ; modes 0Fh-10h when only 64K RAM on adapter */
/*       VIDEOPARAM 2 dup(?)  ; modes 0Fh-10h when >64K RAM on adapter     */
/*       VIDEOPARAM 4 dup(?)  ; modes 0-3 in 350-line mode                 */
/*       VIDEOPARAM ?         ; [VGA] modes 0 or 1 in 400-line mode        */
/*       VIDEOPARAM ?         ; [VGA] modes 2 or 3 in 400-line mode        */
/*       VIDEOPARAM ?         ; [VGA] modes 7 in 400-line mode             */
/*       VIDEOPARAM 3 dup(?)  ; [VGA] modes 11h-13h                        */

typedef struct {
  byte_t VPARAM_width ;     /* screen width in text columns */
  byte_t VPARAM_height ;    /* screen height in text rows-1 */
  byte_t VPARAM_hchar ;     /* character height in scan lines/bytes */
  word_t VPARAM_pagesize ;  /* video page (regen buffer) size in bytes */
  byte_t VPARAM_SEQC [4] ;  /* values for Sequencer Registers 1-4 (3C4h) */
  byte_t VPARAM_MISC ;      /* value for Miscellaneous Output Reg (3C2h) */
  byte_t VPARAM_CRTC [25] ; /* values for CRTC Registers (3x4h) */
  byte_t VPARAM_ATC [20] ;  /* values for Attribute Controller Regs (3C0h) */
  byte_t VPARAM_GRC [9] ;   /* values for Graphics Controller Regs (3CEh) */
} VIDEOPARAM_t ;                               /* sizeof(VIDEOPARAM) == 64 */

typedef struct {

  dword_t IVT [256] ;           /* Interrupt Vectors Table */

  word_t COM_base [4] ;         /* 400: base IO address of serial ports */
  word_t LPT_base [3] ;         /* 408: base IO address of parallel ports */
  word_t XBDA_seg ;             /* 40E: segment of eXtended BIOS data area */
  HWLIST_t HW_list ;            /* 410: detected hardware/equipment list */
  byte_t relleno1 ;
  word_t basemem_K ;            /* 413: base memory size in Kb */
  word_t relleno2 ;

  KBDFLAGS_t KBD_flags ;        /* 417: keyboard status flags */
  byte_t relleno3 ;             /* 419: Alt-nnn keypad workspace */
  word_t KBD_bufhead ;          /* 41A: pointer to next character in buffer */
  word_t KBD_buftail ;          /* 41C: pointer to free space in buffer */
  word_t KBD_buffer [16] ;      /* 41E: keyboard circular buffer */

  DRECSTATUS_t dRecStatus ;     /* 43E: drive recalibration status */
  DMOTORSTATUS_t dMotorStatus ; /* 43F: disquete motor status */
  byte_t motorShutoffCounter ;  /* 440: motor shutoff counter dec by INT8 */
  FDOPSTATUS_t fdOpStatus ;     /* 441: status of last disquete operation */
  byte_t relleno4 [7] ;         /* 442: NEC disquete controller status */

  byte_t VIDEO_mode ;           /* 449: current video mode */
  word_t VIDEO_width ;          /* 44A: screen width in text columns */
  word_t VIDEO_pagesize ;       /* 44C: video page (regen buffer) size in bytes */
  word_t VIDEO_pageoff ;        /* 44E: active page offset in video segment */
  byte_t CURSOR_pos [8][2] ;    /* 450: cursor column/row on each video page */
  byte_t CURSOR_shape [2] ;     /* 460: cursor end/start scan lines */
  byte_t VIDEO_pagActiva ;      /* 462: active page number */
  word_t CRTC_base ;            /* 463: base IO address of CRT controller (3x4h) */
  byte_t CRT_MODE ;             /* 465: value of CRT mode select register (3x8h) */
  byte_t CRT_PALETTE ;          /* 466: value of CRT palette register (3x9h) */
  byte_t relleno5 [5] ;

  dword_t BIOS_timer ;          /* 46C: timer ticks since midnight */
  byte_t BIOS_overflow ;        /* 470: timer overflow flag (timer has counted */
                                /*       past midnight) */

  byte_t BIOS_ctrlbreak ;       /* 471: bit 7 set when Ctrl-Break has been pressed */
  word_t BIOS_POSTreset ;       /* 472: specifies the action at POST when reboot */
  byte_t relleno6 ;
  byte_t BIOS_HD_cnt ;          /* 475: number of fixed disk drives */
  byte_t relleno7 [2] ;
  byte_t LPT_timeout [3] ;      /* 478: parallel device timeout counters */
  byte_t BIOS_4B_flags ;        /* 47B: INT 4B flags */
  byte_t COM_timeout [4] ;      /* 47C: serial device timeout counters */

  word_t KBD_start ;            /* 480: keyboard buffer start offsets in 40:0 */
  word_t KBD_end1 ;             /* 481: keyboard buffer end+1 offsets in 40:0 */

  byte_t VIDEO_lastrow ;        /* 484: last text row on screen (count from 0) */
  word_t VIDEO_hchar ;          /* 485: character height in scan lines/bytes */
  VIDEOCONTROL_t VIDEO_control ;/* 487: EGA/VGA control */
  VIDEOSWITCHES_t VIDEO_switches ; /* 488: EGA/VGA switches */
  VGAOPTIONS_t VGA_options ;    /* 489: MCGA/VGA mode set option control */
  byte_t VGA_DCC_index ;        /* 48A: index into display combination code table */
  byte_t relleno8 [11] ;

  KBDSTATUS_t KBD_status ;      /* 496: keyboard status flags */

  dword_t TIMER_waitflag ;      /* 498: pointer to user wait flag */
  dword_t TIMER_waitmks ;       /* 49C: wait counter in mks */
  byte_t TIMER_active ;         /* 4A0: =0 acknowledged, =1 busy, =80h elapsed */
  byte_t relleno9 [7] ;

  dword_t VIDEO_ptrtable ;      /* 4A8: pointer to video save pointer table */
  byte_t relleno10 [68] ;

  byte_t IAC [16] ;             /* 4F0: Inter-Application Communication Area */

} BIOSAREA_t ;                  /* sizeof(BIOSAREA) = 1024+256 = 1280 */

extern volatile BIOSAREA_t far * ptrBiosArea ;

extern pointer_t ptrFechaBios ;

#endif /* BIOSDATA_H */


