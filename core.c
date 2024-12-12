#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "input.c"
struct neuron_struct
{
    int negative_threshold;
    int positive_threshold;
    int reset_value_negative;
    int reset_value_positive;
    int **connect;
    int *weight;
    int current;
};

struct cord_struct
{
    int dx, dy;
};

struct core_struct
{
    int num_neuron;
    struct neuron_struct *csram;
    struct cord_struct cord;
    int scheduler[256];
    struct cord_struct cord_target;

    int axon_target_first;
    bool is_output_layer;
};

#define num_core 5                      // Ví dụ: số lượng core
struct core_struct core_data[num_core]; // Cố định kích thước mảng
bool output[250] = {0};
int MAX_NEURONS = 256;
void run_core(struct core_struct *core)
{
    for (int i = 0; i < core->num_neuron; i++)
    {
        for (int j = 0; j < 256; j++)
        {
            core->csram[i].current += core->scheduler[j] * core->csram->connect[i][j] * core->csram[i].weight[j % 4];
        }

        if (core->csram[i].current >= core->csram[i].positive_threshold)
        {
            core->csram[i].current = core->csram[i].reset_value_positive;
            for (int k = 0; k < num_core; k++)
            {
                if (core->is_output_layer)
                {
                    output[i] = 1;
                }
                else
                {
                    if (core_data[k].cord.dx == core->cord_target.dx && core_data[k].cord.dy == core->cord_target.dy)
                    {
                        core_data[k].scheduler[core->axon_target_first + i] = 1;
                    }
                }
            }
        }
        else if (core->csram[i].current <= core->csram[i].negative_threshold)
        {
            core->csram[i].current = core->csram[i].reset_value_negative;
        }
    }
}

void insert_data(struct core_struct *core_)
{
    char core_path[] = "D:\\Tai_lieu_20241\\project_1\\code\\data_input\\core0.txt";

    char *core_index = get_index_pos(core_path);

    if (!core_index)
    {
        printf("Đường dẫn file không hợp lệ!\n");
        return;
    }

    MATRIX core[SIZE];

    for (int i = 0; i < SIZE; i++)
    {
        *core_index = i + '0';
        core[i] = create_matrix();
        if (!core[i] || read_file(core[i], core_path) == -1)
        {
            printf("Lỗi khi đọc file core%d\n", i);
            return;
        }
    }

    /////// chèn dữ liệu vào //////////////

    for (int i = 0; i < num_core; i++)
    {
        core_[i].num_neuron = core[i][0][0];

        core_[i].csram->negative_threshold = core[i][1][0];
        core_[i].csram->negative_threshold = core[i][2][0];
        core_[i].csram->reset_value_negative = core[i][3][0];
        core_[i].csram->reset_value_positive = core[i][4][0];

        core_[i].cord.dx = core[i][5][0];
        core_[i].cord.dy = core[i][5][1];

        core_[i].cord_target.dx = core[i][6][0];
        core_[i].cord_target.dy = core[i][6][1];

        core_[i].axon_target_first = core[i][7][0];

        core_[i].is_output_layer = core[i][8][0];

        for (int j = 0; j < core_[i].num_neuron; j++)
        {
            for (int k = 0; k < 256; k++)
            {
                core_[i].csram->connect[j][k] = core[i][j + 8][k];
            }
        }
        int x[4] = {1, -1, 1, -1};
        core_[i].csram->weight = x;
    }

    // giải phóng bộ nhớ
    for (int i = 0; i < SIZE; i++)
        dispose(core[i]);
}
void insert_input(struct core_struct *core_, int image_index)
{
    char input_path[] = "D:\\Tai_lieu_20241\\project_1\\code\\data_input\\input_core1.txt";
    char *input_index = get_index_pos(input_path);

    if (!input_index)
    {
        printf("Đường dẫn file không hợp lệ!\n");
        return;
    }
    MATRIX input_core[4]; // Chỉ cần 3 phần tử

    for (int i = 0; i < 4; i++)
    {
        *input_index = i + '0';
        input_core[i] = create_matrix();
        if (!input_core[i] || read_file(input_core[i], input_path) == -1)
        {
            printf("Lỗi khi đọc file input_core%d\n", i);
            return;
        }
    }

    for (int i = 0; i < num_core; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (input_core[j][0][0] == core_[i].cord.dx && input_core[j][0][1] == core_[i].cord.dy)
            {
                for (int k = 0; k < 256; k++)
                {
                    core_[i].scheduler[k] = input_core[j][image_index + 1][k];
                }
            }
        }
    }
    for (int i = 0; i < 4; i++)
        dispose(input_core[i]);
}
void allocate_memory(struct core_struct *core)
{
    for (int i = 0; i < num_core; i++)
    {
        core[i].csram = malloc(MAX_NEURONS * sizeof(struct neuron_struct));
        for (int j = 0; j < MAX_NEURONS; j++)
        {
            core[i].csram[j].connect = malloc(MAX_NEURONS * sizeof(int *));
            for (int k = 0; k < MAX_NEURONS; k++)
            {
                core[i].csram[j].connect[k] = malloc(256 * sizeof(int));
            }
            core[i].csram[j].weight = malloc(4 * sizeof(int));
        }
    }
}

void free_memory(struct core_struct *core)
{
    for (int i = 0; i < num_core; i++)
    {
        for (int j = 0; j < MAX_NEURONS; j++)
        {
            for (int k = 0; k < MAX_NEURONS; k++)
            {
                free(core[i].csram[j].connect[k]);
            }
            free(core[i].csram[j].connect);
            // free(core[i].csram[j].weight);
        }
        free(core[i].csram);
    }
}
int main()
{
    allocate_memory(core_data);
    insert_data(core_data);
    insert_input(core_data, 0);
    for (int i = 0; i < num_core; i++)
    {
        run_core(&core_data[i]);
    }

    int vote[10] = {0};
    for (int i = 0; i < 250; i++)
    {
        if (output[i])
        {
            vote[i % 10]++;
        }
    }

    int output_final = 0;
    for (int i = 0; i < 10; i++)
    {
        if (vote[output_final] < vote[i])
        {
            output_final = i;
        }
    }

    printf("Output final: %d\n", output_final);
    free_memory(core_data);
    return 0;
}
