#include "stdint.h"
#include "stdarg.h"
#include "uart.h"


// função copia cada caractere da string para o buffer
// retorna o número de caracteres que foram copiados
static int read_string(char *buffer, int position, const char* string)  
{
    int index = 0;

    for (index=0; string[index] !='\0'; index++)
    {
        buffer[position++] = string[index];
    }

    return index;
}

// função copia e converte cada caractere para o formato de hexadecimal
// retorna o número de caracteres que foram copiados
static int hex_to_string(char *buffer, int position, uint64_t digits)  
{
    char digits_buffer[25];                                 //salva os valores convertidos
    char digits_map[16] = "0123456789ABCDEF";               //caracteres em hexa possiveis
    int size = 0;

    //adiciona o 0x no começo para dizer que é em hexa
    buffer[position++] = '0';
    buffer[position++] = 'x';
    //converte todos valores
    do
    {
        digits_buffer[size++] = digits_map[digits % 16];        
        digits /=16;
    }while (digits != 0);

    //inverte as posições da direita para esquerda
    for (int i = size-1; i >= 0; i--)
    {
        buffer[position++] = digits_buffer[i];
    }

    //adiciona o H no final para dizer que é em hexa
    //buffer[position++] = 'H';

    return size + 2;
}

// função copia e converte cada caractere para o formato de decimal sem sinal
// retorna o número de caracteres que foram copiados
static int udecimal_to_string(char *buffer, int position, uint64_t digits)  
{
    char digits_buffer[25];                                 //salva os valores convertidos
    char digits_map[10] = "0123456789";               //caracteres em hexa possiveis
    int size = 0;

    //converte todos valores
    do
    {
        digits_buffer[size++] = digits_map[digits % 10];        
        digits /=10;
    }while (digits != 0);

    //inverte as posições da direita para esquerda
    for (int i = size-1; i >= 0; i--)
    {
        buffer[position++] = digits_buffer[i];
    }

    return size;
}

// função copia e converte cada caractere para o formato de decimal
// retorna o número de caracteres que foram copiados
static int decimal_to_string(char *buffer, int position, int64_t digits)  
{
    int size = 0;

    //se for negativo, converte para positivo e coloca o - no buffer
    if (digits < 0)
    {
        digits = -digits;
        buffer[position++] = '-';
        size = 1;
    }

    size += udecimal_to_string(buffer,position,(uint64_t)digits);

    return size;
}

//envia os caracteres para a UART
static void write_console(const char *buffer, int size)
{
    for(int i = 0; i < size; i++)
    {
        write_char(buffer[i]);
    }

}

int printk(const char *format,...)
{
    char _buffer[1024];             //pode colocar até 1024 caraceteres
    int32_t _buffer_size = 0;
    int64_t integer = 0;
    char *string = 0;
    va_list args;               //armazena argumentos variáveis

    va_start(args, format);

    for (int i =0; format[i] != '\0';i++)               //pega todos caracteres
    {
        if (format[i] != '%')                           //se for diferente de % é um caracterer regular 
        {
            _buffer[_buffer_size++] = format[i];        //copia para o buffer
        }
        else                                            //se for % é uma configuração
        {
            switch (format[++i])                        //verifica o caractere depois do %
            {
            case 'x':                                   //hexadecimal tipo
                integer = va_arg(args, int64_t);                            
                _buffer_size += hex_to_string(_buffer,_buffer_size, (uint64_t)integer);         //conveter de caracter para hexa
                break;
            case 'u':                                   //unsigned inter tipo
                integer = va_arg(args, int64_t);
                _buffer_size += udecimal_to_string(_buffer,_buffer_size,(uint64_t)integer);                            
                break;
            case 'd':                                   //signed inter tipo
                integer = va_arg(args, int64_t);
                _buffer_size += decimal_to_string(_buffer,_buffer_size,(int64_t)integer);                            

                break;    
            case 's':                                   //string  tipo
                string = va_arg(args,char*);                                    //macro para receber o ponteiro
                _buffer_size += read_string(_buffer, _buffer_size,string);      //lê a string do buffer
                break;                   
            default:
                _buffer[_buffer_size++] = '%';
                i--;
                break;
            }
        }
    }

    write_console(_buffer,_buffer_size);
    va_end(args);

    return _buffer_size;
}
