#include "OBJLoader_H.h"

//GLUT Call Back Functions
void display(void);
void keyboard(unsigned char key, int x, int y);
void mouse(int button, int state, int x, int y);
void reshape(int w, int h);
void specialKeyEvent(int key, int x, int y);

//Initial Setup Functions
void other_init();
void init_window(int argc, char** argv);

//Handles post-OBJ File Prompt user input for modify Material Coefficients
void modifySelectedMaterials();
//Handles user input for opening an existing file
void UserOpenFile();

//Helper Function to keep the Modelview Matrix globally accessible and up-to-date
void compileModelView();

//Vector Functions for computing rayIntersectsSphere and Triangle
GLfloat* p2pUVec(GLfloat* sP, GLfloat* eP);
GLfloat* p2pVec(GLdouble* sP, GLdouble* eP);
GLfloat* p2pVec(GLfloat* sP, GLfloat* eP);
GLfloat* projectVectorU2V(GLfloat* u, GLfloat* v);
GLfloat* crossProduct(GLfloat *v1, GLfloat *v2);
GLfloat distance(GLfloat* v);
GLfloat dotProduct(GLfloat *v1, GLfloat *v2);
GLfloat* unitfyVector(GLfloat* v);
GLfloat* MatrixMult(GLdouble* aM, int aR, int aC, GLfloat* bM, int bR, int bC);
//

//Quick Approximation Check to see if a ray is within a OBJ Groups Domain
int rayIntersectsSphere(GLfloat* p, GLfloat* d, GLfloat r, GLfloat* c);
//Hard ray Collision Check against OBJ Group Triangles
int rayIntersectsTriangle(GLfloat *p, GLfloat *d, GLfloat *v0, GLfloat *v1, GLfloat *v2);

//Handler for rayCollision's and responsible for obtaining World Coordinates for a Mouse Click and it's appropriate Direction Vector
Group* findIntersectingGroup(int screenX, int screenY);