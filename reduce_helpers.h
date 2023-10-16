#ifndef REDUCE_HELPERS_H
#define REDUCE_HELPERS_H

/* Merge two lists */
void aggregate_mapper_list(struct list *aggr_list, struct list *mapper_list);

/* Compare function for qsort */
int cmp_func(const void *p1, const void *p2);

#endif /* REDUCE_HELPERS_H */