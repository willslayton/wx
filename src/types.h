#ifndef TYPES_H
#define TYPES_H

typedef enum {
    DATA_TYPE_INT,
    DATA_TYPE_UNKNOWN
} data_type;

int typename_to_int(const char* name);

#endif