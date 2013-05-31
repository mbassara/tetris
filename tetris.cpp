/*
(c) Janusz Ganczarski
http://www.januszg.hg.pl
JanuszG(małpeczka)enter.net.pl
*/

#include <GL/glut.h>
#include <stdlib.h>
#include <vector>
#include <iostream>

using namespace std;

// rozmiary bryły obcinania
const GLdouble board_width = 200.0;
const GLdouble board_height = 200.0;

const GLdouble board_left = -10.0;
const GLdouble board_right = board_width + 50;
const GLdouble bottom = -10.0;
const GLdouble top = board_height - bottom;
const GLdouble near = -1.0;
const GLdouble far = 1.0;


class Block
{
public:
    GLfloat width;
    GLfloat height;
    GLfloat pos_x; 
    GLfloat pos_y;

    Block(GLfloat width=20.f, GLfloat height=20.f){
        this->width = width;
        this->height = height;
        this->pos_x = board_width / 2 - width;
        this->pos_y = board_height - height;
    }

    void draw(){
        glBegin(GL_POLYGON);
            glVertex2f(0.f, 0.f);
            glVertex2f(0.f, height);
            glVertex2f(width, height);
            glVertex2f(width, 0);
        glEnd();
    }

    bool isOnTheBottom(){
        return pos_y == 0;
    }
};

vector<Block> blocks;
Block block;

void DrawFrame(float width, float height)
{
    //linia
    glLineWidth ( 2 ); //grubość linii

    //Rysowanie linii
    glBegin(GL_LINES);
        //kazdy glVertex  - 1 punkt
        glVertex2f(0.f, 0.f);
        glVertex2f(0.f, height);

        glVertex2f(width, 0.f);
        glVertex2f(width, height);

        glVertex2f(0.f, height);
        glVertex2f(width, height);

        glVertex2f(0.f, 0.f);
        glVertex2f(width, 0);

    //Koniec rysowania linii
    glEnd();
}

void Display()
{
    // kolor tła - zawartość bufora koloru
    glClearColor( 1.0, 1.0, 1.0, 1.0 );
    
    // czyszczenie bufora koloru
    glClear( GL_COLOR_BUFFER_BIT );
    
    // wybór macierzy modelowania
    glMatrixMode( GL_MODELVIEW );
    
    // macierz modelowania = macierz jednostkowa
    glLoadIdentity();
    
    // kolor krawędzi obiektu
    glColor3f( 0.0, 0.0, 0.0 );
    
    // rysowanie ramki
    DrawFrame(board_width, board_height);

    // jeśli ostatnio przesuwany klocek jest na dole to wrzucamy nowy klocek do bufora
    if(blocks.back().isOnTheBottom())
        blocks.push_back(Block());

    // rysujemy wszystkie klocki
    for(int i = 0; i < blocks.size(); i++){
        glPushMatrix();
        glTranslatef( blocks[i].pos_x, blocks[i].pos_y, 0.0 );
        blocks[i].draw();
        glPopMatrix();
    }

    // skierowanie poleceń do wykonania
    glFlush();
    
    // zamiana buforów koloru
    glutSwapBuffers();
}

// zmiana wielkości okna

void Reshape( int width, int height )
{
    // obszar renderingu - całe okno
    glViewport( 0, 0, width, height );
    
    // wybór macierzy rzutowania
    glMatrixMode( GL_PROJECTION );
    
    // macierz rzutowania = macierz jednostkowa
    glLoadIdentity();
    
    // wysokość okna większa od wysokości okna
    if( width < height && width > 0 )
        glOrtho( board_left, board_right, bottom * height / width, top * height / width, near, far );
    else

    // szerokość okna większa lub równa wysokości okna
    if( width >= height && height > 0 )
        glOrtho( board_left * width / height, board_right * width / height, bottom, top, near, far );
    
    // generowanie sceny 3D
    Display();
}

// obsługa klawiatury

void Keyboard( unsigned char key, int x, int y )
{
    // odrysowanie okna
    Reshape( glutGet( GLUT_WINDOW_WIDTH ), glutGet( GLUT_WINDOW_HEIGHT ) );
}

// obsługa klawiszy funkcyjnych i klawiszy kursora

void SpecialKeys( int key, int x, int y )
{
    // bierzemy ostatni klocek z bufora
    Block* block = &(blocks.back());

    // jeśli klocek jest na dole to nie przesuwaj
    if(block->isOnTheBottom())
        return;

    switch( key )
    {
        // kursor w lewo
    case GLUT_KEY_LEFT:
        if(block->pos_x > block->width)
            block->pos_x -= block->width;
        else if(block->pos_x > 0)
            block->pos_x = 0;
        break;
        
        // kursor w górę
    case GLUT_KEY_UP:
        break;
        
        // kursor w prawo
    case GLUT_KEY_RIGHT:
        if(block->pos_x < board_width - block->width - block->width)
            block->pos_x += block->width;
        else if(block->pos_x < board_width - block->width)
            block->pos_x = board_width - block->width;
        break;
        
        // kursor w dół
    case GLUT_KEY_DOWN:
        if(block->pos_y > 10)
            block->pos_y -= 10;
        else if(block->pos_y > 0)
            block->pos_y = 0;
        break;
    }
    
    // odrysowanie okna
    Reshape( glutGet( GLUT_WINDOW_WIDTH ), glutGet( GLUT_WINDOW_HEIGHT ) );
}

void Animate(int arg)
{
    // bierzemy ostatni klocek z bufora
    Block* block = &(blocks.back());

    if(block->pos_y > 0){
        block->pos_y -= 1;
    }

    glutPostRedisplay();
    glutTimerFunc(45, Animate, 0);
}

int main( int argc, char * argv[] )
{
    // wrzucamy do bufora klocków pierwszy klocek
    blocks.push_back(Block());

    // inicjalizacja biblioteki GLUT
    glutInit( & argc, argv );
    
    // inicjalizacja bufora ramki
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB );
    
    // rozmiary głównego okna programu
    glutInitWindowSize( 500, 500 );
    
    // utworzenie głównego okna programu
    glutCreateWindow( "Tetris" );

    // dołączenie funkcji generującej scenę 3D
    glutDisplayFunc( Display );
    
    // dołączenie funkcji wywoływanej przy zmianie rozmiaru okna
    glutReshapeFunc( Reshape );
    
    // dołączenie funkcji obsługi klawiatury
    glutKeyboardFunc( Keyboard );
    
    // dołączenie funkcji obsługi klawiszy funkcyjnych i klawiszy kursora
    glutSpecialFunc( SpecialKeys );

    // odpalenie animacji (po 45ms)
    glutTimerFunc(45, Animate, 0);
    
    // wprowadzenie programu do obsługi pętli komunikatów
    glutMainLoop();
    return 0;
}