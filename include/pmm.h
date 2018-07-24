#ifndef INCLUDE_PMM_H_
#define INCLUDE_PMM_H_

#include "multiboot.h"

#define STACK_SIZE 8192
#define PMM_MAX_SZIE 0x20000000
#define PMM_PAGE_SIZE 0x1000
#define PAGE_MAX_SIZE (PMM_MAX_SZIE/PMM_PAGE_SIZE)
#define PHY_PAGE_MASK 0xfffff000

extern uint8_t kernel_start[];
extern uint8_t kernel_end[];
extern uint32_t phy_page_cnt;

void show_mem_map();
void init_pmm();
uint32_t pmm_alloc_page();
void pmm_free_page(uint32_t page);

#endif