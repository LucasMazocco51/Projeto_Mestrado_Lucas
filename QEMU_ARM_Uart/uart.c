#include "uart.h"
#include "lib.h"

void init_uart(void)
{
    out_word(UART0_CR,0);       //control register set 0 

                                // baud rate = 115200
                                //BAUDIV = FUARTCLK/(16 * baud rate)
                                //BAUDIV = 48000000 /(16 * 115200)
                                //BAUDIV = 26
    out_word(UART0_IBRD,26);
    out_word(UART0_FBRD,0); 

    out_word(UART0_LCRH,  (1 << 5) | (1 << 6));  //disable fifo | enable fifo 8 bit data mode
    out_word(UART0_IMSC, (1 << 4));                                //enable recieve interrupt
    out_word(UART0_CR, (1 << 0) | (1 << 8) | (1 << 9));     //control register | enable receiver | enable transmitter


}

void write_char(unsigned char c)
{
    //check status from flag register
    //while (in_word(UART0_FR) & (1 << 5)) {}

    //check flag register if device is busy 
    while (in_word(UART0_FR) & (1 << 3)) {}

    out_word(UART0_DR,c);   //send to data register
}

unsigned char read_char(void)
{
    // while (in_word(UART0_FR) * (1 << 4)) {}
    return in_word(UART0_DR);
}

void write_string(const char *string)
{
    for (int i = 0; string[i] != '\0'; i++)
    {
        write_char(string[i]);
    }

}

void uart_handler(void)
{
    uint32_t status = in_word(UART0_MIS);

    if (status & (1 << 4))
    {
        char ch = read_char();
        if (ch == '\r')
        {
            write_string("\r\n");
        }
        else
        {
            write_char(ch);
        }
        out_word(UART0_ICR, (1 << 4));
    }
}