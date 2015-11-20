#include "Project5_H.h"

OBJLoader objs;

static GLint width, height;
static GLfloat LR = -3.0;

GLfloat scFac = 1;
GLfloat transformations[] = { 0, 0, 0,
							0, 0, 0,
							1, 1, 1 };
GLfloat lightRotations[] = { 0, 0, 0 };
//0:gKa, 1:gKd, 2:gKs, 3:gKe, 4:gSe
GLfloat gKoefficients[] = { 1, 1, 0, 0, 0 };
GLfloat posLightSpecScalar = .5;
GLfloat posLightCutoffAngle = 180;

int debug = false;
GLdouble debugC[6];
Group *selectedG = NULL;

template <class T> void printArr(T* arr, int n){
	printf("\n");
	for (int i = 0; i < n; i++){
		printf(" %g", arr[i]);
	}
}

void modifySelectedMaterials(){
	char command[3];
	GLfloat r, g, b;
	printf("\nEnter ka, kd, ks, ke, se, or c to stop this prompt:\n"); //Prompt User
	scanf_s("%s", command, _countof(command)); //Look for Response
	if (command[0] == 'k'){
		printf("\nEnter <R-Value> <G-Value> <B-Value>: ");
		scanf_s("%f %f %f", &r, &g, &b); //Look for Response

		if (command[1] == 'a'){
			selectedG->kMats[0] = r;
			selectedG->kMats[1] = g;
			selectedG->kMats[2] = b;
		}
		else if (command[1] == 'd'){
			selectedG->kMats[3] = r;
			selectedG->kMats[4] = g;
			selectedG->kMats[5] = b;
		}
		else if (command[1] == 's'){
			selectedG->kMats[6] = r;
			selectedG->kMats[7] = g;
			selectedG->kMats[8] = b;
		}
		else if (command[1] == 'e'){
			selectedG->kMats[9] = r;
			selectedG->kMats[10] = g;
			selectedG->kMats[11] = b;
		}
	}
	else if (command[0] == 's' && command[1] == 'e'){
		printf("\nEnter <se-Value>: ");
		selectedG->kMats[12] = r;
	}


	glutPostRedisplay();
}

GLfloat distance(GLfloat* v){
	return sqrt(pow(v[0], 2) + pow(v[1], 2) + pow(v[2], 2));
}

GLfloat* crossProduct(GLfloat *v1, GLfloat *v2){ //
	GLfloat* rVector = (GLfloat*)calloc(3, sizeof(GLfloat));

	rVector[0] = v1[1] * v2[2] - v1[2] * v2[1];
	rVector[1] = v1[2] * v2[0] - v1[0] * v2[2];
	rVector[2] = v1[0] * v2[1] - v1[1] * v2[0];

	if (debug){
		glBegin(GL_LINES);
		glColor3f(0, 0, 1);
		glVertex3d(v1[0], v1[1], v1[2]);
		glVertex3d(rVector[0], rVector[1], rVector[2]);
		glEnd();
		glFlush();
	}

	return rVector;
}
GLfloat dotProduct(GLfloat *v1, GLfloat *v2){
	return (v1[0] * v2[0]) + (v1[1] * v2[1]) + (v1[2] * v2[2]);
}

GLfloat* p2pVec(GLfloat* sP, GLfloat* eP){
	GLfloat* rVector = (GLfloat*)calloc(3, sizeof(GLfloat));

	rVector[0] = eP[0] - sP[0];
	rVector[1] = eP[1] - sP[1];
	rVector[2] = eP[2] - sP[2];

	if (debug){
		glBegin(GL_LINES);
		glVertex3d(sP[0], sP[1], sP[2]);
		glVertex3d(eP[0], eP[1], eP[2]);
		glEnd();
		glFlush();
	}

	return rVector;
}

