#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "input.c"
#include "parameter.c"
struct neuron_struct
{
    int negative_threshold;
    int positive_threshold;
    int reset_value_negative;
    int reset_value_positive;
    int *connect;
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


struct core_struct core_data[num_core];
int label[number_of_samples];
bool output[num_neuron_output] = {0};

void run_core(struct core_struct *core)
{
    for (int i = 0; i < core->num_neuron; i++)
    {
        core->csram[i].current = 0;
        for (int j = 0; j < num_axon; j++)
        {
            core->csram[i].current += core->scheduler[j] * core->csram[i].connect[j] * core->csram[i].weight[j % 4];
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
        core->csram[i].current = 0;
    }
    for (int i = 0; i < num_axon; i++)
    {
        core->scheduler[i] = 0;
    }
}

void insert_data(struct core_struct *core_)
{
    char core_path[] = "data_input\\core0.txt";

    char *core_index = get_index_pos(core_path);

    if (!core_index)
    {
        printf("Đường dẫn file không hợp lệ!\n");
        return;
    }

    MATRIX core[num_core];

    for (int i = 0; i < num_core; i++)
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

        core_[i].cord.dx = core[i][5][0];
        core_[i].cord.dy = core[i][5][1];

        core_[i].cord_target.dx = core[i][6][0];
        core_[i].cord_target.dy = core[i][6][1];

        core_[i].axon_target_first = core[i][7][0];

        core_[i].is_output_layer = core[i][8][0];

        for (int j = 0; j < core_[i].num_neuron; j++)
        {
            core_[i].csram[j].negative_threshold = core[i][1][0];
            core_[i].csram[j].positive_threshold = core[i][2][0];
            core_[i].csram[j].reset_value_negative = core[i][3][0];
            core_[i].csram[j].reset_value_positive = core[i][4][0];
            for (int k = 0; k < num_axon; k++)
            {
                core_[i].csram[j].connect[k] = core[i][j + 9][k];
            }
            // int x[4] = {1, -1, 1, -1};
            core_[i].csram[j].weight[0] = 1;
            core_[i].csram[j].weight[1] = -1;
            core_[i].csram[j].weight[2] = 1;
            core_[i].csram[j].weight[3] = -1;
        }
    }

    // giải phóng bộ nhớ
    for (int i = 0; i < num_core; i++)
        dispose(core[i]);
}
void insert_input(struct core_struct *core_, int image_index)
{
    char input_path[] = "data_input\\input_core0.txt";
    char *input_index = get_index_pos(input_path);

    if (!input_index)
    {
        printf("Đường dẫn file không hợp lệ!\n");
        return;
    }
    MATRIX input_core[num_core_input]; // Chỉ cần 3 phần tử

    for (int i = 0; i < num_core_input; i++)
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
        for (int j = 0; j < num_core_input; j++)
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
    for (int i = 0; i < num_core_input; i++)
        dispose(input_core[i]);
}
void allocate_memory(struct core_struct *core)
{
    for (int i = 0; i < num_core; i++)
    {
        core[i].csram = malloc(num_axon * sizeof(struct neuron_struct));
        for (int j = 0; j < num_axon; j++)
        {
            core[i].csram[j].connect = malloc(num_axon * sizeof(int));
            core[i].csram[j].weight = malloc(4 * sizeof(int));
        }
    }
}
void label_()
{
    char label_path[] = "data_input\\label.txt";
    read_label(label, label_path);
}
void free_memory(struct core_struct *core)
{
    for (int i = 0; i < num_core; i++)
    {
        for (int j = 0; j < num_axon; j++)
        {
            free(core[i].csram[j].connect);
            free(core[i].csram[j].weight);
        }
        free(core[i].csram);
    }
}
void reset_output()
{
    for (int i = 0; i < num_neuron_output; i++)
    {
        output[i] = 0;
    }
}
int main()
{
    allocate_memory(core_data);
    insert_data(core_data);
    label_();
    
    int count = 0;
    for (int a=0; a < number_of_samples; a++)
    {
        insert_input(core_data, a);
        for (int i = 0; i < num_core; i++)
        {
            run_core(&core_data[i]);
        }
        // for(int i =0; i<256; i++){
        //     core_data[4].scheduler[i] =0;
        // }

        int vote[10] = {0};
        for (int i = 0; i < num_neuron_output; i++)
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
        reset_output();
         //printf("%d", output_final);
         //printf("%d \n", label[a]);

        if (output_final == label[a])
            count++;
    }
   
    printf("%f", (float)count / number_of_samples);
    free_memory(core_data);
    return 0;
}