#pragma once
#include <string>
#include <math.h>
#include <GL/glut.h>

#include <opencv/cv.h>

#include "includes/helper.h"

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
	Figure(int team, bool small, bool female);
	Figure(void);
	~Figure(void);
	
	enum Type {START, BEAT, DEFEAT, WIN, VIC};
	void setMatrix(float resultMatrix[16]);
	void setLookAtMatrix(float resultMatrix[16]);
	float* getMatrix();
	void draw();
	void invokeAnimation(Type t);
	void animate();
    bool animatedSinceLastMove();

    bool wasMoved();

    int getTeam();
    CvPoint2D32f getScreenCoords();
    CvPoint3D32f getWorldCoords();
    void setScreenInfo(CvPoint2D32f sCoords);
    void calcWorldCoords();
    float getDistance(CvPoint3D32f p);
    float getDistanceToFigure(Figure *f);

    bool isInHouse();
    void reachedHouse();
    
private:
    int team;

    CvPoint2D32f screenCoords, screenCoordsTemp;
    CvPoint3D32f worldCoords;   

	bool small;
	bool female;
	GLdouble rad;
	color c;
	
	float look[4];
	GLfloat currHeight;
	bool currentlyBeating;
	bool currentlyDown;
	float resultMatrix[16], resultMatrixTemp[16];
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

    float lastMoveTime;
    bool animatedSinceMove;
    bool moved;

    bool house;
};

