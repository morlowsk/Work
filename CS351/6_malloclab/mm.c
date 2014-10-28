/*
 * mm-naive.c - The fastest, least memory-efficient malloc package.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>

#include "mm.h"
#include "memlib.h"

/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7)


#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

typedef struct header blockHdr;
#define BLK_HDR_SIZE ALIGN(sizeof(blockHdr)) //free bloCK


#define MIN_BLK_SIZE (BLK_HDR_SIZE + SIZE_T_SIZE)

void *find_fit(size_t size);
void *coalesce(size_t *bp); 

struct header {
    size_t size;
    blockHdr *next_p;
    blockHdr *prior_p;
};


/* 
 * mm_init - initialize the malloc package.
 */
int mm_init(void)//first block is the sentinel value of the free_list
{
    blockHdr *p = mem_sbrk(BLK_HDR_SIZE + SIZE_T_SIZE);
    p->size = (BLK_HDR_SIZE + SIZE_T_SIZE) | 1;
    p->next_p = p;
    p->prior_p = p;
    size_t *footer = (size_t *) ((char *)p + BLK_HDR_SIZE);
    *footer = p->size;
    return 0; 
}

void print_heap(){
    blockHdr *bp = mem_heap_lo();
    int i = 0;
    while (bp < (blockHdr *) mem_heap_hi()) {
        printf("%d. %s block at %p, size %ld + \n", i,
                (bp->size&1)?"allocated":"free",
                bp, (long) (bp->size & ~1));
        bp = (blockHdr *) ((char *) bp + (bp->size & ~1));
        i++;
    }
}

void print_freelist(){
    blockHdr *bp = mem_heap_lo();
    printf("block at %p, size %ld + \n",
                bp, (long) (bp->size & ~1));
    for (bp = bp->next_p; bp != (blockHdr *) mem_heap_lo(); bp = bp->next_p) {
        printf("block at %p, size %ld + \n",
                bp, (long) (bp->size & ~1));
    }
}



