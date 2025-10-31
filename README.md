## Features I implemented in this project:
```
-Generic heap that stores void* elements
-Custom comparator function (heap_cmp_fn) → supports min-heap or max-heap
-heap_create() → create an empty heap
-heap_build() → build a heap from existing array (O(n))
-heap_destroy() → free memory
-heap_insert() → insert element (with dynamic resizing)
-heap_extract() → remove and return top element
-heap_peek() → view top element without removing
-heap_replace() → replace top and reheapify
-heap_reserve() → grow capacity
-heap_trim() → shrink capacity
-heap_clear() → clear elements but keep memory
-heap_clone() → duplicate heap (shallow copy of data)
-heap_validate() → check if heap property holds
-heap_debug_print() → print heap contents for debugging
-heap_iter() and heap_iter_next() → simple iterator over elements
-heap_sort() → sort arbitrary array using heap (O(n log n))
-Full demo main() showing insert, build, validate, print, and extract
-Fully heapify via Floyd’s algorithm (O(n) bottom-up build)
-Uses malloc, calloc, realloc, free
-Uses ssize_t for signed indices (properly imported via <sys/types.h>)
-Correct parent/child index helpers and sift-up/down logic
```

## Commands:
```
# Build static library (libheap.a)
make

# Build and run demo
make demo
./heap_demo

# Debug mode (no optimization, symbols)
make debug

# Clean
make clean
```
### I just implemented to learn and for fun.
