#ifndef _LIB_H
#define _LIB_H

#include "stdint.h"

void delay(uint64_t value);
void out_word(uint64_t addr, uint32_t value);
uint32_t in_word(uint64_t addr);


void memset(void *dst, int value, unsigned int size);   //seta valor no endereço de memória
void memcpy(void *dst, void *src, unsigned int size);   //copia valores de um endereço de memória para outro
void memmove(void *dst, void *src, unsigned int size);  //move valores de um endereço de memória para outro
int memcmp(void *scr1, void *scr2, unsigned int size);  //compara valores de memória em dois endereços, se for igual retorna 0
unsigned char get_el(void);

#endif