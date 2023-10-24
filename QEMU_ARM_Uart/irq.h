#ifndef _IRQ_H
#define _IRQ_H
#include "memory.h"

//registradores de timer
//p2v converte de physical address to virtual address
#define CNTP_EL0            P2V(0x40000040)
#define CNTP_STATUS_EL0     P2V(0x40000060)

#define BASE_ADDR           P2V(0x3f000000)
#define IRQ_BASIC_PENDING   (BASE_ADDR + 0xB200)
#define ENBALE_IRQS_1       (BASE_ADDR + 0xB210)    //vai do irq 0 ~ 31 -> tabela de interrupts uart é 57
#define ENABLE_IRQS_2       (BASE_ADDR + 0xB214)    //vai do irq 32 ~ 63
#define ENABLE_BASIC_IRQS   (BASE_ADDR + 0xB218)
#define DISABLE_IRQS_1      (BASE_ADDR + 0xB21C)
#define DISABLE_IRQS_2      (BASE_ADDR + 0xB220)
#define DISABLE_BASIC_IRQS  (BASE_ADDR + 0xB224)

#endif