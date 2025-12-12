#include <stdio.h>
#include <string.h>
#include "stack.h"
#include "io.h"

#define MAX_INT_STR_LEN 12  // Больше без переполнения не будет, с учётом минуса и нуль-терминатора
#define FSCANF_FORMAT "%11s"  // Зависит от макс длины строки с числом
#define MAX_ROW_STR_LEN 1201 // 100 чисел макс. по 11 символов каждое (с учётом минуса), с пробелами между ними, \n и нуль-терминатором

short integerRowInput(Stack *addTo, FILE *source, FILE *save) {
    // Ввод ряда целых чисел из потока source, запись в стек addTo и в файл save
    int toWrite = 0;
    short correctInput = 0, length = 0;
    char rowbuf[MAX_ROW_STR_LEN], strbuf[MAX_INT_STR_LEN], strcheck[MAX_INT_STR_LEN];
    char *rowbufIndex = &rowbuf[0];
    fgets(rowbuf, MAX_ROW_STR_LEN, source);
    correctInput = sscanf(rowbufIndex, FSCANF_FORMAT, strbuf);  // Буфер используется для проверки переполнения при вводе
    while (correctInput == 1) {  // Написать тут нормальное условие конца чтения файла
        length = strlen(strbuf);
        correctInput = sscanf(strbuf, "%d", &toWrite);
        if (correctInput) {
            sprintf(strcheck, "%d", toWrite);
            if (strcmp(strbuf, strcheck)) {
                length = strlen(rowbuf);
                if (rowbuf[length - 1] != '\n') {
                    fscanf(source, "%*[^\n]\n");
                }
                return INPUT_ERR;
            }
            if (addElem(toWrite, addTo) == MEMORY_ERR) {
                return MEMORY_ERR;
            }
            if (save) {  // Сюда может быть передан NULL, тогда печати не будет
                fprintf(save, "%d ", toWrite);
            }
            rowbufIndex += strlen(strbuf) + 1;  // Сдвиг, чтобы не зациклиться на одном элементе
            correctInput = sscanf(rowbufIndex, FSCANF_FORMAT, strbuf);
            continue;
        }
        length = strlen(rowbuf);
        if (rowbuf[length - 1] != '\n') {
            fscanf(source, "%*[^\n]\n");
        }
        return INPUT_ERR;
    }
    return OK;  // Вышли из цикла - ввод кончился (EOF или остались только пробельные символы), ошибок не встречено
}
