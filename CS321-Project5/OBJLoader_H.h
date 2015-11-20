#ifndef OBJLoader_H
#define OBJLoader_H

#include <stdio.h>
#include <GL/freeglut.h>
#include <list>;

class Group{
private:
public:
	char* name;					//*Unused
	int* displayList;			//Display List - Array of 6 * nFaces Size
	int nFaces;					//Number of Triangles/Faces that are members of the Group
	GLfloat* kMats;				//Array of Material Coefficients
	GLfloat bRadius, bCenter[3];//OBJ Groups sphere parameters for Collision Checking

	Group();
	Group(int nFaces);
	Group(char* gName, int nFaces);

	// Computes bCenter, and bRadius based on Coordinate Extremes
	void finalize(GLfloat bUX, GLfloat bUY, GLfloat bUZ, GLfloat bLX, GLfloat bLY, GLfloat bLZ);
};
class Object{		//LinkedList of Groups
private:
public:
	char *name;		//*Unused
	std::list<Group> groups;

	Object();
	Object(char* oName);
};
class OBJLoader{	//LinkedList of Objects
private:
	char* file_name;

	//Reads through File to count v and vn tags in order to call allocateLists
	void initObjects();
	//Initializes vList and vnList to the appropriate sizes
	void allocateLists(int nVertices, int nVNormals);
public:
	GLfloat* vList;			//Vertex List, size nV*3
	GLfloat* vnList;		//Normal List, size nVn*3
	int nV, nVn;			//Number of Vertices and Normals stored by vList and vnList
	int mX, mY, mZ;			//Absolute distance of the Furthest Vertex stored in the OBJ File for each Axis
	std::list<Object> objects;

	OBJLoader();
	//Main Driver of parsing the given OBJ File
	OBJLoader(char file_name[256]);
};

#endif