#ifndef _CPU_H
#define _CPU_H
#include "kernel/types.h"
struct cpu 
{
	char vendor_string[16];
	char name_string[50];
	unsigned int family, model, stepping;
	unsigned int family_ex, model_ex, stepping_ex;
	unsigned int max_cpuid, max_cpuid_ex;
};
extern struct cpu cpu;
void init_cpu();


void asm_cpuid(int id_eax, int id_ecx, int *eax, int *ebx, int *ecx, int *edx);
void asm_rdtsc(int *high, int *low);

#endif

