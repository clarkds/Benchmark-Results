/******************************************************************************
** File: list.h
**
** HPEC Challenge Benchmark Suite
** Database Kernel Benchmark
**
** Contents:
**  Linked list definition.
**
** Author: Edmund L. Wong
**         MIT Lincoln Laboratory
**
******************************************************************************/
#ifndef _LIST_H
#define _LIST_H

#include <stdlib.h>
#include <PcaCArray.h>
#include "dbmem.h"

/**
 * Forward declaration
 */
typedef struct TrackRecordStruct TrackRecord;
typedef struct LinkListStruct LinkList;
typedef struct LinkListNodeStruct LinkListNode;

/**************************************************************************
* LINKED LIST NODE
**************************************************************************/
struct LinkListNodeStruct
{
  struct LinkListNodeStruct* p_prev;
  struct LinkListNodeStruct* p_next;
  TrackRecord*               m_record;
};


/**************************************************************************
* LINKED LIST ITERATOR
**************************************************************************/
typedef struct
{
  LinkList* p_ll;
  LinkListNode* p_current;
  bool atEnd;
} LinkListIterator;



/**************************************************************************
* Initializes a linked list iterator to the passed-in linked list.
**************************************************************************/
void LinkListIterator_init(LinkListIterator*, LinkList*);


/**************************************************************************
* Cleans a linked list iterator.
**************************************************************************/
void LinkListIterator_clean(LinkListIterator*);


/**************************************************************************
* Moves the linked list iterator to the next node.
**************************************************************************/
void LinkListIterator_next(LinkListIterator*);


/**************************************************************************
* Set the iterator to point to the beginning of a particular linked list.
**************************************************************************/
void LinkListIterator_set(LinkListIterator*, LinkList*);


/**************************************************************************
* Delete the node we are currently pointing at, returning the track
* record as the second argument.
**************************************************************************/
bool LinkListIterator_remove(LinkListIterator*, TrackRecord**);


/**************************************************************************
* Returns the pointer to the track record being pointed at.
**************************************************************************/
TrackRecord* LinkListIterator_value(const LinkListIterator*);



/**************************************************************************
* LINKED LIST 
**************************************************************************/
struct LinkListStruct
{
  LinkListNode* p_head;
  LinkListNode* p_tail;
  int length;
};

/**************************************************************************
* Initialize the linked list.
**************************************************************************/
void LinkList_init(LinkList*);


/**************************************************************************
* Destroy the linked list.
**************************************************************************/
void LinkList_clean(LinkList*);


/**************************************************************************
* Returns the ith node in the list.
**************************************************************************/
LinkListNode* LinkList_getNode(const LinkList* const, const int i);


/**************************************************************************
* Initializes a linked list iterator to a particular linked list.
**************************************************************************/
void LinkList_getIterator(LinkListIterator*, LinkList*);


/**************************************************************************
* Insert track record at the end of the list.
**************************************************************************/
void LinkList_append(LinkList*, TrackRecord* const);


/**************************************************************************
* Delete the ith value from the list, returning the track record.
**************************************************************************/
bool LinkList_remove(LinkList*, const int i, TrackRecord**);


/**************************************************************************
* Delete a node from the linked list, returning the track record.
**************************************************************************/
bool LinkList_removeNode(LinkList*, LinkListNode*, TrackRecord**);

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
