/** Triangle
 *  Contem apenas a instru��o de desenho do triangulo
 *  Pontos presentes nele s�o apenas ponteiros para o pontos do Grid
 */

#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Point.h"

class Triangle
{
public:
    /**
     * Contrutor do triangulo
     * Os ponto do triangulo devem ser informados em sequencia anti-hor�ria (culling)
     *
     * @param pt* Pontos do triangulo
     */
    Triangle(Point* pt1, Point* pt2, Point* pt3);

    /**
     *  Destrutor
     */
    ~Triangle();

    /**
     *  Desenha o triangulo
     *
     *  @param wireframe Desenhar wireframe ou n�o
     */
    void Draw(bool wireframe);

private:
    /**
     * Vetor de ponteiros para os pontos do triangulo
     */
    Point **points;
};

#endif //TRIANGLE_H
