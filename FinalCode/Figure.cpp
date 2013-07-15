#include "Figure.h"

float getDist(float height, float trans, float rad){
	return sqrt(rad*rad+(height*height)-(trans*trans));
}

Figure::Figure(int t, bool s, bool f)
{
    int color = t;
	// BLUE
	if(color == 0){
		c.r = 0; c.g = 0; c.b = 1;
	// RED
	}else if(color==1){
        c.r = 1; c.g = 0; c.b = 0;
	// YELLOW
	}else if(color==2){
		c.r = 1; c.g = 1; c.b = 0;
	// GREEN
	}else{
		c.r = 0; c.g = 1; c.b = 0;
	}
	c.a = 1;
	small = s;
	female = f;
	rad = 1.0;
	if(small) rad/=1.5;
	currentlyBeating = false;
	currentlyDown = false;
	currHeight = 0;

    team = t;
    animatedSinceMove = false;
    moved = false;

    house = false;

	// TODO set and use
	for(int i = 0; i<4; i++) look[i] = 0;
}

Figure::Figure(void){
	Figure(0,0,0);
}

Figure::~Figure(void)
{
}

void Figure::setMatrix(float matrix[16]){
	for(int i = 0; i<16; i++){
		resultMatrixTemp[i] = matrix[i];
	}
}

void Figure::setLookAtMatrix(float matrix[16]){
	for(int i = 0; i<16; i++){
		lookAtMatrix[i] = matrix[i];
	}
}

void Figure::multMatrix(float mat[16], float vec[4])
{
	for(int i=0; i<4; i++)
	{
		look[i] = 0;
		for(int j=0; j<4; j++)
			  look[i] += mat[4*i + j] * vec[j];
	}
}

float* Figure::getMatrix(){
	return resultMatrix;
}

void normalize(float vector[3]){
	float help = sqrt( vector[0]*vector[0] + vector[1]*vector[1] + vector[2]*vector[2] );
	vector[0] /= help;
	vector[1] /= help;
	vector[2] /= help;
}

void Figure::rotateToOpponent()
{
	if(lookAtMatrix&&resultMatrix){
		float vector[3];
		vector[0] = lookAtMatrix[3] - resultMatrix[3];
		vector[1] = lookAtMatrix[7] - resultMatrix[7];
		vector[2] = lookAtMatrix[11] - resultMatrix[11];
	
		//normalize vector
		normalize(vector);

		float defaultLook[4] = {1,0,0,0};
		multMatrix(resultMatrix, defaultLook);

		//normalize look
		normalize(look);

		float angle = (180 / PI) * acos( vector[0] * look[0] + vector[1] * look[1] + vector[2] * look[2] );
		if((vector[0] * look[1] - vector[1] * look[0]) < 0 ) angle *= -1;
	
		glRotatef(angle, 0, 0, 1);
	}
}

void Figure::draw(){
	if(resultMatrix){
        glPushMatrix();

        glLoadTransposeMatrixf(resultMatrix);

		if(currentlyDown) {
            rotateToOpponent();
			glRotatef(a.angle, 1, 0, 0);
		}
		if(currentlyBeating) {
			rotateToOpponent();
		}

		glRotatef(-90, 1, 0, 0 );
		glScalef(0.015, 0.015, 0.015);

		// animate
		glTranslatef(0.0, currHeight, 0.0);
	
		// draw body
		glColor4f(c.r,c.g,c.b,c.a);
		glPushMatrix();
		glRotatef(-90, 1, 0, 0);
		glutSolidCone(rad*1.2,rad*2.5,20,10);
		glPopMatrix();
	
		// draw head
		glTranslatef(0.0, 2.8*rad, 0.0);
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
		glColor4f(c.r,c.g,c.b,c.a);
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
			glColor4f(0.6,0.2,0.2,1);
			glPushMatrix();
			// front, up, left
			glTranslatef(0.0, -1.3*rad, (rad*1.2*1.0)/2.5);
			glRotatef(45+a.angle, 0, 1, 0);
			GLUquadricObj *quadObj = gluNewQuadric();
			gluCylinder(quadObj, rad/20, rad/5, rad*5, 20, 10);
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

        glPopMatrix();
	}
}

