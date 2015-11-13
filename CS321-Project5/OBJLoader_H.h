#ifndef OBJLoader_H
#define OBJLoader_H

#include <stdio.h>
#include <GL/freeglut.h>
#include "GLinkedList_H.h"

class Group{
private:
	char* name;
	//Display List, can be Linked List
	// * Format?
	//Array of Material Coefficients
public:
	Group();
	Group(char* gName);
};
class Object{//LinkedList of Groups
private:
	char* name;
	//Vertex List }
	//Normal List } Array
	LinkedList<Group> groups;
public:
	Object();
	Object(char* oName);
};
class OBJLoader{//LinkedList of Objects
private:
	FILE *fp;
	LinkedList<Object> objects;
public:
	OBJLoader(FILE *fp);

	int* getTagCounts();
};

#endif