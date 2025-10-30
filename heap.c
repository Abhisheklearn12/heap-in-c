// Generic binary heap implementation in C
// -----------------------------------------------
// - Fully generic: stores void* elements
// - Custom comparator for min- or max-heap behavior
// - Supports build, insert, extract, replace, clone, validation, and sort
// - Designed for correctness, clarity, and extensibility
//
// This can be used as a base for
// priority queues, schedulers, graph algorithms, etc.
//
// -----------------------------------------------

#include "heap.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <sys/types.h>  // for ssize_t

#define HEAP_DEFAULT_CAP 16

// Core heap structure - opaque to users.
struct Heap {
    void **data;          // dynamic array of pointers
    size_t size;          // current number of elements
    size_t capacity;      // allocated capacity
    heap_cmp_fn cmp;      // user-provided comparison function
};

// --- Utility index helpers ---
// Parent/left/right are derived from binary tree layout in an array.
static inline size_t parent(size_t i) { return (i - 1) / 2; }
static inline size_t left(size_t i)   { return 2 * i + 1; }
static inline size_t right(size_t i)  { return 2 * i + 2; }

// Swap two array slots.
static inline void swap(void **a, void **b) {
    void *tmp = *a;
    *a = *b;
    *b = tmp;
}

// --- Heapify helpers ---

// Bubble element at index `i` up until heap property is restored.
// Runs in O(log n).
static void sift_up(Heap *h, size_t i) {
    while (i > 0) {
        size_t p = parent(i);
        if (h->cmp(h->data[i], h->data[p]) <= 0)
            break;
        swap(&h->data[i], &h->data[p]);
        i = p;
    }
}

// Push element at index `i` down until heap property holds.
// Also O(log n).
static void sift_down(Heap *h, size_t i) {
    for (;;) {
        size_t l = left(i), r = right(i), largest = i;

        // Choose the larger child (for max-heap)
        if (l < h->size && h->cmp(h->data[l], h->data[largest]) > 0)
            largest = l;
        if (r < h->size && h->cmp(h->data[r], h->data[largest]) > 0)
            largest = r;

        if (largest == i)
            break;  // property restored
        swap(&h->data[i], &h->data[largest]);
        i = largest;
    }
}

// -----------------------------------------------------------
// Public API
// -----------------------------------------------------------

// Create an empty heap with optional initial capacity.
Heap *heap_create(heap_cmp_fn cmp, size_t capacity) {
    if (!cmp) return NULL;
    if (capacity == 0) capacity = HEAP_DEFAULT_CAP;

    Heap *h = calloc(1, sizeof(*h));
    if (!h) return NULL;

    h->data = malloc(capacity * sizeof(void *));
    if (!h->data) { free(h); return NULL; }

    h->capacity = capacity;
    h->cmp = cmp;
    return h;
}

// Build heap from an existing array in O(n).
Heap *heap_build(void **arr, size_t n, heap_cmp_fn cmp) {
    if (!cmp) return NULL;

    Heap *h = calloc(1, sizeof(*h));
    if (!h) return NULL;
    h->data = malloc(n * sizeof(void *));
    if (!h->data) { free(h); return NULL; }

    memcpy(h->data, arr, n * sizeof(void *));
    h->size = h->capacity = n;
    h->cmp = cmp;

    // Bottom-up heapify — Floyd’s algorithm (O(n))
    for (ssize_t i = (ssize_t)(n / 2) - 1; i >= 0; i--)
        sift_down(h, (size_t)i);

    return h;
}

// Destroy heap and release memory.
void heap_destroy(Heap *h) {
    if (!h) return;
    free(h->data);
    free(h);
}

// Ensure at least `n` capacity, preserving contents.
int heap_reserve(Heap *h, size_t n) {
    if (h->capacity >= n) return 0;
    void **tmp = realloc(h->data, n * sizeof(void *));
    if (!tmp) return -1;
    h->data = tmp;
    h->capacity = n;
    return 0;
}

// Shrink heap capacity to fit size exactly.
void heap_trim(Heap *h) {
    if (h->capacity == h->size) return;
    void **tmp = realloc(h->data, h->size * sizeof(void *));
    if (tmp) {
        h->data = tmp;
        h->capacity = h->size;
    }
}

