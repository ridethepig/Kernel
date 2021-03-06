#include "timer.h"
#include "common.h"
#include "debug.h"
#include "idt.h"

void timer_callback(pt_regs *regs) {
    static uint32_t tick = 0;
    printk_color(rc_white, rc_black, "Tick:%d\n", tick ++);
}

void init_timer(uint32_t frequency) {
    register_interrupt_handler(IRQ0, timer_callback);
    uint32_t divisor = 1193180 / frequency;
    outb(0x43, 0x36);
    uint8_t low = (uint8_t)(divisor & 0xff);
    uint8_t high = (uint8_t)((divisor >> 8) & 0xff);
    outb(0x40, low);
    outb(0x40, high);
}