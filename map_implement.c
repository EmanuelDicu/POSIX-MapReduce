#include <stdio.h>

#include "handle_error.h"
#include "data_types.h"
#include "numeric_helpers.h"

void 
process_input_file(char *file_name, struct thread_list *thread_list)
{
    FILE *fd;
    int num_cnt, nr, expo, R;

    R = thread_list->expo_list_size; /* number of exponents to check */

    fd = fopen(file_name, "r"); /* open input file for processing */
    if (fd == NULL)
        handle_error("open input file");

    fscanf(fd, "%d", &num_cnt);
    while (num_cnt--) { 
        fscanf(fd, "%d", &nr);  /* Process numbers one by one */

        for (expo = 2; expo < R + 2; ++expo) {
            if (is_power(nr, expo)) /* If it is a power of expo, add to list */
                insert_into_list(&thread_list->expo_list[expo - 2], nr); 
        }
    }
    fclose(fd);
}