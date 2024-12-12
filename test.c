#include <stdio.h>
#include <stdlib.h>

// Hàm đọc file, dữ liệu lưu vào mảng `data` truyền vào
void read_file(int data[300][256], char *file_path)
{
    char line[256];
    FILE *file = fopen(file_path, "r");
    if (file == NULL)
    {
        printf("Không thể mở file: %s\n", file_path);
        return;
    }

    int row = 0;
    while (fgets(line, sizeof(line), file) && row < 300)
    {
        int col = 0;
        char *ptr = line; // Con trỏ duyệt qua chuỗi
        while (*ptr && col < 256)
        {
            // Dùng strtol để chuyển đổi chuỗi thành số nguyên
            data[row][col] = strtol(ptr, &ptr, 10);
            col++;
        }
        row++;
    }

    fclose(file);
}

int main()
{
    // Đường dẫn tới các file dữ liệu
    char *core0_path = "D:\\Tai_lieu_20241\\project_1\\code\\data_input\\core0.txt";
    char *core1_path = "D:\\Tai_lieu_20241\\project_1\\code\\data_input\\core1.txt";
    char *core2_path = "D:\\Tai_lieu_20241\\project_1\\code\\data_input\\core2.txt";
    char *core3_path = "D:\\Tai_lieu_20241\\project_1\\code\\data_input\\core3.txt";
    char *core4_path = "D:\\Tai_lieu_20241\\project_1\\code\\data_input\\core4.txt";

    char *input_core0_path = "D:\\Tai_lieu_20241\\project_1\\code\\data_input\\input_core0.txt";
    char *input_core1_path = "D:\\Tai_lieu_20241\\project_1\\code\\data_input\\input_core1.txt";
    char *input_core2_path = "D:\\Tai_lieu_20241\\project_1\\code\\data_input\\input_core2.txt";
    char *input_core3_path = "D:\\Tai_lieu_20241\\project_1\\code\\data_input\\input_core3.txt";
    char *input_core4_path = "D:\\Tai_lieu_20241\\project_1\\code\\data_input\\input_core4.txt";

    // Khởi tạo mảng để lưu dữ liệu
    int core0[300][256] = {0};
    int core1[300][256] = {0};
    int core2[300][256] = {0};
    int core3[300][256] = {0};
    int core4[300][256] = {0};

    int input_core0[300][256] = {0};
    int input_core1[300][256] = {0};
    int input_core2[300][256] = {0};
    int input_core3[300][256] = {0};
    int input_core4[300][256] = {0};

    // Gọi hàm đọc file
    read_file(core0, core0_path);
    read_file(core1, core1_path);
    read_file(core2, core2_path);
    read_file(core3, core3_path);
    read_file(core4, core4_path);

    read_file(input_core0, input_core0_path);
    read_file(input_core1, input_core1_path);
    read_file(input_core2, input_core2_path);
    read_file(input_core3, input_core3_path);
    read_file(input_core4, input_core4_path);

    // In dòng thứ x từ core0
    int x = 1; // Dòng cần in (bắt đầu từ 0)
    printf("Dữ liệu dòng %d từ core0:\n", x);
    for (int i = 0; i < 256; i++)
    {
        printf("%d ", core0[x][i]);
    }
    printf("\n");

    return 0;
}