GLfloat* p2pVec(GLdouble* sP, GLdouble* eP){
	GLfloat* rVector = (GLfloat*)calloc(3, sizeof(GLfloat));

	rVector[0] = eP[0] - sP[0];
	rVector[1] = eP[1] - sP[1];
	rVector[2] = eP[2] - sP[2];

	if (debug){
		glBegin(GL_LINES);
		glVertex3d(sP[0], sP[1], sP[2]);
		glVertex3d(eP[0], eP[1], eP[2]);
		glEnd();
		glFlush();
	}

	return rVector;
}
GLfloat* p2pUVec(GLfloat* sP, GLfloat* eP){
	GLfloat* rVector = (GLfloat*)calloc(3, sizeof(GLfloat));

	rVector[0] = eP[0] - sP[0];
	rVector[1] = eP[1] - sP[1];
	rVector[2] = eP[2] - sP[2];

	if (debug){
		glBegin(GL_LINES);
		glVertex3d(sP[0], sP[1], sP[2]);
		glVertex3d(eP[0], eP[1], eP[2]);
		glEnd();
		glFlush();
	}

	GLfloat d = distance(rVector);

	rVector[0] /= d;
	rVector[1] /= d;
	rVector[2] /= d;

	return rVector;
}
GLfloat* unitfyVector(GLfloat* v){
	GLfloat* rVector = (GLfloat*)calloc(3, sizeof(GLfloat));

	GLfloat s = distance(v);

	rVector[0] = v[0] / s;
	rVector[1] = v[1] / s;
	rVector[2] = v[2] / s;

	return rVector;
}
GLfloat* projectVectorU2V(GLfloat* u, GLfloat* v){
	GLfloat* rVector = (GLfloat*)calloc(3, sizeof(GLfloat));

	GLfloat s = dotProduct(u, v) / distance(v);

	rVector[0] = s*v[0];
	rVector[1] = s*v[1];
	rVector[2] = s*v[2];

	if (debug){
		glBegin(GL_LINES);
		glColor3f(.5, 0, .5);
		glVertex3f(u[0], u[1], u[2]);
		glVertex3f(rVector[0], rVector[1], rVector[2]);
		glEnd();
	}

	return rVector;
}

//aRxaC * bRxbC = aRxbC
GLfloat* MatrixMult(GLdouble* aM, int aR, int aC, GLfloat* bM, int bR, int bC){
	GLfloat* product = (GLfloat *)calloc(aR*bC, sizeof(GLfloat));
	if (aC == bR){
		for (int row = 0; row < aR; row++) {
			for (int col = 0; col < bC; col++) {
				// Multiply the row of A by the column of B to get the row, column of product.
				for (int inner = 0; inner < aC; inner++) {
					product[row*bC + col] += aM[row*aC + inner] * bM[inner*bC + col];
				}
			}
		}
	}
	return product;
}

//Credit to http://www.lighthouse3d.com/tutorials/maths for both the Triangle and Sphere Intersection Algorithms
int rayIntersectsTriangle(GLfloat *p, GLfloat *d, GLfloat *v0, GLfloat *v1, GLfloat *v2) { // Ray: p/Point, d/Direction ; Triangle: v0-2/Vertices
	glColor3f(1, 0, 0);
	GLfloat *e1, *e2, *h, *s, *q;
	GLdouble a, f, u, v, t;
	e1 = p2pVec(v0, v1);
	e2 = p2pVec(v0, v2);

	h = crossProduct(d, e2);
	a = dotProduct(e1, h);

	if (a > -0.00001 && a < 0.00001)
		return(false);

	f = 1 / a;
	s = p2pVec(v0, p);
	u = f * (dotProduct(s, h));

	if (u < 0.0 || u > 1.0)
		return(false);

	q = crossProduct(s, e1);
	v = f * dotProduct(d, q);

	if (v < 0.0 || u + v > 1.0)
		return(false);

	// at this stage we can compute t to find out where
	// the intersection point is on the line
	t = f * dotProduct(e2, q);

	if (t > 0.00001) // ray intersection
		return(true);

	else // this means that there is a line intersection
		// but not a ray intersection
		return (false);

}
int rayIntersectsSphere(GLfloat* p, GLfloat* d, GLfloat r, GLfloat* c){
	float t;
	glColor3f(0, 1, 0);
	//Compute A, B and C coefficients
	GLfloat *vpc = p2pVec(p, c);
	float A = dotProduct(d, d);
	float B = dotProduct(new GLfloat[]{2 * vpc[0], 2 * vpc[1], 2 * vpc[2]}, d);
	float C = dotProduct(vpc, vpc) - (r * r);

	//Find discriminant
	float disc = B * B - 4 * A * C;

	// if discriminant is negative there are no real roots, so return 
	// false as ray misses sphere
	if (disc < 0)
		return false;

	// compute q as described above
	float distSqrt = sqrtf(disc);
	float q;
	if (B < 0)
		q = (-B - distSqrt) / 2.0;
	else
		q = (-B + distSqrt) / 2.0;

	// compute t0 and t1
	float t0 = q / A;
	float t1 = C / q;

	// make sure t0 is smaller than t1
	if (t0 > t1)
	{
		// if t0 is bigger than t1 swap them around
		float temp = t0;
		t0 = t1;
		t1 = temp;
	}

	// if t1 is less than zero, the object is in the ray's negative direction
	if (t1 < 0)
		return true;

	// if t0 is less than zero, the intersection point is at t1
	if (t0 < 0)
	{
		t = t1;
		return true;
	}
	// else the intersection point is at t0
	else
	{
		t = t0;
		return true;
	}
}

