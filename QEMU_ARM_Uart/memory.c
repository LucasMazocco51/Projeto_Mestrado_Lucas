#include "memory.h"
#include "debug.h"
#include "print.h"
#include "lib.h"
#include "stddef.h"
#include "stdbool.h"

extern char end; //end da mmeória definido no linker
static struct Page free_memory;

static void free_region(uint64_t v, uint64_t e);
void kfree(uint64_t v);
void* kalloc(void);
void checkmm(void);

// divide a regiao em 2mb pages 
static void free_region(uint64_t v, uint64_t e)
{

    for(uint64_t start = PA_UP(v); (start + PAGE_SIZE) <= e; start += PAGE_SIZE)
    {
        if(start + PAGE_SIZE <= MEMORY_END)
        {
            kfree(start);
        }

    }


}

//funcao de inicilização das páginas
void kfree(uint64_t v)
{
    //verifica se a página está alinhada
    ASSERT(v % PAGE_SIZE == 0);
    
    //verifica se endereço não está no kernel
    ASSERT(v >= (uint64_t)&end);

    //verifica os limites de memória livre
    ASSERT(v + PAGE_SIZE <= MEMORY_END);

    struct Page *page_address = (struct Page*)v;

    page_address->next = free_memory.next;

    free_memory.next = page_address;
}

//remove a pagina da lista de paginas
void* kalloc(void)
{
    struct Page *page_address = free_memory.next;

    if (page_address != NULL)
    {
        //verifica se a página está alinhada
        ASSERT((uint64_t)page_address % PAGE_SIZE == 0);
        
        //verifica se endereço não está no kernel
        ASSERT((uint64_t)page_address >= (uint64_t)&end);

        //verifica os limites de memória livre
        ASSERT((uint64_t)page_address + PAGE_SIZE <= MEMORY_END);

        free_memory.next = page_address->next;
    }

    //remove the page
    return page_address;

}

//check free pages
void checkmm(void)
{
    struct Page*v = free_memory.next;
    uint64_t size = 0;
    uint64_t i = 0;

    //se o endereço é nulo é o final da lista  
    while (v != NULL)
    {
        size += PAGE_SIZE;
        printk("%d base is %x \r\n", i++, v);
        v = v->next;
    }

    printk("memory size is %u \r\n",size/1024/1024);
}

void init_memory(void)
{
    free_region((uint64_t)&end, MEMORY_END);
    checkmm();

}
