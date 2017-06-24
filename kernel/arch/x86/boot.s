.set ALIGN, 1<<0
.set MEMINFO, 1<<1
.set FLAGS, ALIGN | MEMINFO
.set MAGIC, 0x1BADB002
.set CHECKSUM, -(MAGIC + FLAGS)

.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

.section .bss
.align 16
stack_bottom:
.skip 16384 # 16 KiB
stack_top:

.section .text
.global _start
.type _start, @function
_start:
    # Setup the stack.
    mov $stack_top, %esp

    # Call the kernel entrypoint.
    call kernel_main

    # Disable interrupts, if we get to this,
    # the system has likely gone into a bad state.
    cli

    # Go into an infinite loop with hlt.
1:  hlt
    jmp 1b

# Apply the new global descriptor table.
.global gdt_flush
gdt_flush:
    mov 4(%esp), %eax
    lgdt (%eax)

    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %ss

    ljmp $0x08, $.flush
.flush:
    ret

# Apply the new interrupt descriptor table.
.global idt_flush
.type idt_flush, @function
idt_flush:
    mov 4(%esp), %eax
    lidt (%eax)
    ret

.size _start, . - _start