Group* findIntersectingGroup(int screenX, int screenY){
	GLint viewport[4]; //var to hold the viewport info
	GLdouble modelview[16]; //var to hold the modelview info
	GLdouble projection[16]; //var to hold the projection matrix info
	GLfloat winX, winY; //variables to hold screen x,y,z coordinates
	GLdouble worldCV[3]; //variables to hold world x,y,z coordinates
	GLdouble worldFV[3]; //variables to hold world x,y,z coordinates

	glGetDoublev(GL_MODELVIEW_MATRIX, modelview); //get the modelview info
	glGetDoublev(GL_PROJECTION_MATRIX, projection); //get the projection matrix info
	glGetIntegerv(GL_VIEWPORT, viewport); //get the viewport info

	winX = (float)screenX;
	winY = (float)viewport[3] - (float)screenY;

	//get the world coordinates from the screen coordinates
	gluUnProject(winX, winY, 0, modelview, projection, viewport, &worldCV[0], &worldCV[1], &worldCV[2]);
	gluUnProject(winX, winY, 1, modelview, projection, viewport, &worldFV[0], &worldFV[1], &worldFV[2]);
	debugC[0] = worldCV[0];
	debugC[1] = worldCV[1];
	debugC[2] = worldCV[2];
	debugC[3] = worldFV[0];
	debugC[4] = worldFV[1];
	debugC[5] = worldFV[2];
	/*printf("\n%f %f %f", worldCV[0], worldCV[1], worldCV[2]);*/
	//printf("\n%f %f %f", worldFV[0], worldFV[1], worldFV[2]);

	GLfloat wPos[] = { (GLfloat)worldCV[0], (GLfloat)worldCV[1], (GLfloat)worldCV[2] };
	GLfloat* dir = unitfyVector(p2pVec(worldCV, worldFV));

	//Need to apply Transformations for Accurate Checking
	//Organize Groups by distance of GroupCenter from Point
	for (std::list<Object>::iterator cObj = objs.objects.begin(); cObj != objs.objects.end(); cObj++){
		for (std::list<Group>::iterator cGroup = cObj->groups.begin(); cGroup != cObj->groups.end(); cGroup++){
			if (rayIntersectsSphere(wPos, dir, cGroup->bRadius, cGroup->bCenter)){
				for (int f = 0; f < cGroup->nFaces; f++){
					if (rayIntersectsTriangle(wPos, dir, objs.vList + cGroup->displayList[f * 6] * 3, objs.vList + cGroup->displayList[f * 6 + 2] * 3, objs.vList + cGroup->displayList[f * 6 + 4] * 3)){
						return &*cGroup;
					}
				}
			}
		}
	}

	return NULL;
}

void UserOpenFile(){
	int validFile = 0;
	FILE *fp;
	char file_name[256];

	while (!validFile){
		printf("Enter File Name: "); //Prompt User
		scanf_s("%s", file_name, _countof(file_name)); //Look for Response
		fopen_s(&fp, file_name, "r"); // Attempt opening File
		if (fp == NULL) printf("\nCan't open %s!\n", file_name); // If cannot open, Loop.
		else validFile = 1; // If file was opened then terminate the loop.
	}
	fclose(fp);
	objs = OBJLoader(file_name);

	scFac = objs.mX;
	if (objs.mY > scFac)scFac = objs.mY;
	if (objs.mZ > scFac)scFac = objs.mZ;
}

