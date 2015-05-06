/** Grid
 *  O terreno propriamente dito, contem a lista de ponto,
 *  sua organização em triangulos, as instruções de desenho e contrução
 */

#ifndef GRID_H
#define GRID_H

#include "Triangle.h"
#include "Point.h"

class Grid
{
public:
    /**
     * Contrutor do Grid.
     *
     * @param worldSizeX    Tamanho do mundo em X do terreno
     * @param worldSizeY    Tamanho do mundo em Y do terreno
     * @param resolutionX   Numero de colunas do terreno
     * @param resolutionY   Numero de linhas do terreno
     */
    Grid(float worldSizeX, float worldSizeY, int resolutionX, int resolutionY);

    ~Grid();

    /**
     * Seleciona um ponto
     * Deve ser passado valores entre 0 e 1 para x e y
     *
     * @param x     Posição x do ponto
     * @param y     Posição y do ponto
     */
    void Select(float x, float y);

    /**
     * Desenha todo o terreno
     *
     * @param radius    Raio da esfera de influencia
     */
    void Draw(float radius, bool wireframe, bool points);

    /**
     * Incrementa a altura do ponto selecionado
     * usando uma função gaussiana
     *
     * @param height    Valor a ser incrementado à altura
     * @param radius    Raio de influencia do ponto
     */
    void gaussianAdd(float height, float radius);

    /**
     * Incrementa a altura do ponto selecionado e seus vizinho mais proximos
     *
     * @param height    Valor a ser incrementado à altura
     */
    void closestAdd(float height, float radius);

    /**
     * Posição do ponto selecionado
     * O vetor deve ser deletado depois
     *
     * @return  Vetor de posição do ponto selecionado (float[3])
     */
    float* SelectedPointPosition();

private:
    /**
     * Tamanho do mundo em X
     */
    float worldSizeX;

    /**
     * Tamanho do mundo em X
     */
    float worldSizeY;

    /**
     * Numero de colunas do terreno
     */
    int resolutionX;

    /**
     * Numero de linhas do terreno
     */
    int resolutionY;

    /**
     * Raio da esfera de seleção (vermelha)
     */
    float pointRadius;

    /**
     * Indice do ponto no vetor
     *
     * @param x     Posição X do ponto
     * @param y     Posição Y do ponto
     */
    int index(int x, int y);

    /**
     * Indice do triangulo no vetor
     *
     * @param x     Posição X do triangulo
     * @param y     Posição Y do triangulo
     */
    int triangleIndex(int x, int y);

    /**
     * Ponteiro para o ponto selecionado
     */
    Point *selectedPoint;

    /**
     * Vetor de pontos do terreno
     */
    Point **pointList;

    /**
     * Vetor de triangulos do terreno
     */
    Triangle **triangleList;
};
#endif // GRID_H
