#include <liblox/string.h>

#include "klist.h"

#define list_invalid_index(list, idx) (idx) >= (list)->size || (idx) < 0

void list_init_node(list_node_t *node) {
    memset(node, 0, sizeof(list_node_t));
}

void list_init(list_t *list) {
    memset(list, 0, sizeof(list_t));
}

list_node_t* list_create_node(list_t *list, void) {
    list_node_t *node = (list_node_t*) malloc(sizeof(list_node_t));

    ensure_allocated(node);

    list_init_node(node);
    return node;
}

list_t* list_create(void) {
    list_t *val = (list_t*) malloc(sizeof(list_t));

    ensure_allocated(val);

    list_init(val);
    return val;
}

list_node_t* list_insert_after(list_node_t *node, void *value) {
    list_node_t *entry = list_create_node();

    ensure_allocated(entry);

    entry->value = value;

    list_node_t *tmp = node->next;
    node->next = entry;
    entry->prev = node;
    entry->next = tmp;

    if (node->list != NULL) {
        node->list->size++;
    }

    return entry;
}

list_node_t* list_add(list_t *list, void *value) {
    list_node_t *entry = list_create_node();

    ensure_allocated(entry);

    entry->value = value;

    if (list->head == NULL) {
        list->head = entry;
        list->size = 1;
        return entry;
    }

    list_node_t *current = list->head;
    while (current->next != NULL) {
        current = current->next;
    }
    list_insert_after(current, entry);
    return entry;
}

list_node_t* list_get_at(list_t *list, size_t index) {
    if (list_invalid_index(list, index)) {
        return NULL;
    }

    size_t current = 0;
    list_node_t *node = list->head;
    while (node != NULL && current != index && current < list->size) {
        node = node->next;
        current++;
    }

    return node;
}

void* list_get_value_at(list_t *list, size_t index) {
    list_node_t *node = list_get_at(list, index);

    if (node == NULL) {
        return NULL;
    }

    return node->value;
}

list_node_t* list_insert_before(list_node_t *node, void *value) {
    list_node_t *entry = list_create_node();

    ensure_allocated(entry);

    entry->value = value;

    list_node_t *tmp = node->prev;
    node->prev = entry;
    entry->prev = tmp;
    entry->next = node;

    if (node->list != NULL) {
        node->list->size++;
    }

    return entry;
}

void list_remove(list_node_t *node) {
    node->prev = node->next;

    if (node->list != NULL) {
        node->list->size--;
    }

    free(node);
}