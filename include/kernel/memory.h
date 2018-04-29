#ifndef _MEMORY_H
#define _MEMORY_H

#include "kernel/types.h"
#include "lib/list.h"


#define PAGE_SIZE 4096

#define PAGE_DIR_PHY_ADDR 0x1000
#define PAGE_TBL_PHY_ADDR 0x2000

#define PAGE_DIR_VIR_ADDR 0x80001000
#define PAGE_TBL_VIR_ADDR 0x80002000


#define PHY_MEM_BASE_ADDR  0x100000

#define PHY_MEM_BITMAP  0x80008000
#define PHY_MEM_BITMAP_SIZE 0x4000

#define VIR_MEM_BASE_ADDR  0x80100000

#define VIR_MEM_BITMAP  0x8000c000
#define VIR_MEM_BITMAP_SIZE 0x4000

#define PDE_IDX(addr) ((addr & 0xffc00000) >> 22)
#define PTE_IDX(addr) ((addr & 0x003ff000) >> 12)

extern int memory_total_size;



void init_memory();
int alloc_mem_page();
int free_mem_page(int address);

int alloc_vir_page();
int free_vir_page(int address);
void fill_vir_page_talbe(int vir_address);
void clean_vir_page_table(int vir_address);

void *kernel_alloc_page(uint32_t pages);
void kernel_free_page(int vaddr, uint32_t pages);
int *copy_kernel_page();

uint32_t *pde_ptr(uint32_t vaddr);
uint32_t *pte_ptr(uint32_t vaddr);
uint32_t addr_v2p(uint32_t vaddr);

void kernel_memroy_map(int phy, int vir, int pages);
int pages_status();
int get_free_memory();

void *get_kernel_page();

/* 内存块 */
struct mem_block {
   struct list_elem free_elem;
};

/* 内存块描述符 */
struct mem_block_desc {
   uint32_t block_size;		 // 内存块大小
   uint32_t blocks_per_arena;	 // 本arena中可容纳此mem_block的数量.
   struct list free_list;	 // 目前可用的mem_block链表
};

#define DESC_CNT 7	   // 内存块描述符个数


/* 内存仓库arena元信息 */
struct arena {
   struct mem_block_desc* desc;	 // 此arena关联的mem_block_desc
/* large为ture时,cnt表示的是页框数。
 * 否则cnt表示空闲mem_block数量 */
   uint32_t cnt;
   bool large;		   
};

void block_desc_init(struct mem_block_desc* desc_array);
static struct mem_block* arena2block(struct arena* a, uint32_t idx);
static struct arena* block2arena(struct mem_block* b);
void* sys_malloc(uint32_t size);
void sys_mfree(void* ptr);












#endif

