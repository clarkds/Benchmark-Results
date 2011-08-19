/******************************************************************************
** File: PcaCMemMgr.h
**
** HPEC Challenge Benchmark Suite
** Common Header File
**
** Contents:
**  Definition of a malloc()/free() replacements.  Allocates larger 
**  blocks of memory at once to ammortize the cost of an allocation over 
**  several malloc()'s.  
**
**  At all times, the memory manager maintains both a free list and a large 
**  block of unallocated memory.  A chunk of memory ends up on the
**  free list ONLY IF when MemManager_free() is called on it.  During
**  MemManager_malloc(), the memory manager first checks the free list
**  and allocates a block of memory from there if available.  Otherwise,
**  the block of memory is allocated from the unallocated memory block.
**  When a large unallocated memory block is totally used, another 
**  such block is allocated; this is the only time malloc() is called. 
**  Note that the memory manager does not track memory usage and thus
**  does not free up blocks when usage is low.  free() is only called
**  within MemManager_clean().
**
**  Note that this memory manager uses freed
**  memory to maintain pointers; thus, the memory manager is very 
**  inefficient when allocating types of sizes less than the size
**  of a pointer (typically 4 bytes).
**
** Author: Edmund L. Wong
**         MIT Lincoln Laboratory
**
******************************************************************************/
#ifndef _PCACMEMMGR_H_
#define _PCACMEMMGR_H_

#include <stdlib.h>

/* useful macros/typedef's */
typedef unsigned int uint;
typedef unsigned char bool;
#define true  1
#define false 0


/**************************************************************************
* Memory manager.  This structure doles out fixed-size memory blocks
* and cleans up through its associated methods.
**************************************************************************/
typedef struct MemBlockStruct MemBlock;
typedef struct 
{
  MemBlock* p_first;
  void*     p_free;
  uint      blksize;
  uint      n_blocks;
  uint      firstblk_free;
} MemManager;


/**************************************************************************
* Initializes the memory manager.  It takes in the block size and
* number of blocks to allocate at once.
**************************************************************************/
void MemManager_init(MemManager* p_mm, uint blksize, uint n_blocks);


/**************************************************************************
* Frees up all the memory used by the memory manager, including the
* MemBlock's it may have allocated.
**************************************************************************/
void MemManager_clean(MemManager* p_mm);


/**************************************************************************
* Allocates a block of memory of the size specified at initialization.
**************************************************************************/
void* MemManager_malloc(MemManager* p_mm);


/**************************************************************************
* Frees up a block of memory allocated by the memory manager.
**************************************************************************/
void MemManager_free(MemManager* p_mm, void* p_block);


#endif
/* ----------------------------------------------------------------------------
Copyright (c) 2006, Massachusetts Institute of Technology
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are  
met:
     * Redistributions of source code must retain the above copyright
       notice, this list of conditions and the following disclaimer.
     * Redistributions in binary form must reproduce the above copyright
       notice, this list of conditions and the following disclaimer in the
       documentation and/or other materials provided with the distribution.
     * Neither the name of the Massachusetts Institute of Technology nor  
       the names of its contributors may be used to endorse or promote 
       products derived from this software without specific prior written 
       permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF  
THE POSSIBILITY OF SUCH DAMAGE.
---------------------------------------------------------------------------- */
