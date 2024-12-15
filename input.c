#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parameter.c"
#define MATRIX int **
int ROWS = number_of_samples + 1;
#define COLUMNS 256

MATRIX create_matrix()
{
    MATRIX data = (int **)malloc(sizeof(int *) * ROWS);
    if (!data)
    {
        printf("Không đủ bộ nhớ để cấp phát mảng!\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < ROWS; i++)
    {
        data[i] = (int *)malloc(sizeof(int) * COLUMNS);
        if (!data[i])
        {
            printf("Không đủ bộ nhớ để cấp phát hàng %d!\n", i);
            exit(EXIT_FAILURE);
        }
    }
    return data;
}

void dispose(MATRIX data)
{
    for (int i = 0; i < ROWS; i++)
        free(data[i]);
    free(data);
}

int read_file(MATRIX data, const char *file_path)
{
    char line[1024];
    FILE *file = fopen(file_path, "r");
    if (file == NULL)
    {
        printf("Không thể mở file: %s\n", file_path);
        return -1;
    }

    int row = 0;
    while (fgets(line, sizeof(line), file) && row < ROWS)
    {
        int col = 0;
        char *ptr = line;
        while (*ptr && col < COLUMNS)
        {
            data[row][col] = strtol(ptr, &ptr, 10);
            col++;
        }
        row++;
    }

    fclose(file);
    return 0;
}
void read_label(int *lable, const char *file_path)
{
    char line[2*number_of_samples];
    FILE *file = fopen(file_path, "r");
    if (file == NULL)
    {
        printf("Không thể mở file: %s\n", file_path);
        return;
    }
    fgets(line, sizeof(line), file);

    char *ptr = line;
    int col = 0;
    while (*ptr && col < ROWS)
    {
        lable[col] = strtol(ptr, &ptr, 10);
        col++;
    }
    return;
}
char *get_index_pos(char *s)
{
    size_t len = strlen(s);
    return len >= 5 ? s + len - 5 : NULL;
}