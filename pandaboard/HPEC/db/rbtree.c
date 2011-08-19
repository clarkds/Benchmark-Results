/******************************************************************************
** File: rbtree.c
**
** HPEC Challenge Benchmark Suite
** Database Kernel Benchmark
**
** Contents:
**  Red-black tree implementation.  Red-black trees are balanced binary
**  search trees that require O(lg n) for inserts/deletes/searches.
**  They are described in greater detail in "Introduction to Algorithms"
**  (Cormen, Leiserson, Rivest, Stein).
**
** Author: Edmund L. Wong
**         MIT Lincoln Laboratory
**
******************************************************************************/
#include "rbtree.h"

#define true 1
#define false 0

/**************************************************************************
* RED-BLACK TREE NODE
**************************************************************************/

/**************************************************************************
* Create a red-black tree node; used internally by the red-black tree.
**************************************************************************/
RedBlackNode* RedBlackNode_create(const float k, 
				  TrackRecord* p_val, 
				  RedBlackNode* p_p, 
				  color_t c)
{
  RedBlackNode* p_rv = (RedBlackNode*) rb_malloc();
  p_rv->p_up = p_p;
  p_rv->p_left = p_rv->p_right = NULL;
  p_rv->color = c;
  p_rv->key = k;
  LinkList_init(&p_rv->valList);
  LinkList_append(&(p_rv->valList), p_val);
  return p_rv;
}


/**************************************************************************
* Destroy a red-black tree node.
**************************************************************************/
void RedBlackNode_destroy(RedBlackNode* p_rb)
{
  LinkList_clean(&p_rb->valList);
  rb_free(p_rb);
}



/**************************************************************************
* RED-BLACK TREE - INTERNAL FUNCTIONS
**************************************************************************/

/**************************************************************************
* Helper function for RedBlackTree_clean().  Used internally by
* the red-black tree.
**************************************************************************/
void RedBlackTree_destroySubtree(RedBlackTree* p_tree, RedBlackNode* p_node) 
{ 
  if (p_node) { 
    RedBlackTree_destroySubtree(p_tree, p_node->p_left); 
    RedBlackTree_destroySubtree(p_tree, p_node->p_right); 
    RedBlackNode_destroy(p_node);
  } 
}


/**************************************************************************
* Reroot the local subtree, updating the appropriate pointers.  Used
* interally by the red-black tree.
**************************************************************************/
void RedBlackTree_reroot(RedBlackTree* p_tree, 
			 RedBlackNode* p_new,
			 RedBlackNode* p_old)
{
  /* change the parent of the old root's left/right child, or reset the root
     if the old root was the global root */
  if (p_old == p_tree->p_root) p_tree->p_root = p_new;
  else if (p_old->p_up->p_left == p_old) p_old->p_up->p_left = p_new;
  else p_old->p_up->p_right = p_new;
  
  /* now let's update the parent pointer */
  if (p_new) p_new->p_up = p_old->p_up;
}


/**************************************************************************
* Perform a left-rotation on a given subtree, assumes node is NOT root.
* Used internally by the red-black tree.
**************************************************************************/
void RedBlackTree_rotateLeft(RedBlackTree* p_tree, 
			     RedBlackNode* p_node,
			     RedBlackNode* p_parent)
{
  p_parent->p_left = p_node->p_right;
  if (p_node->p_right) p_node->p_right->p_up = p_parent;
  p_node->p_right = p_parent;
  p_parent->p_up = p_node;
}


/**************************************************************************
* Perform a right-rotation on a given subtree, assumes node is NOT root.
* Used internally by the red-black tree.
**************************************************************************/
void RedBlackTree_rotateRight(RedBlackTree* p_tree, 
			      RedBlackNode* p_node,
			      RedBlackNode* p_parent)
{
  p_parent->p_right = p_node->p_left;
  if (p_node->p_left) p_node->p_left->p_up = p_parent;
  p_node->p_left = p_parent;
  p_parent->p_up = p_node;
}



