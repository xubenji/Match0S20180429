#include "kernel/memory.h"
#include "kernel/ards.h"
#include "kernel/types.h"
#include "driver/vga.h"
#include "kernel/8259a.h"
#include "lib/bitmap.h"
#include "lib/string.h"

struct bitmap phy_mem_bitmap;
struct bitmap vir_mem_bitmap;
int memory_total_size;
int memory_used_size;
struct mem_block_desc k_block_descs[DESC_CNT];	// 内核内存块描述符数组

void init_memory()
{
	int idx, i; 
	int a,b,*c;
	int *new_page;
	
	memory_total_size = 0;
	//开始获取
	init_ards();
	
	//只支持最大512MB物理内存
	if(memory_total_size >= 512*1024*1024){
		memory_total_size = 512*1024*1024;
	}
	
	int pages = memory_total_size/PAGE_SIZE;
	int page_byte = pages/8;
	
	phy_mem_bitmap.bits = (uint8_t*)PHY_MEM_BITMAP;
	phy_mem_bitmap.btmp_bytes_len = page_byte;
	
	vir_mem_bitmap.bits = (uint8_t*)VIR_MEM_BITMAP;
	vir_mem_bitmap.btmp_bytes_len = page_byte;
	
	bitmap_init(&phy_mem_bitmap);
	bitmap_init(&vir_mem_bitmap);
	
	//将内核的页目录前2048清空，腾出来给任务用，内核占用高2048字节
	memset((void *)PAGE_DIR_VIR_ADDR, 0, 2048);
	
	
	block_desc_init(k_block_descs);
	
	/*
	a = (int )kernel_alloc_page(1);
	put_int(a);
	put_str("\n");
	
	a = (int )kernel_alloc_page(2);
	put_int(a);
	put_str("\n");
	
	kernel_free_page(a, 2);
	
	a = (int )kernel_alloc_page(1);
	put_int(a);
	put_str("\n");
	
	kernel_free_page(a, 1);
	
	a = (int )kernel_alloc_page(3);
	put_int(a);
	put_str("\n");
	
	kernel_free_page(a, 3);

	a = (int )kernel_alloc_page(1);
	put_int(a);
	put_str("\n");
	*/
	/*
	a = alloc_mem_page();
	put_int(a);
	put_str("\n");
	a = alloc_mem_page();
	put_int(a);
	put_str("\n");
	free_mem_page(a);
	
	a = alloc_mem_page();
	put_int(a);
	put_str("\n");
	
	b = alloc_vir_page();
	put_int(b);
	put_str("\n");
	b = alloc_vir_page();
	put_int(b);
	put_str("\n");
	
	free_vir_page( b);
	
	b  = alloc_vir_page();
	put_int(b);
	put_str("\n");
	put_str("\n");
	
	c = kernel_alloc_page(1);
	put_int(c);
	put_str("\n");
	
	*c = 0xaa55;
	
	put_int(*c);
	put_str("\n");*/
	/*new_page = copy_kernel_page();
	put_int(new_page);
	put_str("\n");
	put_int(*new_page);
	put_str("\n");*/
}

/* 返回arena中第idx个内存块的地址 */
static struct mem_block* arena2block(struct arena* a, uint32_t idx) {
  return (struct mem_block*)((uint32_t)a + sizeof(struct arena) + idx * a->desc->block_size);
}

/* 返回内存块b所在的arena地址 */
static struct arena* block2arena(struct mem_block* b) {
   return (struct arena*)((uint32_t)b & 0xfffff000);
}

/* 在堆中申请size字节内存 */
void* sys_malloc(uint32_t size) {
  
	struct mem_block_desc* descs;
  
	descs = k_block_descs;

   /* 若申请的内存不在内存池容量范围内则直接返回NULL */
   if (!(size > 0)) {
      return NULL;
   }
   struct arena* a;
   struct mem_block* b;	
  
/* 超过最大内存块1024, 就分配页框 */
	if (size > 1024) {
		uint32_t page_cnt = DIV_ROUND_UP(size + sizeof(struct arena), PAGE_SIZE);    // 向上取整需要的页框数
		a = kernel_alloc_page(page_cnt);

		if (a != NULL) {
			memset(a, 0, page_cnt * PAGE_SIZE);	 // 将分配的内存清0  

			/* 对于分配的大块页框,将desc置为NULL, cnt置为页框数,large置为true */
			a->desc = NULL;
			a->cnt = page_cnt;
			a->large = true;
			return (void*)(a + 1);		 // 跨过arena大小，把剩下的内存返回
		} else { 	
			return NULL; 
		}
	} else {    // 若申请的内存小于等于1024,可在各种规格的mem_block_desc中去适配
		uint8_t desc_idx;
      
		/* 从内存块描述符中匹配合适的内存块规格 */
		for (desc_idx = 0; desc_idx < DESC_CNT; desc_idx++) {
			if (size <= descs[desc_idx].block_size) {  // 从小往大后,找到后退出
				break;
			}
		}
	
		/* 若mem_block_desc的free_list中已经没有可用的mem_block,
		* 就创建新的arena提供mem_block */
		if (list_empty(&descs[desc_idx].free_list)) {
		a = kernel_alloc_page(1);       // 分配1页框做为arena
		if (a == NULL) {
			return NULL;
		}
		memset(a, 0, PAGE_SIZE);

		/* 对于分配的小块内存,将desc置为相应内存块描述符, 
		 * cnt置为此arena可用的内存块数,large置为false */
		a->desc = &descs[desc_idx];
		a->large = false;
		a->cnt = descs[desc_idx].blocks_per_arena;
		uint32_t block_idx;
		
		 /* 开始将arena拆分成内存块,并添加到内存块描述符的free_list中 */
		for (block_idx = 0; block_idx < descs[desc_idx].blocks_per_arena; block_idx++) {
			b = arena2block(a, block_idx);
			list_append(&a->desc->free_list, &b->free_elem);	
		}
    }

   /* 开始分配内存块 */
    b = elem2entry(struct mem_block, free_elem, list_pop(&(descs[desc_idx].free_list)));
    memset(b, 0, descs[desc_idx].block_size);

    a = block2arena(b);  // 获取内存块b所在的arena
    a->cnt--;		   // 将此arena中的空闲内存块数减1
    return (void*)b;
   }
}

