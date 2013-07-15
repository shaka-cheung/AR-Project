#include "helper.h"

CvPoint3D32f GetOGLPos(int x, int y)
{
    GLint viewport[4];
    GLdouble modelview[16];
    GLdouble projection[16];
    GLfloat winX, winY, winZ;
    GLdouble posX, posY, posZ;
 
    glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
    glGetDoublev( GL_PROJECTION_MATRIX, projection );
    glGetIntegerv( GL_VIEWPORT, viewport );
 
    winX = (float)x;
    winY = (float)viewport[3] - (float)y;
    glReadPixels( x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );

    gluUnProject( winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);
 
    CvPoint3D32f p;
    p.x = posX;
    p.y = posY;
    p.z = posZ;

    return p;
}

float getDistanceOfPoints(CvPoint3D32f p1, CvPoint3D32f p2) {
    float diff_x = p1.x - p2.x;
    float diff_y = p1.y - p2.y;
    float diff_z = p1.z - p2.z;
    
    return sqrt(diff_x*diff_x + diff_y*diff_y + diff_z*diff_z);
}
