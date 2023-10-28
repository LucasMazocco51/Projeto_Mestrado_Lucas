#include "memory.h"
#include "debug.h"
#include "print.h"
#include "lib.h"
#include "stddef.h"
#include "stdbool.h"
#include "lib.h"

extern char end; //end da mmeória definido no linker
void load_pgd(uint64_t map);
static struct Page free_memory;

static void free_region(uint64_t v, uint64_t e);
void kfree(uint64_t v);
void* kalloc(void);
void checkmm(void);

bool map_page(uint64_t map, uint64_t v, uint64_t pa, uint64_t attribute);


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

//create a user space for a process se já não foi implementado
bool setup_uvm(void)
{
    bool status = false;




    return status;
}

//switch the page translation talbes
bool switch_vm(uint64_t map)
{
    //vem do mmu
    load_pgd(V2P(map));

}

//igual a find pud entry mas é com o upper directory table
static uint64_t* find_pgd_entry(uint64_t map, uint64_t v, int alloc, uint64_t attribute)
{
    uint64_t *ptr = (uint64_t*)map;
    void *addr = NULL;
    unsigned int index = (v >> 39) & 0x1fff;

    if(ptr[index] & ENTRY_V)
    {
        addr = (void*)P2V(PDE_ADD(ptr[index]));
    }
    else if (alloc == 1)
    {
        addr = kalloc();
        if(addr != NULL)
        {
            memset(addr, 0, PAGE_SIZE);
            ptr[index] = (V2P(addr) | attribute | TABLE_ENTRY);
        }
    }
    return addr;
}


//return: address of middle directory table
//alloc: indica se deve ou não criar a página caso não exista
static uint64_t* find_pud_entry(uint64_t map, uint64_t v, int alloc, uint64_t attribute)
{
    uint64_t *ptr = NULL;
    void *addr = NULL;
    unsigned int index = (v >> 30) & 0x1fff;    //index value is located from the bits 30

    //chama a função para pegar o global directory table
    ptr = find_pgd_entry(map, v, alloc, attribute);

    if(ptr = NULL)
    {
        return NULL;
    }

    //verifica se o bit é valido
    if (ptr[index] & ENTRY_V)
    {
        //clear the attribute bits to get the address - converte com o p2v
        addr = (void*)P2V(PDE_ADD(ptr[index]));
    }
    else if (alloc == 1)
    {
        //aloca nova págica caso não tenha
        addr = kalloc();
        if (addr != NULL)
        {
            memset(addr, 0, PAGE_SIZE);
            ptr[index] = (V2P(addr) | attribute | TABLE_ENTRY);
        }
    }

    return addr;
}



//mapeia a pagina de virtual address to pyshical page
//map: map page
//v: virtual address
//pa: pyshical page
//attribute: 
bool map_page(uint64_t map, uint64_t v, uint64_t pa, uint64_t attribute)
{
    uint64_t vstart = PA_DOWN(v);
    uint64_t *ptr = NULL;

    //verifica a região inicial
    ASSERT(vstart + PAGE_SIZE < MEMORY_END);

    //verifica se a região está alinhada
    ASSERT(pa % PAGE_SIZE == 0);

    //verifica se o fim do endereço fisico é dentro do range
    ASSERT(pa + PAGE_SIZE <= V2P(MEMORY_END));

    //find upper directory entry
    ptr = find_pud_entry(map, vstart, 1, attribute);

    if (ptr == NULL)
    {
        return false;

    }

    //se retornar um valor verdadeiro, localiza a página correta de acordo com o virtual addr

    unsigned int index = (vstart >> 21) & 0x1ff;
    //verifica se é um bit válido para entrada
    ASSERT((ptr[index]& ENTRY_V) == 0);

    ptr[index] = (pa | attribute | BLOCK_ENTRY);

    return true;
}


void init_memory(void)
{
    free_region((uint64_t)&end, MEMORY_END);
    //checkmm();

}
