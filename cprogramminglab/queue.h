/* Linked list element */
typedef struct list_ele {
    char *value;
    struct list_ele *next;
} list_ele_t;

/* Queue structure */
typedef struct {
    list_ele_t *head; /* First element in the queue */
    int size;
} queue_t;

// Create a new, empty queue.
queue_t *queue_new(void);
//Free all storage used by a queue.
void queue_free(queue_t *queue);
//Attempt to insert a new element at the head of the queue (LIFO discipline).
void queue_insert_head(queue_t *queue, char *value);
//Attempt to insert a new element at the tail of the queue (FIFO discipline).
void queue_insert_tail(queue_t *queue, char *value);
//Attempt to remove the element at the head of the queue.
char *queue_remove_head(queue_t *queue);
//Compute the number of elements in the queue.
int queue_size(queue_t *queue);
/*
* Reorder the list so that the queue elements are reversed in order.
* This function should not allocate or free any list elements
* (either directly or via calls to other functions that allocate or free list elements.)
* Instead, it should rearrange the existing elements.
 */
void queue_reverse(queue_t *queue);
//show all values in the queue
void queue_print(queue_t *queue);