/**************************************************************************
* Fix any red-black violations in the tree on insertion.  Used internally 
* by the red-black tree.
**************************************************************************/
void RedBlackTree_fixInsert(RedBlackTree* p_tree, 
			    RedBlackNode* p_node, 
			    RedBlackNode* p_parent)
{
  /* set grandparent pointer for accessibility */
  RedBlackNode* p_gparent = p_parent->p_up;

  /* red parent, red uncle */
  RedBlackNode* p_uncle = 
    p_gparent->p_left == p_parent ? 
    p_gparent->p_right : p_gparent->p_left;
	
  while (p_parent->color == RED && p_uncle && p_uncle->color == RED) {
    /* percolate the color up the tree */
    p_parent->color = p_uncle->color = BLACK;
    if (p_gparent != p_tree->p_root) p_gparent->color = RED;
	  
    /* check if we're done */
    if (p_gparent == p_tree->p_root || p_gparent->p_up == p_tree->p_root) return;

    /* ... we're not, move everything up */
    p_node = p_gparent;
    p_parent = p_gparent->p_up;
    p_gparent = p_parent->p_up;
    p_uncle = (p_gparent->p_left == p_parent) ?
      p_gparent->p_right : p_gparent->p_left;
  }
	
  /* red parent, black uncle */
  if (p_parent->color == RED && (!p_uncle || p_uncle->color == BLACK)) {
    /* recolor grandparent node */
    p_gparent->color = RED;

    /* four cases of rotations */
    if (p_gparent->p_left == p_parent) {
      if (p_parent->p_left == p_node) {
	/* case 0: zig-zig - rotate left */
	p_parent->color = BLACK;
	RedBlackTree_reroot(p_tree, p_parent, p_gparent);
	RedBlackTree_rotateLeft(p_tree, p_parent, p_gparent);
      }
	  
      else { /* p_parent->p_right == p_node */
	/* case 1: zig-zag */
	p_node->color = BLACK;
	RedBlackTree_reroot(p_tree, p_node, p_gparent);
	RedBlackTree_rotateRight(p_tree, p_node, p_parent);
	RedBlackTree_rotateLeft(p_tree, p_node, p_gparent);
      }
    }
	    
    else { /* p_gparent->p_right == p_parent */
      if (p_parent->p_left == p_node) {
	/* case 2: zag-zig */
	p_node->color = BLACK;
	RedBlackTree_reroot(p_tree, p_node, p_gparent);
	RedBlackTree_rotateLeft(p_tree, p_node, p_parent);
	RedBlackTree_rotateRight(p_tree, p_node, p_gparent);
      }
	    
      else { /* p_parent->p_right == p_node */
	/* case 3: zag-zag - rotate right */
	p_parent->color = BLACK;
	RedBlackTree_reroot(p_tree, p_parent, p_gparent);
	RedBlackTree_rotateRight(p_tree, p_parent, p_gparent);
      }
    }
  }
}



