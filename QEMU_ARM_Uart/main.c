#include "uart.h"
#include "print.h"

void KMain(void)
{
    init_uart();
    int64_t value_int = -1;
    uint64_t value_uint = 20;

    printk("Hello, Rasp\r\n");
    printk("Test number dec negativo %d\r\n",value_int);
    printk("Test number dec pos %u\r\n",value_uint);
    printk("Test number hex %x\r\n", value_uint);

    while (1) {
        ;
    }
}