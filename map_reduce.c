#include <pthread.h>
#include <stdlib.h>

#include "map_implement.h"
#include "reduce_implement.h"
#include "reduce_helpers.h"
#include "handle_error.h"
#include "data_types.h"

void*
map_method(void *arg)
{
    struct thread_info *tinfo; 
    struct thread_data *tdata;
    int file_id, tnum;

    tinfo = (struct thread_info *) arg;
    tdata = tinfo->tdata;
    tnum = tinfo->tnum;
    
    /* While there are stil unprocessed input files */
    while (1) { 
        /* Select a file for processing and increment counter */
        pthread_mutex_lock(tdata->file_counter_mutex); 
        file_id = *tdata->file_counter; 
        if (file_id != tdata->nr_input_files) 
            (*tdata->file_counter) += 1;
        pthread_mutex_unlock(tdata->file_counter_mutex);

        /* If there are no more files to process */
        if (file_id == tdata->nr_input_files) 
            break;
        
        process_input_file(tdata->input_file[file_id], &tdata->thread_list[tnum]);
    }

    /* Signal that mapper has finished */
    pthread_barrier_wait(tdata->thread_barrier); 
    pthread_exit(NULL);
}

void* 
reduce_method(void *arg)
{
    struct thread_info *tinfo = (struct thread_info *) arg;
    struct thread_data *tdata = tinfo->tdata;
    struct list *aggr_list;
    int id, tnum;

    /* Reducers must wait for all mappers to finish */
    pthread_barrier_wait(tdata->thread_barrier);

    /* Create and process an aggregated list of all resulting 
        lists produced by all mappers for a given exponent */
    tnum = tinfo->tnum;
    aggr_list = calloc(1, sizeof(*aggr_list));
    if (aggr_list == NULL)
        handle_error("aggr_list allocation");
    for (id = 0; id < tdata->M; ++id) 
        aggregate_mapper_list(aggr_list, &(tdata->thread_list[id]).expo_list[tnum]);
    process_aggregate_list(aggr_list, tdata->output_file[tnum]);

    free_list(aggr_list);
    free(aggr_list);
    pthread_exit(NULL);
}
