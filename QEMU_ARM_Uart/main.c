#include "uart.h"
#include "print.h"
#include "debug.h"
#include "lib.h"
#include "handler.h"


void KMain(void)
{
    init_uart();
    int64_t value_int = -1;
    uint64_t value_uint = 20;

    printk("Hello, Rasp\r\n");
    printk("We are at EL %u\r\n", (uint64_t)get_el());

    //init_timer();
    init_interrupt_controller();
    enable_irq();

    while (1) {
        ;
    }
}