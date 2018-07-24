#ifndef INCLUDE_MULTIBOOT_H_
#define INCLUDE_MULTIBOOT_H_

#include "types.h"

typedef struct multiboot_t
{
    uint32_t flags;
    uint32_t mem_low, mem_high;
    uint32_t boot_device;
    uint32_t cmdline;
    uint32_t mods_count, mods_addr;
    uint32_t num, size, addr, shndx;
    uint32_t mmap_length, mmap_addr;
    uint32_t drivers_length, drivers_addr;
    uint32_t config_table;
    uint32_t boot_loader_name;
    uint32_t apm_table;
    uint32_t vbe_control_info,
        vbe_mode_info, vbe_mode,
        vbe_interface_seg, vbe_interface_off,
        vbe_interface_len;
} __attribute__((packed)) multiboot_t;

typedef struct mmap_entry_t {
    uint32_t size;
    uint32_t base_addr_low, base_addr_high;
    uint32_t length_low, length_high;
    uint32_t type;
} __attribute__((packed)) mmap_entry_t;

extern multiboot_t *mboot_ptr_tmp;
extern multiboot_t *glb_mboot_ptr;

#endif