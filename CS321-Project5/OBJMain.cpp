#include "OBJLoader_H.h"

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
	OBJLoader obj = OBJLoader(fp);
}

void main(int argc, char** argv)
{
	char ch;

	UserOpenFile();
	
	ch = getchar(); ch = getchar();
}