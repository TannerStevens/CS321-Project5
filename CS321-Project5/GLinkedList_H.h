#ifndef GLinkedList_H
#define GLinkedList_H

#include <stdio.h>

template <class T>
class Node{
private:
	T data;
	Node<T> *previous, *next;
public:
	Node();
	Node(T d);
	~Node();

	void setData(T d);
	T* getData();
	void setNext(Node<T> *n);
	Node<T>* getNext();
	void setPrevious(Node<T> *p);
	Node<T>* getPrevious();
};
template <class T>
class LinkedList{
private:
	Node<T> *first, *last;
	int length;
public:
	LinkedList();
	LinkedList(Node<T> *f);
	~LinkedList();

	void setFirst(Node<T> *f);
	Node<T>* getFirst();
	void setLast(Node<T> *l);
	void removeLast();
	Node<T>* getLast();
	int getLength();

	void addNode(T d);
};

#endif