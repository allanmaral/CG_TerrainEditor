#include "Grid.h"
#include <iostream>
#include <cmath>

Grid::Grid(float wSX, float wSY, int rX, int rY) : worldSizeX(wSX), worldSizeY(wSY), resolutionX(rX), resolutionY(rY)
{
    // Espaçamento entre pontos
    float spacingX = worldSizeX/resolutionX;
    float spacingY = worldSizeY/resolutionY;

    pointRadius = std::min(spacingX, spacingY) * -0.6f;

    // Cria a array de pontos dada a resolução do terreno
    pointList = new Point*[(resolutionX+1)*(resolutionY+1)];
    triangleList = new Triangle*[resolutionX*resolutionY*2];

    // Inicializa a seleção para nulo
    selectedPoint = NULL;

    // Cada ponto é inicializado e posicionado de acordo com sua posição no vetor e espaçamento
    for(int x = 0; x <= resolutionX; x++)
    {
        for(int y = 0; y <= resolutionY; y++)
        {
            pointList[index(x, y)] = new Point(x * spacingX, y * spacingY, 0.0f);
        }
    }

    // Cada triangulo é inicializado e seus vertices organizados
    for(int y = 1; y <= resolutionY; y++)
    {
        for(int x = 0; x <= resolutionX-1; x++ )
        {
            triangleList[triangleIndex(x, y)] = new Triangle(
                                                             pointList[index(x, y)],
                                                             pointList[index(x+1, y-1)],
                                                             pointList[index(x, y-1)]
                                                             );

            triangleList[triangleIndex(x, y) +1] = new Triangle(
                                                                pointList[index(x, y)],
                                                                pointList[index(x+1, y)],
                                                                pointList[index(x+1, y-1)]
                                                                );
        }
    }

    // São calculados os vizinhos de cada ponto
    for(int x = 0; x <= resolutionX; x++)
    {
        for(int y = 0; y <= resolutionY; y++)
        {
            Point **temp = new Point*[4];

            // Verifica se os vertices vizinhos estão dentro da malha
            // x>0 && x<resoluçãoX
            // y>0 && x<resoluçãoY
            if(x-1 < 0)
                temp[0] = NULL;
            else
                temp[0] = pointList[index(x-1, y)];

            if(y+1 > resolutionY)
                temp[1] = NULL;
            else
                temp[1] = pointList[index(x, y+1)];

            if(x+1 > resolutionX)
                temp[2] = NULL;
            else
                temp[2] = pointList[index(x+1, y)];

            if(y-1 < 0)
                temp[3] = NULL;
            else
                temp[3] = pointList[index(x, y-1)];

            // Os vizinhos são passados para o ponto
            pointList[index(x, y)]->SetNeighbors(temp[0], temp[1], temp[2], temp[3]);
            delete [] temp;
        }
    }
}


Grid::~Grid()
{
    for(int i = 0; i <= index(resolutionX, resolutionY); i++)
    {
        delete pointList[i];
        pointList[i] = NULL;
    }
    delete [] pointList;

    for(int i =0; i <= triangleIndex(resolutionX-1, resolutionY)+1; i++)
    {
        delete triangleList[i];
        triangleList[i] = NULL;
    }
    delete [] triangleList;
}

// Calculo do indice do ponto dados x e y
int Grid::index(int x, int y)
{
    return x + y * (resolutionX +1);
}

// Calculo do indice do triangulo dados x e y
int Grid::triangleIndex(int x, int y)
{
    return (y-1) * resolutionX * 2 + x*2;
}

void Grid::Draw(float radius, bool wireframe, bool points)
{
    //glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_TRIANGLES);
        for(int i =0; i <= triangleIndex(resolutionX-1, resolutionY)+1; i++)
        {
            triangleList[i]->Draw(wireframe);
        }
    glEnd();

    if(points)
    {
        for(int i = 0; i <= index(resolutionX, resolutionY); i++)
        {
            pointList[i]->DrawPoint(pointRadius);
        }
    }

    if(selectedPoint)
    {
        selectedPoint->DrawPoint(pointRadius);

        glColor3f(0.5f, 0.5f, 0.5f);
        glPushMatrix();
            glTranslatef(selectedPoint->x, selectedPoint->y, selectedPoint->z);
            glutWireSphere(radius * 1.17741, 16, 16);
        glPopMatrix();
    }
}

void Grid::Select(float x, float y)
{
    int _x = round(x * resolutionX);
    int _y = round(y * resolutionY);

    if(selectedPoint) selectedPoint->Deselect();
    selectedPoint = pointList[index(_x, _y)];
    selectedPoint->Select();
}

float gaussian(float x0, float y0, float x1, float y1, float r)
{
    float d2 = (x1-x0)*(x1-x0) + (y1-y0)*(y1-y0);
    return std::exp(-d2/(r*r)) * 0.01;
}

void Grid::gaussianAdd(float height, float radius)
{
    if(selectedPoint)
    {
        for(int i = 0; i <= index(resolutionX, resolutionY); i++)
        {
            pointList[i]->z += gaussian(selectedPoint->x, selectedPoint->y, pointList[i]->x, pointList[i]->y, radius) * ((height > 0) ? 1 : -1);
            if(pointList[i]->z > 1.0) pointList[i]->z = 1.0;
            if(pointList[i]->z < -1.0) pointList[i]->z = -1.0;
        }
    }
}

void Grid::closestAdd(float height, float radius)
{
    if(selectedPoint)
    {
        selectedPoint->updateHeight(height, radius);
        selectedPoint->resetModifier(height, radius);
    }
}

float* Grid::SelectedPointPosition()
{
    float *position = new float[3];
    if(selectedPoint)
    {
        position[0] = selectedPoint->x;
        position[1] = selectedPoint->y;
        position[2] = selectedPoint->z;
    }
    else
    {
        position[0] = position[1] = position[2] = 0.0f;
    }

    return position;
}
