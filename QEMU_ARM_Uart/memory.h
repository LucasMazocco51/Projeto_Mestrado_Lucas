#ifndef _MEMORY_H
#define _MEMORY_H

#include "stdint.h"

struct Page {
    struct Page* next;

};


#define KERNEL_BASE 0xffff000000000000

#define P2V(p) ((uint64_t)(p) + KERNEL_BASE) //physical addres to virtual addres
#define V2P(v) ((uint64_t)(v) - KERNEL_BASE) //virtual addres to phyisical addres

#define MEMORY_END  P2V(0x30000000)         
#define PAGE_SIZE   (2*1024*1024)

//alinha para os próximos 2mb para cima e para baixo
#define PA_UP(v)    ((((uint64_t)v + PAGE_SIZE - 1) >> 21) << 21)
#define PA_DOWN(v)  (((uint64_t)v >> 21) << 21)

//recieve the next level page or physical page
#define PDE_ADD(p)  ((uint64_t)p & 0xfffffffff000)
#define PTE_ADDR(p) ((uint64_t)p & 0xffffffe00000)

#define ENTRY_V         (1 << 0)
#define TABLE_ENTRY     (1 << 1)
#define BLOCK_ENTRY     (0 << 1)
#define ENTRY_ACCESSED  (1 << 10)
#define NORMAL_MEMORY   (1 << 2)
#define DEVICE_MEMORY   (0 << 2)



void* kalloc(void);
void kfree(uint64_t v);
void init_memory(void);
//bool map_page(uint64_t map, uint64_t v, uint64_t pa, uint64_t attribute);
//bool setup_uvm(void);
//bool switch_vm(uint64_t map);


#endif
