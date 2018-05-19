/*********************************************
Artur Coutinho - 2014230432
Tiago Gomes - 2015238615

ESC: exit
M: ignore mouse

Movement:
	W : move forwards
	S : move backwards
	A : move left
	D : move right
	mouse: look around

**********************************************/

#include "FPSCamera.h"
#include "RgbImage.h"
#include "materiais.h"
#include <stdio.h>
#define FAR_CLIPPING_PLANE 1000
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
int windowHeight = 480;
GLfloat xCenter = (GLfloat)windowWidth/2;
GLfloat yCenter = (GLfloat)windowHeight/2;

RgbImage img;
GLuint textures[6];



void initLights(){
	GLfloat luzGlobalCor[]={0.1,0.1,0.1,1}; 
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzGlobalCor); //ambiente
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	
	GLfloat lightAmbColor[] = {0.1, 0.1, 0.1, 1};
	
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbColor);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, ones);
	glLightfv(GL_LIGHT0, GL_SPECULAR, ones);
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.006);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION,0);
}

void updateLights(){
	GLfloat lightPos[4] = {0, 20, 0, 1};
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	/*
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 90);
	GLfloat dir[] = {0, -1, 0};
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, dir);
	*/
}

void loadTextures() {   
	glGenTextures(6, textures);

	glBindTexture(GL_TEXTURE_2D, textures[0]);
	img.LoadBmpFile("textures/woodfloor.bmp");
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 
	img.GetNumCols(),
		img.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		img.ImageData());

	glBindTexture(GL_TEXTURE_2D, textures[1]);
	img.LoadBmpFile("textures/wall.bmp");
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 
	img.GetNumCols(),
		img.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		img.ImageData());

	glBindTexture(GL_TEXTURE_2D, textures[2]);
	img.LoadBmpFile("textures/keyboard.bmp");
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 
	img.GetNumCols(),
		img.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		img.ImageData());

	glBindTexture(GL_TEXTURE_2D, textures[3]);
	img.LoadBmpFile("textures/keyboardside.bmp");
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 
	img.GetNumCols(),
		img.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		img.ImageData());

	glBindTexture(GL_TEXTURE_2D, textures[4]);
	img.LoadBmpFile("textures/lightwood.bmp");
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 
	img.GetNumCols(),
		img.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		img.ImageData());

	glBindTexture(GL_TEXTURE_2D, textures[5]);
	img.LoadBmpFile("textures/lightwoodturned.bmp");
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 
	img.GetNumCols(),
		img.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		img.ImageData());
}

void squareMesh(int dimX, int dimY, float repeatS, float repeatT) {
	int i, j;
	/*
	int x = 0;
	GLfloat stuff[6][4] = {BLUE, RED, YELLOW, GREEN, WHITE, BLACK};
	*/
	
	if(!dimX) dimX = 1;
	if(!dimY) dimY = 1;
	glPushMatrix();
		glTranslatef(-0.5,-0.5,0);  // meio do poligono 
		glBegin(GL_QUADS);
			for (i=0;i<dimY;i++) {
				for (j=0;j<dimX;j++) {
					/*
					glColor4fv(stuff[x]);
					x = (x+1) % 6;
					*/
					glTexCoord2f(j*repeatS/dimX, i*repeatT/dimY);
					glVertex3d((float)j/dimX,(float)i/dimY,0);

					glTexCoord2f((j+1)*repeatS/dimX,i*repeatT/dimY);
					glVertex3d((float)(j+1)/dimX,(float)i/dimY,0);

					glTexCoord2f((j+1)*repeatS/dimX,(i+1)*repeatT/dimY);
					glVertex3d((float)(j+1)/dimX,(float)(i+1)/dimY,0);

					glTexCoord2f(j*repeatS/dimX,(i+1)*repeatT/dimY);
					glVertex3d((float)j/dimX,(float)(i+1)/dimY,0);
				}
			}
		glEnd();
	glPopMatrix();
}

