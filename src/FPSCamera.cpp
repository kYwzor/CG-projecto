#include "FPSCamera.h"
#include "math.h"
#include <algorithm>
#include <stdio.h>

Boundary boundaries[] = {
	{25, 26, -24, 24},			//parede porta
	{-26, -25, -24, 24},		//parede janela
	{-25, 25, 24, 25},			//parede PC
	{-25, 25, -25, -24},		//parede cama
	{9.5, 24.5, 12.5, 23.5},	//armario
	{15.5, 18.5, 11.5, 12.5},	//macanetas
	{-7.5, 7.5, 14, 24},		//mesa
	{-3, 3, 7.5, 12.5},			//banco
	{-9, 3, -23, -17},			//mesa cabeceira
	{-23.5, -10.5, -22.5, 0.5},	//cama
	{-27, -24, 0, 15}
};

FPSCamera::FPSCamera() {
	// Initializing all variables
	position.x = 0.0;
	position.y = 12; // Start slightly above ground. This never changes.
	position.z = 0.0;

	forward.x = 0.0;
	forward.y = 0.0;
	forward.z = -1.0; // Camera starts facing that direction, so we need to set forward vector accordingly
	
	xAngle = 0.0;
	yAngle = 0.0;

	ignoreCollisions = false;
}

void FPSCamera::rotateHorizontal (GLfloat angle) {
	yAngle += angle;

	if (yAngle > 360)
		yAngle -= 360;
	else if (yAngle < 0)
		yAngle += 360;

	//Rotate direction around Y-axis:
	forward.x = cos((yAngle + 90.0) * RADIAN_MULTIPLIER);
	forward.z = -sin((yAngle + 90.0) * RADIAN_MULTIPLIER);
}

void FPSCamera::rotateVertical (GLfloat angle) {
	xAngle += angle;

	if (xAngle > 90)
		xAngle = 90;
	else if (xAngle < -90)
		xAngle = -90;
}

void FPSCamera::render() {
	// glLoadIdentity always called before
	glRotatef(-xAngle , 1.0, 0.0, 0.0);
	glRotatef(-yAngle , 0.0, 1.0, 0.0);
	glTranslatef( -position.x, -position.y, -position.z );
}

void FPSCamera::move(int direction) {
	int multiplier = 1;

	Vector3 movement;

	if (direction == K_BACK || direction == K_RIGHT)
		multiplier = -1;	//back and right are the negative sides

	if (direction == K_LEFT || direction == K_RIGHT) {
		movement.x = forward.z * MOVE_SPEED * multiplier;
		movement.z = -forward.x * MOVE_SPEED * multiplier;
	}

	else {
		movement.x = forward.x * MOVE_SPEED * multiplier;
		movement.z = forward.z * MOVE_SPEED * multiplier;
	}

	position.x += movement.x;
	//y never changes
	position.z += movement.z;
	if(!ignoreCollisions) avoidCollisions();
}

void FPSCamera::avoidCollisions(){
	for (Boundary b : boundaries){
		GLfloat x1 = position.x - OBSERVER_SIZE;
		GLfloat x2 = position.x + OBSERVER_SIZE;
		GLfloat z1 = position.z - OBSERVER_SIZE;
		GLfloat z2 = position.z + OBSERVER_SIZE;
		if(x1 < b.xMax && x2 > b.xMin && z1 < b.zMax && z2 > b.zMin){
		//if (position.x > b.xMin && position.x < b.xMax && position.z > b.zMin && position.z < b.zMax){
			GLfloat xDeltaMin = b.xMin - x2;
			GLfloat xDeltaMax = b.xMax - x1;
			GLfloat xDelta = std::abs(xDeltaMin) < std::abs(xDeltaMax) ? xDeltaMin : xDeltaMax;

			GLfloat zDeltaMin = b.zMin - z2;
			GLfloat zDeltaMax = b.zMax - z1;
			GLfloat zDelta = std::abs(zDeltaMin) < std::abs(zDeltaMax) ? zDeltaMin : zDeltaMax;

			if (std::abs(xDelta) < std::abs(zDelta)) position.x += xDelta;
			else position.z += zDelta;
		}
	}
}