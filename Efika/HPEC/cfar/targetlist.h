/******************************************************************************
** File: targetlist.h
**
** HPEC Challenge Benchmark Suite
** CFAR Kernel Benchmark
**
** Contents:
**  The list of targets used by CFAR.  Uses PcaCMemMgr.h to allocate larger
**  blocks of memory to reduce the number of calls to malloc()/free().
**
** Author: Edmund Wong 
**         MIT Lincoln Laboratory
**
******************************************************************************/

#ifndef _TARGETLIST_H_
#define _TARGETLIST_H_

#include <stdlib.h>
#include <PcaCMemMgr.h>

/* memory manager used for the target list */
MemManager cfar_mm;

/* simple implementations of the target and target list - singly linked list */
typedef struct CfarTargetStruct CfarTarget;
struct CfarTargetStruct
{
  uint bm, rg, dop;
  struct CfarTargetStruct* p_next;
};
typedef struct
{
  CfarTarget* p_head;
  CfarTarget* p_tail;
  uint length;
} CfarTargetList;

/* useful macros for using the memory manager */
#define mm_init(n)   ( MemManager_init(&cfar_mm, sizeof(CfarTarget), n) )
#define mm_clean()   ( MemManager_clean(&cfar_mm) ) 
#define mm_malloc()  ( MemManager_malloc(&cfar_mm) )
#define mm_free(ptr) ( MemManager_free(&cfar_mm, ptr) )


/**************************************************************************
* Initialize the CFAR target list.
**************************************************************************/
void CfarTargetList_init(CfarTargetList* p_list) 
{ 
  p_list->p_head = p_list->p_tail = NULL;
  p_list->length = 0;
}


/**************************************************************************
* Add a target to the found targets.
**************************************************************************/
void CfarTargetList_add(CfarTargetList* p_list, uint bm, uint dop, uint rg) 
{
  /* create new target */
  CfarTarget* p_nt = (CfarTarget*) mm_malloc();
  p_nt->bm  = bm;
  p_nt->rg  = rg;
  p_nt->dop = dop;
  p_nt->p_next = NULL;

  /* now update list */
  if (p_list->p_head == NULL) p_list->p_head = p_nt;
  else p_list->p_tail->p_next = p_nt;
  p_list->p_tail = p_nt;
  p_list->length++;
}


/**************************************************************************
* Output the target list to the specified file.
**************************************************************************/
void CfarTargetList_output(const char* filename, CfarTargetList* p_list)
{
  uint i;
  CfarTarget* p_tmp;
  struct PcaCArrayInt output;

  /* create the output array */
#ifdef VERBOSE
  printf("Total number of targets found: %d\n", p_list->length);
#endif 
  pca_create_carray_2d(int, output, p_list->length, 3, PCA_REAL);
  for (i = 0, p_tmp = p_list->p_head; i < p_list->length * 3; 
       p_tmp = p_tmp->p_next) 
  {
    output.data[i++] = p_tmp->dop;
    output.data[i++] = p_tmp->rg;
    output.data[i++] = p_tmp->bm;
  }

  /* output to file */
  writeToFile(int, filename, output);

  /* clean up */
  clean_mem(int, output);
}


/**************************************************************************
* Clean up the target list.
**************************************************************************/
void CfarTargetList_clean(CfarTargetList* p_list)
{
  CfarTarget* p_tmp;
  while (p_list->p_head != NULL) {
    p_tmp = p_list->p_head;
    p_list->p_head = p_list->p_head->p_next;
    mm_free(p_tmp);
  }
}

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
