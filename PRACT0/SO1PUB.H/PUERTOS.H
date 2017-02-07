/* ----------------------------------------------------------------------- */
/*                                 puertos.h                               */
/* ----------------------------------------------------------------------- */
/*                                                                         */
/* ----------------------------------------------------------------------- */

#ifndef PUERTOS_H
#define PUERTOS_H

#include <so1pub.h\tipos.h>

#define PORT_DMA_ADDR_2        0x0004
#define PORT_DMA_CNT_2         0x0005
#define PORT_DMA1_MASK_REG     0x000A
#define PORT_DMA1_MODE_REG     0x000B
#define PORT_DMA1_CLEAR_FF_REG 0x000C
#define PORT_DMA1_MASTER_CLEAR 0x000D
#define PORT_PIC1_CMD          0x0020                  /* PIC 8259 Master */
#define PORT_PIC1_DATA         0x0021
#define PORT_PIT_COUNTER0      0x0040
#define PORT_PIT_COUNTER1      0x0041
#define PORT_PIT_COUNTER2      0x0042
#define PORT_PIT_MODE          0x0043
#define PORT_PS2_DATA          0x0060                          /* Teclado */
#define PORT_PS2_CTRLB         0x0061
#define PORT_PS2_STATUS        0x0064
#define PORT_CMOS_INDEX        0x0070
#define PORT_CMOS_DATA         0x0071
#define PORT_DIAG              0x0080
#define PORT_DMA_PAGE_2        0x0081
#define PORT_A20               0x0092
#define PORT_PIC2_CMD          0x00A0                  /* PIC 8259 Slave  */
#define PORT_PIC2_DATA         0x00A1
#define PORT_SMI_CMD           0x00B2
#define PORT_SMI_STATUS        0x00B3
#define PORT_DMA2_MASK_REG     0x00D4
#define PORT_DMA2_MODE_REG     0x00D6
#define PORT_DMA2_MASTER_CLEAR 0x00Da
#define PORT_MATH_CLEAR        0x00F0
#define PORT_ATA2_CMD_BASE     0x0170
#define PORT_ATA1_CMD_BASE     0x01F0
#define PORT_LPT2              0x0278
#define PORT_SERIAL4           0x02E8
#define PORT_SERIAL2           0x02F8
#define PORT_ATA2_CTRL_BASE    0x0374
#define PORT_LPT1              0x0378
#define PORT_SERIAL3           0x03E8
#define PORT_ATA1_CTRL_BASE    0x03F4
#define PORT_FD_DOR            0x03F2
#define PORT_FD_STATUS         0x03F4
#define PORT_FD_DATA           0x03F5
#define PORT_HD_DATA           0x03F6
#define PORT_FD_DIR            0x03F7
#define PORT_SERIAL1           0x03F8
#define PORT_PCI_CMD           0x0CF8
#define PORT_PCI_DATA          0x0CFC
#define PORT_BIOS_DEBUG        0x0402
#define PORT_QEMU_CFG_CTL      0x0510
#define PORT_QEMU_CFG_DATA     0x0511
#define PORT_ACPI_PM_BASE      0xB000
#define PORT_SMB_BASE          0xB100
#define PORT_BIOS_APM          0x8900

                           /* PORT_SERIAL: desplazamiento de los registros */

#define SEROFF_DATA  0
#define SEROFF_THR   0           /* Transmiter Holding Register          W */
#define SEROFF_RBR   0           /* Receiver Buffer Register           R   */
#define SEROFF_DLL   0           /* Divisor Latch Low byte               W */
#define SEROFF_IER   1           /* Interrupt Enable Register            W */
#define SEROFF_DLH   1           /* Divisor Latch High byte              W */
#define SEROFF_IIR   2           /* Interrupt Identification Register  R   */
#define SEROFF_FCR   2           /* Fifo Control Register                W */
#define SEROFF_LCR   3           /* Line Control Register              R W */
#define SEROFF_MCR   4           /* Modem Control Register               W */
#define SEROFF_LSR   5           /* Line Status Register               R   */
#define SEROFF_MSR   6           /* Modem Status Register              R   */

                         /* PORT_PARALLEL: desplazamiento de los registros */

#define PAROFF_DATA     0                                           /* R W */
#define PAROFF_STATUS   1                                           /* R   */
#define PAROFF_CONTROL  2                                           /*   W */

/* PIC1 (Master) */                        /* Inicialization Control Word0 */

#define PORT_PIC1_ICW1         0x0020           /* W         ICW1 0001LBS4 */
#define PORT_PIC1_ICW234       0x0021           /* W         ICW2 VVVVV000 */
                                                /* W Master  ICW3 SSSSSSSS */
                                                /* W Slave   ICW3 00000IRQ */
                                                /* W         ICW4 000NBBE1 */

                                                 /* Operation Control Word */

#define PORT_PIC1_OCW23        0x0020           /* W   ICR   OCW2 EEE00IRL */
                                                /* R/W       OCW3 05S01PAR */
#define PORT_PIC1_OCW1         0x0021           /* R/W IMR   OCW1 MMMMMMMM */

#define PORT_PIC1_IMR          0x0021        /* Interrupt Mask Register    */
#define PORT_PIC1_ICR          0x0020        /* Interrupt Command Register */
#define PORT_PIC1_IRR          0x0020        /* Interrupt Request Register */
#define PORT_PIC1_ISR          0x0020        /* In Service Register        */


/* PIC2 (Slave) */                         /* Inicialization Control Word0 */

#define PORT_PIC2_ICW1         0x00A0           /* W         ICW1 0001LBS4 */
#define PORT_PIC2_ICW234       0x00A1           /* W         ICW2 VVVVV000 */
                                                /* W Master  ICW3 SSSSSSSS */
                                                /* W Slave   ICW3 00000IRQ */
                                                /* W         ICW4 000NBBE1 */

                                                 /* Operation Control Word */

#define PORT_PIC2_OCW23        0x00A0           /* W   ICR   OCW2 EEE00IRL */
                                                /* R/W       OCW3 05S01PAR */
#define PORT_PIC2_OCW1         0x00A1           /* R/W IMR   OCW1 MMMMMMMM */

#define PORT_PIC2_IMR          0x00A1        /* Interrupt Mask Register    */
#define PORT_PIC2_ICR          0x00A0        /* Interrupt Command Register */
#define PORT_PIC2_IRR          0x00A0        /* Interrupt Request Register */
#define PORT_PIC2_ISR          0x00A0        /* In Service Register        */

/* End Of Interrupt (No especifico) */

#define PIC_EOI         0x20           /* 00100000 caso particular de OCW2 */


#define A20_ENABLE_BIT  0x02              /* PORT_A20: bit de habilitacion */

#define NMI_DISABLE_BIT 0x80     /* PORT_CMOS_INDEX: bit de inhibicion NMI */


byte_t inportb ( word_t port ) ;

word_t inport ( word_t port ) ;

void outport ( word_t port, word_t val ) ;

void outportb ( word_t port, byte_t val ) ;

byte_t inportb_r ( byte_t port ) ;

void outportb_r ( byte_t port, byte_t val ) ;

word_t contadorTimer0 ( void ) ;   /* valor actual del contador 0 del timer */

#endif /* PUERTOS_H */
