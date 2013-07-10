#include <math.h>
#include <string>
#include "Figure.h"
#include "glut.h"

GLfloat height = 0;
Figure* f;

void display();
void resize(int width, int height);
void timer(int value);

int main(int argc, char* argv[]) {
	f = new Figure(0.0f,0.0f,1.0f,1.0f,false,false);
	// --------- init GLUT --------
	// initialize GLUT library, negotiate session with window system; value in argcp and argv will be updated
	glutInit(&argc, argv);
	// set initial window size
	glutInitWindowSize(640, 480);
	// set initial display mode 
	// RGBA: bit mask to select RGBA mode window
	// DOUBLE: bit mask to select double buffered window
	// DEPTH: bit mask to select window with depth buffer
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	// create top-level window
	glutCreateWindow("Snowman");

	// register display callback
	glutDisplayFunc(display);
	// register resize callback
	glutReshapeFunc(resize);

	// ------- init GL -------
	// set background color
	glEnable(GL_COLOR_MATERIAL);
	glClearColor(0.5, 0.5, 1.0, 1.0);
	// enable and set depth
	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.0);

	// ------- switch on the light --------
	// set parameters for first light source
	// pos: w = 0 directional light influences (diffuse / specular)
	GLfloat light_pos[] = {1.0, 1.0, 1.0, 0.0};
	GLfloat light_amb[] = {0.2, 0.2, 0.2, 1.0};
	GLfloat light_dif[] = {0.9, 0.9, 0.9, 1.0};
	// enable light source and lighting
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT,  light_amb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_dif);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	// ------- animation -------
	glutTimerFunc(25, timer, 0);

	// call glutMainLoop to hand control to the GLUT library
	glutMainLoop();
}

// drawback function to interact with the window, display is for rendering
void display(){
	// clear thw window
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set active matrix, move to origin
	glMatrixMode(GL_MODELVIEW);
	// clear modelview
	glLoadIdentity();

	// camera by default located at origin, looks down negative z-axis
	// use glTranslatef to move world in negative z direction (the more, the closer)
	// initial transformation
	glTranslatef(0.0, -0.8, -10.0);
	glRotatef(-90, 0, 1, 0);

	/* ------ draw wired cube -------
	glColor4f(1.0, 0.0, 0.0, 1.0);
	glutWireCube(1.0); */
	// drawSnowman(true);
	f->draw();

	// double-buffered window, exchange front and back buffers = redraw
	glutSwapBuffers();
}

// allow resizing the window
void resize(int width, int height){
	// set rendering area to the new size (here: whole-window viewport): x,y,widht,height
	glViewport( 0, 0, width, height );

	// 3D-rendering -> modelview and projection matrix
	// set active matrix (here: perspective projection matrix)
	glMatrixMode(GL_PROJECTION);
	// clear projection at beginning of display callback
	glLoadIdentity();
	// set projection matrix: angle, aspectratio, near-clipping, far-clipping
	gluPerspective(30, ((double)width/(double)height), 0.01, 100);

	// invalidate display
	glutPostRedisplay();
}

void timer(int value){
	float currentTime = glutGet(GLUT_ELAPSED_TIME);
	if(currentTime>500&&currentTime<1000) f->startAnimation(Figure::START,currentTime);
	if(currentTime>2000&&currentTime<3000) f->startAnimation(Figure::BEAT,currentTime);
	if(currentTime>6000&&currentTime<7000) f->startAnimation(Figure::DEFEAT,currentTime);
	if(currentTime>10000&&currentTime<11000) f->startAnimation(Figure::WIN,currentTime);
	if(currentTime>14000&&currentTime<15000) f->startAnimation(Figure::VIC,currentTime);
	if(currentTime>24000&&currentTime<25000) f = new Figure(0,0,1,1,true,false);
	if(currentTime>30000&&currentTime<31000) f = new Figure(0,0,1,1,false,true);
	if(currentTime>36000&&currentTime<37000) f = new Figure(0,0,1,1,true,true);
	if(currentTime>40000&&currentTime<41000) f = new Figure(0,1,0.5,1,false,true);
	if(currentTime>44000&&currentTime<45000) f = new Figure(1,0,0.5,1,false,true);
	if(currentTime>48000&&currentTime<49000) f = new Figure(1,1,0,1,false,true);

	f->animate(currentTime);
	glutTimerFunc(25, timer, 0);
	glutPostRedisplay();
}