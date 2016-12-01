//Arduino doesn't like C libs, apparently. that's why this is here. 
#ifdef __cplusplus
extern "C" {
#endif

#ifndef _KEYQUEUE_H_
#define _KEYQUEUE_H_


//doubly linked node structure
typedef struct node{
 struct node *leading;
 struct node *trailing;
 int key1;
 int key2;
 int key3;
 int key4;
 int key5;
 int key6;
} node;


//queue structure
typedef struct LCqueue{
 struct node *front;
 struct node *back;
 int elements;
} LCqueue;

//node instantiator
node *new_node(int x);

//KeyQueue instantiator
KeyQueue *new_queue(void);

//removes first element and returns value
int pop(KeyQueue* q);

//adds value to end of queue
void push(KeyQueue* q, int a, int b, int c, int d, int e, int f);

//returns number of elements in the queue
int count(const KeyQueue* q);

//returns value of first element without removal of the element
int front(const KeyQueue* q);

//returns value of last element
int end(const KeyQueue* q);

int test(KeyQueue* q);

#endif
#ifdef __cplusplus
}
#endif
