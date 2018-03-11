#include "FPSCamera.h"
#include "math.h"

FPSCamera::FPSCamera() {
	// Initializing all variables
	position.x = 0.0;
	position.y = 0.4; // Start slightly above ground. This never changes.
	position.z = 0.0;

	forward.x = 0.0;
	forward.y = 0.0;
	forward.z = -1.0; // Camera starts facing that direction, so we need to set forward vector accordingly
	
	xAngle = 0.0;
	yAngle = 0.0;
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
	movement.y = 0.0;

	if (direction == K_BACK || direction == K_RIGHT)
		multiplier = -1;

	if (direction == K_LEFT || direction == K_RIGHT) {
		movement.x = forward.z * MOVE_SPEED * multiplier;
		movement.z = -forward.x * MOVE_SPEED * multiplier;
	}

	else {
		movement.x = forward.x * MOVE_SPEED * multiplier;
		movement.z = forward.z * MOVE_SPEED * multiplier;
	}

	position.x += movement.x;
	position.y += movement.y;
	position.z += movement.z;
}