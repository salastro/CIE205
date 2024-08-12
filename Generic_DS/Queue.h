#ifndef __QUEUE_H_
#define __QUEUE_H_
#include "..\Enemies\Enemy.h"
#include <iostream>
#include <fstream>

/*This code is an updated version from "Data Abstraction & Problem Solving with C++,WALLS AND MIRRORS ,SIXTH EDITION"*/

/*
This is a program that implements the queue abstract data type using a linked list.
The queue is implemented as a chain of linked nodes that has two pointers, 
a frontPtr pointer for the front of the queue and a backPtr pointer for the back of the queue.
*/

/*

				The Node: item of type T and a "next" pointer
					------------- 
					| item| next | --->
					-------------
General Queue case:

                 frontPtr																backPtr
					\											   						/		
					 \											  					   /		
					------------- 	  ------------- 	  ------------- 	  ------------- 	  	  
					| item| next |--->| item| next |--->  | item| next |--->  | item| next |---> NULL
					------------- 	  ------------- 	  ------------- 	  -------------	  
		
Empty Case:

                 frontptr	 backptr
						\	 /				
						 \	/				
					---- NULL ------


Single Node Case:
                 frontPtr	 backPtr
					\		/	
					 \	   /			
					-------- 	
					|	|nxt -->NULL
					--------	

*/

#include "Node.h"

template <typename T>
class Queue
{
private :
	
	Node<T>* backPtr;
	Node<T>* frontPtr;
public :
	Queue();	
	bool isEmpty() const ;
	bool enqueue(const T& newEntry);
	bool ASCT_enqueue(const T& newEntry);
	bool dequeue(T& frntEntry);  
	bool peekFront(T& frntEntry)  const;
	bool peekLast(T& lstEntry)  const;
	T getFront();
	void printQueue() const; // New method for printing the queue

	void printQueuex(std::ostream& out) const;

	//toArray function to be used ONLY when drawing the queue items
	const T* toArray(int& count);	//returns array of T (array of items)
	bool removeItem(const T& item);
	T mostRepeatedItem() const;
	int size() const;
	bool contains(const T& item) const;
	void bubbleSort();
	~Queue();
};
/////////////////////////////////////////////////////////////////////////////////////////

/*
Function: Queue()
The constructor of the Queue class.

*/

template <typename T>
Queue<T>::Queue()
{
	backPtr=nullptr;
	frontPtr=nullptr;

}
/////////////////////////////////////////////////////////////////////////////////////////

/*
Function: isEmpty
Sees whether this queue is empty.

Input: None.
Output: True if the queue is empty; otherwise false.
*/
template <typename T>
bool Queue<T>::isEmpty() const
{
	if(frontPtr==nullptr)
		return true;
	else
		return false;
}

/////////////////////////////////////////////////////////////////////////////////////////

/*Function:enqueue
Adds newEntry at the back of this queue.

Input: newEntry .
Output: True if the operation is successful; otherwise false.
*/

template <typename T>
bool Queue<T>::enqueue( const T& newEntry)
{
	Node<T>* newNodePtr = new Node<T>(newEntry);
	// Insert the new node
	if (isEmpty())
		frontPtr = newNodePtr; // The queue is empty
	else
		backPtr->setNext(newNodePtr); // The queue was not empty
	backPtr = newNodePtr; // New node is at back
	return true;
	// end enqueue
} 


