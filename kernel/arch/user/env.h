#pragma once

#include <stddef.h>
#include <stdint.h>

#include <kernel/tty.h>

extern tty_t* console_tty;

void* raptor_user_malloc(size_t size);
void raptor_user_free(void* ptr);
void* raptor_user_realloc(void* ptr, size_t size);
void raptor_user_process_stdin(void);

void raptor_user_output_char(char c);
void raptor_user_output_string(char* str);
void raptor_user_abort(void);
void raptor_user_console_write(tty_t* tty, const uint8_t* buffer, size_t size);

char* raptor_user_get_cmdline(void);