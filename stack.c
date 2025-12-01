#include <stdlib.h>
#include "stack.h"

short createStack(Stack **stack) {
    // Создаёт стек по адресу stack с первым элементом val, возвращает код ошибки
    *stack = malloc(sizeof(Stack));
    if (*stack == NULL) {
        return MEMORY_ERR;
    }
    (*stack)->top = NULL;
    (*stack)->p = NULL;
    return OK;
}

short addElem(int val, Stack *stack) {
    // Добавляет в стек по адресу stack элемент val, возвращает код ошибки
    stack->p = malloc(sizeof(Elem));
    if (stack->p == NULL) {
        return MEMORY_ERR;
    }
    stack->p->val = val;
    stack->p->link = stack->top;
    stack->top = stack->p;
    return OK;
}

int deleteElem(Stack *stack) {
    // Удаляет элемент из стека по адресу stack
    int val = stack->top->val;
    stack->p = stack->top;
    stack->top = stack->top->link;
    free(stack->p);
    return val;
}

void deleteStack(Stack *stack) {
    // Удаляет стек и освобождает всё
    while (stack->top != NULL) {
        deleteElem(stack);
    }
    free(stack);
}

// Ввести проверку на переполнение переменной с длинной. Или защиту от чрезмерной длины
int stackLength(Stack *stack) {
    int length = 1;
    Elem *current = stack->top;
    while (current->link != NULL) {
        length++;
        current = current->link;
    }
    return length;
}

short moveStack(Stack *source, Stack **goal, int size) {
    if (createStack(deleteElem(source), goal) == MEMORY_ERR){
        return MEMORY_ERR;
    }
    for (int i = 0; i < size; i++) {
        if (addElem(deleteElem(source), *goal) == MEMORY_ERR) {
            deleteStack(*goal);
            return MEMORY_ERR;
        }
    }
    return OK;
}
