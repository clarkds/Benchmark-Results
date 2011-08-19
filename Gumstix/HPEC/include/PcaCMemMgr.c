/**************************************************************************
* File: PcaCMemMgr.c
*
* Contents:
*  Implementation of a malloc()/free() replacements.  Allocates larger 
*  blocks of memory at once to ammortize the cost of an allocation over 
*  several malloc()'s.  Note that this memory manager uses freed
*  memory to maintain pointers; thus, the memory manager is very 
*  inefficient when allocating types of sizes less than the size
*  of a pointer (typically 4 bytes).
*
* Author: Edmund Wong
*         MIT Lincoln Laboratory
*
* This code is submitted for Government consideration and intended 
* for limited distribution to Government authorized parties only.
**************************************************************************/
#include <stdlib.h>
#include "PcaCMemMgr.h"

/**************************************************************************
* Memory block structure; this is what the memory manager allocates
* from.  Contains a next pointer to create a linked list of the 
* allocated memory blocks.
**************************************************************************/
struct MemBlockStruct
{
  void* m_block;
  struct MemBlockStruct* p_next;
};


/**************************************************************************
* Allocates n_blocks blocks of size blksize for use by the memory
* manager.  p_next is the next block of memory (the allocated memory 
* blocks form a linked list).  Should only be called by MemManager.
**************************************************************************/
MemBlock* MemBlock_create(uint blksize, uint n_blocks, MemBlock* p_next)
{
  MemBlock* p_block = (MemBlock*) malloc(sizeof(MemBlock));
  p_block->m_block = malloc(blksize * n_blocks);
  p_block->p_next = p_next;

  return p_block;
}


/**************************************************************************
* Destroy the memory block and free up allocated memory.
* Should only be called by MemManager.
**************************************************************************/
void MemBlock_destroy(MemBlock* p_ptr)
{
  free(p_ptr->m_block);
  free(p_ptr);
}


/**************************************************************************
* Initializes the memory manager.  It takes in the block size and
* number of blocks to allocate at once.
**************************************************************************/
void MemManager_init(MemManager* p_mm, uint blksize, uint n_blocks)
{
  /* block size must be at least the size of a pointer */
  blksize = (blksize >= sizeof(void*)) ? blksize : sizeof(void*);

  /* initialize the memory manager */
  p_mm->blksize = blksize;
  p_mm->n_blocks = n_blocks;
  p_mm->p_first = MemBlock_create(blksize, n_blocks, NULL);
  p_mm->p_free  = NULL;
  p_mm->firstblk_free = n_blocks;
}


/**************************************************************************
* Frees up all the memory used by the memory manager, including the
* MemBlock's it may have allocated.
**************************************************************************/
void MemManager_clean(MemManager* p_mm)
{
  /* destroy all the memory blocks */
  MemBlock* p_ptr = p_mm->p_first;
  while (p_ptr) {
    p_ptr = p_ptr->p_next;
    MemBlock_destroy(p_mm->p_first);
    p_mm->p_first = p_ptr;
  }
}


/**************************************************************************
* Allocates a block of memory of the size specified at initialization.
**************************************************************************/
void* MemManager_malloc(MemManager* p_mm)
{
  void* p_rv;

  /* check the free list */
  if (p_mm->p_free) {
    /* allocate off the free list */
    p_rv = p_mm->p_free;
    p_mm->p_free = (void*) *((uint*) p_mm->p_free);
  }
  else {
    if (p_mm->firstblk_free == 0) {
      /* must allocate new block */
      p_mm->p_first = MemBlock_create(p_mm->blksize, 
				      p_mm->n_blocks, 
				      p_mm->p_first);
      p_mm->firstblk_free = p_mm->n_blocks;
    }

    /* allocate from pre-existing block */
    p_rv = (unsigned char*) p_mm->p_first->m_block + --(p_mm->firstblk_free)*p_mm->blksize;
  }
  return p_rv;
}


/**************************************************************************
* Frees up a block of memory allocated by the memory manager.
**************************************************************************/
void MemManager_free(MemManager* p_mm, void* p_block)
{
  /* add the block to the free list */
  *((uint*) p_block) = (uint) p_mm->p_free;
  p_mm->p_free = p_block;
}
