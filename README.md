# MemPool
Memory pooling macros

## Usage
mem_pool.h contains one struct and 4 macros.
mem_pool_t contains all the information about a pool.
```c
  typedef struct mem_pool {
    size_t block_s;
    size_t size_left;
    void* free_start;
    void* current_start;
  } mem_pool_t;
```

EXAMPLE
```c
//this is whatever struct you want as long as it contains
//a field that points to another of it's type.
struct your_type {
  struct your_type *next_field;
  //...
}

//initializing the pool,
//pool is allocated by the macro, do not malloc yourself
mem_pool_t *pool;
PINIT(pool, 256)

//allocating memory
struct your_type *foo;
PALLOC(pool, your_type, next_field, foo)
if (foo == NULL) {
  //allocation failed, check errno for more info
  //see man calloc(3)
}

//freeing memory
PFREE(pool, next_field, foo)

//freeing the pool
//this also frees any blocks returned to the pool
//and allocated memory waiting to be used.
PDESTROY(pool, your_type, next_field)

```
