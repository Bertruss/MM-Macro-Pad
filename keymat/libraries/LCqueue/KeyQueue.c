#include <stdlib.h>
#include "LCqueue.h"

//simplified doubly linked-list queue


//LC queue memory allocation and variable instantiation
LCqueue *new_queue(void){
	LCqueue *temp = (LCqueue *)malloc(sizeof(LCqueue));
	temp->front = NULL;
	temp->back = NULL;
	temp->elements = 0;
	return temp;  		
}

//node memory allocation and variable instantiation
node *new_node(int x){
	node *temp = (node *)malloc(sizeof(node));
	temp->leading = NULL;
	temp->trailing = NULL;
	temp->value = x;
	return temp;
}

//return first elements stored value and removes the element
int pop(LCqueue* q){
	int temp;
	node *tempNode;
	if(q->elements == 0){
		return -1;
	}
	else{		
		//decrements # of elements
		q->elements = q->elements - 1;
		
		//pulls the value of the front node
		temp = q->front->value;
		
		//pulls the pointer to the next node from the front
		tempNode = q->front->trailing;
		
		//de-allocates memory for front nodes
		free(q->front);
		
		//set the following node to front
		q->front = tempNode;

		if(tempNode != NULL){
		//clears the leading pointer on the next node 
			q->front->leading = NULL;
		}
		else{
			q->back = NULL;	
		}
			
		return temp;
	}		
}

//adds element to the top of the queue which stores passed in value
void push(LCqueue* q, int x){
	//allocating memory
        node *temp = new_node(x); 

	//linking all necessary links
	
	//if node is being added to an empty queue, set this node as both the front and back node
	if(q->back == NULL && q->front == NULL){
		q->back = temp;
		q->front = temp;
	}
	else{ //if not empty, links the back node with the new node and vice versa,
	      //then set the back pointer of the queue to point at the new node 
		q->back->trailing = temp;
		temp->leading = q->back;
		q->back = temp;
	}
	
	//incrementing "elements" count
	q->elements = q->elements + 1; 
}

//returns the number of nodes in the queue
int count(const LCqueue* q){
	return q->elements;
}

//returns the value held by the first Node in the queue
int front(const LCqueue* q){
	return q->front->value;
 }

//returns the value held by the last Node in the queue
int end(const LCqueue* q){
	return q->back->value;
}


