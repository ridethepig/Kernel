#include "console.h"
#include "timer.h"
#include "gdt.h"
#include "idt.h"
#include "pmm.h"
#include "debug.h"
#include "heap.h"
#include "vmm.h"

void kernel_init();
void test_heap();
void stop();
multiboot_t *glb_mboot_ptr;
char kernel_stack[STACK_SIZE];

__attribute__((section(".init.data"))) pgd_t *pgd_tmp = (pgd_t *)0x1000;
__attribute__((section(".init.data"))) pgd_t *pte_low = (pgd_t *)0x2000;
__attribute__((section(".init.data"))) pgd_t *pte_high = (pgd_t *)0x3000;

__attribute__((section(".init.text"))) void kernel_entry()
{
    pgd_tmp[0] = (uint32_t)pte_low | PAGE_PRESENT | PAGE_WRITE;
    pgd_tmp[PGD_INDEX(PAGE_OFFSET)] = (uint32_t)pte_high | PAGE_PRESENT;
    int i;
    for (i = 0; i < 1024; ++i)
    {
        pte_low[i] = (i << 12) | PAGE_PRESENT | PAGE_WRITE;
    }
    for (i = 0; i < 1024; ++i)
    {
        pte_high[i] = (i << 12) | PAGE_PRESENT | PAGE_WRITE;
    }
    asm volatile("mov %0, %%cr3"
                 :
                 : "r"(pgd_tmp));
    uint32_t cr0;
    asm volatile("mov %%cr0, %0"
                 : "=r"(cr0));
    cr0 |= 0x80000000;
    asm volatile("mov %0, %%cr0"
                 :
                 : "r"(cr0));
    uint32_t kernel_stack_top = ((uint32_t)kernel_stack + STACK_SIZE) & 0xFFFFFFF0;
    asm volatile("mov %0, %%esp\n\t"
                 "xor %%ebp, %%ebp"
                 :
                 : "r"(kernel_stack_top));
    glb_mboot_ptr = mboot_ptr_tmp + PAGE_OFFSET;
    kernel_init();
}

void kernel_init()
{
    console_clear();
    console_write_color("IKernel Starting!\n", rc_black, rc_light_blue);
    init_debug();
    console_write("Initialize GDT...\n");
    init_gdt();
    console_write("Initialize IDT...\n");
    init_idt();
    console_write("Initialize TIMER...\n");
    init_timer(100);
    printk("KERNEL START:0x%08X\n", kernel_start);
    printk("KERNEL END: 0x%08X\n", kernel_end);
    printk("KERNEL USAGE: %d KB\n\n", (kernel_end - kernel_start + 1023) / 1024);
    show_mem_map();
    printk("Initialize PMM...\n");
    init_pmm();
    printk_color(rc_black, rc_red, "PHYSICAL MEM PAGE COUNT: %u\n\n", phy_page_cnt);
    uint32_t alloc_addr = NULL;
    alloc_addr = pmm_alloc_page();
    printk("ALLOC:0x%08X\n", alloc_addr);
    pmm_free_page(alloc_addr);
    init_vmm();
    init_heap();
    test_heap();
    stop();
}

void stop()
{
    while (1)
    {
        asm volatile("hlt");
    }
}

void test_heap()
{
    printk_color(rc_black, rc_magenta, "Test kmalloc() && kfree() now ...\n\n");

    void *addr1 = kmalloc(50);
    printk("kmalloc    50 byte in 0x%X\n", addr1);
    void *addr2 = kmalloc(500);
    printk("kmalloc   500 byte in 0x%X\n", addr2);
    void *addr3 = kmalloc(5000);
    printk("kmalloc  5000 byte in 0x%X\n", addr3);
    void *addr4 = kmalloc(50000);
    printk("kmalloc 50000 byte in 0x%X\n\n", addr4);

    printk("free mem in 0x%X\n", addr1);
    kfree(addr1);
    printk("free mem in 0x%X\n", addr2);
    kfree(addr2);
    printk("free mem in 0x%X\n", addr3);
    kfree(addr3);
    printk("free mem in 0x%X\n\n", addr4);
    kfree(addr4);
}