/**************************************************************************
* Fix any red-black violations in the tree on removal.  Used internally 
* by the red-black tree.
**************************************************************************/
bool RedBlackTree_fixRemove(RedBlackTree* p_tree,
			    RedBlackNode* p_node)
{
  RedBlackNode* p_deficit_parent;
  RedBlackNode* p_deficit;
  bool origColorIsBlack;

  /* node who had an element already deleted (D) */
  if (!p_node) return false;

  /* if there are still elements in that list, we're done */
  if (p_node->valList.length > 0) return true;

  /* now try to find a replacement node (R) */
  if (p_node->p_right) {
    /* replacement node exists; find it */
    RedBlackNode* p_replace = p_node->p_right;
    while (p_replace->p_left) p_replace = p_replace->p_left;

    /* set color of replacement node to be that of the deleted node */
    origColorIsBlack = p_replace->color == BLACK;
    p_replace->color = p_node->color;

    /* determine where the problem location will be */
    p_deficit_parent = (p_replace->p_up == p_node) ? p_replace : p_replace->p_up;
    p_deficit = p_replace->p_right;
      
    /* update R's left pointer and its parent */
    p_replace->p_left = p_node->p_left;
    if (p_node->p_left) p_node->p_left->p_up = p_replace;
    if (p_replace != p_node->p_right) {
      /* D is not R's right child
	 update R's right child's parent pointer and R's parent pointer */
      p_replace->p_up->p_left = p_replace->p_right;
      if (p_replace->p_right) p_replace->p_right->p_up = p_replace->p_up;

      /* now update R's right pointer and its parent */
      p_replace->p_right = p_node->p_right;
      p_node->p_right->p_up = p_replace;
    }
    /* update R's parent pointer and D's parent */
    RedBlackTree_reroot(p_tree, p_replace, p_node);
  }
  else {
    /* no replacement node; just attach children as necessary */
    p_deficit_parent = p_node->p_up;
    p_deficit = p_node->p_left;
    origColorIsBlack = p_node->color == BLACK;
    RedBlackTree_reroot(p_tree, p_node->p_left, p_node);
  }

  /* now we must fix any red-black violations */
  if (p_tree->p_root && origColorIsBlack) {
    while (p_deficit_parent) { /* among many other termination conditions */
      bool sibRight = !p_deficit_parent->p_left || p_deficit_parent->p_left == p_deficit;
      RedBlackNode* p_sibling = sibRight ? 
	p_deficit_parent->p_right : p_deficit_parent->p_left;
      bool sibLeftBlack = !p_sibling->p_left || p_sibling->p_left->color == BLACK;
      bool sibRightBlack = !p_sibling->p_right || p_sibling->p_right->color == BLACK;

      if (p_deficit && p_deficit->color == RED) {
	/* case 0 - node is red */
	p_deficit->color = BLACK;
	break;
      }

      else if (p_sibling->color == RED) {
	/* case 1 - parent is black, sibling is red */
	p_deficit_parent->color = RED;
	p_sibling->color = BLACK;
	  
	RedBlackTree_reroot(p_tree, p_sibling, p_deficit_parent);
	if (sibRight) 
	  RedBlackTree_rotateRight(p_tree, p_sibling, p_deficit_parent);
	else 
	  RedBlackTree_rotateLeft(p_tree, p_sibling, p_deficit_parent);
	continue; /* to case 3 */
      }
      /* p_sibling->color == BLACK */
      else if (sibLeftBlack && sibRightBlack) {
	if (p_deficit_parent->color == BLACK)
	  {
	    /* case 2 - parent is black, sibling is black, 
	       sibling's children are black */
	    p_sibling->color = RED;
	    p_deficit = p_deficit_parent;
	    p_deficit_parent = p_deficit_parent->p_up;
	    continue; 
	  }
	else { /* p_deficit_parent->color == RED */
	  /* case 3 - parent is red, sibling is black,
	     sibling's children are black */
	  p_sibling->color = RED;
	  p_deficit_parent->color = BLACK;
	  break;
	}
      }
      else if (sibRight) {
	if (sibRightBlack) {
	  /* case 4a - sibling is black/right, sibling's right child is black */
	  p_sibling->color = RED;
	  p_sibling->p_left->color = BLACK;
	  RedBlackTree_reroot(p_tree, p_sibling->p_left, p_sibling);
	  RedBlackTree_rotateLeft(p_tree, p_sibling->p_left, p_sibling);
	  continue;
	}
	else { /* !sibRightBlack */
	  /* case 5a - sibling is black/right, sibling's right child is red */
	  p_sibling->color = p_deficit_parent->color;
	  p_sibling->p_right->color = BLACK;
	  p_deficit_parent->color = BLACK;
	  RedBlackTree_reroot(p_tree, p_sibling, p_deficit_parent);
	  RedBlackTree_rotateRight(p_tree, p_sibling, p_deficit_parent);
	  break;
	}
      }
      else {
	if (sibLeftBlack) {
	  /* case 4b - sibling is black/left, sibling's left child is black */
	  p_sibling->color = RED;
	  p_sibling->p_right->color = BLACK;
	  RedBlackTree_reroot(p_tree, p_sibling->p_right, p_sibling);
	  RedBlackTree_rotateRight(p_tree, p_sibling->p_right, p_sibling);
	  continue;
	}
	else { /* !sibLeftBlack */
	  /* case 5b - sibling is black/left, sibling's left child is red */
	  p_sibling->color = p_deficit_parent->color;
	  p_sibling->p_left->color = BLACK;
	  p_deficit_parent->color = BLACK;
	  RedBlackTree_reroot(p_tree, p_sibling, p_deficit_parent);
	  RedBlackTree_rotateLeft(p_tree, p_sibling, p_deficit_parent);
	  break;
	}
      }
    }	
  }
    
  /* now free memory */
  RedBlackNode_destroy(p_node);

  /* return success */
  return true;
}


