/******************************************************************************
** File: list.c
**
** HPEC Challenge Benchmark Suite
** Database Kernel Benchmark
**
** Contents:
**  Linked list implementation.
**
** Author: Edmund L. Wong
**         MIT Lincoln Laboratory
**
******************************************************************************/
#include "list.h"

#define true 1
#define false 0

/**************************************************************************
* Creates a linked list node.
**************************************************************************/
LinkListNode* LinkListNode_create(TrackRecord* p_val,
				  LinkListNode* p_next)
{
  LinkListNode* m_node = (LinkListNode*) ll_malloc();
  m_node->p_prev = NULL;
  m_node->p_next = p_next;
  m_node->m_record = p_val;
  return m_node;
}


/**************************************************************************
* Destroy a linked list node.
**************************************************************************/
void LinkListNode_destroy(LinkListNode* p_node)
{
  ll_free(p_node);
}


/**************************************************************************
* Initialize a linked list iterator.
**************************************************************************/
void LinkListIterator_init(LinkListIterator* p_it, LinkList* p_list)
{
  LinkListIterator_set(p_it, p_list);
}


/**************************************************************************
* "Clean up" a linked list iterator.  Placeholder for consistency.
**************************************************************************/
void LinkListIterator_clean(LinkListIterator* p_it)
{
  /* nothing to do */
}


/**************************************************************************
* Move to the next node.
**************************************************************************/
void LinkListIterator_next(LinkListIterator* p_it)
{
  if (!p_it->atEnd) p_it->atEnd = !(p_it->p_current = p_it->p_current->p_next);
}


/**************************************************************************
* Checks whether we are at the end of the list.
**************************************************************************/
bool LinkListIterator_atEnd(const LinkListIterator* p_it)
{
  return p_it->atEnd;
}


/**************************************************************************
* Set the iterator to point to a particular node.
**************************************************************************/
void LinkListIterator_set(LinkListIterator* p_it, LinkList* p_list)
{
  p_it->atEnd = !(p_it->p_current = p_list->p_head);
  p_it->p_ll = p_list;
}


/**************************************************************************
* Delete the node we are currently on and move to the next one.
**************************************************************************/
bool LinkListIterator_remove(LinkListIterator* p_it, TrackRecord** p_rec)
{
  LinkListNode* p_del;

  /* check if at end */
  if (p_it->atEnd) return false;
  
  /* now remove */
  p_del = p_it->p_current;
  LinkListIterator_next(p_it);
  return LinkList_removeNode(p_it->p_ll, p_del, p_rec);
}


/**************************************************************************
* Returns the value in the current node.
**************************************************************************/
TrackRecord* LinkListIterator_value(const LinkListIterator* p_it)
{
  return p_it->p_current->m_record;
}


/**************************************************************************
* Initialize the linked list.
**************************************************************************/
void LinkList_init(LinkList* p_list)
{
  p_list->p_head = p_list->p_tail = NULL;
  p_list->length = 0;
}


/**************************************************************************
* Gets the ith node in the list.
**************************************************************************/
LinkListNode* LinkList_getNode(const LinkList* const p_list, const int i)
{
  int x;
  if (i < 0 || i >= p_list->length) return NULL;
  else if (i == 0) return p_list->p_head;
  else if (i == p_list->length-1) return p_list->p_tail;
  else {
    LinkListNode *p_node = p_list->p_head->p_next;
    for (x=1; x < i; x++) p_node = p_node->p_next;
    return p_node;
  }
}


/**************************************************************************
* Get an iterator to the linked list.
**************************************************************************/
void LinkList_getIterator(LinkListIterator* p_it, LinkList* p_list)
{
  LinkListIterator_init(p_it, p_list);
}


/**************************************************************************
* Destroy the linked list.
**************************************************************************/
void LinkList_clean(LinkList* p_list)
{
  if (p_list->p_head) {
    LinkListNode* p_node = p_list->p_head; 
    while (p_node->p_next) {
      p_node = p_node->p_next;
      LinkListNode_destroy(p_node->p_prev);
    }
    LinkListNode_destroy(p_node);
  }
}


/**************************************************************************
* Insert value at the end of the list.
**************************************************************************/
void LinkList_append(LinkList* p_list, TrackRecord* const p_val)
{
  LinkListNode* p_node = LinkListNode_create(p_val, NULL);
  p_node->p_prev = p_list->p_tail;
  if (p_list->p_tail) p_list->p_tail->p_next = p_node;
  else p_list->p_head = p_node;
  p_list->p_tail = p_node;
  p_list->length++;
}


/**************************************************************************
* Delete a node from the linked list.
**************************************************************************/
bool LinkList_removeNode(LinkList* p_list, LinkListNode* p_node, 
			 TrackRecord** p_val)
{
  if (p_node) {
    /* we found the node; manipulate a bunch of pointers */
    if (p_node != p_list->p_head) p_node->p_prev->p_next = p_node->p_next;
    else p_list->p_head = p_node->p_next;
    if (p_node != p_list->p_tail) p_node->p_next->p_prev = p_node->p_prev;
    else p_list->p_tail = p_node->p_prev;
    if (p_val) *p_val = p_node->m_record;
    p_list->length--;

    /* free up the memory */
    LinkListNode_destroy(p_node);
    return true;
  }
  else return false;
}


/**************************************************************************
* Delete the ith value from the list.
**************************************************************************/
bool LinkList_remove(LinkList* p_list, const int i, TrackRecord** p_val)
{
  /* find the node that has this index */
  LinkListNode* p_lln = LinkList_getNode(p_list, i);
  if (p_lln) return LinkList_removeNode(p_list, p_lln, p_val);
  else return false;
}

  
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
