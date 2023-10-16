#include <pthread.h>
#include <stdlib.h>

#include "data_types.h"
#include "handle_error.h"

struct thread_data* 
create_thread_data(
    int M, 
    int R, 
    int nr_input_files,
    int *file_counter, 
    char **input_file,
    char **output_file,
    struct thread_list *thread_list, 
    pthread_mutex_t *file_counter_mutex,
    pthread_barrier_t *thread_barrier
) {
    struct thread_data *data;

    data = calloc(1, sizeof(*data));
    data->M = M;
    data->R = R;
    data->nr_input_files = nr_input_files;
    data->file_counter = file_counter;
    data->input_file = input_file;
    data->output_file = output_file;
    data->thread_list = thread_list;
    data->file_counter_mutex = file_counter_mutex;
    data->thread_barrier = thread_barrier;

    return data;
}

struct node*
create_node(int value, struct node *next)
{
    struct node *node;
    
    node = calloc(1, sizeof(*node));
    if (node == NULL)
        handle_error("create node");

    node->value = value;
    node->next = next;

    return node;
}

struct list*
insert_into_list(struct list *list, int value)
{
    struct node *new_head;
    
    new_head = create_node(value, list->head);
    list->head = new_head;
    list->length += 1;

    return list;
}

void 
free_list(struct list *list)
{
    struct node *node, *next;

    node = list->head;
    while (node != NULL) {
        next = node->next;
        free(node);
        node = next;
    }
}