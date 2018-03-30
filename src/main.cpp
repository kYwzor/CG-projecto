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

void drawNet(GLint linesX, GLint linesZ) {
	glBegin(GL_LINES);
	GLfloat half = 0.5;
	for (int xc = 0; xc < linesX; xc++) {
		glVertex3f(-half + xc / (GLfloat)(linesX-1), 0.0, half);
		glVertex3f(-half + xc / (GLfloat)(linesX-1), 0.0, -half);
	}
	for (int zc = 0; zc < linesX; zc++) {
		glVertex3f(half, 0.0,-half + zc / (GLfloat)(linesZ-1));
		glVertex3f(-half, 0.0, -half + zc / (GLfloat)(linesZ-1));
	}
	glEnd();
}


void drawBox(GLfloat sizeX, GLfloat sizeY, GLfloat sizeZ) {
	GLint linesX = 30;
	GLint linesZ = 30;
	glPushMatrix();
		glTranslatef(0, sizeY/2, 0);
		glScalef(sizeX,sizeY,sizeZ);

		glColor4f(WHITE);
		glPushMatrix();
			glTranslatef(0, -0.5, 0);
			drawNet(linesX,linesZ);
			glTranslatef(0 ,1, 0);
			drawNet(linesX,linesZ);
		glPopMatrix();

		glColor4f(BLUE);
		glPushMatrix();
			glTranslatef(-0.5, 0, 0);
			glRotatef(90, 0, 0, 1);
			drawNet(linesX,linesZ);
			glTranslatef(0, -1, 0);
			drawNet(linesX,linesZ);
		glPopMatrix();

		glColor4f(RED);
		glPushMatrix();
			glTranslatef(0, 0, -0.5);
			glRotatef(90,1,0,0);
			drawNet(linesX,linesZ);
			glTranslatef(0, 1, 0);
			drawNet(linesX,linesZ);
		glPopMatrix();
	glPopMatrix();
}

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
	glPushMatrix();
		glColor4f(WHITE);
		glScalef(50, 1, 48);
		glutWireCube(1);	//chao

		glTranslatef(0, 29, 0);
		glutWireCube(1);	//teto
	glPopMatrix();
	glPushMatrix();
		glColor4f(BLUE);
		glTranslatef(-25.5, 14.5, 0);
		glScalef(1,30,50);
		glutWireCube(1);	//parede da porta

		glTranslatef(51, 0, 0);
		glutWireCube(1);	//parede da janela
	glPopMatrix();
	glPushMatrix();
		glColor4f(RED);
		glTranslatef(0, 14.5, 24.5);
		glScalef(50, 30, 1);
		glutWireCube(1);	//parede do computador

		glTranslatef(0, 0, -49);
		glutWireCube(1);	//parede da cabeceira
	glPopMatrix();
}

void drawWardrobe() {
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
		glutWireCube(1);

		glTranslatef(7.5, 0, 0);
		glutWireCube(1);
	glPopMatrix();
}

void drawNightstand() {
	glPushMatrix();
		glTranslatef(4, 3.5, -20);
		glScalef(10, 6, 6);
		glutWireCube(1);
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

	glScalef(-1, 1, 1);
	drawAxis();
	//drawBox(6, 2, 6);
	//drawNet(30, 30);
	drawWalls();
	drawWardrobe();
	//drawNightstand();

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
