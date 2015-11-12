#include <stdio.h>
#include <GL/freeglut.h>

class Group;
class Object;
class OBJLoader;

class OBJLoader{
private:
	//LinkedList of Objects
public:
	OBJLoader(FILE *fp);
	~OBJLoader();
};
class Object{//LinkedList of Groups
private:
public:
	Object(char* oName);
	~Object();
};

class Group{
private:
	//Vertex List }
	//Normal List } Array
	//Display List, can be Linked List
	//Vector of Material Coefficients
public:
	Group(char* gName);
	~Group();
};