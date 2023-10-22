#include "uart.h"
#include "print.h"
#include "debug.h"

void KMain(void)
{
    init_uart();
    int64_t value_int = -1;
    uint64_t value_uint = 20;

    printk("Hello, Rasp\r\n");

    ASSERT(0);
    
    while (1) {
        ;
    }
}