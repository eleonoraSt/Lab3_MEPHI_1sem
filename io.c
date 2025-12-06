#include <stdio.h>
#include <string.h>
#include "stack.h"

#define MAX_INT_STR_LEN 12  // Больше без переполнения не будет, с учётом минуса и нуль-терминатора
#define FSCANF_FORMAT "%11s"  // Зависит от макс длины строки с числом

short integerRowInput(Stack *addTo, FILE *file) {
    // Ввод ряда целых чисел
    int toWrite = 0;
    short correctInput = 0, length = 0;
    char strbuf[MAX_INT_STR_LEN], strcheck[MAX_INT_STR_LEN];
    char charbuf[2];
    correctInput = fscanf(file, FSCANF_FORMAT, strbuf);  // Буфер используется для проверки переполнения при вводе
    while (correctInput == 1) {  // Написать тут нормальное условие конца чтения файла
        length = strlen(strbuf);
        correctInput = sscanf(strbuf, "%d", &toWrite);
        if (correctInput) {
            sprintf(strcheck, "%d", toWrite);
            if (strcmp(strbuf, strcheck)) {
                puts("Ошибка при вводе ряда");
                scanf("%*[^\n]\n");
                return INPUT_ERR;
            }
            if (addElem(toWrite, addTo) == MEMORY_ERR) {
                return MEMORY_ERR;
            }
            fscanf(file, FSCANF_FORMAT, strbuf);
            continue;
        }
        puts("Ошибка при вводе ряда");
        scanf("%*[^\n]\n");
        return INPUT_ERR;
    }
    return OK;  // Вышли из цикла - ввод кончился (EOF или остались только пробельные символы), ошибок не встречено
}
