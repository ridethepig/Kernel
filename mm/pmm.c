#include "common.h"
#include "debug.h"
#include "pmm.h"

static uint32_t pmm_stack[PAGE_MAX_SIZE + 1];
static uint32_t pmm_stack_top;

uint32_t phy_page_cnt;

void show_mem_map()
{
    uint32_t mmap_addr = glb_mboot_ptr->mmap_addr;
    uint32_t mmap_len = glb_mboot_ptr->mmap_length;
    printk("MEMORY MAP:\n");
    mmap_entry_t *mmap = (mmap_entry_t *)mmap_addr;
    for (mmap = (mmap_entry_t *)mmap_addr; (uint32_t)mmap < mmap_addr + mmap_len; mmap++)
    {
        printk("BASE_ADDR = 0x%X%08X, LENGTH = 0x%X%08X, TYPE = 0x%X\n",
               (uint32_t)mmap->base_addr_high, (uint32_t)mmap->base_addr_low,
               (uint32_t)mmap->length_high, (uint32_t)mmap->length_low,
               (uint32_t)mmap->type);
    }
}

void init_pmm() {
    mmap_entry_t *mmap_start_addr = (mmap_entry_t *) glb_mboot_ptr->mmap_addr;
    mmap_entry_t *mmap_end_addr = (mmap_entry_t *)glb_mboot_ptr->mmap_addr + glb_mboot_ptr->mmap_length;
    mmap_entry_t *map_entry;
    for (map_entry = mmap_start_addr; map_entry < mmap_end_addr; map_entry ++) {
        if (map_entry->type == 1 && map_entry->base_addr_low == 0x100000) {
            uint32_t page_addr = map_entry->base_addr_low + (uint32_t)(kernel_end - kernel_start);
            uint32_t length = map_entry->base_addr_low + map_entry->length_low;
            while (page_addr < length && page_addr <= PMM_MAX_SZIE) {
                pmm_free_page(page_addr);
                page_addr += PMM_PAGE_SIZE;
                phy_page_cnt ++;
            }
        }
    }
}

uint32_t pmm_alloc_page() {
    assert(pmm_stack_top != 0, "out of memory");
    uint32_t page = pmm_stack[pmm_stack_top --];
    return page;
}

void pmm_free_page(uint32_t page) {
    assert(pmm_stack_top != PAGE_MAX_SIZE, "out of stack");
    pmm_stack[++pmm_stack_top] = page;
}