void Figure::invokeAnimation(Type t){
	//if(a.startTime<0){
        cout << "Invoke animation " << t << endl;
        animatedSinceMove = true;

		float startTime = glutGet(GLUT_ELAPSED_TIME);
		a.type = t;
		a.startTime = startTime;
		a.angle = 0;
		a.dropDown = -1;
		a.tearTrans = -1;
		switch (t){
			case START:
				a.time = 1000;
				a.heightJumpTears = 2;
				a.timesJump = 1;	
			break;
			case BEAT:
				a.time = 5000;
				a.heightJumpTears = 2;
				a.timesJump = 10;
				currentlyBeating = true;
			break;
			case DEFEAT:
				a.time = 5000;
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
				a.heightJumpTears = 2.5;
				a.timesJump = 12;
			break;
	
		}
		a.endTime = startTime + a.time;
	//}
}

void Figure::animate(){
	float time = glutGet(GLUT_ELAPSED_TIME);
	if(time<=a.endTime) {
		switch (a.type){
			case BEAT:
				currHeight = jump(time);
				if(currentlyBeating){
					if(a.angle>=90) a.angle = 90;
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
	}else {
		a.startTime=-1;
		currHeight=0;
		if(currentlyBeating) currentlyBeating=false;
		if(currentlyDown) currentlyDown=false;
	}
}

bool Figure::animatedSinceLastMove() {
    return animatedSinceMove;
}   

float Figure::jump(float time){
	float timeInterval = time-a.startTime;
	float interval = a.time/a.timesJump;
	while(timeInterval>interval) timeInterval = timeInterval - interval;
	return sin((timeInterval/interval)*PI)*a.heightJumpTears;
}

int Figure::getTeam() {
    return team;
}

CvPoint2D32f Figure::getScreenCoords() {
    return screenCoords;
}

CvPoint3D32f Figure::getWorldCoords() {
    return worldCoords;
}

// real pawn position is just updated if there was no move for 0.5 second
void Figure::setScreenInfo(CvPoint2D32f sCoords) {
    float diff_x = screenCoordsTemp.x - sCoords.x;
    float diff_y = screenCoordsTemp.y - sCoords.y;

    float dist = sqrt(diff_x*diff_x + diff_y*diff_y);
    if (dist > 4.0) {
        moved = true;
        lastMoveTime = glutGet(GLUT_ELAPSED_TIME);   
    }

    float timeElapsed = glutGet(GLUT_ELAPSED_TIME) - lastMoveTime;
    
    screenCoordsTemp = sCoords;
    if (timeElapsed > 1000) {
        lastMoveTime = glutGet(GLUT_ELAPSED_TIME);
        screenCoords = screenCoordsTemp;
    
        for (int i = 0; i < 16; i++) {
            resultMatrix[i] = resultMatrixTemp[i];
        }
    
        if (moved) {
            moved = false;
            calcWorldCoords();
            animatedSinceMove = false;
        }
    }
}
    
void Figure::calcWorldCoords() {
    worldCoords = GetOGLPos(screenCoords.x, screenCoords.y);
}

float Figure::getDistance(CvPoint3D32f p) {

    return getDistanceOfPoints(worldCoords, p);
}

float Figure::getDistanceToFigure(Figure *f) {
    CvPoint3D32f w2 = f->getWorldCoords();

    return getDistance(w2);
}

bool Figure::wasMoved() {
    return (glutGet(GLUT_ELAPSED_TIME) - lastMoveTime) < 0.1;
}

bool Figure::isInHouse() {
    return house;
}   

void Figure::reachedHouse() {
    house = true;
}
