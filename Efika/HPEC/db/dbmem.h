/******************************************************************************
** File: dbmem.h
**
** HPEC Challenge Benchmark Suite
** Database Kernel Benchmark
**
** Contents:
**  Memory management used by database.  See PcaCMemMgr.h for details.
**
** Author: Edmund Wong 
**         MIT Lincoln Laboratory
**
******************************************************************************/
#ifndef _DBMEM_H_
#define _DBMEM_H_

#include <stdlib.h>

/**************************************************************************
* If PCA_DB_MEM is declared, use custom memory management.
**************************************************************************/
#ifdef PCA_DB_MEM

#include <PcaCMemMgr.h>
MemManager ll_mm, rb_mm, tr_mm;

/**************************************************************************
* Functions that are used for convenience.  n represents the number of
* blocks to allocate at once.
*
*  ll_malloc()/ll_free() - memory allocation for linked list nodes.
*  rb_malloc()/rb_free() - memory allocation for red-black tree nodes.
*  tr_malloc()/tr_free() - memory allocation for track records.
**************************************************************************/
#define ll_init(n)   ( MemManager_init(&ll_mm, sizeof(LinkListNode), n) )
#define ll_clean()   ( MemManager_clean(&ll_mm) )
#define ll_malloc()  ( MemManager_malloc(&ll_mm) )
#define ll_free(ptr) ( MemManager_free(&ll_mm, ptr) )
#define rb_init(n)   ( MemManager_init(&rb_mm, sizeof(RedBlackNode), n) )
#define rb_clean()   ( MemManager_clean(&rb_mm) )
#define rb_malloc()  ( MemManager_malloc(&rb_mm) )
#define rb_free(ptr) ( MemManager_free(&rb_mm, ptr) )
#define tr_init(n)   ( MemManager_init(&tr_mm, sizeof(TrackRecord), n) )
#define tr_clean()   ( MemManager_clean(&tr_mm) )
#define tr_malloc()  ( MemManager_malloc(&tr_mm) )
#define tr_free(ptr) ( MemManager_free(&tr_mm, ptr) )


/**************************************************************************
* If PCA_DB_MEM is not defined, use standard malloc/free.
**************************************************************************/
#else

typedef unsigned int uint;
typedef unsigned char bool;

/* define them to standard malloc()/free() calls */
#define ll_init(n)     
#define ll_clean(ptr)  
#define ll_malloc()    ( malloc(sizeof(LinkListNode)) )
#define ll_free(ptr)   ( free(ptr) )
#define rb_init(n)     
#define rb_clean(ptr)  
#define rb_malloc()    ( malloc(sizeof(RedBlackNode)) )
#define rb_free(ptr)   ( free(ptr) )
#define tr_init(n)     
#define tr_clean(ptr)  
#define tr_malloc(ptr) ( malloc(sizeof(TrackRecord)) )
#define tr_free(ptr)   ( free(ptr) )

#endif

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
