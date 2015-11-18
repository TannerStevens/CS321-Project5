#include "OBJLoader_H.h"

OBJLoader objs;

static GLint width, height;
static GLfloat LR = -3.0;

GLfloat transformations[] = {0,0,0,
							0,0,0,
							1,1,1};
GLfloat lightRotations[] = { 0, 0, 0 };
//0:gKa, 1:gKd, 2:gKs, 3:gKe, 4:gSe
GLfloat gKoefficients[] = { 1, 1, 0, 0, 0};
GLfloat posLightSpecScalar = .5;
GLfloat posLightCutoffAngle = 0;

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
<<<<<<< HEAD
	fclose(fp);
	objs = OBJLoader(file_name);
=======
	objs = OBJLoader(fp);
<<<<<<< HEAD
=======
>>>>>>> origin/master
	getchar();
	/*for (std::list<Object>::iterator cObj = objs.objects.begin(); cObj != objs.objects.end(); ++cObj){
		for (std::list<Group>::iterator cGroup = cObj->groups.begin(); cGroup != cObj->groups.end(); ++cGroup){
			for (int f = 0; f < cGroup->nFaces; f++){

				printf("\n%i//%i %i//%i %i//%i\n\n", cGroup->displayList[f * 6], cGroup->displayList[f * 6 + 1], cGroup->displayList[f * 6 + 2], cGroup->displayList[f * 6 + 3], cGroup->displayList[f * 6 + 4], cGroup->displayList[f * 6 + 5]);

				printf("vn %f %f %f\n", objs.vnList[cGroup->displayList[f * 6 + 1] * 3], objs.vnList[cGroup->displayList[f * 6 + 1] * 3 + 1], objs.vnList[cGroup->displayList[f * 6 + 1] * 3 + 2]);
				printf("v  %f %f %f\n\n", objs.vList[cGroup->displayList[f * 6] * 3], objs.vList[cGroup->displayList[f * 6] * 3 + 1], objs.vList[cGroup->displayList[f * 6] * 3 + 2]);
				printf("vn %f %f %f\n", objs.vnList[cGroup->displayList[f * 6 + 3] * 3], objs.vnList[cGroup->displayList[f * 6 + 3] * 3 + 1], objs.vnList[cGroup->displayList[f * 6 + 3] * 3 + 2]);
				printf("v  %f %f %f\n\n", objs.vList[cGroup->displayList[f * 6 + 2] * 3], objs.vList[cGroup->displayList[f * 6 + 2] * 3 + 1], objs.vList[cGroup->displayList[f * 6 + 2] * 3 + 2]);
				printf("vn %f %f %f\n", objs.vnList[cGroup->displayList[f * 6 + 5] * 3], objs.vnList[cGroup->displayList[f * 6 + 5] * 3 + 1], objs.vnList[cGroup->displayList[f * 6 + 5] * 3 + 2]);
				printf("v  %f %f %f\n\n", objs.vList[cGroup->displayList[f * 6 + 4] * 3], objs.vList[cGroup->displayList[f * 6 + 4] * 3 + 1], objs.vList[cGroup->displayList[f * 6 + 4] * 3 + 2]);
			}
		}

	}*/
>>>>>>> origin/master
}

void display(void){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(transformations[0], transformations[1], transformations[2]);
	glRotatef(transformations[3], 1, 0, 0);
	glRotatef(transformations[4], 0, 1, 0);
	glRotatef(transformations[5], 0, 0, 1);
	glPushMatrix();
		glLightfv(GL_LIGHT0, GL_POSITION, new GLfloat[]{0, 1.5, 0, 0});
		glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, new GLfloat[]{0, -1, 0});
		glRotatef(lightRotations[0], 1, 0, 0);
		glRotatef(lightRotations[1], 0, 1, 0);
		glRotatef(lightRotations[2], 0, 0, 1);
		glLightfv(GL_LIGHT1, GL_POSITION, new GLfloat[]{0, 0, 1.5, 1});
		glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, new GLfloat[]{0, 0, -1});
	glPopMatrix();
	glScalef(transformations[6], transformations[7], transformations[8]);

	for (std::list<Object>::iterator cObj = objs.objects.begin(); cObj != objs.objects.end();++cObj){
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
				glBegin(GL_TRIANGLES);
					glNormal3fv(objs.vnList + cGroup->displayList[f*6 + 1] * 3);
					glVertex3fv(objs.vList + cGroup->displayList[f*6] * 3);
					glNormal3fv(objs.vnList + cGroup->displayList[f*6 + 3] * 3);
					glVertex3fv(objs.vList + cGroup->displayList[f*6 + 2] * 3);
					glNormal3fv(objs.vnList + cGroup->displayList[f*6 + 5] * 3);
					glVertex3fv(objs.vList + cGroup->displayList[f*6 + 4] * 3);
				glEnd();
			}
		}
		
	}
	glFlush();
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
			gKoefficients[2] = gKoefficients[2] - .1 < 0 ? 1 : gKoefficients[2] - .1;
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
			glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, posLightCutoffAngle);
			break;
		case '-':
			if (posLightCutoffAngle == 180) posLightCutoffAngle = 90;
			else if (posLightCutoffAngle >= 0) posLightCutoffAngle -= 10;
			glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, posLightCutoffAngle);
			break;
		case 'P':
			lightRotations[1] += 15;
			break;
		case 'p':
			lightRotations[1] -= 15;
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
	}
	glutPostRedisplay();
}
void specialKeyEvent(int key, int x, int y){
	int mod = glutGetModifiers();
	if (key == GLUT_KEY_UP){
		if (mod == GLUT_ACTIVE_ALT) transformations[2] += .1;
		else transformations[1] += .1;
	}
	if (key == GLUT_KEY_DOWN){
		if (mod == GLUT_ACTIVE_ALT) transformations[2] -= .1;
		else  transformations[1] -= .1;
	}
	if (key == GLUT_KEY_RIGHT){
		transformations[0] += .1;
	}
	if (key == GLUT_KEY_LEFT){
		transformations[0] -= .1;
	}
	glutPostRedisplay();
}


void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (w <= h)
		glOrtho(LR, -LR, LR * (GLfloat)h / (GLfloat)w,
		-LR * (GLfloat)h / (GLfloat)w, LR, -LR);
	else
		glOrtho(LR * (GLfloat)w / (GLfloat)h,
		-LR * (GLfloat)w / (GLfloat)h, LR, -LR, LR, -LR);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	width = w;	height = h;
}

void other_init()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
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
	glutCreateWindow("asd");
}


void main(int argc, char** argv)
{
	char ch;

	UserOpenFile();
	init_window(argc, argv);
	other_init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	
	ch = getchar(); ch = getchar();
}