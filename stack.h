#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED

typedef struct Elem {
    int val;
    struct Elem *link;
} Elem;

typedef struct {
    Elem *top;
    Elem *p;
} Stack;

enum ERRORS {
    OK = 0,
    MEMORY_ERR = 1,
    INPUT_ERR = 2,
    INPUT_END = 3
};

short createStack(int val, Stack **stack);

short addElem(int val, Stack *stack);

int deleteElem(Stack *stack);

void deleteStack(Stack *stack);

int stackLength(Stack *stack);

short moveStack(Stack *source, Stack **goal, int size);

#endif // STACK_H_INCLUDED
