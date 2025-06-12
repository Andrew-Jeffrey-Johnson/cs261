/*
 * In this file, you will write the structures and functions needed to
 * implement a priority queue.  Feel free to implement any helper functions
 * you need in this file to implement a priority queue.  Make sure to add your
 * name and @oregonstate.edu email address below:
 *
 * Name: Andrew Johnson
 * Email: johnand4@oregonstate.edu
 */

#include <stdlib.h>

#include "pq.h"
#include "dynarray.h"

/*
 * This structure represents a single node in the tree. It holds the value
 * and the priority of the value. Nodes are inserted into the dynarray.
 */
struct node {
  void* value;
  int priority;
};


/*
 * This is the structure that represents a priority queue.  You must define
 * this struct to contain the data needed to implement a priority queue.
 */
struct pq {
  struct dynarray* array; 
};


/*
 * This function should allocate and initialize an empty priority queue and
 * return a pointer to it.
 */
struct pq* pq_create() {
  struct pq* new_pq = malloc(sizeof(struct pq));
  new_pq->array = dynarray_create();
  return new_pq;
}


/*
 * This function should free the memory allocated to a given priority queue.
 * Note that this function SHOULD NOT free the individual elements stored in
 * the priority queue.  That is the responsibility of the caller.
 *
 * Params:
 *   pq - the priority queue to be destroyed.  May not be NULL.
 */
void pq_free(struct pq* pq) {
  for(int i = 0; i < dynarray_size(pq->array); i++) {
    free(dynarray_get(pq->array, i));
  }
  dynarray_free(pq->array);
  free(pq);
  return;
}


/*
 * This function should return 1 if the specified priority queue is empty and
 * 0 otherwise.
 *
 * Params:
 *   pq - the priority queue whose emptiness is to be checked.  May not be
 *     NULL.
 *
 * Return:
 *   Should return 1 if pq is empty and 0 otherwise.
 */
int pq_isempty(struct pq* pq) {
  if(dynarray_size(pq->array) > 0) {
    return 0;
  }
  return 1;
}


/*
 * This function swaps the values of two nodes, but does not touch their
 * placement in the dynamic array.
 *
 * Params:
 *   one - the first node
 *   two - the second node
 */
void swap_nodes(struct node* one, struct node* two) {
  struct node temp;
  temp.value = one->value;
  temp.priority = one->priority;
  one->value = two->value;
  one->priority = two->priority;
  two->value = temp.value;
  two->priority = temp.priority;
}


/*
 * This function performs the percolation through the queue upwords starting
 * from a particular node denoted at a given index. The percolation ends
 * when the node at the given index is properly integrated into the priority
 * queue structure. Nodes are swapped with their parents until the structure
 * is achieved.
 *
 * Params:
 *   pq - the priority queue to perform the percolation on
 *   index - the index of the node to be integrated
 */
void percolate_up(struct pq* pq, int index) {
  struct node* curr = dynarray_get(pq->array, index);
  struct node* parent = dynarray_get(pq->array, (index-1)/2);
  while(parent->priority > curr->priority) {
    swap_nodes(curr,parent);
    curr = parent;
    index = (index-1)/2;
    parent = dynarray_get(pq->array, (index-1)/2);
  }
}


/*
 * This function should insert a given element into a priority queue with a
 * specified priority value.  Note that in this implementation, LOWER priority
 * values are assigned to elements with HIGHER priority.  In other words, the
 * element in the priority queue with the LOWEST priority value should be the
 * FIRST one returned.
 *
 * Params:
 *   pq - the priority queue into which to insert an element.  May not be
 *     NULL.
 *   value - the value to be inserted into pq.
 *   priority - the priority value to be assigned to the newly-inserted
 *     element.  Note that in this implementation, LOWER priority values
 *     should correspond to elements with HIGHER priority.  In other words,
 *     the element in the priority queue with the LOWEST priority value should
 *     be the FIRST one returned.
 */
void pq_insert(struct pq* pq, void* value, int priority) {
  struct node* new_node = malloc(sizeof(struct node));
  new_node->value = value;
  new_node->priority = priority;
  dynarray_insert(pq->array, new_node);
  percolate_up(pq, dynarray_size(pq->array)-1);
  return;
}


/*
 * This function should return the value of the first item in a priority
 * queue, i.e. the item with LOWEST priority value.
 *
 * Params:
 *   pq - the priority queue from which to fetch a value.  May not be NULL or
 *     empty.
 *
 * Return:
 *   Should return the value of the first item in pq, i.e. the item with
 *   LOWEST priority value.
 */
void* pq_first(struct pq* pq) {
  struct node* node = dynarray_get(pq->array, 0);
  return node->value;
}


/*
 * This function should return the priority value of the first item in a
 * priority queue, i.e. the item with LOWEST priority value.
 *
 * Params:
 *   pq - the priority queue from which to fetch a priority value.  May not be
 *     NULL or empty.
 *
 * Return:
 *   Should return the priority value of the first item in pq, i.e. the item
 *   with LOWEST priority value.
 */
int pq_first_priority(struct pq* pq) {
  struct node* node = dynarray_get(pq->array, 0);
  return node->priority;
}


/*
 * This function performs percolation through a given priority queue downwards
 * on a particular node at a given index. This is used when the root  node is 
 * removed from the queue, and the nodes beneath it need to be adjusted in 
 * order to maintain the priority queue structure. Nodes are swapped with the
 * child who has the lowest priority until reaching the end of the queue or 
 * finding all children of the node have a greater priority value than the node.
 *
 * Params:
 *   pq    - priority queue to perform the percolation on
 *   index - the index of the node in the queue to be percolated
 */
void percolate_down(struct pq* pq, int index) {
  struct node* curr = dynarray_get(pq->array, index);
  struct node* left;
  struct node* right;
  while(2*index+1 < dynarray_size(pq->array)) {
    left = dynarray_get(pq->array, 2*index+1);
    if(2*index+2 < dynarray_size(pq->array)) {
      right = dynarray_get(pq->array, 2*index+2);
      if(left->priority < right->priority) {
        swap_nodes(curr, left);
	curr = left;
        index = 2*index+1;
      }
      else {
        swap_nodes(curr, right);
	curr = right;
        index = 2*index+2;
      }
    }
    else {
      swap_nodes(curr, left);
      curr = left;
      index = 2*index+1;
    }
  }
}


/*
 * This function should return the value of the first item in a priority
 * queue, i.e. the item with LOWEST priority value, and then remove that item
 * from the queue.
 *
 * Params:
 *   pq - the priority queue from which to remove a value.  May not be NULL or
 *     empty.
 *
 * Return:
 *   Should return the value of the first item in pq, i.e. the item with
 *   LOWEST priority value.
 */
void* pq_remove_first(struct pq* pq) {
  void* value = pq_first(pq);
  if(dynarray_size(pq->array)-1 > 0) {
    swap_nodes(dynarray_get(pq->array, 0), dynarray_get(pq->array, dynarray_size(pq->array)-1));
    struct node* node = dynarray_get(pq->array, dynarray_size(pq->array)-1);
    free(node);
    dynarray_remove(pq->array, dynarray_size(pq->array)-1);
    percolate_down(pq, 0);
  }
  else {
    struct node* node = dynarray_get(pq->array, 0);
    free(node);
    dynarray_remove(pq->array, 0);
  }
  return value;
}
