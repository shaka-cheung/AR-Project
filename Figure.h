#pragma once
#include <string>
#include <math.h>
#include "glut.h"

#define PI 3.14159265
using namespace std;

struct color{
	GLfloat r;
	GLfloat b;
	GLfloat g;
	GLfloat a;
};

class Figure
{
public:
	Figure(int color, bool small, bool female);
	Figure(void);
	~Figure(void);
	
	enum Type {START, BEAT, DEFEAT, WIN, VIC};
	void setMatrix(float resultMatrix[16]);
	void setLookAtMatrix(float resultMatrix[16]);
	float* getMatrix();
	void draw();
	void invokeAnimation(Type t);
	void animate();
private:
	bool small;
	bool female;
	GLdouble rad;
	color c;
	
	float look[4];
	GLfloat currHeight;
	bool currentlyBeating;
	bool currentlyDown;
	float resultMatrix[16];
	float lookAtMatrix[16];

	float jump(float time);
	void multMatrix(float mat[16], float vec[4]);
	void rotateToOpponent();

	struct Animation 
	{
		Type type;
		float time;
		float startTime;
		float endTime;
		float heightJumpTears;
		float timesJump;
		float angle;
		float dropDown;
		float tearTrans;
	} a;
};

