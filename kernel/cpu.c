#include "kernel/cpu.h"
#include "lib/string.h"
#include "gui/layer.h"
#include "gui/graphic.h"
struct cpu cpu;

extern struct layer *layer_desktop;
void init_cpu()
{
	int i;
	
	int *p;
	unsigned int eax, ebx, ecx, edx;
	
	//vendor ID
	asm_cpuid(0x00000000,0x00, &eax, &ebx, &ecx, &edx);
	cpu.max_cpuid = eax;
	
	memcpy(cpu.vendor_string    , &ebx, 4);
	memcpy(cpu.vendor_string + 4, &edx, 4);
	memcpy(cpu.vendor_string + 8, &ecx, 4);
	cpu.vendor_string[12] = '\0';

	/*draw_hex16(layer_desktop->buf,layer_desktop->wide, 0, 0,  COLOR_BLACK, cpu.max_cpuid);
	draw_string16(layer_desktop->buf,layer_desktop->wide, 0, 16,  COLOR_BLACK, cpu.vendor_string);*/
	//Processor Signiture and Feature
	asm_cpuid(0x00000001, 0x0, &eax, &ebx, &ecx, &edx);
	cpu.family   = (((eax >> 20) & 0xFF) << 4)
             + ((eax >> 8) & 0xF);
	cpu.model    = (((eax >> 16) & 0xF) << 4)
             + ((eax >> 4) & 0xF);
	cpu.stepping = (eax >> 0) & 0xF;
	
	/*draw_string16(layer_desktop->buf,layer_desktop->wide, 0, 16*2,  COLOR_BLACK, "Family = ");
	draw_hex16(layer_desktop->buf,layer_desktop->wide, 200, 16*2,  COLOR_BLACK, cpu.family);
	draw_string16(layer_desktop->buf,layer_desktop->wide, 0, 16*3,  COLOR_BLACK, "Model = ");
	draw_hex16(layer_desktop->buf,layer_desktop->wide, 200, 16*3,  COLOR_BLACK, cpu.model);
	draw_string16(layer_desktop->buf,layer_desktop->wide, 0, 16*4,  COLOR_BLACK, "Stepping = ");
	draw_hex16(layer_desktop->buf,layer_desktop->wide, 200, 16*4,  COLOR_BLACK, cpu.stepping);*/
	//Extern
	asm_cpuid(0x80000000, 0x0, &eax, &ebx, &ecx, &edx);
	cpu.max_cpuid_ex = eax;
	if(cpu.max_cpuid_ex >= 0x80000001){
		asm_cpuid(0x80000001, 0x0, &eax, &ebx, &ecx, &edx);
		cpu.family_ex   = (((eax >> 20) & 0xFF) << 4)
                    + ((eax >> 8) & 0xF);
		cpu.model_ex    = (((eax >> 16) & 0xF) << 4)
                    + ((eax >> 4) & 0xF);
		cpu.stepping_ex = (eax >> 0) & 0xF;
		/*draw_string16(layer_desktop->buf,layer_desktop->wide, 0, 16*5,  COLOR_BLACK, "FamilyEx = ");
		draw_hex16(layer_desktop->buf,layer_desktop->wide, 200, 16*5,  COLOR_BLACK, cpu.family_ex);
		draw_string16(layer_desktop->buf,layer_desktop->wide, 0, 16*6,  COLOR_BLACK, "ModelEx = ");
		draw_hex16(layer_desktop->buf,layer_desktop->wide, 200, 16*6,  COLOR_BLACK, cpu.model_ex);
		draw_string16(layer_desktop->buf,layer_desktop->wide, 0, 16*7,  COLOR_BLACK, "SteppingEx = ");
		draw_hex16(layer_desktop->buf,layer_desktop->wide, 200, 16*7,  COLOR_BLACK, cpu.stepping_ex);*/
	}
	if(cpu.max_cpuid_ex >= 0x80000004){
		asm_cpuid(0x80000002, 0x0, &eax, &ebx, &ecx, &edx);
		memcpy(cpu.name_string      , &eax, 4);
		memcpy(cpu.name_string  +  4, &ebx, 4);
		memcpy(cpu.name_string  +  8, &ecx, 4);
		memcpy(cpu.name_string  + 12, &edx, 4);
		asm_cpuid(0x80000003, 0x0, &eax, &ebx, &ecx, &edx);
		memcpy(cpu.name_string  + 16, &eax, 4);
		memcpy(cpu.name_string  + 20, &ebx, 4);
		memcpy(cpu.name_string  + 24, &ecx, 4);
		memcpy(cpu.name_string  + 28, &edx, 4);
		asm_cpuid(0x80000004, 0x0, &eax, &ebx, &ecx, &edx);
		memcpy(cpu.name_string  + 32, &eax, 4);
		memcpy(cpu.name_string  + 36, &ebx, 4);
		memcpy(cpu.name_string  + 40, &ecx, 4);
		memcpy(cpu.name_string  + 44, &edx, 4);
		cpu.name_string[49] = '\0';
		for (i = 0; i < 49; i++){
			if (cpu.name_string[i] > 0x20) {
				break;
			}
		}
		//draw_string16(layer_desktop->buf,layer_desktop->wide, 0, 16*8,  COLOR_BLACK, cpu.name_string + i);
	}
	
	//layer_refresh(layer_desktop, 0, 0, layer_desktop->wide, layer_desktop->high);
	
}
