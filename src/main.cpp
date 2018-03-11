/*******************************************************
ESC: exit
Movement:
w : move forwards
s : move backwards
a : move left
d : move right
mouse: look around

********************************************************/

#include "FPSCamera.h"
#include <stdio.h>
#define FAR_CLIPPING_PLANE 20.0
#define NEAR_CLIPPING_PLANE 0.5
#define VIEWING_ANGLE 40
#define BLUE     0.0, 0.0, 1.0, 1.0
#define RED		 1.0, 0.0, 0.0, 1.0
#define YELLOW	 1.0, 1.0, 0.0, 1.0
#define GREEN    0.0, 1.0, 0.0, 1.0
#define WHITE    1.0, 1.0, 1.0, 1.0
#define BLACK    0.0, 0.0, 0.0, 1.0

FPSCamera camera;
bool keyState[8];
int windowWidth = 640;
int windowHeight = 400;
GLfloat xCenter = (GLfloat)windowWidth/2;
GLfloat yCenter = (GLfloat)windowHeight/2;

void drawNet(GLfloat size, GLint LinesX, GLint LinesZ) {
	glBegin(GL_LINES);
	for (int xc = 0; xc < LinesX; xc++)
	{
		glVertex3f(	-size / 2.0 + xc / (GLfloat)(LinesX-1)*size,
					0.0,
					size / 2.0);
		glVertex3f(	-size / 2.0 + xc / (GLfloat)(LinesX-1)*size,
					0.0,
					size / -2.0);
	}
	for (int zc = 0; zc < LinesX; zc++)
	{
		glVertex3f(	size / 2.0,
					0.0,
					-size / 2.0 + zc / (GLfloat)(LinesZ-1)*size);
		glVertex3f(	size / -2.0,
					0.0,
					-size / 2.0 + zc / (GLfloat)(LinesZ-1)*size);
	}
	glEnd();
}


void drawBox() {
	GLfloat size = 2.0;
	GLint LinesX = 30;
	GLint LinesZ = 30;
	GLfloat halfsize = size / 2.0;
	glPushMatrix();
		glTranslatef(0.0,0.8,0.0);

		glScalef(3.0,1.0,3.0);
		

		glColor4f(WHITE);
		glPushMatrix();
			glTranslatef(0.0,-halfsize ,0.0);
			drawNet(size,LinesX,LinesZ);
			glTranslatef(0.0,size,0.0);
			drawNet(size,LinesX,LinesZ);
		glPopMatrix();

		glColor4f(BLUE);
		glPushMatrix();
			glTranslatef(-halfsize,0.0,0.0);	
			glRotatef(90.0,0.0,0.0,halfsize);
			drawNet(size,LinesX,LinesZ);
			glTranslatef(0.0,-size,0.0);
			drawNet(size,LinesX,LinesZ);
		glPopMatrix();

		glColor4f(RED);
		glPushMatrix();
			glTranslatef(0.0,0.0,-halfsize);	
			glRotatef(90.0,halfsize,0.0,0.0);
			drawNet(size,LinesX,LinesZ);
			glTranslatef(0.0,size,0.0);
			drawNet(size,LinesX,LinesZ);
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

void reshape(int newWidth, int newHeight) {
	windowWidth = newWidth;
	windowHeight = newHeight;
	xCenter = (GLfloat)windowWidth/2;
	yCenter = (GLfloat)windowHeight/2;

	if (newWidth == 0 || newHeight == 0) return;  // avoid division by 0

	glMatrixMode(GL_PROJECTION);  
	glLoadIdentity();
	gluPerspective(VIEWING_ANGLE,(GLdouble)newWidth/newHeight, NEAR_CLIPPING_PLANE, FAR_CLIPPING_PLANE); //adapt perspective to new window

	glMatrixMode(GL_MODELVIEW);
	glViewport(0,0,newWidth,newHeight);  // Fit to the new window
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

	drawAxis();
	drawBox();

	glutSwapBuffers();
}

void keyDown(unsigned char key, int x, int y){
	switch (key) {
		case 27:		//ESC
			exit(0);
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

void keyUp(unsigned char key, int x, int y){
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

void mouseMovement(int x, int y){
	if (x==xCenter && y==yCenter) return;
	camera.rotateHorizontal((xCenter - x)/20);
	camera.rotateVertical((yCenter - y)/20);

	glutWarpPointer(xCenter, yCenter);
	glutPostRedisplay();
}

void animate(){
	// This is needed to keep moving while keys are pressed down
	glutPostRedisplay();
}

void initialization(){
	glClearColor(BLACK);
	glEnable(GL_DEPTH_TEST);	//Profundidade
	glShadeModel(GL_SMOOTH);	//Interpolacao de cores

	glEnable(GL_CULL_FACE);		//Faces visiveis
	glCullFace(GL_BACK);		//Mostrar so as da frente

	glutSetCursor(GLUT_CURSOR_NONE);
}

int main(int argc, char **argv){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
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
