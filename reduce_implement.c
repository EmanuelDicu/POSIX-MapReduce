#include <stdio.h>
#include <stdlib.h>

#include "handle_error.h"
#include "data_types.h"
#include "reduce_helpers.h"

void
process_aggregate_list(struct list *aggr_list, char *file_name) 
{
    FILE *fd;
    int pos, nr_dist, length;
    int *array;
    struct node *node;

    /* Convert the list into an array */
    length = aggr_list->length;
    array = calloc(length, sizeof(*array));
    if (array == NULL)
        handle_error("array creation");
    for (node = aggr_list->head, pos = 0; node != NULL; node = node->next, ++pos)
        array[pos] = node->value;

    /* Sort array and check number of unique elements */
    qsort(array, length, sizeof(*array), cmp_func);
    nr_dist = 0;
    for (pos = 0; pos < length; ++pos) {
        if (pos == 0 || array[pos - 1] != array[pos])
            ++nr_dist;
    }
    
    /* Print number of unique elements to output file */
    fd = fopen(file_name, "w");
    if (fd == NULL)
        handle_error("output file creation");
    fprintf(fd, "%d", nr_dist);

    free(array);
    fclose(fd);
}