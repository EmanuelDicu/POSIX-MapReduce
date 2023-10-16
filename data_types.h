#ifndef DATA_TYPES_H
#define DATA_TYPES_H

#include <pthread.h>

/* Node of a list */
struct node
{
    int value;
    struct node *next;
};

/* List of integers */
struct list
{
    int length;
    struct node *head;
};

/* For a specific thread, keep an array of lists for each exponent */
struct thread_list
{
    int expo_list_size;
    struct list *expo_list;
};

/* Data shared between threads */
struct thread_data 
{
    int M; /* Number of mappers */
    int R; /* Number of reducers */
    int nr_input_files; /* Number of available input files */
    int *file_counter; /* Current unprocessed input file */
    char **input_file; /* Array of all input file names */
    char **output_file; /* Array of all output file names */
    struct thread_list *thread_list; /* Lists returned by each mapper */
    pthread_mutex_t *file_counter_mutex; /* Mutex for current file */
    pthread_barrier_t *thread_barrier; /* Barrier for map-reduce pipeline */
};

/* Thread info struct */
struct thread_info 
{
    pthread_t thread_id; /* ID returned by pthread_create() */
    int tnum; /* Application-defined thread number */
    struct thread_data *tdata; /* Thread data */
};

/* Create a thread data struct */
struct thread_data* create_thread_data(
    int M, 
    int R, 
    int nr_input_files,
    int *file_counter, 
    char **input_file,
    char **output_file,
    struct thread_list *thread_list, 
    pthread_mutex_t *file_counter_mutex,
    pthread_barrier_t *thread_barrier
);

/* Create list node */
struct node* create_node(int value, struct node *next);

/* Insert node at the beginning of list */
struct list* insert_into_list(struct list *list, int value);

/* Free list memory */
void free_list(struct list *list);

#endif /* DATA_TYPES_H */