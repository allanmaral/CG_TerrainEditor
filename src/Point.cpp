#include "Point.h"
#include <cmath>

Point::Point(float _x, float _y, float _z) : selected(false), x(_x), y(_y), z(_z)
{
    lastModifier = 0.0;
    neighbors = new Point*[4];
    for(int i = 0; i <4; i++)
    {
        neighbors[i] = NULL;
    }
}

Point::Point() : selected(false), x(0.0f), y(0.0f), z(0.0f)
{
    neighbors = new Point*[4];
    for(int i = 0; i <4; i++)
    {
        neighbors[i] = NULL;
    }
}

void Point::SetNeighbors(Point* p1, Point* p2,Point* p3, Point* p4)
{
    neighbors[0] = p1;
    neighbors[1] = p2;
    neighbors[2] = p3;
    neighbors[3] = p4;
}

void Point::SetPosition(float _x, float _y, float _z)
{
    x = _x;
    y = _y;
    z = _z;
}

// check if its selected or not, if not selecte just draw a simple point on screen
void Point::DrawPoint(float radius)
{
    if(!selected)
    {
        glColor3f(POINT_COLOR);
        glBegin(GL_POINTS);
            glVertex3f(x, y, z+0.0001);
        glEnd();
    } else
    {
        DrawAsSelected(radius);
    }
}

// if selected, draw a red sphere at the point center
void Point::DrawAsSelected(float radius)
{
    // Draw sphere core cames here
    glPushMatrix();
        glColor3f(SELECTED_COLOR);
        glTranslatef(x, y, z);
        glutSolidSphere(radius, SPHERE_SLICES, SPHERE_SLICES);
    glPopMatrix();

    for(int i = 0; i < 4; i++)
    {
        if(neighbors[i])
            neighbors[i]->DrawAsNeighbor(radius);
    }

}

// if a neighbor is selected, draw a small yellow sphere at the point center
void Point::DrawAsNeighbor(float radius)
{
    glPushMatrix();
        glColor3f(NEIGHBOR_COLOR);
        glTranslatef(x, y, z);
        glutSolidSphere(radius*0.5, SPHERE_SLICES, SPHERE_SLICES);
    glPopMatrix();
}

void Point::updateHeight(float height, float radius)
{
    if(std::abs(height) > std::abs(lastModifier))
    {
        z += height-lastModifier;
        lastModifier = height;

        if (z > 1) z = 1;
        if (z < -1) z = -1;

        if(std::abs(height*radius*3.0) > 0.01)
        {
            for(int i = 0; i < 4; i++)
            {
                if(neighbors[i])
                {
                    neighbors[i]->updateHeight(height*radius*3.0, radius);
                }
            }
        }
    }
}

void Point::resetModifier(float height, float radius)
{
    lastModifier = 0;
    if(std::abs(height*radius*3.0) > 0.01)
    {
        for(int i = 0; i < 4; i++)
        {
            if(neighbors[i])
            {
                neighbors[i]->resetModifier(height*radius*3.0, radius);
            }
        }
    }

}

void Point::Select()
{
    selected = true;
}

void Point::Deselect()
{
    selected = false;
}
