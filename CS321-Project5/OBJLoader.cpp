#include "OBJLoader_H.h"

OBJLoader::OBJLoader(){}
OBJLoader::OBJLoader(char file_name[256]){
	this->file_name = file_name;

	initObjects();
	printf("Objects Initialized\n");

	FILE *fp;
	fopen_s(&fp, file_name, "r");

	std::list<Object>::iterator cObj = objects.begin();
	std::list<Group>::iterator cGroup = cObj->groups.begin();

	GLfloat bUX = 0, bUY = 0, bUZ = 0, bLX = 0, bLY = 0, bLZ = 0;
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

				//printf("vn %f %f %f : %i\n", x, y, z, vn/3);
				vn+=3;
				lt = 'vn';
			}
			else { //Not a Normal, just Vertex
				fscanf_s(fp, "%f %f %f\n", &x, &y, &z);

				vList[v] = x;
				vList[v + 1] = y;
				vList[v + 2] = z;

				if (x > mX) mX = abs(x);
				if (y > mY) mY = abs(y);
				if (z > mZ) mZ = abs(z);
				//printf("v %f %f %f : %i\n", x, y, z, v/3);
				v+=3;
				lt = 'v';
			}
		}
		else if (c == 'f'){
			int v1, vn1, v2, vn2, v3, vn3;
			fscanf_s(fp, " %i//%i %i//%i %i//%i\n", &v1, &vn1, &v2, &vn2, &v3, &vn3);

			cGroup->displayList[f] = --v1;
			cGroup->displayList[f + 1] = --vn1;
			cGroup->displayList[f + 2] = --v2;
			cGroup->displayList[f + 3] = --vn2;
			cGroup->displayList[f + 4] = --v3;
			cGroup->displayList[f + 5] = --vn3;
			
			if (vList[v1] > bUX) bUX = vList[v1];
			if (vList[v1 + 1] > bUY) bUY = vList[v1 + 1];
			if (vList[v1 + 2] > bUZ) bUZ = vList[v1 + 2];
			if (vList[v2] > bUX) bUX = vList[v2];
			if (vList[v2 + 1] > bUY) bUY = vList[v2 + 1];
			if (vList[v2 + 2] > bUZ) bUZ = vList[v2 + 2];
			if (vList[v3] > bUX) bUX = vList[v3];
			if (vList[v3 + 1] > bUY) bUY = vList[v3 + 1];
			if (vList[v3 + 2] > bUZ) bUZ = vList[v3 + 2];

			if (vList[v1] < bLX) bLX = vList[v1];
			if (vList[v1 + 1] < bLY) bLY = vList[v1 + 1];
			if (vList[v1 + 2] < bLZ) bLZ = vList[v1 + 2];
			if (vList[v2] < bLX) bLX = vList[v2];
			if (vList[v2 + 1] < bLY) bLY = vList[v2 + 1];
			if (vList[v2 + 2] < bLZ) bLZ = vList[v2 + 2];
			if (vList[v3] < bLX) bLX = vList[v3];
			if (vList[v3 + 1] < bLY) bLY = vList[v3 + 1];
			if (vList[v3 + 2] < bLZ) bLZ = vList[v3 + 2];

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
					cGroup->finalize(bUX, bUY, bUZ, bLX, bLY, bLZ);
					++cGroup;
					f = 0, bUX = 0, bUY = 0, bUZ = 0, bLX = 0, bLY = 0, bLZ = 0;
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
			if (fgetc(fp) == 'e' && fgetc(fp) == ' '){
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
				cGroup->finalize(bUX, bUY, bUZ, bLX, bLY, bLZ);
				cGroup = cObj->groups.begin();
				f = 0, bUX = 0, bUY = 0, bUZ = 0, bLX = 0, bLY = 0, bLZ = 0;
				lt = 'o';
			}
			else while (c != '\n'){ c = fgetc(fp); }
		}
		else if (c == 'g'){
			//printf("g\n");
			while (c != '\n'){ c = fgetc(fp); }

			if (f > 0){
				cGroup->finalize(bUX, bUY, bUZ, bLX, bLY, bLZ);
				++cGroup;
				f = 0, bUX = 0, bUY = 0, bUZ = 0, bLX = 0, bLY = 0, bLZ = 0;
				lt = 'g';
			}
		}
		else if(c!=EOF){ //if un-supported tag is found ignore it & that 'tag' is not the EOF
			while (c != '\n'){ c = fgetc(fp); }
			lt = -1;
		}
	}
	printf("v:%i/%i vn:%i/%i", v / 3, nV, vn / 3, nVn);

	fclose(fp);
}

void OBJLoader::initObjects(){
	FILE *fp;
	fopen_s(&fp, file_name, "r");

	Object cObj = *new Object();
	int v = 0, vBase = 0, vn = 0, vnBase = 0, f = 0, o = 0, c = 0, lt = -1;
	while (c != EOF){
		c = fgetc(fp);
		if (c == 'v'){
			int nc = fgetc(fp);
			if (nc == 'n'){
				vn++;

				lt = 'vn';
			}
			else { 
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
				o++;
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
		o++;
		objects.push_back(cObj);
	}
	allocateLists(v, vn);
	printf("v:%i vn:%i o:%i\n", v, vn, o);

	fclose(fp);
}
void OBJLoader::allocateLists(int nVertices, int nVNormals){
	nV = nVertices;
	vList = (GLfloat *)calloc(nVertices * 3, sizeof(GLfloat));
	nVn = nVNormals;
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
void Group::finalize(GLfloat bUX, GLfloat bUY, GLfloat bUZ, GLfloat bLX, GLfloat bLY, GLfloat bLZ){
	this->bCenter[0] = (bUX + bLX) / 2;
	this->bCenter[1] = (bUY + bLY) / 2;
	this->bCenter[2] = (bUZ + bLZ) / 2;

	GLfloat xr = bUX - bLX;
	GLfloat yr = bUY - bLY;
	GLfloat zr = bUX - bLX;

	this->bRadius = xr;
	if (yr > this->bRadius) this->bRadius = yr;
	if (zr > this->bRadius) this->bRadius = zr;
	this->bRadius /= 2;
}