void cubeMesh(float scaleX, float scaleY, float scaleZ, float dim, float repeatS, float repeatT, GLuint tex1, GLuint tex2, GLuint tex3){
	int x = scaleX * dim;
	int y = scaleY * dim;
	int z = scaleZ * dim;
	glPushMatrix();
		glScalef(scaleX, scaleY, scaleZ);

		if(tex1){
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, tex1);
		}
		glPushMatrix();
			glTranslatef(0, 0, 0.5);
			squareMesh(x, y, repeatS, repeatT);		//front
			glTranslatef(0, 0, -1);
			glRotatef(180, 0, 1, 0);
			squareMesh(x, y, repeatS, repeatT);		//back
		glPopMatrix();
		glDisable(GL_TEXTURE_2D);

		if(tex2){
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, tex2);
		}
		glPushMatrix();
			glTranslatef(0.5, 0, 0);
			glRotatef(90, 0, 1, 0);	
			squareMesh(z, y, repeatS, repeatT);		//right
		glPopMatrix();
		glPushMatrix();
			glTranslatef(-0.5, 0, 0);
			glRotatef(-90, 0, 1, 0);
			squareMesh(z, y, repeatS, repeatT);		//left
		glPopMatrix();
		glDisable(GL_TEXTURE_2D);

		if(tex3){
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, tex3);
		}
		glPushMatrix();
			glTranslatef(0, -0.5, 0);
			glRotatef(90, 1, 0, 0);
			squareMesh(x, z, repeatS, repeatT);		//bottom
		glPopMatrix();
		glPushMatrix();
			glTranslatef(0, 0.5, 0);
			glRotatef(-90, 1, 0, 0);
			squareMesh(x, z, repeatS, repeatT);		//top
		glPopMatrix();
		glDisable(GL_TEXTURE_2D);
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
	GLfloat brown[] = {0.423, 0.3, 0.234, 1};
	GLfloat sh = 0.8 *128;
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, brown);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, ones);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &sh);
	cubeMesh(50, 1, 48, 0.5, 4, 4, textures[0], textures[0], textures[0]); //chao

	sh = 0.8 * 128;
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, ones);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, ones);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &sh);
	glPushMatrix();
		glTranslatef(0, 29, 0);
		cubeMesh(50, 1, 48, 0.5, 8, 8, textures[1], textures[1], textures[1]); //teto
	glPopMatrix();
	glPushMatrix();
		glTranslatef(25.5, 14.5, 0);
		cubeMesh(1, 30, 50, 0.5, 8, 8, textures[1], textures[1], textures[1]); //parede da porta
		glTranslatef(-51, 0, 0);
		cubeMesh(1, 30, 50, 0.5, 8, 8, textures[1], textures[1], textures[1]); //parede da janela
	glPopMatrix();
	glPushMatrix();
		glTranslatef(0, 14.5, 24.5);
		cubeMesh(50, 30, 1, 0.5, 8, 8, textures[1], textures[1], textures[1]);	//parede do computador
		glTranslatef(0, 0, -49);
		cubeMesh(50, 30, 1, 0.5, 8, 8, textures[1], textures[1], textures[1]);	//parede da cabeceira
	glPopMatrix();
}

void drawWardrobe() {
	glColor4f(YELLOW);
	glPushMatrix();
		glTranslatef(24, 10.5, 18);
		glScalef(1, 18, 11);
		glutWireCube(1);	//lado esquerdo
		glTranslatef(-14, 0, 0);
		glutWireCube(1);	//lado direito
	glPopMatrix();
	glPushMatrix();
		glTranslatef(17, 1, 18);
		glScalef(15, 1, 11);
		glutWireCube(1);	//baixo
		glTranslatef(0, 19, 0);
		glutWireCube(1);	//teto
	glPopMatrix();
	glPushMatrix();
		glTranslatef(17, 10.5, 23);
		glScalef(13, 18, 1);
		glutWireCube(1);	//tras
	glPopMatrix();
	glPushMatrix();
		glTranslatef(20.25, 10.5, 13);
		glScalef(6.5, 18, 1);
		glutWireCube(1);	//porta esquerda
	glPopMatrix();
	glPushMatrix();
		glTranslatef(13.75, 10.5, 13);
		glScalef(6.5, 18, 1);
		glutWireCube(1);	//porta direita
	glPopMatrix();
	glPushMatrix();
		glTranslatef(18, 9, 12);
		glutWireSphere(0.5, 6, 6);	//macaneta esquerda
		glTranslatef(-2, 0, 0);
		glutWireSphere(0.5, 6, 6);	//macaneta direita
	glPopMatrix();
}

void drawNightstand() {
	glColor4f(RED);
	glPushMatrix();
		glTranslatef(-4, 3.5, -20);
		glScalef(10, 6, 6);
		glutWireCube(1);
	glPopMatrix();

	glColor4f(GREEN);
	glPushMatrix();
		glTranslatef(-1.5, 6.5, -20);
		glRotatef(-90, 1, 0, 0);
		GLUquadric* quad = gluNewQuadric();
		gluCylinder(quad, 0.5, 0.5, 1.5, 8, 8);	//lata
		//glRotatef(180, 1, 0, 0);
		//gluDisk(quad, 0, 0.5, 8, 1);
		//glRotatef(180, 1, 0, 0);
		glTranslatef(0, 0, 1.5);
		gluDisk(quad, 0, 0.5, 8, 1);		//parte de cima
		gluDeleteQuadric(quad);
	glPopMatrix();
}

