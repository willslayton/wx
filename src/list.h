#ifndef LIST_H
#define LIST_H
#include <stdlib.h>

typedef struct LIST {
    void** items;
    size_t size;
    size_t item_size;
} list_t;

list_t* init_list(size_t item_size);
void list_push(list_t* list, void* item);

#endif