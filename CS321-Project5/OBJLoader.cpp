#include "OBJLoader_H.h"

char getNC(FILE *fp){
	char c = fgetc(fp);
	printf("%c", c);
	return c;
}

OBJLoader::OBJLoader(){}
OBJLoader::OBJLoader(char file_name[256]){
	this->file_name = file_name;

	initObjects();
	printf("Objects Initialized\n");

	FILE *fp;
	fopen_s(&fp, file_name, "r");

	std::list<Object>::iterator cObj = objects.begin();
	std::list<Group>::iterator cGroup = cObj->groups.begin();

	int v = 0, vn = 0, f = 0, c = 0, lt = -1;
	while (c != EOF){
		//Try changing fgetc to all fscanf_s
		c = fgetc(fp);
		if (c == '#'){
			//scan to end of line
			//printf("#\n");
			while (c != '\n'){ c = fgetc(fp); }
		}
		else if (c == 'v'){
			int nc = fgetc(fp);
			GLfloat x, y, z;

			if (nc == 'n'){
				fscanf_s(fp, " %f %f %f\n", &x, &y, &z);
				vnList[vn] = x;
				vnList[vn + 1] = y;
				vnList[vn + 2] = z;

				//printf("vn %f %f %f\n", x, y, z);
				vn+=3;
				lt = 'vn';
			}
			else { //Not a Normal, just Vertex
				fseek(fp, -1, SEEK_CUR);
				fscanf_s(fp, " %f %f %f\n", &x, &y, &z);

				vList[v] = x;
				vList[v + 1] = y;
				vList[v + 2] = z;

				//printf("v %f %f %f : %i\n", x, y, z, v/3);
				v+=3;
				lt = 'v';
			}
		}
		else if (c == 'f'){
			int v1, vn1, v2, vn2, v3, vn3;
			fscanf_s(fp, " %i//%i %i//%i %i//%i\n", &v1, &vn1, &v2, &vn2, &v3, &vn3);

			cGroup->displayList[f] = v1-1;
			cGroup->displayList[f + 1] = vn1-1;
			cGroup->displayList[f + 2] = v2-1;
			cGroup->displayList[f + 3] = vn2-1;
			cGroup->displayList[f + 4] = v3-1;
			cGroup->displayList[f + 5] = vn3-1;

			//printf("f %i//%i %i//%i %i//%i\n", cGroup->displayList[f], cGroup->displayList[f + 1], cGroup->displayList[f + 2], cGroup->displayList[f + 3], cGroup->displayList[f + 4], cGroup->displayList[f + 5]);
			f+=6;
			lt = 'f';
		}
		else if (c == 'k'){
			int nc = fgetc(fp);
			if (nc == 'a' || nc == 'd' || nc == 's' || nc == 'e'){
				GLfloat r, g, b;
				fscanf_s(fp, " %f %f %f\n", &r, &g, &b);
				if ( (lt != 'g' || lt != 'k') && f > 0){
					++cGroup;
					f = 0;
					lt = 'g';
				}

				if (nc == 'a'){
					cGroup->kMats[0] = r;
					cGroup->kMats[1] = g;
					cGroup->kMats[2] = b;
					//printf("ka %f %f %f\n", cGroup->kMats[0], cGroup->kMats[1], cGroup->kMats[2]);
				}
				else if (nc == 'd'){
					cGroup->kMats[3] = r;
					cGroup->kMats[4] = g;
					cGroup->kMats[5] = b;
					//printf("kd %f %f %f\n", cGroup->kMats[3], cGroup->kMats[4], cGroup->kMats[5]);
				}
				else if (nc == 's'){
					cGroup->kMats[6] = r;
					cGroup->kMats[7] = g;
					cGroup->kMats[8] = b;
					//printf("ks %f %f %f\n", cGroup->kMats[6], cGroup->kMats[7], cGroup->kMats[8]);
				}
				else if (nc == 'e'){
					cGroup->kMats[9] = r;
					cGroup->kMats[10] = g;
					cGroup->kMats[11] = b;
					//printf("ke %f %f %f\n", cGroup->kMats[9], cGroup->kMats[10], cGroup->kMats[11]);
				}
			}
		}
		else if (c == 's'){
			if (fgetc(fp) == 'e'){
				GLfloat shiny;
				fscanf_s(fp, "%f\n", &shiny);
				
				cGroup->kMats[12] = shiny;

				//printf("se %f\n", cGroup->kMats[12]);
				lt = 'se';
			}
			else { 
				while (c != '\n'){ c = fgetc(fp); }
				lt = -1;
			}
		}
		else if (c == 'o'){
			if (v > 0 && vn > 0){
				//printf("o\n");
				while (c != '\n'){ c = fgetc(fp); }

				++cObj;
				cGroup = cObj->groups.begin();
				f = 0;
				lt = 'o';
			}
		}
		else if (c == 'g'){
			//printf("g\n");
			while (c != '\n'){ c = fgetc(fp); }

			if (f > 0){
				++cGroup;
				f = 0;
				lt = 'g';
			}
		}
		else if(c!=EOF){ //if un-supported tag is found ignore it & that 'tag' is not the EOF
			while (c != '\n'){ c = fgetc(fp); }
			lt = -1;
		}
	}
	fclose(fp);
}

void OBJLoader::initObjects(){
	FILE *fp;
	fopen_s(&fp, file_name, "r");

	Object cObj = *new Object();
	int v = 0, vBase = 0, vn = 0, vnBase = 0, f = 0, c = 0, lt = -1;
	while (c != EOF){
		c = fgetc(fp);
		if (c == 'v'){
			int nc = fgetc(fp);
			if (nc == 'n'){
				vn++;

				lt = 'vn';
			}
			else { 
				fseek(fp, -1, SEEK_CUR); 

				v++;
				lt = 'v';
			}
			
		}
		else if (c == 'k'){
			if ((lt != 'g' || lt != 'k') && f>0){
				cObj.groups.push_back(Group(f));
				f = 0;
			}

			lt = 'k';
		}
		else if (c == 'f'){
			f++;

			lt = 'f';
		}
		else if (c == 'o'){
			if (v > 0 && vn > 0){
				if (f > 0){
					cObj.groups.push_back(Group(f));
					f = 0;
				}
				objects.push_back(cObj);

				cObj = *new Object();
				vBase = v;
				vnBase = vn;
				lt = 'o';
			}
		}
		else if (c == 'g'){
			if (f > 0){
				cObj.groups.push_back(Group(f));
				f = 0;
			}
			lt = 'g';
		}
		else lt = -1;

		if (c != EOF){
			while (c != '\n'){ c = fgetc(fp); }
		}
	}
	if (f > 0){ cObj.groups.push_back(Group(f)); }
	if (v-vBase>0 && vn-vnBase>0) { 
		objects.push_back(cObj);
	}
	allocateLists(v, vn);

	fclose(fp);
}
void OBJLoader::allocateLists(int nVertices, int nVNormals){
	vList = (GLfloat *)calloc(nVertices * 3, sizeof(GLfloat));
	vnList = (GLfloat *)calloc(nVNormals * 3, sizeof(GLfloat));
}

Object::Object(){}
Object::Object(char* oName){
	
}


Group::Group(){}
Group::Group(int nFaces){
	// f #//# #//# #//# = 6 per Face
	displayList = (int *)calloc(6 * nFaces, sizeof(int));
	kMats = (GLfloat *)calloc(13, sizeof(GLfloat));
	this->nFaces = nFaces;
}
Group::Group(char* gName, int nFaces){}