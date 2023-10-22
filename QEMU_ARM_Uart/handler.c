#include "stdio.h"
#include "print.h"
#include "lib.h"
#include "irq.h"

void enable_timer(void);
uint32_t read_timer_status(void);
void set_timer_interval(uint32_t value);
uint32_t read_timer_freq(void);

static uint32_t timer_interval = 0;
static uint64_t ticks = 0;

void init_timer(void)
{
    timer_interval = read_timer_freq() / 100;
    enable_timer();
    out_word(CNTP_EL0, (1<< 1));
}

static void timer_interrupt_handler(void)
{
    uint32_t status = read_timer_status();

    if (status & (1 << 2))  //interrupt is fired
    {
        ticks++;
        if (ticks % 100 == 0)
        {
            printk("timer %d \r\n",ticks);
        }
        set_timer_interval(timer_interval);
    }


}
void handler(uint64_t numid, uint64_t esr, uint64_t elr)
{
    uint32_t irq;

    switch (numid)
    {
    case 1:                                 //só trata o erro de sync
        printk("sync error at %x: %x\r\n", elr,esr);
        while (1){}
        break;
    

    case 2:                                 //trata a interrupção do timer
        irq = in_word(CNTP_STATUS_EL0);
        if (irq & (1<<1))                   //verifica se o bit da interrupção está ativo
        {
            timer_interrupt_handler();      //chama a função de interrup
        }
        else
        {
            printk("unknow irq\r\n");
        }
        break;
    
    default:
        printk("unknown exception\r\n");
        while(1) {}
        break;
    }

}