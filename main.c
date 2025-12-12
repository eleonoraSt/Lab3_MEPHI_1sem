#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#include "stack.h"
#include "sort.h"
#include "io.h"

#define ARG_NUM 1  // 0 доп. аргументов
#define ARG_NUM_FILE 3 // 2 доп. аргумента: имя аргумента и адрес файла
#define MAX_FILENAME_LEN 262  // Максимум в Windows + 2 для \n и \0
#define MAX_ROW_STR_LEN 1200 // 100 чисел макс. по 11 символов каждое (с учётом минуса), с пробелами между ними и с нуль-терминатором

// Если стек длиной больше 2, говорит, что не удалось выполнить сортировку.
// При этом первый ряд записан в файл.
// В случае неудачной сортировки надо очищать файл

int main(int argc, char *argv[]) {
    int inputVal = 0;
    short running = 1, length = 0;
    short inputRes = OK, sortRes = 0;
    char filename[MAX_FILENAME_LEN], rowBuf[MAX_ROW_STR_LEN];
    FILE *rowFile = NULL;
    Stack *stack, *unsorted, *sorted;

    setlocale(LC_ALL, "");  // Печатать кириллицу

    if (argc == ARG_NUM) {
        while (running) {
            puts("Введите адрес файла для сохранения ряда чисел, или введите quit, чтобы закрыть программу");
            fgets(filename, MAX_FILENAME_LEN, stdin);
            if (strcmp(filename, "quit\n") == 0) {
                break;
            }
            length = strlen(filename);
            filename[length - 1] = 0;  // В конце имени файла записан конец строки, а это нам не нужно
            rowFile = fopen(filename, "w");
            if (rowFile == NULL) {  // Не смог открыть или создать
                puts("Произошла ошибка при открытии/создании файла");
                continue;
            }
            if (createStack(&stack) == MEMORY_ERR) {
                puts("Не хватает памяти при создании стека");
                continue;
            }
            puts("Введите ряд чисел через пробел");
            inputRes = integerRowInput(stack, stdin, rowFile);
            if (inputRes == MEMORY_ERR) {
                puts("Не хватает памяти при создании стека");
                deleteStack(stack);
                continue;
            }
            if (inputRes == INPUT_ERR) {
                puts("Ошибка ввода");
                deleteStack(stack);
                continue;
            }

            sortRes = insertionSort(stack);  // Сортировка прямым включением
            //sortRes = mergeSort(stack, 0);  // Сортировка слиянием
            if (sortRes == MEMORY_ERR) {  // Ошибка возникает тут
                puts("Не удалось выполнить сортировку");
                deleteStack(stack);
                continue;
            }

            fputc('\n', rowFile);
            while (stack->top != NULL) {
                fprintf(rowFile, "%d ", deleteElem(stack));
            }
            free(stack);
            puts("Данные записаны в файл");
        }
    }
    else if (argc == ARG_NUM_FILE) {  // Передан аргумент
        if (strcmp(argv[1], "--file")) {  // Это не --file
            puts("Неправильный аргумент");
            return 0;
        }
        rowFile = fopen(argv[2], "r");
        if (rowFile == NULL) {  // Не смог открыть
            puts("Неправильный аргумент");
            return 0;
        }
        if (createStack(&unsorted) == MEMORY_ERR) {
            puts("Не хватает памяти при создании стека");
            return 0;
        }
        if (createStack(&sorted) == MEMORY_ERR) {
            puts("Не хватает памяти при создании стека");
            free(unsorted);
            return 0;
        }
        if (createStack(&stack) == MEMORY_ERR) {
            puts("Не хватает памяти при создании стека");
            return 0;
        }
        // Вводим изначальный ряд
        inputRes = integerRowInput(unsorted, rowFile, NULL);
        if (inputRes == MEMORY_ERR) {
            puts("Не хватает памяти при создании стека");
            deleteStack(unsorted);
            free(sorted);
            return 0;
        }
        if (inputRes == INPUT_ERR) {
            puts("Ошибка ввода");
            deleteStack(unsorted);
            free(sorted);
            return 0;
        }
        // Вводим отсортированный ряд
        inputRes = integerRowInput(sorted, rowFile, NULL);
        if (inputRes == MEMORY_ERR) {
            puts("Не хватает памяти при создании стека");
            deleteStack(unsorted);
            deleteStack(sorted);
            return 0;
        }
        if (inputRes == INPUT_ERR) {
            puts("Ошибка ввода");
            deleteStack(unsorted);
            deleteStack(sorted);
            return 0;
        }
        // Выводим изначальный ряд
        if (moveStack(unsorted, stack, stackLength(unsorted)) == MEMORY_ERR) {  // Т.к. при вводе он перевернулся
            puts("Не хватает памяти при создании стека");
            deleteStack(unsorted);
            deleteStack(sorted);
            return 0;
        }
        while (stack->top != NULL) {
            printf("%d ", deleteElem(stack));
        }
        free(unsorted);
        putchar('\n');
        // Выводим отсортированный ряд
        if (moveStack(sorted, stack, stackLength(sorted)) == MEMORY_ERR) {  // Т.к. при вводе он перевернулся
            puts("Не хватает памяти при создании стека");
            deleteStack(sorted);
            return 0;
        }
        while (stack->top != NULL) {
            printf("%d ", deleteElem(stack));
        }
        free(sorted);
        free(stack);
    }
    else {
        puts("Неправильный аргумент");
    }
    return 0;
}
