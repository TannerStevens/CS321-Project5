#ifndef OBJLoader_H
#define OBJLoader_H

#include <stdio.h>
#include <GL/freeglut.h>

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

class Group{
private:
	char* name;
	int* displayList;	//Display List - Array of 6 * f Size?
	int* kMats;			//Array of Material Coefficients
public:
	Group();
	Group(int nFaces);
	Group(char* gName, int nFaces);
	~Group();
};
class Object{		//LinkedList of Groups
private:
	char *name;
	int *vList;			//Vertex List
	int *vnList;		//Normal List
	LinkedList<Group> *groups = new LinkedList<Group>;
public:
	Object();
	Object(int nVertices, int nVNormals);
	Object(char* oName, int nVertices, int nVNormals);
	~Object();
};
class OBJLoader{	//LinkedList of Objects
private:
	FILE *fp;
	LinkedList<Object> *objects = new LinkedList<Object>;
public:
	OBJLoader(FILE *fp);
	~OBJLoader();

	void initObjects();
};

#endif