void display(void){
	if (!debug) glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);

	for (std::list<Object>::iterator cObj = objs.objects.begin(); cObj != objs.objects.end(); ++cObj){
		for (std::list<Group>::iterator cGroup = cObj->groups.begin(); cGroup != cObj->groups.end(); ++cGroup){
			GLfloat tEmissive[] = { cGroup->kMats[9] + 1 * gKoefficients[3], cGroup->kMats[10] + 1 * gKoefficients[3], cGroup->kMats[11] + 1 * gKoefficients[3] };
			GLfloat tShiny = cGroup->kMats[12] + gKoefficients[4] > 128 ? 128 : cGroup->kMats[12] + gKoefficients[4];
			tShiny = tShiny < 0 ? 0 : tShiny;

			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, cGroup->kMats);
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, cGroup->kMats + 3);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, cGroup->kMats + 6);
			glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, tEmissive);
			glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, tShiny);

			for (int f = 0; f < cGroup->nFaces; f++){
				/*printf("\n");
				printf("%i//%i %i//%i %i//%i\n", (cGroup->displayList + f * 6)[0], (cGroup->displayList + f * 6)[1], (cGroup->displayList + f * 6)[2], (cGroup->displayList + f * 6)[3], (cGroup->displayList + f * 6)[4], (cGroup->displayList + f * 6)[5]);
				printf("	%f %f %f\n", (objs.vList + cGroup->displayList[f * 6] * 3)[0], (objs.vList + cGroup->displayList[f * 6] * 3)[1], (objs.vList + cGroup->displayList[f * 6] * 3)[2]);
				printf("	%f %f %f\n", (objs.vnList + cGroup->displayList[f * 6 + 1] * 3)[0], (objs.vnList + cGroup->displayList[f * 6 + 1] * 3)[1], (objs.vnList + cGroup->displayList[f * 6 + 1] * 3)[2]);
				printf("	%f %f %f\n", (objs.vList + cGroup->displayList[f * 6 + 2] * 3)[0], (objs.vList + cGroup->displayList[f * 6 + 2] * 3)[1], (objs.vList + cGroup->displayList[f * 6 + 2] * 3)[2]);
				printf("	%f %f %f\n", (objs.vnList + cGroup->displayList[f * 6 + 3] * 3)[0], (objs.vnList + cGroup->displayList[f * 6 + 3] * 3)[1], (objs.vnList + cGroup->displayList[f * 6 + 3] * 3)[2]);
				printf("	%f %f %f\n", (objs.vList + cGroup->displayList[f * 6 + 4] * 3)[0], (objs.vList + cGroup->displayList[f * 6 + 4] * 3)[1], (objs.vList + cGroup->displayList[f * 6 + 4] * 3)[2]);
				printf("	%f %f %f\n", (objs.vnList + cGroup->displayList[f * 6 + 5] * 3)[0], (objs.vnList + cGroup->displayList[f * 6 + 5] * 3)[1], (objs.vnList + cGroup->displayList[f * 6 + 5] * 3)[2]);*/

				glBegin(GL_TRIANGLES);
				glNormal3fv(objs.vnList + cGroup->displayList[f * 6 + 1] * 3);
				glVertex3fv(objs.vList + cGroup->displayList[f * 6] * 3);
				glNormal3fv(objs.vnList + cGroup->displayList[f * 6 + 3] * 3);
				glVertex3fv(objs.vList + cGroup->displayList[f * 6 + 2] * 3);
				glNormal3fv(objs.vnList + cGroup->displayList[f * 6 + 5] * 3);
				glVertex3fv(objs.vList + cGroup->displayList[f * 6 + 4] * 3);
				glEnd();
				if (selectedG == &*cGroup){
					glDisable(GL_LIGHTING);
					glDisable(GL_DEPTH_TEST);
					glBegin(GL_LINE_LOOP);
					glColor3d(1, .655, .098);
					glVertex3fv(objs.vList + cGroup->displayList[f * 6] * 3);
					glVertex3fv(objs.vList + cGroup->displayList[f * 6 + 2] * 3);
					glVertex3fv(objs.vList + cGroup->displayList[f * 6 + 4] * 3);
					glEnd();
					glEnable(GL_DEPTH_TEST);
					glEnable(GL_LIGHTING);
				}
			}
		}

	}
	if (debug){
		glDisable(GL_LIGHTING);
		glDisable(GL_DEPTH_TEST);
		glPointSize(10);
		glBegin(GL_LINES);
		glColor3d(0, 0, 0);
		glVertex3d(debugC[0], debugC[1], debugC[2]);
		glVertex3d(debugC[3], debugC[4], debugC[5]);
		glEnd();
		glBegin(GL_POINTS);
		glVertex3d(debugC[0], debugC[1], debugC[2]);
		glEnd();
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LIGHTING);
	}

	glFlush();
}

