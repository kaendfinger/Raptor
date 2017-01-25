#include "keyboard.h"
#include "irq.h"
#include "io.h"
#include "vga.h"
#include "x86.h"
#include <string.h>
#include <stdbool.h>

/**
 * Callback for keyboard interrupt(IRQ 1).
 */
static int keyboard_callback(regs_t *regs) {
    char c = 0;

    c = inb(0x60);
    c2 = inb(0x60);

    switch (c) {
        case 0x2A:
        case 0x36:
            vga_writestring("Shift pressed");
            shift = true;
            break;
        case 0xAA:
        case 0xB6:
            vga_writestring("Shift released");
            shift = false;
            break;
    }
    
    {
        char kt = key_types[c];
        switch (kt) {
            case N:
                vga_putchar((shift ? kb_usu[c] : kb_usl[c]));
                break;
        }
    }

    char* cs;
    itoa(c, cs, 16);
    vga_writestring("Code ");
    vga_writestring(cs);
    vga_writestring("\n");

    return 1;
}

void keyboard_init(void) {
    outb(0xF0, 0x00);
    vga_writebyte(inb(0x60));
    vga_writebyte(inb(0x60));
    vga_writebyte(inb(0x60));
    irq_add_handler(IRQ1, &keyboard_callback);
}
