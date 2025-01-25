#include <stdio.h>

#include "queue.h"

int main(void) {
    queue_t *q = queue_new();

    queue_insert_head(q, "dolphin");
    queue_insert_head(q, "bear");
    queue_insert_head(q, "gerbil");

    queue_insert_tail(q, "meerkat");
    queue_insert_tail(q, "bear");

    queue_reverse(q);

    printf("%d", queue_size(q));

    queue_free(q);

    return 0;
}