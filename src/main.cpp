/*********************************************
ESC: exit
Movement:
w : move forwards
s : move backwards
a : move left
d : move right
mouse: look around

m: ignore mouse

**********************************************/

#include "FPSCamera.h"
#include <stdio.h>
#define FAR_CLIPPING_PLANE 10000
#define NEAR_CLIPPING_PLANE 0.01
#define VIEWING_ANGLE 40
#define BLUE 		0.0, 0.0, 1.0, 1.0
#define RED 		1.0, 0.0, 0.0, 1.0
#define YELLOW 		1.0, 1.0, 0.0, 1.0
#define GREEN 		0.0, 1.0, 0.0, 1.0
#define WHITE 		1.0, 1.0, 1.0, 1.0
#define BLACK 		0.0, 0.0, 0.0, 1.0

FPSCamera camera;
bool ignoreMouse = false;
bool keyState[8];
int windowWidth = 640;
int windowHeight = 400;
GLfloat xCenter = (GLfloat)windowWidth/2;
GLfloat yCenter = (GLfloat)windowHeight/2;

void drawAxis() {
	// Eixo X
	glColor4f(RED);
	glBegin(GL_LINES);
		glVertex3i( 0, 0, 0);
		glVertex3i(10, 0, 0);
	glEnd();

	// Eixo Y
	glColor4f(GREEN);
	glBegin(GL_LINES);
		glVertex3i(0,  0, 0);
		glVertex3i(0, 10, 0);
	glEnd();

	// Eixo Z
	glColor4f(BLUE);
	glBegin(GL_LINES);
		glVertex3i( 0, 0, 0);
		glVertex3i( 0, 0,10);
	glEnd();
}

void drawWalls() {
	glColor4f(WHITE);
	glPushMatrix();
		//glColor4f(WHITE);
		glScalef(50, 1, 48);
		glutWireCube(1);	//chao

		glTranslatef(0, 29, 0);
		glutWireCube(1);	//teto
	glPopMatrix();
	glPushMatrix();
		//glColor4f(BLUE);
		glTranslatef(-25.5, 14.5, 0);
		glScalef(1,30,50);
		glutWireCube(1);	//parede da porta

		glTranslatef(51, 0, 0);
		glutWireCube(1);	//parede da janela
	glPopMatrix();
	glPushMatrix();
		//glColor4f(RED);
		glTranslatef(0, 14.5, 24.5);
		glScalef(50, 30, 1);
		glutWireCube(1);	//parede do computador

		glTranslatef(0, 0, -49);
		glutWireCube(1);	//parede da cabeceira
	glPopMatrix();
}

void drawWardrobe() {
	glColor4f(YELLOW);
	glPushMatrix();
		glTranslatef(-24, 10.5, 18);
		glScalef(1, 18, 11);
		glutWireCube(1);	//lado esquerdo
		glTranslatef(14, 0, 0);
		glutWireCube(1);	//lado direito
	glPopMatrix();
	glPushMatrix();
		glTranslatef(-17, 1, 18);
		glScalef(15, 1, 11);
		glutWireCube(1);	//baixo
		glTranslatef(0, 19, 0);
		glutWireCube(1);	//teto
	glPopMatrix();
	glPushMatrix();
		glTranslatef(-17, 10.5, 23);
		glScalef(13, 18, 1);
		glutWireCube(1);	//tras
	glPopMatrix();
	glPushMatrix();
		glTranslatef(-20.25, 10.5, 13);
		glScalef(6.5, 18, 1);
		glutWireCube(1);	//porta esquerda
	glPopMatrix();
	glPushMatrix();
		glTranslatef(-13.75, 10.5, 13);
		glScalef(6.5, 18, 1);
		glutWireCube(1);	//porta direita
	glPopMatrix();
	glPushMatrix();
		glTranslatef(-18, 9, 12);
		glutWireSphere(0.5, 6, 6);	//macaneta esquerda
		glTranslatef(2, 0, 0);
		glutWireSphere(0.5, 6, 6);	//macaneta direita
	glPopMatrix();
}

void drawNightstand() {
	glColor4f(RED);
	glPushMatrix();
		glTranslatef(4, 3.5, -20);
		glScalef(10, 6, 6);
		glutWireCube(1);
	glPopMatrix();
}

void drawBed(){
	glColor4f(YELLOW);
	glPushMatrix();
		//pernas da cama
		glTranslatef(23, 2.5, -22);
		glScalef(1, 4, 1);
		glutWireCube(1);
		glTranslatef(-12, 0, 0);
		glutWireCube(1);
		glTranslatef(0, 0, 22);
		glutWireCube(1);
		glTranslatef(12, 0, 0);
		glutWireCube(1);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(17, 5, -11);
		glScalef(13, 1, 23);
		glutWireCube(1);	//armacao
	glPopMatrix();
	glPushMatrix();
		glTranslatef(17, 9.5, -22);
		glScalef(13, 8, 1);
		glutWireCube(1);	//cabeceira
	glPopMatrix();
	glPushMatrix();
		glTranslatef(17, 7, -10.75);
		glScalef(12, 2.5, 21.5);
		glutWireCube(1);	//colchao
	glPopMatrix();
}

void drawSeat(){
	glColor4f(RED);
	glPushMatrix();
		//pernas da cadeira
		glTranslatef(3, 2.5, 12);
		glScalef(1, 4, 1);
		glutWireCube(1);
		glTranslatef(-6, 0, 0);
		glutWireCube(1);
		glTranslatef(0, 0, -4);
		glutWireCube(1);
		glTranslatef(6, 0, 0);
		glutWireCube(1);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(0, 5, 10);
		glScalef(7, 1 , 5);
		glutWireCube(1);
	glPopMatrix();
}

