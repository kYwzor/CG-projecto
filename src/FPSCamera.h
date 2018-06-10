#include <GL/glut.h>
#define RADIAN_MULTIPLIER 3.14159265359/180.0	//para conversao graus -> radianos
#define DEGREE_DIVISION 3.14159265359*180.0		//para conversao radianos -> graus
#define K_FRONT 0
#define K_BACK 1
#define K_LEFT 2
#define K_RIGHT 3
#define MOVE_SPEED 0.2

struct Vector3 {
	GLfloat x,y,z;
};

class FPSCamera {
	public:
		GLfloat xAngle, yAngle;
		Vector3 position;
		Vector3 forward;
		FPSCamera();
		void rotateHorizontal (GLfloat angle);
		void rotateVertical (GLfloat angle);
		void render ();
		void move (int direction);
};