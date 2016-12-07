//Arduino doesn't like C libs, apparently. that's why this is here. 
//uncomment these lines for use as an arduino library
//#ifdef __cplusplus
//extern "C" {
//#endif

#include <stdbool.h>

#ifndef _LCQUEUE_H_
#define _LCQUEUE_H_

//doubly linked node structure
typedef struct node{
 struct node *leading;
 struct node *trailing;
 int value;
} node;


//queue structure
typedef struct LCqueue{
 struct node *front;
 struct node *back;
 int elements;
} LCqueue;

//node instantiator
node *new_node(int x);

//LCqueue instantiator
LCqueue *new_queue(void);

//removes first element and returns value
int pop(LCqueue* q);

//adds value to end of queue
void push(LCqueue* q,int x);

//returns number of elements in the queue
int count(const LCqueue* q);

//returns value of first element without removal of the element
int front(const LCqueue* q);

//returns value of last element
int end(const LCqueue* q);

//returns true if the queue is empty 
bool isEmpty(const LCqueue* q);

#endif

//also, uncomment these lines.
//#ifdef __cplusplus
//}
//#endif
