#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "data_types.h"
#include "handle_error.h"
#include "map_reduce.h"

#define MAX_FILESIZE 256

int 
main(int argc, const char *argv[])
{
    int M;
    int R;
    int file_counter;
    int nr_input_files;
    int nr_output_files;
    char *file_name;
    char **input_file;
    char **output_file;
    struct thread_info *mapper_tinfo, *reducer_tinfo;
    void *res;
    struct thread_list *thread_list;
    pthread_mutex_t mutex;
    pthread_barrier_t barrier;

    int tnum, s, expo, fnum;
    FILE *fd;

    if (argc != 4)
        handle_error(
            "usage: ./tema1 <num_mappers> <num_reducers> <input_file>");

    M = atoi(argv[1]);
    if (M == 0)
        handle_error("number of mapper threads argument");
    R = atoi(argv[2]);
    if (R == 0)
        handle_error("number of reducer threads argument");

    /* Create synchronization primitives */

    s = pthread_mutex_init(&mutex, NULL);
    if (s)
        handle_error_en(s, "mutex init");
    s = pthread_barrier_init(&barrier, NULL, M + R);
    if (s)
        handle_error_en(s, "barrier init");
    
    /* Add input files to thread data */

    file_name = strndup(argv[3], MAX_FILESIZE);
    if (file_name == NULL)
        handle_error("file_name allocation");
    fd = fopen(file_name, "r");
    if (fd == NULL)
        handle_error("open input file");
    fscanf(fd, "%d\n", &nr_input_files);
    input_file = calloc(nr_input_files, sizeof(*input_file));
    if (input_file == NULL)
        handle_error("input_file array allocation");
    for (fnum = 0; fnum < nr_input_files; ++fnum) {
        input_file[fnum] = calloc(MAX_FILESIZE, sizeof(*input_file[fnum]));
        if (input_file[fnum] == NULL)
            handle_error("input file allocation");
        fscanf(fd, "%s\n", input_file[fnum]);
    }
    fclose(fd);

    /* Add output files to thread data */

    nr_output_files = R;
    output_file = calloc(nr_output_files, sizeof(*output_file));
    if (output_file == NULL)
        handle_error("output_file array allocation");
    for (fnum = 0; fnum < nr_output_files; ++fnum) {
        expo = fnum + 2;
        output_file[fnum] = 
            calloc(MAX_FILESIZE, sizeof(*output_file[fnum]));
        if (output_file[fnum] == NULL)
            handle_error("output file allocation");
        snprintf(output_file[fnum], MAX_FILESIZE, "out%d.txt", expo);
    }

    /* Allocate thread lists */

    file_counter = 0;
    thread_list = calloc(M, sizeof(*thread_list));
    if (thread_list == NULL)
        handle_error("map list allocation");
    for (tnum = 0; tnum < M; ++tnum) {
        thread_list[tnum].expo_list_size = R;
        thread_list[tnum].expo_list = 
            calloc(R, sizeof(*thread_list[tnum].expo_list));
        if (thread_list[tnum].expo_list == NULL)
            handle_error("map list allocation");
        for (expo = 2; expo < R + 2; ++expo) {
            thread_list[tnum].expo_list[expo - 2].length = 0;
            thread_list[tnum].expo_list[expo - 2].head = NULL;
        }
    }
    
    /* Set thread data structures */

    mapper_tinfo = calloc(M, sizeof(*mapper_tinfo));
    if (mapper_tinfo == NULL)
        handle_error("mapper thread allocation");
    for (tnum = 0; tnum < M; ++tnum) {
        mapper_tinfo[tnum].tnum = tnum;
        mapper_tinfo[tnum].tdata = create_thread_data(
            M, R, nr_input_files, &file_counter, 
            input_file, output_file, thread_list, &mutex, &barrier);
    }

    reducer_tinfo = calloc(R, sizeof(*reducer_tinfo));
    if (reducer_tinfo == NULL)
        handle_error("reducer thread allocation");
    for (tnum = 0; tnum < R; ++tnum) {
        reducer_tinfo[tnum].tnum = tnum;
        reducer_tinfo[tnum].tdata = create_thread_data(
            M, R, nr_input_files, &file_counter, 
            input_file, output_file, thread_list, &mutex, &barrier);
    }

    /* Create threads */

    for (tnum = 0; tnum < M; ++tnum) {
        s = pthread_create(&mapper_tinfo[tnum].thread_id, 
                NULL, &map_method, &mapper_tinfo[tnum]);
        if (s)
            handle_error_en(s, "create mapper thread");
    }

    for (tnum = 0; tnum < R; ++tnum) {
        s = pthread_create(&reducer_tinfo[tnum].thread_id, 
                NULL, &reduce_method, &reducer_tinfo[tnum]);
        if (s)
            handle_error_en(s, "create reducer thread");
    }

    /* Join threads */ 

    for (tnum = 0; tnum < M; ++tnum) {
        s = pthread_join(mapper_tinfo[tnum].thread_id, &res);
        if (s)
            handle_error_en(s, "wait mapper thread");
    }

    for (tnum = 0; tnum < R; ++tnum) {
        s = pthread_join(reducer_tinfo[tnum].thread_id, &res);
        if (s)
            handle_error_en(s, "wait reducer thread");
    }

    /* Freeup memory */

    pthread_mutex_destroy(&mutex);
    pthread_barrier_destroy(&barrier);

    for (fnum = 0; fnum < nr_input_files; ++fnum) 
        free(input_file[fnum]);
    free(input_file);
    for (fnum = 0; fnum < nr_output_files; ++fnum)
        free(output_file[fnum]);
    free(output_file);

    free(file_name);

    for (tnum = 0; tnum < M; ++tnum) {
        for (expo = 2; expo < R + 2; ++expo) 
            free_list(&thread_list[tnum].expo_list[expo - 2]);
        free(thread_list[tnum].expo_list);
    }
    free(thread_list);

    for (tnum = 0; tnum < M; ++tnum)
        free(mapper_tinfo[tnum].tdata);
    free(mapper_tinfo);

    for (tnum = 0; tnum < R; ++tnum)
        free(reducer_tinfo[tnum].tdata);
    free(reducer_tinfo);

    return 0;
}