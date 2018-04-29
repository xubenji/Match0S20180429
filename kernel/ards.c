#include "kernel/ards.h"
#include "kernel/memory.h"

struct ards *ards;
void init_ards()
{
	uint16_t ards_nr =  *((uint16_t *)ARDS_NR);	//ards 结构数
	ards = (struct ards *) ARDS_ADDR;	//ards 地址
	
	int i;
	for(i = 0; i < ards_nr; i++){
		//draw_hex16(layer_desktop->buf,layer_desktop->wide, 0, 0+i*16, COLOR_WHITE,ards->base_low);
		//draw_hex16(layer_desktop->buf,layer_desktop->wide, 100+i*8, 0+i*16, COLOR_WHITE,ards->length_low);
		//draw_hex16(layer_desktop->buf,layer_desktop->wide, 200+i*8, 0+i*16, COLOR_WHITE,ards->type);

		//寻找可用最大内存
		if(ards->type == 1){
			//冒泡排序获得最大内存
			if(ards->base_low+ards->length_low > memory_total_size){
				memory_total_size = ards->base_low+ards->length_low;
			}
		}
		ards++;
	}
	put_str("mem:\n");
	put_int(memory_total_size/(1024*1024));
	put_str("\n");
	//draw_hex16(layer_desktop->buf,layer_desktop->wide, 500, 0, COLOR_WHITE,mem_size);
}