void drawBed(){
	glColor4f(YELLOW);
	glPushMatrix();
		//pernas da cama
		glTranslatef(-23, 2.5, -22);
		glScalef(1, 4, 1);
		glutWireCube(1);
		glTranslatef(12, 0, 0);
		glutWireCube(1);
		glTranslatef(0, 0, 22);
		glutWireCube(1);
		glTranslatef(-12, 0, 0);
		glutWireCube(1);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(-17, 5, -11);
		glScalef(13, 1, 23);
		glutWireCube(1);	//armacao
	glPopMatrix();
	glPushMatrix();
		glTranslatef(-17, 9.5, -22);
		glScalef(13, 8, 1);
		glutWireCube(1);	//cabeceira
	glPopMatrix();
	glPushMatrix();
		glTranslatef(-17, 6.5, -10.75);
		glScalef(12, 1.5, 21.5);
		glutWireCube(1);	//colchao
	glPopMatrix();
}

void drawSeat(){
	glColor4f(RED);
	glPushMatrix();
		//pernas da cadeira
		glTranslatef(-3, 2.5, 12);
		glScalef(1, 4, 1);
		glutWireCube(1);
		glTranslatef(6, 0, 0);
		glutWireCube(1);
		glTranslatef(0, 0, -4);
		glutWireCube(1);
		glTranslatef(-6, 0, 0);
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
	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	GLfloat brown[] = {0.423, 0.3, 0.234, 1};
	GLfloat sh = 0.4 *128;
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, brown);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, ones);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &sh);
	glBindTexture(GL_TEXTURE_2D, textures[4]);
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	glScalef(0.25, 1, 1);
	glMatrixMode(GL_MODELVIEW);
	GLUquadric* quad = gluNewQuadric();
	gluQuadricTexture(quad, GLU_TRUE);
	glPushMatrix();
		glTranslatef(-5, 0, 22.5);
		glRotatef(-90, 1, 0, 0);
		gluCylinder(quad, 0.5, 0.5, 7.5 , 8, 8);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(5, 0, 22.5);
		glRotatef(-90, 1, 0, 0);
		gluCylinder(quad, 0.5, 0.5, 7.5, 8, 8);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(5, 0, 15.5);
		glRotatef(-90, 1, 0, 0);
		gluCylinder(quad, 0.5, 0.5, 7.5, 8, 8);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(-5, 0, 15.5);
		glRotatef(-90, 1, 0, 0);
		gluCylinder(quad, 0.5, 0.5, 7.5, 8, 8);
	glPopMatrix();
	gluDeleteQuadric(quad);

	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	glRotatef(-90, 0, 0, 1);
	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();
		glTranslatef(0, 8, 19);
		//glScalef(15, 1, 10);
		//glutWireCube(1);	//tampo
		cubeMesh(15, 1, 10, 1, 1, 1, textures[5], textures[5], textures[4]);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
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

	glEnable(GL_LIGHTING);
	
	GLfloat sh = 0.8 * 128;
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, ones);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, ones);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &sh);
	glPushMatrix();
		glTranslatef(2, 8.625, 16);
		glRotatef(180, 0, 1, 0);
		cubeMesh(5, 0.25, 2, 1, 1, 1, textures[3], textures[3], textures[2]); //teclado
	glPopMatrix();
	
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, blackPlasticAmb);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, blackPlasticDif);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, blackPlasticSpec);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &blackPlasticCoef);
	glPushMatrix();
		glTranslatef(2, 8.6, 19.375);
		cubeMesh(0.05, 0.05, 4.75, 1, 1, 1, 0, 0, 0); //fio teclado
	glPopMatrix();
	glDisable(GL_LIGHTING);
	glPushMatrix();
		glTranslatef(-3, 8.75, 17);
		glScalef(1, 0.5, 1.5);
		glutWireCube(1);	//rato
	glPopMatrix();
	glPushMatrix();
		glTranslatef(-3, 8.6, 19.75);
		glScalef(0.05, 0.05, 4);
		glutWireCube(1);	//fio rato
	glPopMatrix();
	glPushMatrix();
		glTranslatef(6, 9.5, 18);
		//glutWireTeapot(1);	//TODO: Chavena cilindrica
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
	updateLights();
	drawAxis();
	glEnable(GL_LIGHTING);
	drawWalls();
	glDisable(GL_LIGHTING);
	drawWardrobe();
	drawNightstand();
	drawBed();
	drawSeat();
	drawTable();
	drawObjects();

	/*
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	*/

	glPushMatrix();
		glTranslatef(0, -10, 10);
		//glutWireCube(1);
	glPopMatrix();
	//cubeMesh(40, 1, 40, 1, textures[1], 4, 4);

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
				glutSetCursor(GLUT_CURSOR_INHERIT);
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

	glEnable(GL_NORMALIZE);
	initLights();
	glEnable(GL_LIGHT0);

	loadTextures();
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(windowWidth,windowHeight);
	glutCreateWindow("Meta 1 CG");

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
