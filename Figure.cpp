#include "Figure.h"

float getDist(float height, float trans, float rad){
	GLdouble newRad = sqrt(rad*rad+(height*height));
	return sqrt(newRad*newRad-(trans*trans));
}


Figure::Figure(float r, float g, float b, float a, bool s, bool f)
{
	currHeight = 0;
	c.r = r;
	c.g = g;
	c.b = b;
	c.a = a;
	small = s;
	female = f;
	rad = 0.8;
	if(small) rad/=1.5;
	currentlyBeating = false;
	currentlyDown = false;
}


Figure::~Figure(void)
{
}

void Figure::draw(){
	// animate
	glTranslatef(0.0, currHeight, 0.0);

	if(currentlyDown){
		glRotatef(a.angle, 1, 0, 0);
	}
	glTranslatef(0.0, 1.0, 0.0);
	glColor4f(c.r,c.g,c.b,c.a);
	// draw head
	glutSolidSphere(rad,20,10);
	// draw eyes
	glPushMatrix();
	glColor4f(0,0,0,0);
	float height = rad/4.0;
	float trans = 0.375*rad;
	glTranslatef(getDist(height,trans,rad), height, trans);
	glutSolidSphere(rad/10.0,20,10);
	glTranslatef(0.0, 0.0, -0.75*rad);
	glutSolidSphere(rad/10.0,20,10);
	glPopMatrix();
	// draw body
	glColor4f(c.r,c.g,c.b,c.a);
	glPushMatrix();
	glTranslatef(0.0, -2.8*rad, 0.0);
	glRotatef(-90, 1, 0, 0);
	glutSolidCone(rad*1.2,rad*2.5,20,10);
	glPopMatrix();
	// draw boobs
	if(female){
		glPushMatrix();
		glTranslatef(0.375*rad, -1.5*rad, 0.3*rad);
		glutSolidSphere(rad/3.0,20,10);
		glTranslatef(0.0, 0.0, -0.6*rad);
		glutSolidSphere(rad/3.0,20,10);
		glPopMatrix();
	}
	if(currentlyBeating){
		glPushMatrix();
		// front, up, left
		glTranslatef(0.0, -1.3*rad, (rad*1.2*1.0)/2.5);
		glRotatef(45+a.angle, 0, 1, 0);
		GLUquadricObj *quadObj = gluNewQuadric();
		gluCylinder(quadObj, rad/20, rad/5, rad*2, 20, 10);
		glPopMatrix();
	}

	if(currentlyDown){
		glPushMatrix();
		glColor4f(0,0.5,1.0,1.0);
		// draw tears
		float height = rad/4.0+a.dropDown;
		float trans = a.tearTrans+a.heightJumpTears;
		glTranslatef(getDist(height,trans,rad), height, trans);
		glutSolidSphere(rad/5.0,20,10);
		trans *= -2.0;
		glTranslatef(0, 0, trans);
		glutSolidSphere(rad/5.0,20,10);  
		glPopMatrix();
	}
}

void Figure::startAnimation(Type t, float start){
	if(a.startTime<0){
		a.type = t;
		a.startTime = start;
		a.angle = 0;
		a.dropDown = -1;
		a.tearTrans = -1;
		switch (t){
			case START:
				a.time = 1000;
				a.heightJumpTears = 1;
				a.timesJump = 1;	
			break;
			case BEAT:
				a.time = 3000;
				a.heightJumpTears = 0.5;
				a.timesJump = 10;
				currentlyBeating = true;
			break;
			case DEFEAT:
				a.time = 3000;
				a.heightJumpTears = 0.5*rad;
				a.timesJump = 0.5;
				a.dropDown=0;
				a.tearTrans = 0.375*rad+2*rad/10.0;
				currentlyDown = true;
			break;
			case WIN:
				a.time = 3000;
				a.heightJumpTears = 1.5;
				a.timesJump = 7;
			break;

			case VIC:
				a.time = 9000;
				a.heightJumpTears = 1.5;
				a.timesJump = 12;
			break;
	
		}
		a.endTime = start + a.time;
	}
}

void Figure::animate(float time){
	if(time<=a.endTime) {
		switch (a.type){
			case BEAT:
				currHeight = jump(time);
				if(currentlyBeating){
					if(a.angle>=90) currentlyBeating = false;
					else a.angle += ((time-a.startTime)*45)/(0.5*a.time);
				}
			break;
			case DEFEAT:
				a.tearTrans = jump(time);
				if(currentlyDown){
					if(a.angle<90) a.angle += ((time-a.startTime)*45)/(0.5*a.time);
					if(a.angle>90) a.angle = 90;
					if(abs(a.dropDown)<0.75*rad) a.dropDown = -((time-a.startTime)/a.time)*0.75*rad;
				}

			break;
			default:
				currHeight = jump(time);
			break;
		}
	}else if(a.startTime>0) {
		a.startTime=-1;
		if(currentlyBeating) currentlyBeating=false;
		if(currentlyDown) currentlyDown=false;
	}
}

float Figure::jump(float time){
	float timeInterval = time-a.startTime;
	float interval = a.time/a.timesJump;
	while(timeInterval>interval) timeInterval = timeInterval - interval;
	return sin((timeInterval/interval)*PI)*a.heightJumpTears;
}
