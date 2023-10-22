#include "stdio.h"
#include "print.h"

void handler(uint64_t numid, uint64_t esr, uint64_t elr)
{
    switch (numid)
    {
    case 1:                                 //só trata o erro de sync
        printk("sync error at %x: %x\r\n", elr,esr);
        while (1){}
        break;
    
    default:
        printk("unknown exception\r\n");
        while(1) {}
        break;
    }

}