#ifndef STACK_H
#define STACK_H
#include <stdbool.h>
#include "level.h"

// A stack of Level snapshots (last in, first out). Grows automatically.
typedef struct {
    Level *items;
    int top;        // number of snapshots stored
    int capacity;   // how many fit before it must grow
} Stack;

void stack_init(Stack *s);
void stack_free(Stack *s);
void stack_clear(Stack *s);                       // forget everything, keep memory
bool stack_push(Stack *s, const Level *lv);       // false if out of memory
bool stack_pop(Stack *s, Level *out);             // false if the stack is empty

#endif
