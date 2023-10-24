#ifndef _MEMORY_H
#define _MEMORY_H

#include "stdint.h"

#define KERNEL_BASE 0xffff000000000000

#define P2V(p) ((uint64_t)(p) + KERNEL_BASE) //physical addres to virtual addres
#define V2P(v) ((uint64_t)(v) - KERNEL_BASE) //virtual addres to phyisical addres


#endif
