#include "OBJLoader_H.h"

//Generic Node for the implementation of a Generic Doubly Linked List
template <class T> Node<T>::Node(){ this->data = new T(); }
template <class T> Node<T>::Node(T d){ data = d; }
template <class T> Node<T>::~Node(){
	delete &data;
	this->previous = NULL;
	this->next = NULL;
}
template <class T> void Node<T>::setData(T d){ data = d; }
template <class T> T* Node<T>::getData(){ return &data; }
template <class T> void Node<T>::setNext(Node<T> *tnext){
	if (next != NULL) next->previous = tnext;
	next = tnext;
	if (tnext != NULL) tnext->previous = this;
}
template <class T> Node<T>* Node<T>::getNext(){ return next; }
template <class T> void Node<T>::setPrevious(Node<T> *tprevious){
	if (previous != NULL) previous->next = tprevious;
	previous = tprevious;
	if (tprevious != NULL) tprevious->next = this;
}
template <class T> Node<T>* Node<T>::getPrevious(){ return previous; }

//Generic Doubly Linked List
template <class T> LinkedList<T>::LinkedList(){ length = 0; }
template <class T> LinkedList<T>::LinkedList(Node<T> *f){ first = f; }
template <class T> LinkedList<T>::~LinkedList(){
	if (length > 0){
		Node<T> *s = first;
		Node<T> *t = s->getNext();
		do{
			delete s;
			s = t;
			t = t->getNext();
		} while (s != NULL);
	}
}
template <class T> void LinkedList<T>::setFirst(Node<T>* f){
	first = f;
}
template <class T> Node<T>* LinkedList<T>::getFirst() { return first; }
template <class T> void LinkedList<T>::setLast(Node<T>* l){ last = l; }
template <class T> void LinkedList<T>::removeLast(){
	if (length == 0) return;
	if (length == 1){
		delete first;
		delete last;
		first = NULL;
		last = NULL;
	}
	else{ // length > 1
		Node<T> *tN = last->getPrevious();
		last->setPrevious(NULL);
		delete last;
		last = tN;
	}
	length--;
}
template <class T> Node<T>* LinkedList<T>::getLast() { return last; }
template <class T> int LinkedList<T>::getLength(){ return length; }
template <class T> void LinkedList<T>::addNode(T d){
	Node<T> *tN = new Node<T>(d);
	if (length == 0) setFirst(tN);
	else last->setNext(tN);
	setLast(tN);
	length++;
}

OBJLoader::OBJLoader(FILE *fp){
	this->fp = fp;

	initObjects();

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
		else if(c!=EOF){ //if un-supported tag is found ignore it & that 'tag' is not the EOF
			while (c != '\n'){ c = fgetc(fp); }
		}
	}
	fclose(fp);
}
OBJLoader::~OBJLoader(){
	delete objects;
}

void OBJLoader::initObjects(){
	fseek(fp, 0, SEEK_SET);

	Object *cObj = new Object();
	Group *cGrp = new Group();
	int v = 0, vn = 0, f = 0, c = 0;
	while (c != EOF){
		c = fgetc(fp);
		if (c == 'v'){
			int nc = fgetc(fp);
			if (nc == 'n'){
				vn++;
			}
			else { 
				fseek(fp, -1, SEEK_CUR); 

				v++;
			}
			
		}
		else if (c == 'o'){
			objects->addNode();
			v = 0;
			vn = 0;
		}
		else if (c == 'g'){
			f = 0;
		}

		if (c != EOF){
			while (c != '\n'){ c = fgetc(fp); }
		}

	}

	fseek(fp, 0, SEEK_SET);
}

Object::Object(){}
Object::Object(int nVertices, int nVNormals){
	vList = (int *)calloc(nVertices, sizeof(int));
	vnList = (int *)calloc(nVNormals, sizeof(int));
}
Object::Object(char* oName, int nVertices, int nVNormals){
	this->name = oName;
	vList = (int *)calloc(nVertices, sizeof(int));
	vnList = (int *)calloc(nVNormals, sizeof(int));
}
Object::~Object(){
	if (vList) delete[] vList;
	if (vnList) delete[] vnList;
}

Group::Group(){}
Group::Group(char* gName, int nFaces){}
Group::~Group(){}