template<typename T>
inline bool Queue<T>::ASCT_enqueue(const T& newEntry)
{
	Enemy* pE = static_cast<Enemy*>(newEntry);
	if (!pE) return false;

	Node<T>* newNodePtr = new Node<T>(newEntry);
	
	if (isEmpty() || frontPtr->getItem()->GetArrvTime() > newEntry->GetArrvTime())
	{
		newNodePtr->setNext(frontPtr);
		frontPtr = newNodePtr;
		if (backPtr == nullptr)
			backPtr = newNodePtr;
		return true;
	}

	Node<T>* prevPtr = nullptr;
	Node<T>* currPtr = frontPtr;

	while (currPtr != nullptr && currPtr->getItem()->GetArrvTime() <= newEntry->GetArrvTime())
	{
		prevPtr = currPtr;
		currPtr = currPtr->getNext();
	}

	newNodePtr->setNext(currPtr);
	prevPtr->setNext(newNodePtr);

	if (currPtr == nullptr)
		backPtr = newNodePtr;

	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

/*Function: dequeue
Removes the front of this queue. That is, removes the item that was added
earliest.

Input: None.
Output: True if the operation is successful; otherwise false.
*/

template <typename T>
bool Queue<T>:: dequeue(T& frntEntry)  
{
	if(isEmpty())
		return false;

	Node<T>* nodeToDeletePtr = frontPtr;
	frntEntry = frontPtr->getItem();
	frontPtr = frontPtr->getNext();
	// Queue is not empty; remove front
	if (nodeToDeletePtr == backPtr)	 // Special case: one node in queue
		backPtr = nullptr ;	
		
	// Free memory reserved by the dequeued node
	delete nodeToDeletePtr;


	return true;

}

/////////////////////////////////////////////////////////////////////////////////////////

/*
Function: peekFront
gets the front of this queue. The operation does not modify the queue.

Input: None.
Output: The front of the queue.
return: flase if Queue is empty
*/

template <typename T>
bool Queue<T>:: peekFront(T& frntEntry) const 
{
	if(isEmpty())
		return false;

	frntEntry = frontPtr->getItem();
	return true;

}

template <typename T>
int Queue<T>::size() const
{
	int count = 0;
	Node<T>* currentNode = frontPtr;
	while (currentNode)
	{
		count++;
		currentNode = currentNode->getNext();
	}
	
	return count;
}

template <typename T>
bool Queue<T>::contains(const T& item) const
{
	Node<T>* currPtr = frontPtr;
	while (currPtr != nullptr)
	{
		if (currPtr->getItem() == item)
			return true;
		currPtr = currPtr->getNext();
	}
	return false;
}


template<typename T>
inline bool Queue<T>::peekLast(T& lstEntry) const
{
	if (isEmpty())
		return false;
	lstEntry = backPtr->getItem();
	return false;
}

template<typename T>
inline T Queue<T>::getFront()
{
	if (isEmpty())
		return nullptr;
	frontPtr->getItem();

	return frontPtr->getItem();
}

template <typename T>
void Queue<T>::printQueue() const
{
	if (isEmpty())
	{
		std::cout << "The queue is empty.\n";
		return;
	}

	Node<T>* currPtr = frontPtr;
	while (currPtr != nullptr)
	{
		const T& item = currPtr->getItem();
		if (item)
		{
			 std::cout << item->GetDetails();
			 std::cout << "-----------------------------" << "\n";
		}
		currPtr = currPtr->getNext();
	}
}

template <typename T>
void Queue<T>::printQueuex(std::ostream& out) const
{
	Node<T>* temp = frontPtr;
	while (temp != nullptr)
	{
		out << temp->getItem()->GetDetails() << std::endl; // Assuming each Enemy has a GetDetails method
		temp = temp->getNext();
	}
}

///////////////////////////////////////////////////////////////////////////////////
template <typename T>
Queue<T>::~Queue()
{
	T dummy; // Temporary variable for dequeuing
	while (!isEmpty()) {
		dequeue(dummy);
	}
}
/////////////////////////////////////////////////////////////////////////////////////////

/*
Function: toArray
returns an array of "T" 
Output: count: the length of the returned array (zero if Queue is empty)
returns: The array of T. (nullptr if Queue is empty)
*/

//IMPORTANT:
//toArray function to be used ONLY when drawing the queue items

template <typename T>
const T* Queue<T>::toArray(int& count)
{

	//IMPORTANT:
	//toArray function to be used ONLY when drawing the queue items

	count=0;

	if(!frontPtr)
		return nullptr;
	//counting the no. of items in the Queue
	Node<T>* p = frontPtr;
	while(p)
	{
		count++;
		p = p->getNext();
	}


	T* Arr= new T[count];
	p = frontPtr;
	for(int i=0; i<count;i++)
	{
		Arr[i] = p->getItem();
		p = p->getNext();
	}
	return Arr;
	//IMPORTANT:
	//toArray function to be used ONLY when drawing the queue items

}

template<typename T>
inline bool Queue<T>::removeItem(const T& item)
{
	if (isEmpty())
		return false;

	if (frontPtr->getItem() == item)
	{
		Node<T>* nodeToDeletePtr = frontPtr;
		frontPtr = frontPtr->getNext();
		if (nodeToDeletePtr == backPtr)
			backPtr = nullptr;
		delete nodeToDeletePtr;
		return true;
	}

	Node<T>* prevPtr = frontPtr;
	Node<T>* currPtr = frontPtr->getNext();

	while (currPtr != nullptr)
	{
		if (currPtr->getItem() == item)
		{
			prevPtr->setNext(currPtr->getNext());
			if (currPtr == backPtr)
				backPtr = prevPtr;
			delete currPtr;
			return true;
		}
		prevPtr = currPtr;
		currPtr = currPtr->getNext();
	}

	return false; // Item not found
}
template <typename T>
T Queue<T>::mostRepeatedItem() const
{
	if (isEmpty())
		return NULL;

	// Variables to store the most repeated item and its count
	T mostRepeated;
	int maxCount = 0;

	// Traverse the queue with a pointer
	Node<T>* outerNode = frontPtr;

	while (outerNode != nullptr)
	{
		T currentItem = outerNode->getItem();
		int currentCount = 0;

		// Inner loop to count occurrences of currentItem
		Node<T>* innerNode = frontPtr;
		while (innerNode != nullptr)
		{
			if (innerNode->getItem() == currentItem)
			{
				currentCount++;
			}
			innerNode = innerNode->getNext();
		}

		// Update most repeated item and its count if needed
		if (currentCount > maxCount)
		{
			maxCount = currentCount;
			mostRepeated = currentItem;
		}

		outerNode = outerNode->getNext();
	}

	return mostRepeated;
}
template <typename T>
void Queue<T>::bubbleSort() {
	if (isEmpty())
		return;

	bool swapped;
	Node<T>* ptr1;
	Node<T>* lptr = nullptr;
	if (frontPtr->getItem() == static_cast<Enemy*>(frontPtr->getItem()))
	{
		do {
			swapped = false;
			ptr1 = frontPtr;

			while (ptr1->getNext() != lptr) {
				if (ptr1->getItem()->getKTS() > ptr1->getNext()->getItem()->getKTS()) {
					// Swap the items
					T temp = ptr1->getItem();
					ptr1->setItem(ptr1->getNext()->getItem());
					ptr1->getNext()->setItem(temp);
					swapped = true;
				}
				ptr1 = ptr1->getNext();
			}
			lptr = ptr1;
		} while (swapped);
	}
	else
	{
		do {
			swapped = false;
			ptr1 = frontPtr;

			while (ptr1->getNext() != lptr) {
				if (ptr1->getItem() > ptr1->getNext()->getItem()) {
					// Swap the items
					T temp = ptr1->getItem();
					ptr1->setItem(ptr1->getNext()->getItem());
					ptr1->getNext()->setItem(temp);
					swapped = true;
				}
				ptr1 = ptr1->getNext();
			}
			lptr = ptr1;
		} while (swapped);
	}
}
#endif
