#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

struct heap_element {
    int priority;
    const char *data;
};

struct heap {
    struct heap_element *data;
    size_t capacity, length;
};

// Change made to code here: Formulas for left, middle and right child adjusted to accomodate the additional child, added middle child
// Computes the index of the first child, given the parent index.
size_t left_child_of(size_t index) {
    return 3 * index + 1;
}

// Computes the index of the second child, given the parent index.
size_t middle_child_of(size_t index) {
    return 3 * index + 2;
}

// Computes the index of the third child, given the parent index.
size_t right_child_of(size_t index) {
    return 3 * index + 3;
}

// Computes the index of the parent, given a child index.
size_t parent_of(size_t index) {
    return (index - 1) / 3;
}

// Swap two heap elements.
void heap_swap(struct heap_element *a, struct heap_element *b) {
    struct heap_element temp = *a;
    *a = *b;
    *b = temp;
}

// Move the element at the given index up the heap until it is in the correct position.
// All ancestors of the given element must be correctly ordered; descendants of the element are ignored.
void heap_up(struct heap *h, size_t index) {
    // Loop until we reach the root element.
    while (index > 0) {
        size_t parent = parent_of(index);

        // If the element is correctly ordered with respect to its parent, then it is in the correct position.
        if (h->data[parent].priority <= h->data[index].priority)
            break;

        // If it is incorrectly ordered, swap with its parent then continue.
        heap_swap(&h->data[index], &h->data[parent]);
        index = parent;
    }
}

// Move the element at the given index down the heap until it is in the correct position (for min-heap).
void heap_down(struct heap *h, size_t index) {
    while (true) {
        // Compute the index of the left, right and middle child. 
        size_t left = left_child_of(index);
        size_t middle = middle_child_of(index);
        size_t right = right_child_of(index);

        // Start with the current index as the smallest
        size_t smallest = index;

        // Check if the left / middle / right child exists and is smaller than the current smallest. 
        // If the smallest is still the current index, the heap property is satisfied
        if (left < h->length && h->data[left].priority < h->data[smallest].priority) smallest = left;
        if (middle < h->length && h->data[middle].priority < h->data[smallest].priority) smallest = middle;
        if (right < h->length && h->data[right].priority < h->data[smallest].priority) smallest = right;
        if (smallest == index) break;

        // Swap the current element with the smallest of its children
        heap_swap(&h->data[index], &h->data[smallest]);
        index = smallest;
    }
}

void heap_insert(struct heap *h, int priority, const char *data) {
    // Allocate or re-allocate as needed.
    if (h->capacity == 0) {
        h->capacity = 1;
        h->data = malloc(sizeof(struct heap_element));
    } else if (h->length == h->capacity) {
        h->capacity *= 2;
        h->data = realloc(h->data, h->capacity * sizeof(struct heap_element));
    }

    // Set up new element.
    struct heap_element el = {
            .priority = priority,
            data = data,
    };

    // Insert new element at bottom of heap, then heap-up to correctly place it.
    size_t index = h->length++;
    h->data[index] = el;
    heap_up(h, index);
}

bool heap_remove(struct heap *h, int *priority_out, const char **data_out) {
    // Check if the heap is empty.
    if (h->length == 0)
        return false;

    // Extract root element.
    *priority_out = h->data[0].priority;
    *data_out = h->data[0].data;

    // Move last element to root, then heap-down to correctly place it.
    h->data[0] = h->data[--h->length];
    heap_down(h, 0);

    // Shrink allocation if applicable.
    if (h->length == 0) {
        h->capacity = 0;
        free(h->data);
        h->data = NULL;
    } else if (h->length * 2 < h->capacity) {
        h->capacity /= 2;
        h->data = realloc(h->data, h->capacity * sizeof(struct heap_element));
    }

    return true;
}

int main() {
    struct heap h = {
            .data = NULL,
            .capacity = 0,
            .length = 0,
    };

    heap_insert(&h, 7, "this");
    heap_insert(&h, 3, "is");
    heap_insert(&h, 0, "an");
    heap_insert(&h, 9, "example");
    heap_insert(&h, 3, "piece");
    heap_insert(&h, 4, "of");
    heap_insert(&h, 8, "text");
    heap_insert(&h, 8, "to");
    heap_insert(&h, 2, "make");
    heap_insert(&h, 1, "sure");
    heap_insert(&h, 7, "your");
    heap_insert(&h, 5, "code");
    heap_insert(&h, 3, "works");

    for (size_t i = 0; i < h.length; i++)
        printf("%d: %d %s\n", (int) i, h.data[i].priority, h.data[i].data);

    puts("Processing all elements according to priority:");

    int priority;
    const char *data;
    while (heap_remove(&h, &priority, &data))
        printf("%d %s\n", priority, data);

    return 0;
}
