#include "Triangle.h"
#include <algorithm>
#include <cmath>

Triangle::Triangle(Point* pt1, Point* pt2, Point* pt3)
{
    points = new Point*[3];
    points[0] = pt1;
    points[1] = pt2;
    points[2] = pt3;
}

Triangle::~Triangle()
{
    delete [] points;
}

void Triangle::Draw(bool wireframe)
{
    glBegin(GL_TRIANGLES);
    for(int i = 0; i < 3; i ++)
    {
        float z =  points[i]->z;
        glColor3f(1-std::abs(z), z+1, 1-z);
        glVertex3f(points[i]->x, points[i]->y, points[i]->z);
    }
    glEnd();

    if(wireframe)
    {
        glColor3f(0.0, 0.0, 0.0);
        glBegin(GL_LINES);
            glVertex3f(points[0]->x, points[0]->y, points[0]->z+0.0002);
            glVertex3f(points[1]->x, points[1]->y, points[1]->z+0.0002);

            glVertex3f(points[1]->x, points[1]->y, points[1]->z+0.0002);
            glVertex3f(points[2]->x, points[2]->y, points[2]->z+0.0002);

            glVertex3f(points[2]->x, points[2]->y, points[2]->z+0.0002);
            glVertex3f(points[0]->x, points[0]->y, points[0]->z+0.0002);
        glEnd();
    }
}