/* 回收内存ptr */
void sys_mfree(void* ptr) {
   if (ptr != NULL) {
		struct mem_block* b = ptr;
		struct arena* a = block2arena(b);	     // 把mem_block转换成arena,获取元信息
		if (a->desc == NULL && a->large == true) { // 大于1024的内存
			kernel_free_page((int )a, a->cnt); 
		} else {				 // 小于等于1024的内存块
			/* 先将内存块回收到free_list */
			list_append(&a->desc->free_list, &b->free_elem);

			/* 再判断此arena中的内存块是否都是空闲,如果是就释放arena */
			if (++a->cnt == a->desc->blocks_per_arena) {
				uint32_t block_idx;
				for (block_idx = 0; block_idx < a->desc->blocks_per_arena; block_idx++) {
					struct mem_block*  b = arena2block(a, block_idx);
					//ASSERT(elem_find(&a->desc->free_list, &b->free_elem));
					list_remove(&b->free_elem);
				}
				kernel_free_page((int )a, 1); 
			} 
		}   
	}
}


/* 为malloc做准备 */
void block_desc_init(struct mem_block_desc* desc_array) {				   
   uint16_t desc_idx, block_size = 16;

   /* 初始化每个mem_block_desc描述符 */
   for (desc_idx = 0; desc_idx < DESC_CNT; desc_idx++) {
      desc_array[desc_idx].block_size = block_size;
      /* 初始化arena中的内存块数量 */
      desc_array[desc_idx].blocks_per_arena = (PAGE_SIZE - sizeof(struct arena)) / block_size;	  

      list_init(&desc_array[desc_idx].free_list);

      block_size *= 2;         // 更新为下一个规格内存块
   }
}

/**检测页的分配状况*/
int pages_status()
{
	int i , j = 0;
	for(i = 0; i < memory_total_size/PAGE_SIZE/8; i++){
		if(bitmap_scan_test(&phy_mem_bitmap, i) == 0)j++;
	}
	return j;
}
/**返回没有使用的内存数*/
int get_free_memory()
{
	int pages = pages_status()*8;
	int free_size = pages*PAGE_SIZE;
	return free_size;
}

int *copy_kernel_page()
{
	void *new_page;
	new_page = kernel_alloc_page(1);
	/*由于内核前2048字节已经清空，所以现在使用的话就只有高2048字节，低2048字节给未来的任务使用*/
	/*
	*因为地址是int类型，所以new_page+512相当于访问	(char )new_page+2048
	*/
	//memcpy((void *)(new_page+2048), (void *)(PAGE_DIR_VIR_ADDR+2048), 2048);
	memcpy((void *)new_page, (void *)PAGE_DIR_VIR_ADDR, PAGE_SIZE);
	
	
	return new_page;
}

void *get_kernel_page()
{
	return (void *)PAGE_DIR_VIR_ADDR;
}

/*
把虚拟地址转换成物理地址
*/
uint32_t addr_v2p(uint32_t vaddr)
{
	uint32_t *pte = pte_ptr(vaddr);
	uint32_t phy_addr;
	//put_int((int )*pte);
	phy_addr = ((*pte & 0xfffff000) + (vaddr & 0x00000fff));
	//put_int((int )phy_addr);
	return phy_addr;
}

uint32_t *pte_ptr(uint32_t vaddr)
{
	uint32_t *pte = (uint32_t *)(0xffc00000 + ((vaddr & 0xffc00000) >> 10) + PTE_IDX(vaddr) * 4);
	return pte;
}

uint32_t *pde_ptr(uint32_t vaddr)
{
	uint32_t *pde = (uint32_t *)((0xfffff000) + PDE_IDX(vaddr) * 4);
	return pde;
}