void compileModelView(){
	if (debug)glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(transformations[0], transformations[1], transformations[2]);
	glScalef(transformations[6] / scFac, transformations[7] / scFac, transformations[8] / scFac);
	glPushMatrix();
	glLightfv(GL_LIGHT0, GL_POSITION, new GLfloat[]{0, 1.5, 0, 0});
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, new GLfloat[]{0, -1, 0});
	glRotatef(lightRotations[0], 1, 0, 0);
	glRotatef(lightRotations[1], 0, 1, 0);
	glRotatef(lightRotations[2], 0, 0, 1);
	glLightfv(GL_LIGHT1, GL_POSITION, new GLfloat[]{0, 0, 1.5, 1});
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, new GLfloat[]{0, 0, -1});
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, posLightCutoffAngle);
	glPopMatrix();
	glRotatef(transformations[3], 1, 0, 0);
	glRotatef(transformations[4], 0, 1, 0);
	glRotatef(transformations[5], 0, 0, 1);
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key){
	case '1':
		glEnable(GL_LIGHT0);
		break;
	case '2':
		glEnable(GL_LIGHT1);
		break;
	case '0':
		glDisable(GL_LIGHT0);
		glDisable(GL_LIGHT1);
		break;
	case 'A':
		gKoefficients[0] = gKoefficients[0] + .1 > 1 ? 1 : gKoefficients[0] + .1;
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, new GLfloat[]{1 * gKoefficients[0], 1 * gKoefficients[0], 1 * gKoefficients[0], 1.0});
		break;
	case 'a':
		gKoefficients[0] = gKoefficients[0] - .1 < 0 ? 0 : gKoefficients[0] - .1;
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, new GLfloat[]{1 * gKoefficients[0], 1 * gKoefficients[0], 1 * gKoefficients[0], 1.0});
		break;
	case 'D':
		gKoefficients[1] = gKoefficients[1] + .1 > 1 ? 1 : gKoefficients[1] + .1;
		glLightfv(GL_LIGHT0, GL_DIFFUSE, new GLfloat[]{1 * gKoefficients[1], 1 * gKoefficients[1], 1 * gKoefficients[1], 1.0});
		glLightfv(GL_LIGHT1, GL_DIFFUSE, new GLfloat[]{1 * gKoefficients[1], 1 * gKoefficients[1], 1 * gKoefficients[1], 1.0});
		break;
	case 'd':
		gKoefficients[1] = gKoefficients[1] - .1 < 0 ? 0 : gKoefficients[1] - .1;
		glLightfv(GL_LIGHT0, GL_DIFFUSE, new GLfloat[]{1 * gKoefficients[1], 1 * gKoefficients[1], 1 * gKoefficients[1], 1.0});
		glLightfv(GL_LIGHT1, GL_DIFFUSE, new GLfloat[]{1 * gKoefficients[1], 1 * gKoefficients[1], 1 * gKoefficients[1], 1.0});
		break;
	case 'S':
		gKoefficients[2] = gKoefficients[2] + .1 > 1 ? 1 : gKoefficients[2] + .1;
		glLightfv(GL_LIGHT1, GL_DIFFUSE, new GLfloat[]{1 * gKoefficients[2], 1 * gKoefficients[2], 1 * gKoefficients[2], 1.0});
		break;
	case 's':
		gKoefficients[2] = gKoefficients[2] - .1 < 0 ? 0 : gKoefficients[2] - .1;
		glLightfv(GL_LIGHT1, GL_DIFFUSE, new GLfloat[]{1 * gKoefficients[2], 1 * gKoefficients[2], 1 * gKoefficients[2], 1.0});
		break;
	case 'H':
		gKoefficients[4] = gKoefficients[4] + 8 > 128 ? 128 : gKoefficients[4] + 8;
		break;
	case 'h':
		gKoefficients[4] = gKoefficients[4] - 8 < -128 ? 0 : gKoefficients[4] - 8;
		break;
	case 'E':
		gKoefficients[3] = gKoefficients[3] + .1 > 1 ? 1 : gKoefficients[3] + .1;
		break;
	case 'e':
		gKoefficients[3] = gKoefficients[3] - .1 < 0 ? 0 : gKoefficients[3] - .1;
		break;
	case '+':
		if (posLightCutoffAngle == 90) posLightCutoffAngle = 180;
		else if (posLightCutoffAngle < 90) posLightCutoffAngle += 10;
		break;
	case '-':
		if (posLightCutoffAngle == 180) posLightCutoffAngle = 90;
		else if (posLightCutoffAngle >= 0) posLightCutoffAngle -= 10;
		break;
	case 'P':
		lightRotations[1] += 15;
		break;
	case 'p':
		lightRotations[1] -= 15;
		break;
	case 'Q':case'q':
		exit(42);
		break;

	case '4':
		transformations[3] += 15;
		break;
	case '6':
		transformations[3] -= 15;
		break;
	case '8':case 'O':case'o':
		transformations[4] += 15;
		break;
	case '5':
		transformations[4] -= 15;
		break;
	case '7':
		transformations[5] += 15;
		break;
	case '9':
		transformations[5] -= 15;
		break;
	case '*':
		transformations[6] += .1;
		transformations[7] += .1;
		transformations[8] += .1;
		break;
	case '/':
		transformations[6] -= .1;
		transformations[7] -= .1;
		transformations[8] -= .1;
		break;
	case '`':
		debug = !debug;
		break;
	}
	compileModelView();
	glutPostRedisplay();
}
void specialKeyEvent(int key, int x, int y){
	int mod = glutGetModifiers();
	if (key == GLUT_KEY_UP){
		if (mod == GLUT_ACTIVE_ALT) transformations[2] += .1;
		else transformations[1] += .1;
	}
	else if (key == GLUT_KEY_DOWN){
		if (mod == GLUT_ACTIVE_ALT) transformations[2] -= .1;
		else  transformations[1] -= .1;
	}
	else if (key == GLUT_KEY_RIGHT){
		transformations[0] += .1;
	}
	else if (key == GLUT_KEY_LEFT){
		transformations[0] -= .1;
	}
	else if (key == GLUT_KEY_INSERT){
		modifySelectedMaterials();
	}
	compileModelView();
	glutPostRedisplay();
}

