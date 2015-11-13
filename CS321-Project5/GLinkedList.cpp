#include "GLinkedList_H.h";

//Generic Node for the implementation of a Generic Doubly Linked List
template <class T> Node<T>::Node(){ this->data = new T(); }
template <class T> Node<T>::Node(T d){ data = d; }
template <class T> Node<T>::~Node(){
	delete data;
	this->previous = NULL;
	this->next = NULL;
	this->data = NULL;
}
template <class T> void Node<T>::setData(T d){ data = d; }
template <class T> T* Node<T>::getData(){ return &data; }
template <class T> void Node<T>::setNext(Node<T> *tnext){
	if (next != NULL) next->previous = tnext;
	next = tnext;
	if (tnext != NULL) tnext->previous = this;
}
template <class T> Node<T>* Node<T>::getNext(){ return next; }
template <class T> void Node<T>::setPrevious(Node<T> *tprevious){
	if (previous != NULL) previous->next = tprevious;
	previous = tprevious;
	if (tprevious != NULL) tprevious->next = this;
}
template <class T> Node<T>* Node<T>::getPrevious(){ return previous; }

//Generic Doubly Linked List
template <class T> LinkedList<T>::LinkedList(){ length = 0; }
template <class T> LinkedList<T>::LinkedList(Node<T> *f){ first = f; }
template <class T> LinkedList<T>::~LinkedList(){
	Node<T> *s = first;
	Node<T> *t = s->getNext();
	do{
		delete s;
		s = t;
		t = t->getNext();
	} while (s != NULL);
}
template <class T> void LinkedList<T>::setFirst(Node<T>* f){
	first = f;
}
template <class T> Node<T>* LinkedList<T>::getFirst() { return first; }
template <class T> void LinkedList<T>::setLast(Node<T>* l){ last = l; }
template <class T> void LinkedList<T>::removeLast(){
	if (length == 0) return;
	if (length == 1){
		delete first;
		delete last;
		first = NULL;
		last = NULL;
	}
	else{ // length > 1
		Node<T> *tN = last->getPrevious();
		last->setPrevious(NULL);
		delete last;
		last = tN;
	}
	length--;
}
template <class T> Node<T>* LinkedList<T>::getLast() { return last; }
template <class T> int LinkedList<T>::getLength(){ return length; }
template <class T> void LinkedList<T>::addNode(T d){
	Node<T> *tN = new Node<T>(d);
	if (length == 0) setFirst(tN);
	else last->setNext(tN);
	setLast(tN);
	length++;
}