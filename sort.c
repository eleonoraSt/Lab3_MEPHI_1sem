#include "stack.h"
#include "sort.h"

#define NULL 0

short insertionSort(Stack *stack) {
    Elem *current = stack->top, *compareWith = 0;
    Stack *beforeCurrent, *afterCurrent;
    int bufCompareWith = 0, bufCurrent = 0, currentIndex = 0, compareWithIndex = 0;  // Индексы передаются функции moveStack
    while (current->link != 0) {  // Не дошли до конца стека
        current = current->link;  // Следующий
        currentIndex++;
        compareWith = stack->top;
        compareWithIndex = 0;
        while (compareWith != current && compareWith->val <= current->val) {
            compareWith = compareWith->link;  // Следующий
            compareWithIndex++;
        }
        if (compareWith == current) {  // Не надо двигать current
            continue;
        }
        if (createStack(beforeCurrent) == NULL) {
            return MEMORY_ERR;
        }
        if (moveStack(stack, beforeCurrent, compareWithIndex) == MEMORY_ERR) {  // Убираем до compareWith
            deleteStack(beforeCurrent);
            return MEMORY_ERR;
        }
        bufCompareWith = deleteElem(stack);  // Убираем compareWith
        if (createStack(afterCurrent) == NULL) {
            deleteStack(beforeCurrent);
            return MEMORY_ERR;
        }
        if (moveStack(stack, afterCurrent, currentIndex - compareWithIndex - 1) == MEMORY_ERR) {  // Убираем до current
            deleteStack(beforeCurrent);
            deleteStack(afterCurrent);
            return MEMORY_ERR;
        }
        bufCurrent = deleteElem(stack);
        if (moveStack(afterCurrent, stack, currentIndex - compareWithIndex - 1) == MEMORY_ERR) {  // Возвращаем после current
            deleteStack(afterCurrent);
            deleteStack(beforeCurrent);
            return MEMORY_ERR;
        }
        deleteStack(afterCurrent);
        if (addElem(bufCompareWith, stack) == MEMORY_ERR) {  // Возвращаем compareWith
            deleteStack(beforeCurrent);
            return MEMORY_ERR;
        }
        if (addElem(bufCurrent, stack) == MEMORY_ERR) {  // Возвращаем current
            deleteStack(beforeCurrent);
            return MEMORY_ERR;
        }
        if (moveStack(beforeCurrent, stack, compareWithIndex) == MEMORY_ERR) {  // Возвращаем до compareWith
            deleteStack(beforeCurrent);
            return MEMORY_ERR;
        }
        deleteStack(beforeCurrent);
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
    if (createStack(&firstSubstack) == NULL) {
        return MEMORY_ERR;
    }
    if (moveStack(stack, firstSubstack, length / 2) == MEMORY_ERR) {
        deleteStack(firstSubstack);
        return MEMORY_ERR;
    }
    if (createStack(&secondSubstack) == NULL) {
        deleteStack(firstSubstack);
        return MEMORY_ERR;
    }
    if (moveStack(stack, secondSubstack, length / 2 + length % 2) == MEMORY_ERR) {
        deleteStack(firstSubstack);
        deleteStack(secondSubstack);
        return MEMORY_ERR;
    }
    if (mergeSort(firstSubstack, step + 1) == MEMORY_ERR || mergeSort(secondSubstack, step + 1) == MEMORY_ERR) {
        deleteStack(firstSubstack);
        deleteStack(secondSubstack);
        return MEMORY_ERR;
    }
    while (firstSubstack->top != NULL || secondSubstack->top != NULL) {
        addFirst = secondSubstack->top == NULL;
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
    deleteStack(firstSubstack);
    deleteStack(secondSubstack);
    return OK;
}