/**************************************************************************
* RED/BLACK TREE
**************************************************************************/

/**************************************************************************
* Initialize the red-black tree.
**************************************************************************/
void RedBlackTree_init(RedBlackTree* p_tree)
{ 
  p_tree->p_root = NULL;
  p_tree->size = 0;
}


/**************************************************************************
* Clean up the red-black tree.
**************************************************************************/
void RedBlackTree_clean(RedBlackTree* p_tree)
{
  RedBlackTree_destroySubtree(p_tree, p_tree->p_root);
}


/**************************************************************************
* Return the size of the tree.
**************************************************************************/
uint RedBlackTree_size(const RedBlackTree* const p_tree)
{
  return p_tree->size; 
}


/**************************************************************************
* Initializes the red-black iterator by searching the tree for a 
* particular key and pointing it to that node.  NULL iterator if value 
* cannot be found.
**************************************************************************/
void RedBlackTree_find(RedBlackIterator* p_rv, RedBlackTree* p_tree, 
		       float key)
{
  RedBlackIterator_init(p_rv, p_tree, 
			RedBlackTree_findNode(p_tree, key));
}


/**************************************************************************
* Search the tree for a particular key.  Returns a pointer to the node,
* or NULL if it cannot be found.
**************************************************************************/
RedBlackNode* RedBlackTree_findNode(RedBlackTree* p_tree, float key)
{
  /* let's start from the root */
  RedBlackNode* p_node = p_tree->p_root;
  
  /* begin traversing */
  while (p_node && p_node->key != key)
    p_node = (key < p_node->key) ? p_node->p_left : p_node->p_right;
  
  /* return what p_node points to */
  return p_node;
}


/**************************************************************************
* Initialize the red-black iterator to the element in the tree equal to
* or larger than the specified key.
**************************************************************************/
void RedBlackTree_findNextLargest(RedBlackIterator* p_rv, 
				  RedBlackTree* p_tree, float key)
{
  RedBlackNode* p_node = p_tree->p_root;
  RedBlackNode* p_min = NULL;
  
  while (p_node) {
    if (p_node->key > key) {
      if (!p_min || p_node->key < p_min->key) p_min = p_node;
      p_node = p_node->p_left;
    }
    else p_node = p_node->p_right;
  }
  RedBlackIterator_init(p_rv, p_tree, p_min);
}


/**************************************************************************
* Initializes an iterator to the beginning of the red-black tree.
**************************************************************************/
void RedBlackTree_getIterator(RedBlackIterator* p_rv,
			      RedBlackTree* p_tree)
{
  RedBlackNode* p_node = p_tree->p_root;
  if (!p_node) {
    RedBlackIterator_init(p_rv, p_tree, NULL);
    return;
  }
  while (p_node->p_left) p_node = p_node->p_left;
  RedBlackIterator_init(p_rv, p_tree, p_node);
}


