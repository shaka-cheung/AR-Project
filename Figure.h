#pragma once
#include <string>
#include "glut.h"

#define PI 3.14159265

class Figure
{
public:
	Figure(float r, float g, float b, float a, bool small, bool female);
	~Figure(void);
	struct color{
		GLfloat r;
		GLfloat b;
		GLfloat g;
		GLfloat a;
	};
	enum Type {START, BEAT, DEFEAT, WIN, VIC};
	void draw();
	void startAnimation(Type t, float start);
	void animate(float time);
private:
	bool small;
	bool female;
	GLdouble rad;
	GLfloat currHeight;
	color c;

	bool currentlyBeating;
	bool currentlyDown;

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
	};
	Animation a;
	float jump(float time);
};

