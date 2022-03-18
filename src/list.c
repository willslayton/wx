#include "list.h"

// Implements a general list struct
list_t* init_list(size_t item_size) {
    list_t* list = calloc(1, sizeof(struct LIST));

    list->size = 0;
    list->item_size = item_size;
    list->items = 0;

    return list;
}

// Function to add items to list
void list_push(list_t* list, void* item) {

    list->size += 1;

    if(!list->items) {
        // Allocate memory for the first element
        list->items = calloc(1, list->item_size);
    } else {
        // Reallocate for new elements
        list->items = realloc(list->items, (list->size * list->item_size));
    }

    list->items[list->size - 1] = item;
}