#include "queue.h"
#include <stdlib.h>

void init_queue(struct queue *q) {
    q->data = NULL;
    q->capacity = 0;
    q->offset = 0;
    q->length = 0;
}

void clear_queue(struct queue *q) {
    free(q->data);
    init_queue(q);
}

void enqueue(struct queue *q, int value) {
    // If there is no more space, allocate more space
    if (q->length == q->capacity) {
        size_t new_capacity = q->capacity > 0 ? q->capacity * 2 : 4;
        int *new_data = realloc(q->data, sizeof(int) * new_capacity);
        if (new_data == NULL) {
            // Return some error or something
            return;
        }
        q->data = new_data;
        q->capacity = new_capacity;
    }

    // Perform enqueue
    size_t index = (q->offset + q->length) % q->capacity;
    q->data[index] = value;
    q->length++;
}

bool dequeue(struct queue *q, int *out) {
    // If there is nothing to dequeue then return false
    if (queue_empty(q)) {
        return false;
    }

    // Perform dequeue
    *out = q->data[q->offset];
    q->offset = (q->offset + 1) % q->capacity;
    q->length--;

    // Dynamic circular queue implementation
    if (q->length > 0 && q->length == q->capacity / 4) {
        size_t new_capacity = q->capacity / 2;
        int *new_data = malloc(sizeof(int) * new_capacity);
        if (new_data == NULL) {
            // Return some error or something
            return true;
        }
        for (size_t i = 0; i < q->length; i++) {
            new_data[i] = q->data[(q->offset + i) % q->capacity];
        }
        free(q->data);
        q->data = new_data;
        q->capacity = new_capacity;
        q->offset = 0;
    }

    return true;
}

bool queue_empty(struct queue *q) {
    return q->length == 0;
}
