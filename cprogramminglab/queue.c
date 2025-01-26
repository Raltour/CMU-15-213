#include "queue.h"

#include <stdio.h>
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
    q->size = 1;
    return q;
}

void queue_free(queue_t *q) {
    if (q->head != NULL) {
        list_ele_t *curr = q->head;
        while (curr->next != NULL) {
            list_ele_t *p = curr->next;
            if (curr->value != NULL) {
                free(curr->value);  // 释放节点的value
            }
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
        q->head = new;
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
        list_ele_t *temp = q->head;
        q->head = q->head->next;
        q->size--;
        if (temp->value == NULL) {
            free(temp);
            return NULL;
        } else {
            free(temp);
            return temp->value;
        }
    }
}

int queue_size(queue_t *q) {
    return q->size;
}

void list_ele_exchange(queue_t *q, list_ele_t *tail) {
    list_ele_t *temp = q->head;
    q->head = temp->next;
    temp->next = tail->next;
    tail->next = temp;
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
    for (int i = 0; i < q->size - 1; i++) {
        list_ele_exchange(q, tail);
    }
}

void queue_print(queue_t *q) {
    if (q->head == NULL) {
        printf("Queue is empty\n");
    } else {
        list_ele_t *curr = q->head;
        while (curr->next != NULL) {
            if (curr->value != NULL) {
                printf("%s\n", curr->value);
            } else {
                printf("NULL in this palce\n");
            }
            curr = curr->next;
        }
        if (curr->value != NULL) {
            printf("%s\n", curr->value);
        } else {
            printf("NULL in this palce.\n");
        }
    }
    printf("\n");
}