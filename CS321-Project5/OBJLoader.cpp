#include "OBJLoader_H.h"

OBJLoader::OBJLoader(FILE *fp){
	int c = 0;
	while (c != EOF){
		c = fgetc(fp);
		if (c == '#'){
			//scan to end of line
			printf("#\n");
			while (c != '\n'){ c = fgetc(fp); }
		}
		else if (c == 'v'){
			int nc = fgetc(fp);
			GLfloat x, y, z;
			fscanf_s(fp, " %f %f %f\n", &x, &y, &z);
			if (nc == 'n'){
				printf("vn %f %f %f\n", x, y, z);
			}
			else { //Not a Normal, just Vertex
				fseek(fp, -1, SEEK_CUR); 

				printf("v %f %f %f\n", x, y, z);
			}
		}
		else if (c == 'f'){
			int v1, vn1, v2, vn2, v3, vn3;
			fscanf_s(fp, " %i//%i %i//%i %i//%i\n", &v1, &vn1, &v2, &vn2, &v3, &vn3);

			printf("f %i//%i %i//%i %i//%i\n", v1, vn1, v2, vn2, v3, vn3);
		}
		else if (c == 'k'){
			int nc = fgetc(fp);
			GLfloat r, g, b;
			fscanf_s(fp, " %f %f %f\n", &r, &g, &b);

			if (nc == 'a') printf("ka %f %f %f\n", r,g,b);
			else if (nc == 'd') printf("kd %f %f %f\n", r, g, b);
			else if (nc == 's') printf("ks %f %f %f\n", r, g, b);
			else if (nc == 'e') printf("ke %f %f %f\n", r, g, b);
		}
		else if (c == 's'){
			if (fgetc(fp) == 'e'){
				GLfloat shiny;
				fscanf_s(fp, "%f\n", &shiny);

				printf("se %f\n", shiny);
			}
			else fseek(fp, -1, SEEK_CUR);
		}
		else if (c == 'o'){
			//ignore for now
			printf("o\n");
			while (c != '\n'){ c = fgetc(fp); }
		}
		else if (c == 'g'){
			//ignore for now
			printf("g\n");
			while (c != '\n'){ c = fgetc(fp); }
		}
		else{ //if un-supported tag is found
			while (c != '\n'){ c = fgetc(fp); }
		}
	}
}

OBJLoader::~OBJLoader(){}