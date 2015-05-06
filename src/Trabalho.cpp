#include <stdio.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <cmath>
#include <cstring>
#include <string>

#include "Grid.h"
#include "Shader.h"

#define WINDOW_SIZE 500
#define M_PI 3.141592652


enum CONTRUCTION_FUNCTION
{
    GAUSSIAN,
    CLOSEST_NEIGHBOR,
    LAST // facilita o loop no controle, não é nescessario saber o numero de opções ou qual a ultima
};
char* functionNames[2] = {
    (char*)"Gaussian",
    (char*)"Closest Neighbor"
};


void display(void);
void init (void);
void keyboard(unsigned char key, int x, int y);
void mouse(int button, int state, int x, int y);
void mouseMove(int x, int y);
void reshape(int width, int height);
void idle();
void close();

static Shader *shader;

// Controles do mouse
bool drag = false;  //Mouse sendo precionado
int lx = 0, ly = 0; //Ultima posição do mouse

// O terreno e sua resolução
Grid *grid;
int resolution[2];
int contructionFunction = CLOSEST_NEIGHBOR;

// Controle da camera (rotação do terreno, na verdade)
float DISTANCE = 2.0f;
float THETA = 45.0f;
float PHI = 30.0f;
float position[3] = {0, 0, 0}; // Posição do terreno(simulando a movimentação da camera)


//  Raio de influencia do cursor
float radius = 0.1f;
//  Força do contrutor
float brushStrength = 0.1f;

//  Exibe o wireframe na visão 3D
bool wireframe = true;

int main(int argc, char** argv)
{

    printf("Digite o numero de linhas e colunas: ");
    scanf("%d %d", &resolution[1], &resolution[0]);

    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize (WINDOW_SIZE*2, WINDOW_SIZE);
    glutInitWindowPosition (100, 100);
    glutCreateWindow ("Terreno Interativo");
    init ();

    // O grid é criado
    grid = new Grid(1.0, 1.0, resolution[0], resolution[1]);

    // Callbacks do glut
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMotionFunc(mouseMove);
    glutReshapeFunc(reshape);

    glutCloseFunc(close); // Chamada ao fechar a janela (deleta o grid, vazamento de memoria)

    glutMainLoop();

    return 0;
}

void display(void)
{
    // Limpa todos os pixels
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /**
     *  Vizualização 2D do terreno
     */
    glViewport(0, 0, WINDOW_SIZE, WINDOW_SIZE);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, 1.0, 1.0, 0.0, -1.0, 1.0); // Projeção ortogonal da visão

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    grid->Draw((contructionFunction == GAUSSIAN) ? radius : 0.0, wireframe, true); // Desenha o terreno com a esfera de influencia

    /**
     *  Vizualização 3D
     */
    glViewport(WINDOW_SIZE, 0, WINDOW_SIZE, WINDOW_SIZE);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    /** Desenho do texto no display */
    // O desenho do texto é feito antes de serem aplicadas as transformações de projeção
    glColor3f(0.7f, 0.7f, 0.7f);

    glRasterPos3f(-0.9, 0.9, 0);
    float *pos = grid->SelectedPointPosition();
    char text[256];
    sprintf(text, "Infuence Radius: %.2f \nX=%.2f Y=%.2f Z=%.2f", radius, pos[0], pos[1], pos[2]);
    delete[] pos;
    pos = NULL;
    glutBitmapString(GLUT_BITMAP_HELVETICA_12, (const unsigned char*)text);

    glRasterPos3f(0.1, 0.9, 0);
    sprintf(text, "Brush Strength: %.2f\nContruction Function: %s", brushStrength, functionNames[contructionFunction]);
    glutBitmapString(GLUT_BITMAP_HELVETICA_12, (const unsigned char*)text);

    glRasterPos3f(-0.92, -0.9, 0);
    sprintf(text, "+/-: Change the influence radius   Scroll: Zoom(3D view) - Add/Remove(2D view)\nC: Change contruction function      V: Wireframe");
    glutBitmapString(GLUT_BITMAP_HELVETICA_12, (const unsigned char*)text);
    /** Desenho do texto no display */

    gluPerspective(45, 1, 0.1, 100);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(DISTANCE, 0, 0.0, 0, 0, 0, 0, 0, 1);


    glTranslatef(position[0], position[1], position[2]);
    // Ajusta rotaciona o terreno de acordo com os controles do mouse
    glRotatef(PHI, 0, 1, 0);
    glRotatef(THETA, 0, 0, 1);


    // O plano é gerado com o y invertido com 0 <= x <= 1; 0 <= y <= 1
    // As matrizes corrigem a inversão e transladam o plano para a origem
    glScalef(1, -1, 1);
    glTranslatef(-0.5f , -0.5f, 0);
    grid->Draw(0.0, wireframe, false);

    glutSwapBuffers ();
}


