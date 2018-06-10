/*********************************************
Artur Coutinho - 2014230432
Tiago Gomes - 2015238615

ESC: exit
M: ignore mouse
N: cycle through day/night
J: reboot computer WIP
K: control candle flame
L: control ceiling light

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
#include "math.h"
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
int windowHeight = 480;
GLfloat xCenter = (GLfloat)windowWidth/2;
GLfloat yCenter = (GLfloat)windowHeight/2;

RgbImage img;
GLuint textures[17];

GLuint skybox[12];
int skyboxoffset = 6;
bool day = false;

GLuint flame[34];
int currentflame = 0;

bool ceilingLamp = true;
bool candleFlame = true;

void initLights(){
	GLfloat luzGlobalCor[]={0,0,0,1}; 
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzGlobalCor); //ambiente
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	
	glLightfv(GL_LIGHT0, GL_AMBIENT, ones);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, ones);
	glLightfv(GL_LIGHT0, GL_SPECULAR, ones);
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.2);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.2);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0);
	glEnable(GL_LIGHT0);	//luz teto

	GLfloat blue[] = {0, 0, 1, 1};
	glLightfv(GL_LIGHT1, GL_AMBIENT, blue);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, blue);
	glLightfv(GL_LIGHT1, GL_SPECULAR, blue);
	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1);
	glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0);
	glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.02);
	glEnable(GL_LIGHT1);	//luz computador

	GLfloat yellowish[] = {1, 0.7, 0, 1};
	glLightfv(GL_LIGHT2, GL_AMBIENT, yellowish);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, yellowish);
	glLightfv(GL_LIGHT2, GL_SPECULAR, yellowish);
	glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, 1.4);
	glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, 0);
	glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, 0.02);
	glEnable(GL_LIGHT2);	//luz vela

	glEnable(GL_LIGHTING);
}

void updateLights(){
	GLfloat light0Pos[4] = {0, 21, 0, 1};
	glLightfv(GL_LIGHT0, GL_POSITION, light0Pos);
	
	GLfloat light1Pos[4] = {0, 13, 22.5, 1};
	glLightfv(GL_LIGHT1, GL_POSITION, light1Pos);
	GLfloat dir[] = {0, 0, -1};
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, dir);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 80);

	GLfloat light2Pos[4] = {-6, 10.5, -20, 1};
	glLightfv(GL_LIGHT2, GL_POSITION, light2Pos);
}

void loadTextures() {
	glGenTextures(17, textures);

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
	img.LoadBmpFile("textures/tabletop.bmp");
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 
	img.GetNumCols(),
		img.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		img.ImageData());

	glBindTexture(GL_TEXTURE_2D, textures[5]);
	img.LoadBmpFile("textures/tableside.bmp");
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 
	img.GetNumCols(),
		img.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		img.ImageData());

	glBindTexture(GL_TEXTURE_2D, textures[6]);
	img.LoadBmpFile("textures/mattress.bmp");
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 
	img.GetNumCols(),
		img.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		img.ImageData());

	glBindTexture(GL_TEXTURE_2D, textures[7]);
	img.LoadBmpFile("textures/box.bmp");
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 
	img.GetNumCols(),
		img.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		img.ImageData());

	glBindTexture(GL_TEXTURE_2D, textures[8]);
	img.LoadBmpFile("textures/can.bmp");
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 
	img.GetNumCols(),
		img.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		img.ImageData());

	glBindTexture(GL_TEXTURE_2D, textures[9]);
	img.LoadBmpFile("textures/screen.bmp");
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 
	img.GetNumCols(),
		img.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		img.ImageData());

	glBindTexture(GL_TEXTURE_2D, textures[10]);
	img.LoadBmpFile("textures/screenside.bmp");
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 
	img.GetNumCols(),
		img.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		img.ImageData());

	glBindTexture(GL_TEXTURE_2D, textures[11]);
	img.LoadBmpFile("textures/cubemouse.bmp");
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 
	img.GetNumCols(),
		img.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		img.ImageData());

	glBindTexture(GL_TEXTURE_2D, textures[12]);
	img.LoadBmpFile("textures/cubemouseside.bmp");
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 
	img.GetNumCols(),
		img.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		img.ImageData());

	glBindTexture(GL_TEXTURE_2D, textures[13]);
	img.LoadBmpFile("textures/wardrobewood.bmp");
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 
	img.GetNumCols(),
		img.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		img.ImageData());

	glBindTexture(GL_TEXTURE_2D, textures[14]);
	img.LoadBmpFile("textures/wardrobeframe.bmp");
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 
	img.GetNumCols(),
		img.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		img.ImageData());

	glBindTexture(GL_TEXTURE_2D, textures[15]);
	img.LoadBmpFile("textures/lampshade.bmp");
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 
	img.GetNumCols(),
		img.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		img.ImageData());

	glBindTexture(GL_TEXTURE_2D, textures[16]);
	img.LoadBmpFile("textures/wax.bmp");
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

void loadSkybox() {
	glGenTextures(12, skybox);

	glBindTexture(GL_TEXTURE_2D, skybox[0]);
	img.LoadBmpFile("skybox/frontD.bmp");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 
	img.GetNumCols(),
		img.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		img.ImageData());

	glBindTexture(GL_TEXTURE_2D, skybox[1]);
	img.LoadBmpFile("skybox/rightD.bmp");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 
	img.GetNumCols(),
		img.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		img.ImageData());

	glBindTexture(GL_TEXTURE_2D, skybox[2]);
	img.LoadBmpFile("skybox/backD.bmp");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 
	img.GetNumCols(),
		img.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		img.ImageData());

	glBindTexture(GL_TEXTURE_2D, skybox[3]);
	img.LoadBmpFile("skybox/leftD.bmp");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 
	img.GetNumCols(),
		img.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		img.ImageData());

	glBindTexture(GL_TEXTURE_2D, skybox[4]);
	img.LoadBmpFile("skybox/topD.bmp");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 
	img.GetNumCols(),
		img.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		img.ImageData());

	glBindTexture(GL_TEXTURE_2D, skybox[5]);
	img.LoadBmpFile("skybox/bottomD.bmp");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 
	img.GetNumCols(),
		img.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		img.ImageData());

	glBindTexture(GL_TEXTURE_2D, skybox[6]);
	img.LoadBmpFile("skybox/frontN.bmp");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 
	img.GetNumCols(),
		img.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		img.ImageData());

	glBindTexture(GL_TEXTURE_2D, skybox[7]);
	img.LoadBmpFile("skybox/rightN.bmp");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 
	img.GetNumCols(),
		img.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		img.ImageData());

	glBindTexture(GL_TEXTURE_2D, skybox[8]);
	img.LoadBmpFile("skybox/backN.bmp");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 
	img.GetNumCols(),
		img.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		img.ImageData());

	glBindTexture(GL_TEXTURE_2D, skybox[9]);
	img.LoadBmpFile("skybox/leftN.bmp");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 
	img.GetNumCols(),
		img.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		img.ImageData());

	glBindTexture(GL_TEXTURE_2D, skybox[10]);
	img.LoadBmpFile("skybox/topN.bmp");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 
	img.GetNumCols(),
		img.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		img.ImageData());

	glBindTexture(GL_TEXTURE_2D, skybox[11]);
	img.LoadBmpFile("skybox/bottomN.bmp");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 
	img.GetNumCols(),
		img.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		img.ImageData());
}

void loadFlame() {
	glGenTextures(34, flame);
	char aux[32];
	for (int i=0; i<34; i++){
		glBindTexture(GL_TEXTURE_2D, flame[i]);
		sprintf(aux, "flame/_%04d_flame.bmp", i);
		img.LoadBmpFile(aux);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, 
		img.GetNumCols(),
			img.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
			img.ImageData());
	}
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

		glNormal3f(0, 0, 1);
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
	glDisable(GL_LIGHTING);
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
	glEnable(GL_LIGHTING);
}

void drawWalls() {
	GLfloat brown[] = {0.423, 0.3, 0.234, 1};
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, brown);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, ones);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.8 *128);
	cubeMesh(50, 1, 48, 0.5, 4, 4, textures[0], textures[0], textures[0]); //chao

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, ones);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, ones);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.3 * 128);
	glPushMatrix();
		glTranslatef(0, 29, 0);
		cubeMesh(50, 1, 48, 0.5, 3, 3, textures[1], textures[1], textures[1]); //teto
	glPopMatrix();
	glPushMatrix();
		glTranslatef(25.5, 14.5, 0);
		cubeMesh(1, 30, 50, 0.5, 3, 3, textures[1], textures[1], textures[1]); //parede da porta
		//glTranslatef(-51, 0, 0);
		//cubeMesh(1, 30, 50, 0.5, 3, 3, textures[1], textures[1], textures[1]); //parede da janela
	glPopMatrix();
	
	// parede com buraco para janela
	glPushMatrix();
		glTranslatef(-25.5, 14.5, 20);
		cubeMesh(1, 30, 10, 0.5, 1, 3, textures[1], textures[1], textures[1]);		//esquerda da janela
	glPopMatrix();
	glPushMatrix();
		glTranslatef(-25.5, 25.75, 7.5);
		cubeMesh(1, 7.5, 15, 0.5, 0.9, 0.75, textures[1], textures[1], textures[1]);	//cima da janela
	glPopMatrix();
		glPushMatrix();
		glTranslatef(-25.5, 3.25, 7.5);
		cubeMesh(1, 7.5, 15, 0.5, 0.9, 0.75, textures[1], textures[1], textures[1]);	//baixo da janela
	glPopMatrix();
	glPushMatrix();
		glTranslatef(-25.5, 14.5, -12.5);
		cubeMesh(1, 30, 25, 0.5, 1.5, 3, textures[1], textures[1], textures[1]);	//direita da janela
	glPopMatrix();

	glPushMatrix();
		glTranslatef(0, 14.5, 24.5);
		cubeMesh(50, 30, 1, 0.5, 3, 3, textures[1], textures[1], textures[1]);	//parede do computador
		glTranslatef(0, 0, -49);
		cubeMesh(50, 30, 1, 0.5, 3, 3, textures[1], textures[1], textures[1]);	//parede da cabeceira
	glPopMatrix();

	//janela
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, halves);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, halves);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.3 * 128);
	glPushMatrix();
		glTranslatef(-25.5, 14.5, 14.5);
		cubeMesh(3, 15, 1, 1, 1, 1, 0, 0, 0);	//armacao esquerda
		glTranslatef(0, 0, -14);
		cubeMesh(3, 15, 1, 1, 1, 1, 0, 0, 0);	//armacao direita
	glPopMatrix();
	glPushMatrix();
		glTranslatef(-25.5, 18.5, 7.5);
		cubeMesh(2, 7, 1, 1, 1, 1, 0, 0, 0);	//armacao meio cima
	glPopMatrix();
	glPushMatrix();
		glTranslatef(-25.5, 10.5, 7.5);
		cubeMesh(2, 7, 1, 1, 1, 1, 0, 0, 0);	//armacao meio baixo
	glPopMatrix();
	glPushMatrix();
		glTranslatef(-25.5, 21.5, 7.5);
		cubeMesh(3, 1, 13, 1, 1, 1, 0, 0, 0);	//armacao cima
		glTranslatef(0, -7, 0);
		cubeMesh(2, 1, 13, 1, 1, 1, 0, 0, 0);	//armacao meio
		glTranslatef(0, -7, 0);
		cubeMesh(3, 1, 13, 1, 1, 1, 0, 0, 0);	//armacao baixo
	glPopMatrix();
}

void drawWardrobe() {
	GLfloat brown[] = {0.846, 0.6, 0.468, 1};
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, brown);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, ones);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.4 *128);
	glPushMatrix();
		glTranslatef(24, 10.5, 18);
		cubeMesh(1, 18, 11, 1, 1, 1, textures[14], textures[13], textures[14]);	//lado esquerdo
		glTranslatef(-14, 0, 0);
		cubeMesh(1, 18, 11, 1, 1, 1, textures[14], textures[13], textures[14]);	//lado direito
	glPopMatrix();
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	glRotatef(90, 0, 0, 1);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
		glTranslatef(17, 1, 18);
		cubeMesh(15, 1, 11, 1, 1, 1, textures[14], textures[14], textures[13]);	//baixo
		glTranslatef(0, 19, 0);
		cubeMesh(15, 1, 11, 1, 1, 1, textures[14], textures[14], textures[13]);	//teto
	glPopMatrix();
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
		glTranslatef(17, 10.5, 23);
		cubeMesh(13, 18, 1, 1, 1, 1, textures[13], textures[14], textures[14]);	//tras
	glPopMatrix();
	glPushMatrix();
		glTranslatef(20.35, 10.5, 13);
		cubeMesh(6.3, 18, 1, 1, 1, 1, textures[13], textures[14], textures[14]);	//porta esquerda
	glPopMatrix();
	glPushMatrix();
		glTranslatef(13.65, 10.5, 13);
		cubeMesh(6.3, 18, 1, 1, 1, 1, textures[13], textures[14], textures[14]);	//porta direita
	glPopMatrix();
	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[14]);
	GLUquadric* quad = gluNewQuadric();
	gluQuadricTexture(quad, GLU_TRUE);
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	glRotatef(90, 0, 0, 1);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
		glTranslatef(18, 10.5, 12);
		gluSphere(quad, 0.5, 10, 10);	//macaneta esquerda
		glTranslatef(-2, 0, 0);
		gluSphere(quad, 0.5, 10, 10);	//macaneta direita
	glPopMatrix();
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	gluDeleteQuadric(quad);
	glDisable(GL_TEXTURE_2D);
}

void drawBed(){
	GLfloat brown[] = {0.423, 0.3, 0.234, 1};
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, brown);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, ones);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.4 *128);
	glPushMatrix();
		//pernas da cama
		glTranslatef(-23, 2.5, -22);
		cubeMesh(1, 4, 1, 1, 1, 1, textures[5], textures[5], textures[5]);
		glTranslatef(12, 0, 0);
		cubeMesh(1, 4, 1, 1, 1, 1, textures[5], textures[5], textures[5]);
		glTranslatef(0, 0, 22);
		cubeMesh(1, 4, 1, 1, 1, 1, textures[5], textures[5], textures[5]);
		glTranslatef(-12, 0, 0);
		cubeMesh(1, 4, 1, 1, 1, 1, textures[5], textures[5], textures[5]);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(-17, 5, -11);
		cubeMesh(13, 1, 23, 1, 1, 1, textures[5], textures[5], textures[5]);	//armacao
	glPopMatrix();
	glPushMatrix();
		glTranslatef(-17, 9.5, -22);
		cubeMesh(13, 8, 1, 1, 1, 1, textures[5], textures[5], textures[5]);	//cabeceira

	glPopMatrix();
	
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, ones);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, ones);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.1 * 128);
	glPushMatrix();
		glTranslatef(-17, 6.5, -10.75);
		cubeMesh(12, 1.5, 21.5, 1, 2, 2, textures[6], textures[6], textures[6]);	//colchao
	glPopMatrix();
}

void drawSeat(){
	GLfloat brown[] = {0.846, 0.6, 0.468, 1};
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, brown);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, ones);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.4 *128);

	glPushMatrix();
		//pernas da cadeira
		glTranslatef(-2.5, 2.5, 12);
		//glScalef(1, 4, 1);
		cubeMesh(1, 4, 1, 1, 1, 1, textures[13], textures[13], textures[13]);
		glTranslatef(5, 0, 0);
		cubeMesh(1, 4, 1, 1, 1, 1, textures[13], textures[13], textures[13]);
		glTranslatef(0, 0, -4);
		cubeMesh(1, 4, 1, 1, 1, 1, textures[13], textures[13], textures[13]);
		glTranslatef(-5, 0, 0);
		cubeMesh(1, 4, 1, 1, 1, 1, textures[13], textures[13], textures[13]);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(0, 4.875, 10);
		cubeMesh(6, 0.75, 5, 1, 1, 1, textures[13], textures[13], textures[13]);
	glPopMatrix();
}

void drawTable(){
	//pernas
	glEnable(GL_TEXTURE_2D);
	GLfloat brown[] = {0.846, 0.6, 0.468, 1};
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, brown);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, ones);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.4 *128);
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
		cubeMesh(15, 1, 10, 1, 1, 1, textures[5], textures[5], textures[4]); //tampo
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
}

void drawObjects(){
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, ones);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, ones);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.8 * 128);
	glPushMatrix();
		glTranslatef(0, 13, 22);
		cubeMesh(10, 7, 1, 1, 1, 1, textures[9], textures[10], textures[10]); //monitor
	glPopMatrix();
	glPushMatrix();
		glTranslatef(0, 13, 22.51);
		glScalef(10, 7, 1);
		glEnable(GL_TEXTURE_2D);
		squareMesh(10, 7, 20, 1);	//back
		glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(0, 9, 22);
		cubeMesh(7, 1, 0.5, 1, 10, 10, textures[10], textures[10], textures[10]);
	glPopMatrix();
	
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, ones);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, ones);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.8 * 128);
	glPushMatrix();
		glTranslatef(2, 8.625, 16);
		glRotatef(180, 0, 1, 0);
		cubeMesh(5, 0.25, 2, 1, 1, 1, textures[3], textures[3], textures[2]); //teclado
	glPopMatrix();
	
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, blackPlasticAmb);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, blackPlasticDif);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, blackPlasticSpec);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, blackPlasticCoef);
	glPushMatrix();
		glTranslatef(2, 8.6, 19.375);
		cubeMesh(0.05, 0.05, 4.75, 1, 1, 1, 0, 0, 0); //fio teclado
	glPopMatrix();
	glPushMatrix();
		glTranslatef(-3, 8.6, 19.75);
		cubeMesh(0.05, 0.05, 4, 1, 1, 1, 0, 0, 0); //fio rato
	glPopMatrix();
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, ones);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, ones);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.5 * 128);
	glPushMatrix();
		glTranslatef(-3, 8.625, 17);
		glRotatef(180, 0 , 1, 0);
		cubeMesh(1, 0.25, 1.5, 1, 1, 1, textures[12], textures[12], textures[11]); //rato
	glPopMatrix();

	//glEnable(GL_TEXTURE_2D);
	//glBindTexture(GL_TEXTURE_2D, textures[15]);
	GLUquadric* quad = gluNewQuadric();
	//gluQuadricTexture(quad, GLU_TRUE);
	glPushMatrix();
		//glTranslatef(0, 4.875, 10);
		//cubeMesh(6, 0.75, 5, 1, 1, 1, textures[13], textures[13], textures[13]);	
		//glTranslatef(6, 8.75, 18);
		glTranslatef(2, 5.26, 10);
		glRotatef(-90, 1, 0, 0);
		
		gluCylinder(quad, 0.75, 0.75, 1.5, 12, 16);
		gluQuadricOrientation(quad, GLU_INSIDE);
		gluCylinder(quad, 0.6, 0.6, 1.5, 12, 16);
		gluQuadricOrientation(quad, GLU_OUTSIDE);
		gluDisk(quad, 0, 0.6, 12, 1);
		glTranslatef(0, 0, 1.5);
		gluDisk(quad, 0.6, 0.75, 12, 1);
	glPopMatrix();
	//glDisable(GL_TEXTURE_2D);
	gluDeleteQuadric(quad);
}

void drawCeilingLamp(){
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, ones);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, ones);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 128);

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, blackPlasticAmb);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, blackPlasticDif);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, blackPlasticSpec);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, blackPlasticCoef);
	glPushMatrix();
		glTranslatef(0, 25, 0);
		glScalef(0.1, 7, 0.1);
		glutSolidCube(1);	//fio
	glPopMatrix();

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, halves);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, halves);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.5 * 128);
	glPushMatrix();
		glTranslatef(0, 21.5, 0);
		glScalef(3.9, 0.1, 0.1);
		glutSolidCube(1);	//armacao
	glPopMatrix();
	glPushMatrix();
		glTranslatef(0, 21.5, 0);
		glScalef(0.1, 0.1, 3.9);
		glutSolidCube(1);	//armacao
	glPopMatrix();

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[15]);
	GLUquadric* quad = gluNewQuadric();
	gluQuadricTexture(quad, GLU_TRUE);
	glPushMatrix();
		glTranslatef(0, 20, 0);
		glRotatef(-90, 1, 0, 0);
		
		gluCylinder(quad, 2, 2, 2, 12, 16);
		gluQuadricOrientation(quad, GLU_INSIDE);
		gluCylinder(quad, 2, 2, 2, 12, 16);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	gluDeleteQuadric(quad);

	quad = gluNewQuadric();
	glPushMatrix();
		glTranslatef(0, 21, 0);
		glScalef(0.6, 1, 0.6);
		gluSphere(quad, 0.5, 10, 10);	//lampada
	glPopMatrix();
	gluDeleteQuadric(quad);
}

void drawSkybox(){
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glColor4f(WHITE);
	glPushMatrix();
		glScalef(10000, 10000, 10000);		
		glNormal3f(0, 0, 1);

		glBindTexture(GL_TEXTURE_2D, skybox[0 + skyboxoffset]);
		glPushMatrix();
			glTranslatef(0, 0, 0.5);
			glRotatef(180, 0, 1, 0);
			squareMesh(1, 1, 1, 1);		//front	
		glPopMatrix();

		glBindTexture(GL_TEXTURE_2D, skybox[1 + skyboxoffset]);
		glPushMatrix();
			glTranslatef(0.5, 0, 0);
			glRotatef(-90, 0, 1, 0);
			squareMesh(1, 1, 1, 1);		//right
		glPopMatrix();

		glBindTexture(GL_TEXTURE_2D, skybox[2 + skyboxoffset]);
		glPushMatrix();
			glTranslatef(0, 0, -0.5);
			squareMesh(1, 1, 1, 1);		//back
		glPopMatrix();

		glBindTexture(GL_TEXTURE_2D, skybox[3 + skyboxoffset]);
		glPushMatrix();
			glTranslatef(-0.5, 0, 0);
			glRotatef(90, 0, 1, 0);
			squareMesh(1, 1, 1, 1);		//left
		glPopMatrix();

		glBindTexture(GL_TEXTURE_2D, skybox[4 + skyboxoffset]);
		glPushMatrix();
			glTranslatef(0, 0.5, 0);
			glRotatef(90, 1, 0, 0);
			squareMesh(1, 1, 1, 1);		//top
		glPopMatrix();

		glBindTexture(GL_TEXTURE_2D, skybox[5 + skyboxoffset]);
		glPushMatrix();
			glTranslatef(0, -0.5, 0);
			glRotatef(-90, 1, 0, 0);
			squareMesh(1, 1, 1, 1);		//bottom
		glPopMatrix();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
}

void drawNightstand() {
	GLfloat brown[] = {0.6, 0.4, 0.3, 1};
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, brown);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, ones);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.4 *128);
	
	glPushMatrix();
		glTranslatef(-4, 3.5, -20);
		cubeMesh(10, 6, 6, 1, 1, 1, textures[7], textures[7], textures[7]);	//mesa de cabeceira
	glPopMatrix();

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, halves);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, halves);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.8 * 128);
	GLUquadric* quad = gluNewQuadric();
	glPushMatrix();
		glTranslatef(-6, 6.5, -20);
		glRotatef(-90, 1, 0, 0);
		gluCylinder(quad, 1, 1, 0.5, 12, 12);	//base da vela
		glTranslatef(0, 0, 0.5);
		gluDisk(quad, 0, 1, 12, 1);		//parte de cima
	glPopMatrix();
	gluDeleteQuadric(quad);

		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, ones);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, ones);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 128);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[16]);
	quad = gluNewQuadric();
	gluQuadricTexture(quad, GLU_TRUE);
	glPushMatrix();
		glTranslatef(-6, 7, -20);
		glRotatef(-90, 1, 0, 0);
		gluCylinder(quad, 0.2, 0.15, 3, 12, 12);	//vela
		glTranslatef(0, 0, 3);
		gluDisk(quad, 0, 0.15, 12, 1);		//parte de cima
	glPopMatrix();
	gluDeleteQuadric(quad);

	glPushMatrix();
		glTranslatef(-6, 10.15, -20);
		glScalef(0.05, 0.3, 0.05);
		glutSolidCube(1);	// rastilho
	glPopMatrix();

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, ones);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, ones);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.9 * 128);
	glBindTexture(GL_TEXTURE_2D, textures[8]);
	quad = gluNewQuadric();
	gluQuadricTexture(quad, GLU_TRUE);
	glPushMatrix();
		glTranslatef(-1.5, 6.5, -20);
		glRotatef(-90, 1, 0, 0);
		gluCylinder(quad, 0.5, 0.5, 1.5, 12, 12);	//lata
		//glRotatef(180, 1, 0, 0);
		//gluDisk(quad, 0, 0.5, 8, 1);
		//glRotatef(180, 1, 0, 0);

		glDisable(GL_TEXTURE_2D);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, halves);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, halves);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.8 * 128);
		glTranslatef(0, 0, 1.5);
		gluDisk(quad, 0, 0.5, 12, 1);		//parte de cima
	glPopMatrix();
	gluDeleteQuadric(quad);

	if(candleFlame){
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		glBindTexture(GL_TEXTURE_2D, flame[currentflame]);
		currentflame = (currentflame + 1) % 34;
		glPushMatrix();
			glTranslatef(-6, 11, -20);
			GLfloat angle = atan2(camera.position.z + 20, camera.position.x + 6) / DEGREE_DIVISION;
			glRotatef(90 - angle, 0, 1, 0);
			glBegin(GL_QUADS);								//chama da vela
				glTexCoord2d(0,0); glVertex3f(-1, -1, 0);
				glTexCoord2d(1,0); glVertex3f( 1, -1, 0);
				glTexCoord2d(1,1); glVertex3f( 1,  1, 0);
				glTexCoord2d(0,1); glVertex3f(-1,  1, 0);
			glEnd();
		glPopMatrix();
		glDisable(GL_BLEND);
		glDisable(GL_TEXTURE_2D);
	}
}

void drawGlass() {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glPushMatrix();
		glTranslatef(-25.5 , 18, 11);
		glScalef(0.25, 6, 6);
		glutSolidCube(1);	//vidro cima esquerda
	glPopMatrix();
	glPushMatrix();
		glTranslatef(-25.5 , 18, 4);
		glScalef(0.25, 6, 6);
		glutSolidCube(1);	//vidro cima direita
	glPopMatrix();
	glPushMatrix();
		glTranslatef(-25.5 , 11, 11);
		glScalef(0.25, 6, 6);
		glutSolidCube(1);	//vidro baixo esquerda
	glPopMatrix();
	glPushMatrix();
		glTranslatef(-25.5 , 11, 4);
		glScalef(0.25, 6, 6);
		glutSolidCube(1);	//vidro baixo direita
	glPopMatrix();
	glDisable(GL_BLEND);
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
	drawAxis();;
	drawWalls();
	drawWardrobe();
	drawBed();
	drawSeat();
	drawTable();
	drawObjects();
	drawCeilingLamp();
	drawSkybox();
	drawNightstand();
	drawGlass();

	/*
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	*/

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
		case 'N':
		case 'n':
			day = !day;
			if (day){
				GLfloat luzGlobalCor[]={0.3, 0.3, 0.3, 1}; 
				glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzGlobalCor);
				skyboxoffset = 0;
			}
			else{
				GLfloat luzGlobalCor[]={0, 0, 0, 1}; 
				glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzGlobalCor);
				skyboxoffset = 6;
			}
			break;
		case 'J':
		case 'j':
			// computer
			break;
		case 'K':
		case 'k':
			candleFlame = !candleFlame;
			if (candleFlame) glEnable(GL_LIGHT2);
			else glDisable(GL_LIGHT2);
			break;
		case 'L':
		case 'l':
			ceilingLamp = !ceilingLamp;
			if (ceilingLamp) glEnable(GL_LIGHT0);
			else glDisable(GL_LIGHT0);
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

	loadTextures();
	loadSkybox();
	loadFlame();
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(windowWidth,windowHeight);
	glutCreateWindow("Projeto CG");

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