void fill_vir_page_talbe(int vir_address)
{
	int va = vir_address;
	int *pde, *pte;
	int pte_addr;
	pde = (int *)(((va&0xffc00000)>>20)|0xfffff000);	//获得页目录项的地址
	if(((*pde)&0x00000001) != 0x00000001){	//不存在页表
		int pt = alloc_mem_page();	//分配页表地址
		pt |= 0x00000007;
		*pde = pt;		//填写页目录项为页表的地址
	}
	pte_addr = (int )(((va>>10)&0x003ff000)|0xffc00000);	//获得页目表项的地址
	va &= 0x003ff000;
	va >>= 10;
	pte_addr |= va;
	pte = (int *)pte_addr;
	int page = alloc_mem_page();	//分配页地址
	page |= 0x00000007;
	*pte = page;	//填写页表项为页的地址
}

void clean_vir_page_table(int vir_address)
{
	int va = vir_address;
	int *pde, *pte;
	int pte_addr;
	int page_phy_addr;
	pde = (int *)(((va & 0xffc00000) >> 20) | 0xfffff000);
	if(((*pde)&0x00000001) != 0x00000001){	//不存在页表,直接产生页故障
		//error
		put_str("clean vir page failed.");
		while(1);
	}
	pte_addr = (int )(((va>>10)&0x003ff000)|0xffc00000);	//获得页目表项的地址
	va &= 0x003ff000;
	va >>= 10;
	pte_addr |= va;
	pte = (int *)pte_addr;
	
	
	page_phy_addr = *pte;	//获得页表项中页的物理地址
	*pte = 0;	//清空页表项
	
	page_phy_addr &= 0xfffff000;	//保留高22位，对齐
	free_mem_page(page_phy_addr);	//释放对应的物理页
}


/*
	分配一个物理页
	成功返回地址，失败返回-1
*/
int alloc_mem_page()
{
	int idx;
	int mem_addr;
	idx = bitmap_scan(&phy_mem_bitmap, 1);
	if(idx != -1){
		bitmap_set(&phy_mem_bitmap, idx, 1);
	}else{
		return -1;
	}
	mem_addr = idx*0x1000 + PHY_MEM_BASE_ADDR;
	/*put_int(mem_addr);
	put_str("\n");
	*/
	return mem_addr;
}
/*
	释放一个物理页
	成功返回0，失败返回-1
*/
 int free_mem_page(int address)
{
	int addr = address;
	int idx, i;

	idx = (addr-PHY_MEM_BASE_ADDR)/0x1000;
	if(bitmap_scan_test(&phy_mem_bitmap, idx)){
		bitmap_set(&phy_mem_bitmap, idx, 0);
	}else{
		return -1;
	}
	return 0;
}

/*
	分配一个物理页
	成功返回地址，失败返回-1
*/
int alloc_vir_page()
{
	int idx;
	int vir_addr;
	idx = bitmap_scan(&vir_mem_bitmap, 1);
	if(idx != -1){
		bitmap_set(&vir_mem_bitmap, idx, 1);
	}else{
		return -1;
	}
	vir_addr = idx*0x1000 +  VIR_MEM_BASE_ADDR;
	
	
	return vir_addr;
}

/*
	释放一个物理页
	成功返回0，失败返回-1
*/
int free_vir_page(int address)
{
	int addr = address;
	int idx, i;

	idx = (addr-VIR_MEM_BASE_ADDR)/0x1000;
	if(bitmap_scan_test(&vir_mem_bitmap, idx)){
		bitmap_set(&vir_mem_bitmap, idx, 0);
	}else{
		return -1;
	}
	return 0;
}

void *kernel_alloc_page(uint32_t pages)
{
	int i;
	int vir_page_addr, vir_page_addr_more;
	vir_page_addr = alloc_vir_page();	//分配一个虚拟地址的页
	fill_vir_page_talbe(vir_page_addr);		//把页添加到当前页目录表系统中，使他可以被使用
	
	if(pages == 1){	//如果只有一个页
		return (void *)vir_page_addr;
	}else if(pages > 1){
		for(i = 1; i < pages; i++){
			vir_page_addr_more = alloc_vir_page();	//分配一个虚拟地址的页
			fill_vir_page_talbe(vir_page_addr_more);		//把页添加到当前页目录表系统中，使他可以被使用	
		}
		return (void *)vir_page_addr;
	}else{
		return (void *)-1;
	}
	//应该不会到这里来
	return (void *)-1;
}


void kernel_free_page(int vaddr, uint32_t pages)
{
	int i;
	int vir_page_addr = vaddr;
	free_vir_page(vir_page_addr);
	clean_vir_page_table(vir_page_addr);		//把页添加到当前页目录表系统中，使他可以被使用
	if(pages == 1){	//如果只有一个页
		return;
	}else if(pages > 1){
		for(i = 1; i < pages; i++){
			vir_page_addr += PAGE_SIZE;
			free_vir_page(vir_page_addr);
			clean_vir_page_table(vir_page_addr);		//把页添加到当前页目录表系统中，使他可以被使用
		}
		return ;
	}
}

