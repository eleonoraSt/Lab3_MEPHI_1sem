#include "stack.h"

void insertionSort(Stack *stack) {
    Elem *current = stack->top, *compareWith = 0;
    while (current->link != 0) {  // Не дошли до конца стека
        current = current->link;  // Следующий
        compareWith = stack->top;
        while (compareWith != current && compareWith->val <= current->val) {
            compareWith = compareWith->link;  // Следующий
        }
        while (compareWith != current) {  // Если current перемещать не нужно, мы не входим в цикл
            current->val += compareWith->val;
            compareWith->val = current->val - compareWith->val;
            current->val -= compareWith->val;  // Меняем местами compareWith->val и current->val
            compareWith = compareWith->link;  // Следующий
        }
    }
}

short mergeSort(Stack *stack, int step) {
    // При каждом слиянии стек переворачивается. Поэтому на чётном шаге (нумерация начинается с 0) первым идёт больший элемент,
    // а на нечётном - меньший элемент
    int length = stackLength(stack);
    short addFirst = 0;
    Stack *firstSubstack, *secondSubstack;
    if (length == 1) {
        return OK;
    }
    if (moveStack(stack, firstSubstack, length / 2) == MEMORY_ERR) {
        return MEMORY_ERR;
    }
    if (moveStack(stack, secondSubstack, length / 2 + length % 2) == MEMORY_ERR) {
        deleteStack(firstSubstack);
        return MEMORY_ERR;
    }
    if (mergeSort(firstSubstack, step + 1) == MEMORY_ERR || mergeSort(secondSubstack, step + 1) == MEMORY_ERR) {
        return MEMORY_ERR;
    }
    while (firstSubstack->top != NULL || secondSubstack->top != NULL) {
        addFirst == secondSubstack->top == NULL;
        if (!addFirst && firstSubstack->top != NULL) {  // Если оба подстека ещё не кончились
            addFirst = firstSubstack->top >= secondSubstack->top && step % 2 == 0 || firstSubstack->top < secondSubstack->top && step % 2 == 1;
        }
        if (addFirst) {
            if (addElem(deleteElem(firstSubstack), stack) == MEMORY_ERR) {
                deleteStack(firstSubstack);
                deleteStack(secondSubstack);
                return MEMORY_ERR;
            }
        } else {
            if (addElem(deleteElem(secondSubstack), stack) == MEMORY_ERR) {
                deleteStack(firstSubstack);
                deleteStack(secondSubstack);
                return MEMORY_ERR;
            }
        }
    }
    return OK;
}
