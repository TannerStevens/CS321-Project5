#include "OBJLoader_H.h"

OBJLoader objs;

static GLint width, height;
static GLfloat LR = -3.0;

GLfloat transformations[] = {0,0,0,
							0,0,0,
							1,1,1};

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
	objs = OBJLoader(fp);

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
}

void display(void){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(transformations[0], transformations[1], transformations[2]);
	glRotatef(transformations[3], 1, 0, 0);
	glRotatef(transformations[4], 0, 1, 0);
	glRotatef(transformations[5], 0, 0, 1);
	glScalef(transformations[6], transformations[7], transformations[8]);

	for (std::list<Object>::iterator cObj = objs.objects.begin(); cObj != objs.objects.end();++cObj){
		for (std::list<Group>::iterator cGroup = cObj->groups.begin(); cGroup != cObj->groups.end(); ++cGroup){
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, cGroup->kMats);
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, cGroup->kMats + 3);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, cGroup->kMats + 6);
			glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, cGroup->kMats + 9);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, cGroup->kMats + 12);

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
		case '0':
			glDisable(GL_LIGHT0);
			glDisable(GL_LIGHT1);
			break;

		case 'w':
			transformations[1] += .1;
			break;
		case 's':
			transformations[1] -= .1;
			break;
		case 'a':
			transformations[0] += .1;
			break;
		case 'd':
			transformations[0] -= .1;
			break;
		case 'q':
			transformations[2] += .1;
			break;
		case 'e':
			transformations[2] -= .1;
			break;
		case '4':
			transformations[3] += 10;
			break;
		case '6':
			transformations[3] -= 10;
			break;
		case '8':
			transformations[4] += 10;
			break;
		case '5':
			transformations[4] -= 10;
			break;
		case '7':
			transformations[5] += 10;
			break;
		case '9':
			transformations[5] -= 10;
			break;
		case '+':
			transformations[6] += .1;
			transformations[7] += .1;
			transformations[8] += .1;
			break;
		case '-':
			transformations[6] -= .1;
			transformations[7] -= .1;
			transformations[8] -= .1;
			break;
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
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glShadeModel(GL_SMOOTH);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, new GLfloat[]{.5, .5, .5, 1.0});
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