// Insert new element into heap.
// Automatically grows if needed.
int heap_insert(Heap *h, void *item) {
    if (h->size == h->capacity) {
        if (heap_reserve(h, h->capacity * 2) < 0)
            return -1;
    }
    h->data[h->size] = item;
    sift_up(h, h->size);
    h->size++;
    return 0;
}

// Return top element without removing it.
void *heap_peek(const Heap *h) {
    return (h && h->size > 0) ? h->data[0] : NULL;
}

// Remove and return top element.
void *heap_extract(Heap *h) {
    if (!h || h->size == 0) return NULL;
    void *root = h->data[0];
    h->data[0] = h->data[h->size - 1];
    h->size--;
    if (h->size > 0)
        sift_down(h, 0);
    return root;
}

// Replace top element and reheapify.
void *heap_replace(Heap *h, void *item) {
    if (!h || h->size == 0) return NULL;
    void *root = h->data[0];
    h->data[0] = item;
    sift_down(h, 0);
    return root;
}

// Return number of elements in heap.
size_t heap_size(const Heap *h) {
    return h ? h->size : 0;
}

// Clear heap contents (does not free memory).
void heap_clear(Heap *h) {
    if (h) h->size = 0;
}

// Clone heap (deep copy of metadata, shallow copy of items).
Heap *heap_clone(const Heap *h) {
    if (!h) return NULL;
    Heap *c = heap_create(h->cmp, h->capacity);
    if (!c) return NULL;
    memcpy(c->data, h->data, h->size * sizeof(void *));
    c->size = h->size;
    return c;
}

// Validate heap structure (for debugging/testing).
bool heap_validate(const Heap *h) {
    if (!h) return false;
    for (size_t i = 1; i < h->size; i++) {
        size_t p = parent(i);
        if (h->cmp(h->data[i], h->data[p]) > 0)
            return false;
    }
    return true;
}

// Print heap contents (debug utility).
void heap_debug_print(const Heap *h, void (*print_fn)(const void *)) {
    if (!h || !print_fn) return;
    printf("Heap(size=%zu, cap=%zu):\n", h->size, h->capacity);
    for (size_t i = 0; i < h->size; i++) {
        printf("[%zu] ", i);
        print_fn(h->data[i]);
        printf("\n");
    }
}

// --- Iteration API ---

HeapIter heap_iter(const Heap *h) {
    HeapIter it = { .heap = h, .index = 0 };
    return it;
}

bool heap_iter_next(HeapIter *it, void **out) {
    if (!it || !it->heap || it->index >= it->heap->size)
        return false;
    *out = it->heap->data[it->index++];
    return true;
}

// --- Heap sort utility ---
// Turns arbitrary array into a heap, then repeatedly extracts.
// Runs in O(n log n), in-place but not stable.
void heap_sort(void **arr, size_t n, heap_cmp_fn cmp) {
    Heap *h = heap_build(arr, n, cmp);
    if (!h) return;
    for (size_t i = n; i-- > 0;)
        arr[i] = heap_extract(h);
    heap_destroy(h);
}

// -----------------------------------------------------------
// Demo (compile with -DDEMO_HEAP_MAIN)
// -----------------------------------------------------------

#ifdef DEMO_HEAP_MAIN
static int int_cmp(const void *a, const void *b) {
    int x = *(const int *)a;
    int y = *(const int *)b;
    return (x > y) - (x < y);
}

static void print_int(const void *x) {
    printf("%d", *(const int *)x);
}

int main(void) {
    int vals[] = {5, 2, 9, 1, 6, 7, 3};
    void *arr[7];
    for (int i = 0; i < 7; i++) arr[i] = &vals[i];

    Heap *h = heap_build(arr, 7, int_cmp);

    printf("Initial heap valid: %d\n", heap_validate(h));
    heap_debug_print(h, print_int);

    int newv = 10;
    heap_insert(h, &newv);

    printf("After insert:\n");
    heap_debug_print(h, print_int);

    while (heap_size(h))
        printf("Pop: %d\n", *(int *)heap_extract(h));

    heap_destroy(h);
}
#endif
