#ifndef HEAP_H
#define HEAP_H

#include <stddef.h> // size_t
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef int (*heap_cmp_fn)(const void *a, const void *b);

typedef struct Heap Heap;

/**
 * @brief Create a new heap.
 *
 * @param cmp Comparator function. Must return positive if a > b.
 * @param capacity Optional initial capacity (0 for default).
 * @return Pointer to Heap or NULL on failure.
 */
Heap *heap_create(heap_cmp_fn cmp, size_t capacity);

/**
 * @brief Build a heap from existing array (heapify).
 *
 * @param arr Pointer to array of void*.
 * @param n Number of elements.
 * @param cmp Comparator function.
 * @return Pointer to Heap or NULL on failure.
 */
Heap *heap_build(void **arr, size_t n, heap_cmp_fn cmp);

/**
 * @brief Free heap memory.
 */
void heap_destroy(Heap *h);

/**
 * @brief Insert new element into heap.
 * @return 0 on success, -1 on allocation failure.
 */
int heap_insert(Heap *h, void *item);

/**
 * @brief Get root element without removing it.
 */
void *heap_peek(const Heap *h);

/**
 * @brief Remove and return root element.
 */
void *heap_extract(Heap *h);

/**
 * @brief Replace root element with new item.
 */
void *heap_replace(Heap *h, void *item);

/**
 * @brief Number of elements in heap.
 */
size_t heap_size(const Heap *h);

/**
 * @brief Ensure heap capacity for at least n elements.
 */
int heap_reserve(Heap *h, size_t n);

/**
 * @brief Trim heap capacity down to current size.
 */
void heap_trim(Heap *h);

/**
 * @brief Remove all elements (keeps capacity).
 */
void heap_clear(Heap *h);

/**
 * @brief Clone a heap (deep copy of structure, shallow copy of elements).
 */
Heap *heap_clone(const Heap *h);

/**
 * @brief Check heap validity (for debugging).
 * @return true if valid heap property.
 */
bool heap_validate(const Heap *h);

/**
 * @brief Print heap as tree (debug).
 */
void heap_debug_print(const Heap *h, void (*print_fn)(const void *));

/**
 * @brief Heap iterator
 */
typedef struct HeapIter {
    const Heap *heap;
    size_t index;
} HeapIter;

HeapIter heap_iter(const Heap *h);
bool heap_iter_next(HeapIter *it, void **out);

/**
 * @brief Heap sort utility
 */
void heap_sort(void **arr, size_t n, heap_cmp_fn cmp);

#ifdef __cplusplus
}
#endif
#endif
