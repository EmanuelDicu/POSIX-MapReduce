#include "data_types.h"

void 
aggregate_mapper_list(struct list *aggr_list, struct list *mapper_list)
{
    struct node *node;
    
    for (node = mapper_list->head; node != NULL; node = node->next)
        insert_into_list(aggr_list, node->value);
}

int 
cmp_func(const void *p1, const void *p2)
{
    return *(int *)p1 - *(int *)p2;
}