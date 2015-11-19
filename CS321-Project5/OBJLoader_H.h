#ifndef OBJLoader_H
#define OBJLoader_H

#include <stdio.h>
#include <GL/freeglut.h>
#include <list>;

class Group{
private:
public:
	char* name;
	int* displayList;	//Display List - Array of 6 * f Size
	int nFaces; 
	GLfloat* kMats;			//Array of Material Coefficients
	GLfloat bRadius, bCenter[3];

	Group();
	Group(int nFaces);
	Group(char* gName, int nFaces);

	void finalize(GLfloat bUX, GLfloat bUY, GLfloat bUZ, GLfloat bLX, GLfloat bLY, GLfloat bLZ);
};
class Object{		//LinkedList of Groups
private:
public:
	char *name;
	std::list<Group> groups;

	Object();
	Object(char* oName);
};
class OBJLoader{	//LinkedList of Objects
private:
	char* file_name;

	void initObjects();
	void allocateLists(int nVertices, int nVNormals);
public:
	GLfloat* vList;			//Vertex List
	GLfloat* vnList;		//Normal List
	int nV, nVn, mX, mY, mZ;
	std::list<Object> objects;

	OBJLoader();
	OBJLoader(char file_name[256]);
};

#endif