#include "queue.h"

#include <stdlib.h>

queue_t *queue_new() {
    queue_t *q = (queue_t *)malloc(sizeof(queue_t));
    if (q == NULL) {
        return NULL;  // 分配失败时返回 NULL
    }
    list_ele_t *head = (list_ele_t *)malloc(sizeof(list_ele_t));
    head->value = NULL;
    head->next = NULL;
    q->head = head;
    q->size = 0;
    return q;
}

void queue_free(queue_t *q) {
    if (q->head != NULL) {
        list_ele_t *curr = q->head;
        while (curr->next != NULL) {
            list_ele_t *p = curr->next;
            free(curr);
            curr = p;
        }
        free(curr);
    }
    free(q);
}

void queue_insert_head(queue_t *q, char *val) {
    list_ele_t *new = (list_ele_t *)malloc(sizeof(list_ele_t));
    new->value = val;
    if (q->head != NULL) {
        new->next = q->head;
    } else {
        new->next = NULL;
    }
    q->head = new;
    q->size++;
}

void queue_insert_tail(queue_t *q, char *val) {
    list_ele_t *new = (list_ele_t *)malloc(sizeof(list_ele_t));
    new->value = val;
    new->next = NULL;
    if (q->head == NULL) {
        new->next = q->head;
    } else {
        list_ele_t *curr = q->head;
        while (curr->next != NULL) {
            curr = curr->next;
        }
        curr->next = new;
    }
    q->size++;
}

char *queue_remove_head(queue_t *q) {
    if (q->head == NULL) {
        return NULL;
    } else {
        if (q->head->value== NULL) {
            return NULL;
        } else {
            char *val = q->head->value;
            if (q->head->next == NULL) {
                q->head = NULL;
            } else {
                q->head = q->head->next;
            }
            return val;
        }
    }
}

int queue_size(queue_t *q) {
    return q->size;
}

void list_ele_exchange(list_ele_t *head, list_ele_t *tail) {
    list_ele_t *head_next = head->next;
    head->next = head_next->next;
    head_next->next = tail->next;
    tail->next = head_next;
}

list_ele_t *queue_gettail(queue_t* q) {
    if (q->head == NULL) {
        return NULL;
    } else {
        list_ele_t *curr = q->head;
        while (curr->next != NULL) {
            curr = curr->next;
        }
        return curr;
    }
}

void queue_reverse(queue_t *q) {
    list_ele_t *tail = queue_gettail(q);
    for (int i = 0; i < q->size; i++) {
        list_ele_exchange(q->head, tail);
    }
}