void mouse(int button, int state, int x, int y){
	if (button == GLUT_LEFT_BUTTON){
		if (state == GLUT_DOWN){
			if (debug){ glDisable(GL_LIGHTING); glDisable(GL_DEPTH_TEST); }
			selectedG = findIntersectingGroup(x, y);
			if (selectedG){
				printf("\n\nka: %f %f %f", selectedG->kMats[0], selectedG->kMats[1], selectedG->kMats[2]);
				printf("\nkd: %f %f %f", selectedG->kMats[3], selectedG->kMats[4], selectedG->kMats[5]);
				printf("\nks: %f %f %f", selectedG->kMats[6], selectedG->kMats[7], selectedG->kMats[8]);
				printf("\nke: %f %f %f", selectedG->kMats[9], selectedG->kMats[10], selectedG->kMats[11]);
				printf("\nse: %f ", selectedG->kMats[12]);
			}
			if (debug){ glEnable(GL_LIGHTING); glEnable(GL_DEPTH_TEST); }
		}
	}
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-1, 1, -1, 1, -1, 1);
	if (w <= h)
		glOrtho(LR, -LR, LR * (GLfloat)h / (GLfloat)w,
		-LR * (GLfloat)h / (GLfloat)w, LR, -LR);
	else
		glOrtho(LR * (GLfloat)w / (GLfloat)h,
		-LR * (GLfloat)w / (GLfloat)h, LR, -LR, LR, -LR);
	compileModelView();
	width = w;	height = h;
}

void other_init()
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glShadeModel(GL_SMOOTH);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, new GLfloat[]{1 * gKoefficients[0], 1 * gKoefficients[0], 1 * gKoefficients[0], 1.0});

	glLightfv(GL_LIGHT1, GL_AMBIENT, new GLfloat[]{0, 0, 0, 1});
	glLightfv(GL_LIGHT1, GL_DIFFUSE, new GLfloat[]{1, 1, 1, 1});
	glLightfv(GL_LIGHT1, GL_SPECULAR, new GLfloat[]{1, 1, 1, 1});

	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
}

void init_window(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH);
	width = height = 500;
	glutInitWindowSize(width, height);
	glutInitWindowPosition(50, 50);
	glutCreateWindow("Stevte01-CS321_Project5");
}
