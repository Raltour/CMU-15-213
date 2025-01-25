#include <stdio.h>

#include "queue.h"

int main(void) {
    queue_t *q = queue_new();
    queue_print(q);

    queue_insert_head(q, "dolphin");
    queue_insert_head(q, "bear");
    queue_insert_head(q, "gerbil");
    queue_print(q);

    queue_insert_tail(q, "meerkat");
    queue_insert_tail(q, "bear");
    queue_print(q);

    queue_reverse(q);
    queue_print(q);

    printf("%d\n", queue_size(q));

    queue_free(q);

    return 0;
}