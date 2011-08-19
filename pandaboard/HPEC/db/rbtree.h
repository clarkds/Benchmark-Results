/******************************************************************************
** File: rbtree.h
**
** HPEC Challenge Benchmark Suite
** Database Kernel Benchmark
**
** Contents:
**  Red-black tree definition.  Red-black trees are balanced binary
**  search trees that require O(lg n) for inserts/deletes/searches.
**  They are described in greater detail in "Introduction to Algorithms"
**  (Cormen, Leiserson, Rivest, Stein).
**
** Author: Edmund L. Wong
**         MIT Lincoln Laboratory
**
******************************************************************************/
#ifndef _RBTREE_H_
#define _RBTREE_H_

#include "list.h"

typedef enum { RED, BLACK } color_t;

/* forward declaration */
typedef struct RedBlackTreeStruct RedBlackTree;

/**************************************************************************
* RED-BLACK TREE NODE
**************************************************************************/
struct RedBlackNodeStruct
{
  /* directional pointers */
  struct RedBlackNodeStruct* p_up;
  struct RedBlackNodeStruct* p_left;
  struct RedBlackNodeStruct* p_right;

  /* color, key and values with a certain key */
  color_t color;
  float key;
  LinkList valList;
};
typedef struct RedBlackNodeStruct RedBlackNode;


/**************************************************************************
* RED-BLACK TREE ITERATOR
**************************************************************************/
typedef struct
{
  RedBlackTree* p_tree;
  RedBlackNode* p_current;
  LinkListIterator itList;
  bool atEnd;
} RedBlackIterator;



/**************************************************************************
* Initializes a particular red-black iterator to a particular tree/node.
**************************************************************************/
void RedBlackIterator_init(RedBlackIterator*, RedBlackTree*, RedBlackNode*);


/**************************************************************************
* Cleans up the red-black iterator.
**************************************************************************/
void RedBlackIterator_clean(RedBlackIterator*);


/**************************************************************************
* Move the specified red-black iterator to the next (largest) node.
**************************************************************************/
void RedBlackIterator_next(RedBlackIterator*);


/**************************************************************************
* Delete the node that the iterator is pointing at and moves it to 
* the next node.  The track record is returned.
**************************************************************************/
bool RedBlackIterator_remove(RedBlackIterator*, TrackRecord**);


/**************************************************************************
* Returns the current value being pointed at by the iterator.
**************************************************************************/
TrackRecord* RedBlackIterator_value(RedBlackIterator*);




/**************************************************************************
* RED-BLACK TREE
**************************************************************************/
struct RedBlackTreeStruct
{
  RedBlackNode* p_root;
  int size;
};


/**************************************************************************
* Initialize the red-black tree.
**************************************************************************/
void RedBlackTree_init(RedBlackTree*);


/**************************************************************************
* Clean up the red-black tree.
**************************************************************************/
void RedBlackTree_clean(RedBlackTree*);


/**************************************************************************
* Return the number of nodes in the red-black tree.
**************************************************************************/
uint RedBlackTree_size(const RedBlackTree* const);


/**************************************************************************
* Insert key/value pair into the tree.
**************************************************************************/
void RedBlackTree_insert(RedBlackTree*, float, TrackRecord*);


/**************************************************************************
* Initializes the red-black iterator by searching the tree for a 
* particular key and pointing it to that node.  NULL iterator if value 
* cannot be found.
**************************************************************************/
void RedBlackTree_find(RedBlackIterator*, RedBlackTree*, float);


/**************************************************************************
* Initializes an iterator to the beginning of the red-black tree.
**************************************************************************/
void RedBlackTree_getIterator(RedBlackIterator*, RedBlackTree*);


/**************************************************************************
* Initialize the red-black iterator to the element in the tree larger
* than the specified key.
**************************************************************************/
void RedBlackTree_findNextLargest(RedBlackIterator*, RedBlackTree*, float);



/**************************************************************************
* Search the tree for a particular key.  Returns a pointer to the node,
* or NULL if it cannot be found.
**************************************************************************/
RedBlackNode* RedBlackTree_findNode(RedBlackTree*, float);


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
