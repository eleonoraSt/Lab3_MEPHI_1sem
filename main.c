#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#include "stack.h"
#include "sort.h"
#include "io.h"

#define ARG_NUM 1  // 0 доп. аргументов
#define ARG_NUM_FILE 3  // 2 доп. аргумента: имя аргумента и адрес файла
#define MAX_FILENAME_LEN 261  // Максимум в Windows с учётом \0 + \n перед \0
#define MAX_ROW_STR_LEN 1200  // 100 чисел макс. по 11 символов каждое (с учётом минуса), с пробелами между ними и с нуль-терминатором

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
                fclose(rowFile);
                continue;
            }
            puts("Введите ряд чисел через пробел");
            inputRes = integerRowInput(stack, stdin, rowFile);
            if (inputRes == MEMORY_ERR) {
                puts("Не хватает памяти при создании стека");
                deleteStack(stack);
                fclose(rowFile);
                continue;
            }
            if (inputRes == INPUT_ERR) {
                puts("Ошибка ввода");
                deleteStack(stack);
                fclose(rowFile);
                continue;
            }

            sortRes = insertionSort(stack);  // Сортировка прямым включением
            //sortRes = mergeSort(stack, 0);  // Сортировка слиянием
            if (sortRes == MEMORY_ERR) {
                puts("Не удалось выполнить сортировку");
                deleteStack(stack);
                fclose(rowFile);
                continue;
            }

            fputc('\n', rowFile);
            while (stack->top != NULL) {
                fprintf(rowFile, "%d ", deleteElem(stack));
            }
            free(stack);
            puts("Данные записаны в файл");
            fclose(rowFile);
        }
    }
    else if (argc == ARG_NUM_FILE && strcmp(argv[1], "--file") == 0) {// Передан аргумент --file
        rowFile = fopen(argv[2], "r");
        if (rowFile == NULL) {
            puts("Произошла ошибка при открытии файла");
            return 0;
        }
        if (createStack(&unsorted) == MEMORY_ERR) {
            puts("Не хватает памяти при создании стека");
            fclose(rowFile);
            return 0;
        }
        if (createStack(&sorted) == MEMORY_ERR) {
            puts("Не хватает памяти при создании стека");
            free(unsorted);
            fclose(rowFile);
            return 0;
        }
        if (createStack(&stack) == MEMORY_ERR) {
            puts("Не хватает памяти при создании стека");
            free(unsorted);
            free(sorted);
            fclose(rowFile);
            return 0;
        }
        // Вводим изначальный ряд
        inputRes = integerRowInput(unsorted, rowFile, NULL);
        if (inputRes == MEMORY_ERR) {
            puts("Не хватает памяти при создании стека");
            deleteStack(unsorted);
            free(sorted);
            fclose(rowFile);
            return 0;
        }
        if (inputRes == INPUT_ERR) {
            puts("Ошибка ввода");
            deleteStack(unsorted);
            free(sorted);
            fclose(rowFile);
            return 0;
        }
        // Вводим отсортированный ряд
        inputRes = integerRowInput(sorted, rowFile, NULL);
        if (inputRes == MEMORY_ERR) {
            puts("Не хватает памяти при создании стека");
            deleteStack(unsorted);
            deleteStack(sorted);
            fclose(rowFile);
            return 0;
        }
        if (inputRes == INPUT_ERR) {
            puts("Ошибка ввода");
            deleteStack(unsorted);
            deleteStack(sorted);
            fclose(rowFile);
            return 0;
        }
        // Выводим изначальный ряд
        if (moveStack(unsorted, stack, stackLength(unsorted)) == MEMORY_ERR) {  // Т.к. при вводе он перевернулся
            puts("Не хватает памяти при создании стека");
            deleteStack(unsorted);
            deleteStack(sorted);
            fclose(rowFile);
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
            fclose(rowFile);
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