/**************************************************************************
* Insert key/value pair into the tree.
**************************************************************************/
void RedBlackTree_insert(RedBlackTree* p_tree, float key, TrackRecord* val)
{
  RedBlackNode* p_node = p_tree->p_root;

  /* first increment size */
  p_tree->size++;

  /* if we have an empty tree, insert at root */
  if (!p_node) {
    p_tree->p_root = RedBlackNode_create(key, val, NULL, BLACK);
    return;
  }

  /* insert into subtree*/
  while (p_node) {
    if (key < p_node->key) {
      /* key is smaller than current node, move left */
      if (p_node->p_left) p_node = p_node->p_left;
      else { 
	p_node->p_left = RedBlackNode_create(key, val, p_node, RED);
	if (p_node != p_tree->p_root) 
	  RedBlackTree_fixInsert(p_tree, p_node->p_left, p_node);
	return;
      }
    }
    else if (key > p_node->key) {
      /* key is larger than current node, move right */
      if (p_node->p_right) p_node = p_node->p_right;
      else { 
	p_node->p_right = RedBlackNode_create(key, val, p_node, RED);
	if (p_node != p_tree->p_root) 
	  RedBlackTree_fixInsert(p_tree, p_node->p_right, p_node);
	return;
      }
    }
    else {
      /* key is equal to current node, insert */
      LinkList_append(&p_node->valList, val);
      return;
    }
  }
}



/**************************************************************************
* RED-BLACK TREE ITERATOR
**************************************************************************/

/**************************************************************************
* Initializes a particular red-black iterator to a particular tree/node.
**************************************************************************/
void RedBlackIterator_init(RedBlackIterator* p_rv, 
			   RedBlackTree* p_tree, 
			   RedBlackNode* p_p)
{ 
  p_rv->p_tree = p_tree;
  p_rv->p_current = p_p;
  if (p_p) {
    LinkListIterator_init(&p_rv->itList, &p_p->valList);
    p_rv->atEnd = p_rv->itList.atEnd;
  }
  else p_rv->atEnd = true;
}


/**************************************************************************
* Cleans up the red-black iterator.
**************************************************************************/
void RedBlackIterator_clean(RedBlackIterator* p_it)
{
  LinkListIterator_clean(&p_it->itList);
}


/**************************************************************************
* Move the specified red-black iterator to the next (largest) node.
**************************************************************************/
void RedBlackIterator_next(RedBlackIterator* p_it)
{
  RedBlackNode* p_node = p_it->p_current;
  LinkListIterator_next(&p_it->itList);
  if (p_it->itList.atEnd) {
    /* now let's start traversing */
    if (p_node->p_right) {
      p_node = p_node->p_right;
      while (p_node->p_left) p_node = p_node->p_left;
    }
    else {
      /* no right child; keep moving up the tree until 
	 we aren't a right child */
      while (p_node->p_up && p_node->p_up->p_right == p_node) 
	p_node = p_node->p_up;
      if (p_node->p_up) p_node = p_node->p_up;
      else p_it->atEnd = true;
    }
    LinkListIterator_set(&p_it->itList, &p_node->valList);
  }
  p_it->p_current = p_node;
}


/**************************************************************************
* Delete the node that the iterator is pointing at and move it to 
* the next node.  The track record is returned.
**************************************************************************/
bool RedBlackIterator_remove(RedBlackIterator* p_it, TrackRecord** p_rv)
{
  RedBlackNode* p_del;
  LinkListIterator ll_it;

  /* check if at end */
  if (p_it->atEnd) return false;
  
  /* now remove */
  p_del = p_it->p_current;
  ll_it = p_it->itList;

  LinkListIterator_remove(&ll_it, p_rv);
  RedBlackIterator_next(p_it);

  /* decrement size */
  p_it->p_tree->size--;

  /* fix problems in the tree */
  return RedBlackTree_fixRemove(p_it->p_tree, p_del);
}


/**************************************************************************
* Returns the current value being pointed at by the iterator.
**************************************************************************/
TrackRecord* RedBlackIterator_value(RedBlackIterator* p_it)
{ return LinkListIterator_value(&p_it->itList); }

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
