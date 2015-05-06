/** Point
 *  Classe que contem as informa��es de cada ponto,
 *  seus quatro vizinhos mais proximos e suas fun��es de desenho
 *
 *  A ideia original dos quatro vizinhos era pra fun��o de constru��o,
 *  mas acabou sendo trocada pela fun��o gaussiana que � aplicada em toda a malha
 */

#ifndef POINT_H
#define POINT_H

#include <GL/glut.h>

#define SELECTED_COLOR 0.8515625f, 0.14453125f, 0.11328125f
#define NEIGHBOR_COLOR 0.952941176f, 0.91372549f, 0.015686275f
#define POINT_COLOR 0.0f, 0.5f, 0.0f

#define SPHERE_SLICES 8

class Point
{
public:
    /**
     *  Coordenadas do ponto
     */
    float x, y, z;

    /**
     *  Ultimo valor de modifica��o do ponto
     */
    float lastModifier;

    /**
     *  Construtor do ponto (autoexplicativo)
     */
    Point(float x, float y, float z);
    Point();

    /**
     *  Define os vizinhos do ponto
     *
     *  @param  p* Ponteiro para o ponto vizinho
     */
    void SetNeighbors(Point* p1, Point* p2,Point* p3, Point* p4);

    /**
     *  Define a posi��o do ponto
     */
    void SetPosition(float x, float y, float z);

    /**
     *  Desenha o ponto na tela
     *  Se o ponto estiver sendo selecionado DrawAsSelected ser� chamada
     *
     *  @param radius raio da esfera caso o ponto esteja selecionado
     */
    void DrawPoint(float radius);

    /**
     *  Desenha o ponto como um vizinho
     *  Sera desenhada uma esfera amarela com centro no ponto
     *
     *  @param radius Raio da esfera a ser desenhada
     */
    void DrawAsNeighbor(float radius);

    /**
     *  Marca o ponto como selecionado
     */
    void Select();

    /**
     *  Remove a marca��o de sele��o
     */
    void Deselect();

    /**
     *  Incrementa a altura do ponto
     *
     *  @param height Altura a ser incrementada
     */
    void updateHeight(float height, float radius);

    /**
     *  Reseta os valores da ultima edi��o
     *
     *  @param height Atura ultilizada na ultima edi��o
     */
    void resetModifier(float height, float radius2);

private:

    /**
     *  Vetor com os vizinhos do ponto
     */
    Point** neighbors;

    /**
     *  Indica se o ponto est� selecionado ou n�o
     */
    bool selected;

    /**
     *  Desenha ponto como selecionado e chama o desenho de seus vizinhos
     */
    void DrawAsSelected(float radius);

};

#endif //POINT_H
