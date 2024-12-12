#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MATRIX int **
#define SIZE 5
#define ROWS 300
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

char *get_index_pos(char *s)
{
    size_t len = strlen(s);
    return len >= 5 ? s + len - 5 : NULL;
}

int main()
{
    char core_path[] = "D:\\Tai_lieu_20241\\project_1\\code\\data_input\\core0.txt";
    char input_path[] = "D:\\Tai_lieu_20241\\project_1\\code\\data_input\\input_core1.txt";

    char *core_index = get_index_pos(core_path);
    char *input_index = get_index_pos(input_path);
    if (!core_index || !input_index)
    {
        printf("Đường dẫn file không hợp lệ!\n");
        return EXIT_FAILURE;
    }

    MATRIX core[SIZE];
    MATRIX input_core[4]; // Chỉ cần 3 phần tử

    for (int i = 0; i < SIZE; i++)
    {
        *core_index = i + '0';
        core[i] = create_matrix();
        if (!core[i] || read_file(core[i], core_path) == -1)
        {
            printf("Lỗi khi đọc file core%d\n", i);
            return EXIT_FAILURE;
        }
    }

    for (int i = 0; i < 4; i++)
    {
        *input_index = i + '0';
        input_core[i] = create_matrix();
        if (!input_core[i] || read_file(input_core[i], input_path) == -1)
        {
            printf("Lỗi khi đọc file input_core%d\n", i);
            return EXIT_FAILURE;
        }
    }
    int x = 10; // Dòng cần in (bắt đầu từ 0)
    printf("Dữ liệu dòng %d từ core0:\n", x);
    for (int i = 0; i < 256; i++)
    {
        printf("%d ", input_core[3][x][i]);
    }
    printf("\n");
    for (int i = 0; i < SIZE; i++)
        dispose(core[i]);
    for (int i = 0; i < 3; i++)
        dispose(input_core[i]);

    return 0;
}