void drawTable(){
	glColor4f(BLUE);
	//pernas
	glPushMatrix();
		glTranslatef(5, 4, 22.5);
		glScalef(0.75, 7, 0.75);
		glutWireCube(1);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(-5, 4, 22.5);
		glScalef(0.75, 7, 0.75);
		glutWireCube(1);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(-5, 4, 15.5);
		glScalef(0.75, 7, 0.75);
		glutWireCube(1);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(5, 4, 15.5);
		glScalef(0.75, 7, 0.75);
		glutWireCube(1);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(0, 8, 19);
		glScalef(15, 1, 10);
		glutWireCube(1);	//tampo
	glPopMatrix();
}

void drawObjects(){
	glColor4f(GREEN);
	glPushMatrix();
		glTranslatef(0, 13, 22);
		glScalef(10, 7, 2);
		glutWireCube(1);	//monitor
	glPopMatrix();
	glPushMatrix();
		glTranslatef(0, 9, 22);
		glScalef(7, 1, 0.5);
		glutWireCube(1);	//apoio
	glPopMatrix();
	glPushMatrix();
		glTranslatef(-2, 8.75, 16);
		glScalef(6, 0.5, 2);
		glutWireCube(1);	//teclado
	glPopMatrix();
	glPushMatrix();
		glTranslatef(-2, 8.75, 19.375);
		glScalef(0.1, 0.25, 4.75);
		glutWireCube(1);	//fio teclado
	glPopMatrix();
	glPushMatrix();
		glTranslatef(3, 8.75, 17);
		glScalef(1, 0.5, 1.5);
		glutWireCube(1);	//rato
	glPopMatrix();
	glPushMatrix();
		glTranslatef(3, 8.75, 19.75);
		glScalef(0.1, 0.25, 4);
		glutWireCube(1);	//fio rato
	glPopMatrix();
	glPushMatrix();
		glTranslatef(-6, 9.5, 18);
		glutWireTeapot(1);	//TODO: Chavena cilindrica
	glPopMatrix();
}


void applyKeys() {
	// Reads keyState array and moves camera accordingly
	int i;
	for (i = K_FRONT; i <= K_RIGHT; i++){
		if (keyState[i])
			camera.move(i);
	}
}

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	applyKeys();
	camera.render();

	glScalef(-1, 1, 1);	//mirroring x axis
	drawAxis();

	drawWalls();
	drawWardrobe();
	drawNightstand();
	drawBed();
	drawSeat();
	drawTable();
	drawObjects();

	glutSwapBuffers();
}

void reshape(int newWidth, int newHeight) {
	windowWidth = newWidth;
	windowHeight = newHeight;
	xCenter = (GLfloat)windowWidth/2;
	yCenter = (GLfloat)windowHeight/2;

	if (windowWidth == 0 || windowHeight == 0) return;  // avoid division by 0

	glViewport(0,0,windowWidth,windowHeight);  // Fit to the new window

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(VIEWING_ANGLE,(GLdouble)windowWidth/windowHeight, NEAR_CLIPPING_PLANE, FAR_CLIPPING_PLANE); //adapt perspective to new window

	glMatrixMode(GL_MODELVIEW);
}

void keyDown(unsigned char key, int x, int y) {
	switch (key) {
		case 27:		//ESC
			exit(0);
			break;
		case 'M':
		case 'm':
			ignoreMouse = !ignoreMouse;
			if (ignoreMouse)
				glutSetCursor(GLUT_CURSOR_INHERIT); //TODO: Find default name
			else
				glutSetCursor(GLUT_CURSOR_NONE);
			break;

		case 'W':
		case 'w':
			keyState[K_FRONT] = true;
			break;
		case 'S':
		case 's':
		    keyState[K_BACK] = true;
			break;
		case 'A':
		case 'a':
			keyState[K_LEFT] = true;
			break;
		case 'D':
		case 'd':
			keyState[K_RIGHT] = true;
			break;
	}
	glutPostRedisplay();
}

void keyUp(unsigned char key, int x, int y) {
	switch (key) {
		case 'W':
		case 'w':
			keyState[K_FRONT] = false;
			break;
		case 'S':
		case 's':
		    keyState[K_BACK] = false;
			break;
		case 'A':
		case 'a':
			keyState[K_LEFT] = false;
			break;
		case 'D':
		case 'd':
			keyState[K_RIGHT] = false;
			break;
	}
	glutPostRedisplay();
}

void mouseMovement(int x, int y) {
	if (ignoreMouse) return;
	if (x==(int)xCenter && y==(int)yCenter) return;		// Typecast needed for correct comparison

	camera.rotateHorizontal((xCenter - x)/20);
	camera.rotateVertical((yCenter - y)/20);

	glutWarpPointer(xCenter, yCenter);
	glutPostRedisplay();
}

void animate() {
	// This is needed to keep moving while keys are pressed down
	glutPostRedisplay();
}

void initialization() {
	glClearColor(BLACK);
	glEnable(GL_DEPTH_TEST);	//Profundidade
	glShadeModel(GL_SMOOTH);	//Interpolacao de cores

	glEnable(GL_CULL_FACE);		//Faces visiveis
	glCullFace(GL_BACK);		//Mostrar so as da frente

	glutSetCursor(GLUT_CURSOR_NONE);
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(windowWidth,windowHeight);
	glutCreateWindow("Teste");

	initialization();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);

	glutIgnoreKeyRepeat(1);
	glutKeyboardFunc(keyDown);
	glutKeyboardUpFunc(keyUp);

	glutPassiveMotionFunc(mouseMovement);
	glutMotionFunc(mouseMovement);

	glutIdleFunc(animate);

	glutMainLoop();
	return 0;
}