/* 
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size)
{
    int newsize = ALIGN(size + SIZE_T_SIZE + SIZE_T_SIZE);
    newsize = (newsize < MIN_BLK_SIZE) ? MIN_BLK_SIZE : newsize;
    blockHdr *bp = find_fit(newsize);
    size_t *footer;
    if (bp == NULL) { //Can't find fit
        bp = mem_sbrk(newsize); //testing binary trace...
        if ((long)bp == -1)
            return NULL;
        else {
            bp->size = newsize;
            footer = (size_t *) ((char *)bp + bp->size - SIZE_T_SIZE);
            bp->size = newsize | 1; 
            *footer = bp->size;
        }
    } else { //Found fit
        if (newsize < bp->size) { //Try to see if there's a split block
            if (bp->size - newsize < MIN_BLK_SIZE) {//Can't split block
                //Deallocate from free list
                
                bp->prior_p->next_p = bp->next_p; 
                bp->next_p->prior_p = bp->prior_p;
                
                footer = (size_t *)((char *)bp + bp->size - SIZE_T_SIZE);
                bp->size = bp->size | 1; 
                *footer = bp->size;
                
            }
            else { //Can in fact split block
                                
                //Free block
                size_t *split_block_header = (size_t *)((char *)bp + newsize);
                blockHdr *split_block = (blockHdr *) split_block_header; 
                split_block->size = (bp->size & ~1L) - newsize;
                footer = (size_t *)((char *)split_block + split_block->size - SIZE_T_SIZE);
                *footer = split_block->size;
                //add split block (leftover) to list
                bp->prior_p->next_p = split_block;
                split_block->prior_p = bp->prior_p;
                bp->next_p->prior_p = split_block;
                split_block->next_p = bp->next_p;

                //Allocated block
                bp->size = newsize;  
                footer = (size_t *)((char *)bp + bp->size - SIZE_T_SIZE);
                bp->size = newsize | 1; 
                *footer = bp->size;

                                
            }
        }
        else { //newsize is equal to bp->size
            bp->prior_p->next_p = bp->next_p;
            bp->next_p->prior_p = bp->prior_p;

            bp->size = newsize;
            footer = (size_t *)((char *)bp + bp->size - SIZE_T_SIZE);
            bp->size = newsize | 1;
            *footer = bp->size;
        }
    }
    return (char *) bp + SIZE_T_SIZE;
}

void *find_fit(size_t size) {
    blockHdr *p;
    for (p = ((blockHdr *) mem_heap_lo())->next_p;
         p != mem_heap_lo() && p ->size < size;
         p = p->next_p);
    if (p != mem_heap_lo()) {
        return p;
    }
    else {
        return NULL;
    }
}


/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *ptr) //LIFO policy of adding to free list
{
    blockHdr *bp = ptr-SIZE_T_SIZE;
    blockHdr *nu_bp = coalesce((size_t *) bp);
    nu_bp->size &= ~1;
    
}

void *coalesce(size_t *bp) {
    size_t *next = (size_t *)((char *)bp + (*bp & ~1L));
    size_t *prev = (size_t *)((char *)bp - ((*(size_t *)((char *)bp - SIZE_T_SIZE)) & ~1L));

    int prev_alloc, next_alloc;
    blockHdr *head = (blockHdr *)mem_heap_lo();
    blockHdr *v = (blockHdr *)bp;              
 
    if (prev > (size_t *)mem_heap_lo()) {
        prev_alloc = *prev & 1;
    }
    else {
        prev_alloc = 1;
    }
    if (next < ((size_t *)mem_heap_hi())) {
        next_alloc = *next & 1;
    }
    else {
        next_alloc = 1;
    }
    if (prev_alloc && next_alloc) {
        v->next_p = head->next_p;
        v->prior_p = head;
        head->next_p = v;
        v->next_p->prior_p = v;
        return v;
    }
    else if (!prev_alloc && next_alloc) {//if previous free
        *prev += (*bp & ~1); //bp is not yet freed 
        *(size_t *) ((char *)bp + (*bp & ~1) - SIZE_T_SIZE) = *prev;
        return prev;
    }
    else if (prev_alloc && !next_alloc) { //if next free
        *bp += (*next & ~1);
        *(size_t *) ((char *)next + (*next & ~1) - SIZE_T_SIZE) = *bp;
        
        v->next_p = ((blockHdr *) next)->next_p;
        v->prior_p = ((blockHdr *) next)->prior_p;
        v->prior_p->next_p = v;
        v->next_p->prior_p = v;
        return v;
       
    }
    else if (!prev_alloc && !next_alloc) {
        *prev += (*next & ~1) + (*bp & ~1);
        *(size_t *) ((char *)next + (*next & ~1)  - SIZE_T_SIZE) = *prev;
        /*((blockHdr *) prev)->next_p = ((blockHdr *) next)->next_p;
        ((blockHdr *) prev)->next_p->prior_p = (blockHdr *) prev;*/

        ((blockHdr*)next)->prior_p->next_p = ((blockHdr *)next)->next_p;
        ((blockHdr*)next)->next_p->prior_p = ((blockHdr *)next)->prior_p;

        return prev;
    }
        return NULL;
} 

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *ptr, size_t size)
{
    /*
    size_t *header = (size_t *)((char *)ptr - SIZE_T_SIZE);
    size_t oldsize = *header & ~1L,
         newsize = ALIGN(size + SIZE_T_SIZE + SIZE_T_SIZE);
    void *newptr;
    if (oldsize >= newsize) {
        return ptr;
    } else {
        newptr = malloc(size);
        memcpy(newptr, ptr, oldsize - SIZE_T_SIZE);
        free(ptr);
        return newptr;
    } */
	
    blockHdr *bp = ptr-BLK_HDR_SIZE;
    void *newptr = mm_malloc(size);
    if (newptr == NULL)
        return NULL;
    int copySize = bp->size-BLK_HDR_SIZE;
    if (size < copySize)
        copySize = size;
    memcpy(newptr, ptr, copySize);
    mm_free(ptr);
    return newptr;
}
