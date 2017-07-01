#include "rkmalloc.h"

typedef struct {
    bool free;
    size_t used_size;
    size_t block_size;
    void *ptr;
} rkmalloc_entry;

void rkmalloc_init_heap(rkmalloc_heap *heap) {
#define CHKSIZE(size) \
    if ((size) == 0) { \
        heap->error_code = RKMALLOC_ERROR_TYPE_TOO_SMALL; \
        return; \
    }

    if (heap->kmalloc == NULL) {
        heap->error_code = RKMALLOC_ERROR_INVALID_POINTER;
        return;
    }

    heap->error_code = RKMALLOC_ERROR_NONE;
    heap->total_allocated_blocks_size = 0;
    heap->last_allocated_entry = 0;
    list_init(&heap->index);

    CHKSIZE(heap->types.tiny)
    CHKSIZE(heap->types.small)
    CHKSIZE(heap->types.medium)
    CHKSIZE(heap->types.large)
    CHKSIZE(heap->types.huge)
}

static size_t get_block_size(rkmalloc_heap_types types, size_t size) {
    if (size <= types.tiny) {
        return types.tiny;
    }

    if (size <= types.small) {
        return types.small;
    }

    if (size <= types.medium) {
        return types.medium;
    }

    if (size <= types.large) {
        return types.large;
    }

    if (size <= types.huge) {
        return types.huge;
    }

    return size;
}

static bool is_block_usable(rkmalloc_entry *entry, size_t block_size) {
    if (!entry->free) {
        return false;
    }

    if (block_size == entry->block_size) {
        return true;
    }

    return false;
}

void* rkmalloc_allocate(rkmalloc_heap *heap, size_t size) {
    size_t block_size = get_block_size(heap->types, size);
    list_node_t *node = heap->index.head;

    while (node != NULL && !is_block_usable(node->value, block_size)) {
        node = node->next;
    }

    /*
     * Our best case is that we find a node in the index that can fit the size.
     */
    if (node != NULL) {
        rkmalloc_entry *entry = node->value;
        entry->free = false;
        entry->used_size = size;
        entry->block_size = block_size;
        heap->total_allocated_blocks_size += block_size;
        heap->total_allocated_used_size += size;

        return entry->ptr;
    }

    node = heap->index.head;


}