void init (void)
{
    // Define a cor do fundo como preto
    glClearColor (0.0, 0.0, 0.0, 1.0);

    glewInit();
    shader = new Shader("vert.glsl", "frag.glsl");
    shader->UseShader();

    // Ativa o teste de profundidade
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glPointSize(1.5f);
    glLineWidth(1.0f);
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    // ESQ fecha a janela
    case 27:
        exit(0);
    break;

    // + incrementa o raio de influencia do cursor
    case '+':
        radius += 0.01f;
        break;

    // - decrementa o raio de influencia do cursor
    case '-':
        radius -= 0.01f;
        if (radius < 0.1) radius = 0.1;
        break;

    // Alterna entre os modos de construção
    case 'C':
    case 'c':
        contructionFunction++;
        if(contructionFunction == LAST) contructionFunction=0;
        break;

    // Liga e desliga o wireframe
    case 'V':
    case 'v':
        wireframe = !wireframe;
        break;

    // Controla a força do construtor
    case '9':
    case '(':
        brushStrength -= 0.01f;
        if(brushStrength < 0.01) brushStrength = 0.01;
        break;
    case '0':
    case ')':
        brushStrength += 0.01f;
        break;

    case 'W':
    case 'w':
        position[0] += 0.01f;
        break;

    case 'S':
    case 's':
        position[0] -= 0.01f;
        break;

    case 'A':
    case 'a':
        position[1] += 0.01f;
        break;

    case 'D':
    case 'd':
        position[1] -= 0.01f;
        break;
    }
    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
    /** Controle da viewport ortogonal */
    // Quando clicar no terreno na visão ortogonal
    if(button == 0 && state == 0 && x <= WINDOW_SIZE)
    {
        // X e Y são convertidos do espaço da janena para valores entre 0 e 1
        float _y = ((float)y)/WINDOW_SIZE;
        float _x = ((float)x)/WINDOW_SIZE;

        grid->Select(_x, _y);

        //Depois de selecionar o ponto, redesenha a tela
        glutPostRedisplay();
    }
    // button 3 e 4 correspondem ao scroll in e scroll out, respectivamente
    // Quando der scroll in, a altura do ponto e seus vizinhos são decrementadas
    else if(button == 3 && state == 1 && x <= WINDOW_SIZE)
    {
        switch(contructionFunction)
        {
        case GAUSSIAN:
            grid->gaussianAdd(-brushStrength, radius);
            break;
        case CLOSEST_NEIGHBOR:
            grid->closestAdd(-brushStrength, (radius < 0.26) ? radius : 0.26);
            break;
        }
        glutPostRedisplay();
    }

    // Quando der scroll out, a altura do ponto e seus vizinhos são incrementadas
    else if(button == 4 && state == 1 && x <= WINDOW_SIZE)
    {
        switch(contructionFunction)
        {
        case GAUSSIAN:
            grid->gaussianAdd(brushStrength, radius);
            break;
        case CLOSEST_NEIGHBOR:
            grid->closestAdd(brushStrength, (radius < 0.26) ? radius : 0.26);
            break;
        }
        glutPostRedisplay();
    }


    /** Controle da viewport de projeção */
    // Quando o botão esquerdo do mouse é precionado
    else if(button == 0 && state == 0 && x > WINDOW_SIZE)
    {
        // A flag é setada como true
        // e a ultima posição do mouse é definida como a posição no momento do click
        drag = true;
        lx = x, ly = y;
    }
    // Quando botão esquerdo é solto
    else if(button == 0 && state == 1)
    {
        drag = false;
    }
    //Scroll in do mouse (zoom in)
    else if(button == 3 && state == 1 && x > WINDOW_SIZE)
    {
        DISTANCE /= 1.2f;
        glutPostRedisplay();
    }
    //Scroll out do mouse (zoom out)
    else if(button == 4 && state == 1 && x > WINDOW_SIZE)
    {
        DISTANCE *= 1.2f;
        glutPostRedisplay();
    }
}

// Callback chamada quando o mouse é movido
void mouseMove(int x, int y)
{
    //Se o mouse estiver sendo precionado
    if(drag)
    {
        // Calcula a distancia percorrida pelo cursor
        int dx = x-lx;
        int dy = y-ly;

        // Calcula os angulos de acordo com a distancia
        THETA += dx/(2*M_PI);
        PHI += dy/(2*M_PI);

        // Ultima posção do mouse é atualizada
        lx = x, ly = y;

        glutPostRedisplay();
    }
}

// Chamada ao fechar a janela
void close()
{
    delete grid;
}

// Impede que a janela tenha o tamanho alterado
void reshape(int width, int height)
{
    glutReshapeWindow(WINDOW_SIZE*2, WINDOW_SIZE);
}

