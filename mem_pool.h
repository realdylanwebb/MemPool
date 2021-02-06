/*
*   MIT License
*
*   Copyright (c) 2021 realdylanwebb
*
*   Permission is hereby granted, free of charge, to any person obtaining a copy
*   of this software and associated documentation files (the "Software"), to deal
*   in the Software without restriction, including without limitation the rights
*   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*   copies of the Software, and to permit persons to whom the Software is
*   furnished to do so, subject to the following conditions:
*
*   The above copyright notice and this permission notice shall be included in all
*   copies or substantial portions of the Software.
*
*   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
*   SOFTWARE.
*/

/*
*   This defines generic memory pooling.
*   Memory pools consist of a linked list of returned blocks that have already been used,
*   and a preallocated section of memory to use in case the freed list is empty.
*   Memory pools will block allocate more memory when both the free list and preallocated
*   memory is empty.
*/

#ifndef __D_MEM_POOL__
#define __D_MEM_POOL__

#include <stdlib.h>
#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif


typedef struct mem_pool {
    size_t block_s;
    size_t size_left;
    void* free_start;
    void* current_start;
} mem_pool_t;


//Initializes a memory pool with block size _block_s
//memory block will not be allocated until PALLOC is called
//with a type.
#define PINIT(_pool, _block_s)                          \
    _pool = (mem_pool_t*)malloc(sizeof(mem_pool_t));    \
    _pool->size_left = 0;                               \
    _pool->free_start = NULL;                           \
    _pool->current_start = NULL;                        \
    _pool->block_s = _block_s;


//Recursive frees returned blocks and memory pool
//then frees the pool itself.
//_next is the name of the field used to link the blocks.
#define PDESTROY(_pool, _type, _next)       \
    _type *cast, *tmp;                      \
    free(_pool->current_start);             \
    while(pool->free_start != NULL) {       \
        cast = (_type*)_pool->free_start;   \
        _pool->free_start = cast->_next;    \
        free(cast);                         \
    }                                       \
    free(_pool);


//Palloc allocates memory from a memory pool to _ptr
//Palloced memory is not null filled!
//_next is the name of the field used to link the blocks.
#define PALLOC(_pool, _type, _next, _ptr)                                   \
    _type *cast;                                                            \
    cast = (_type *) _pool->free_start;                                     \
    if (_pool->free_start != NULL) {                                        \
        _ptr = (_type *)_pool->free_start;                                  \
        _pool->free_start = cast->_next;                                    \
    } else {                                                                \
        if (_pool->current_start == NULL || _pool->size_left == 0) {        \
            _pool->current_start = calloc(_pool->block_s, sizeof(_type));   \
            _pool->size_left = _pool->block_s;                              \
        }                                                                   \
        _ptr = _pool->current_start;                                        \
        if (_ptr != NULL) {                                                 \
            cast = (_type *) _pool->current_start;                          \
            cast++;                                                         \
            _pool->size_left -= 1;                                          \      
        }                                                                   \
    }                                                                       \
    _ptr->_next = NULL; 


//Pfree returns the block pointed to by _ptr top a memory pool, 
//_next is the name of the field that will be used to link the block.
#define PFREE(_pool, _next, _ptr)       \
    _ptr->_next = _pool->free_start;    \
    _pool->free_start = _ptr;           


#ifdef __cplusplus
}
#endif

#endif
