#include "../lox-internal.h"
#include "../memory.h"
#include "../string.h"

#if !defined(USE_EXTERNAL_MALLOC)
__attribute__((malloc))
void* malloc(size_t size) {
    return lox_allocate_provider(size);
}

__attribute__((malloc))
void* calloc(size_t count, size_t size) {
    return zalloc(count * size);
}

__attribute__((malloc))
void* realloc(void* ptr, size_t size) {
    return lox_reallocate_provider(ptr, size);
}

__attribute__((malloc))
void* valloc(size_t size) {
    return lox_aligned_allocate_provider(size);
}

void free(void *ptr) {
    lox_free_provider(ptr);
}
#endif

__attribute__((malloc))
void* zalloc(size_t size) {
    void* ptr = malloc(size);
    if (ptr != NULL) {
        memset(ptr, 0, size);
    }
    return ptr;
}
