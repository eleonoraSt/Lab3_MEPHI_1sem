#include <stdio.h>
#include <time.h>
#include "stack.h"
#include "sort.h"

short insertionSort(Stack *stack) {
    Elem *current = stack->top, *compareWith = 0;
    Stack *buf;
    int bufCompareWith = 0, bufCurrent = 0, currentIndex = 0, compareWithIndex = 0;  // Индексы передаются функции moveStack
    while (current->link != NULL) {  // Не дошли до конца стека
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
        if (createStack(&buf) == MEMORY_ERR) {
            return MEMORY_ERR;
        }
        if (moveStack(stack, buf, compareWithIndex) == MEMORY_ERR) {  // Убираем до compareWith
            deleteStack(buf);
            return MEMORY_ERR;
        }
        bufCompareWith = deleteElem(stack);  // Убираем compareWith
        if (moveStack(stack, buf, currentIndex - compareWithIndex - 1) == MEMORY_ERR) {  // Убираем до current
            deleteStack(buf);
            return MEMORY_ERR;
        }
        bufCurrent = deleteElem(stack);
        if (moveStack(buf, stack, currentIndex - compareWithIndex - 1) == MEMORY_ERR) {  // Возвращаем после current
            deleteStack(buf);
            return MEMORY_ERR;
        }
        if (addElem(bufCompareWith, stack) == MEMORY_ERR) {  // Возвращаем compareWith
            deleteStack(buf);
            return MEMORY_ERR;
        }
        if (addElem(bufCurrent, stack) == MEMORY_ERR) {  // Возвращаем current
            deleteStack(buf);
            return MEMORY_ERR;
        }
        if (moveStack(buf, stack, compareWithIndex) == MEMORY_ERR) {  // Возвращаем до compareWith
            deleteStack(buf);
            return MEMORY_ERR;
        }
        deleteStack(buf);
    }
    return OK;
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
    if (createStack(&firstSubstack) == MEMORY_ERR) {
        return MEMORY_ERR;
    }
    if (moveStack(stack, firstSubstack, length / 2) == MEMORY_ERR) {
        deleteStack(firstSubstack);
        return MEMORY_ERR;
    }
    if (createStack(&secondSubstack) == MEMORY_ERR) {
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
            addFirst = firstSubstack->top->val >= secondSubstack->top->val && step % 2 == 0 || firstSubstack->top->val < secondSubstack->top->val && step % 2 == 1;
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
