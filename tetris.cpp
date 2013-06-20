#include <GL/glut.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <string>

using namespace std;

// czas pomiędzy kolejnymi klatkami (40ms -> 25 fps)
int animationSpeed = 40;

// czy gra trwa nadal
bool gameOver = false;

// rozmiary bryły obcinania
const GLdouble board_width = 200.0;
const GLdouble board_height = 200.0;

const GLdouble window_left = -10.0;
const GLdouble window_right = board_width + 10;
const GLdouble window_bottom = -10.0;
const GLdouble window_top = board_height + 10;
const GLdouble near = -1.0;
const GLdouble far = 1.0;

class Painter {
public:
    enum Color {RED = 0, GREEN = 1, BLUE = 2, YELLOW = 3, ORANGE  = 4,
                VIOLET = 5, PINK = 6, BROWN = 7, GREY = 8, LAST = 9
    };

    static void paint(Color color){
        switch(color) {
            case RED:
                glColor3f( 1.0, 0.0, 0.0 );
                break;
            case GREEN:
                glColor3f( 0.0, 1.0, 0.0 );
                break;
            case BLUE:
                glColor3f( 0.0, 0.0, 1.0 );
                break;
            case YELLOW:
                glColor3f( 1.0, 1.0, 0.0 );
                break;
            case ORANGE:
                glColor3f( 1.0, 0.5, 0.0 );
                break;
            case VIOLET:
                glColor3f( 0.5, 0.0, 1.0 );
                break;
            case PINK:
                glColor3f( 1.0, 0.0, 1.0 );
                break;
            case BROWN:
                glColor3f( 0.5, 0.25, 0.0 );
                break;
            default:    // grey
                glColor3f( 0.5, 0.5, 0.5 );
                break;
        }
    }
};

class Block
{
public:
    GLfloat width;
    GLfloat height;
    GLfloat pos_x; 
    GLfloat pos_y;
    Painter::Color color;

    Block(Painter::Color color=Painter::GREY, GLfloat width=20.f, GLfloat height=20.f){
        this->width = width;
        this->height = height;
        this->pos_x = board_width / 2 - width;
        this->pos_y = board_height - height;
        this->color = color;
    }

    void draw(){
        Painter::paint(color);
        glBegin(GL_POLYGON);
            glVertex2f(0.f, 0.f);
            glVertex2f(0.f, height);
            glVertex2f(width, height);
            glVertex2f(width, 0);
        glEnd();
    }

    bool isOnTheBottom(){
        return pos_y <= 0;
    }

    bool isOnTheLeft(){
        return pos_x <= 0;
    }

    bool isOnTheRight(){
        return pos_y >= board_width;
    }

    bool canCollideVertically(Block otherBlock){
        return  // czy przesuwając klocek w górę lub w dół możemy trafić na inny klocek
                (pos_x + width > otherBlock.pos_x &&
                pos_x  < otherBlock.pos_x + otherBlock.width);
    }

    bool canCollideHorizontally(Block otherBlock){
        return  // czy przesuwając klocek w lewo lub w prawo możemy trafić na inny klocek
                (pos_y + height > otherBlock.pos_y &&
                pos_y  < otherBlock.pos_y + otherBlock.height);
    }

};

class BlocksBuffer : public vector<Block>
{
public:
    GLfloat freeSpaceBelowLastBlock(){
        if(back().isOnTheBottom())
            return 0;

        GLfloat distance = back().pos_y;
        for(int i = 0; i < size() - 1; i++)
            if(back().canCollideVertically(this->at(i))){
                GLfloat tmpDist = back().pos_y - this->at(i).pos_y - this->at(i).height;
                if(tmpDist < distance)
                    distance = tmpDist;
            }

        return distance;
    }
    GLfloat freeSpaceOnTheLeftOfLastBlock(){
        if(back().isOnTheLeft())
            return 0;

        GLfloat distance = back().pos_x;
        for(int i = 0; i < size() - 1; i++)
            // jeśli ostatni klocek jest po prawej stronie od i-tego i nie koliduje z i-tym
            if(back().pos_x > this->at(i).pos_x && back().canCollideHorizontally(this->at(i))){
                GLfloat tmpDist = back().pos_x - this->at(i).pos_x - this->at(i).width;
                if(tmpDist < distance)
                    distance = tmpDist;
            }

        return distance;
    }
    GLfloat freeSpaceOnTheRightOfLastBlock(){
        if(back().isOnTheRight())
            return 0;

        GLfloat distance = board_width - back().width - back().pos_x;
        for(int i = 0; i < size() - 1; i++)
            // jeśli ostatni klocek jest po lewej stronie od i-tego i nie koliduje z i-tym
            if(back().pos_x < this->at(i).pos_x && back().canCollideHorizontally(this->at(i))){
                GLfloat tmpDist = this->at(i).pos_x - back().pos_x - back().width;
                if(tmpDist < distance)
                    distance = tmpDist;
            }

        return distance;
    }

    void translateAndDraw(){
        for(int i = 0; i < size(); i++){
            glPushMatrix();
            glTranslatef( this->at(i).pos_x, this->at(i).pos_y, 0.0 );
            this->at(i).draw();
            glPopMatrix();
        }
    }
};

BlocksBuffer blocks;

void DrawFrame(float width, float height)
{
    // kolor krawędzi ramki
    glColor3f( 0.0, 0.0, 0.0 );

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

void DrawText(string text){
    glPushMatrix();
    glTranslatef(50, board_height / 2, 0);
    glScalef(0.2, 0.2, 0.2);
    for(int i = 0; i < text.length(); i++)
        glutStrokeCharacter(GLUT_STROKE_ROMAN, text[i]);
    glPopMatrix();
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
    // kolor krawędzi ramki
    glColor3f( 0.0, 0.0, 0.0 );

    if(gameOver)
        DrawText("Game Over");
    else {
        // rysowanie ramki
        DrawFrame(board_width, board_height);

        // jeśli ostatnio przesuwany klocek spadł na inny klocek (lub jest na samym dole) to wrzucamy nowy klocek do bufora
        if(blocks.freeSpaceBelowLastBlock() <= 0){
            blocks.push_back(Block(Painter::Color(rand() % Painter::LAST)));
            if(blocks.freeSpaceBelowLastBlock() <= 0)
                gameOver = true;
        }

        // rysujemy wszystkie klocki i przesuwamy każdy jeśli zmieniła się jego pozycja
        blocks.translateAndDraw();
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
        glOrtho( window_left, window_right, window_bottom * height / width, window_top * height / width, near, far );
    else

    // szerokość okna większa lub równa wysokości okna
    if( width >= height && height > 0 )
        glOrtho( window_left * width / height, window_right * width / height, window_bottom, window_top, near, far );
    
    // generowanie sceny 3D
    Display();
}

// obsługa klawiatury

void Keyboard( unsigned char key, int x, int y )
{
    // 27 - klawisz ESC
    if(key == 27)
        exit(0);
}

// obsługa klawiszy funkcyjnych i klawiszy kursora

void SpecialKeys( int key, int x, int y )
{
    // bierzemy ostatni klocek z bufora
    Block* block = &(blocks.back());

    GLfloat freeSpace;
    switch( key )
    {
            // strzałka w lewo
        case GLUT_KEY_LEFT:
            freeSpace = blocks.freeSpaceOnTheLeftOfLastBlock();
            if(freeSpace > block->width)
                block->pos_x -= block->width;
            else
                block->pos_x -= freeSpace;
            break;
            
            // strzałka w górę
        case GLUT_KEY_UP:
            break;
            
            // strzałka w prawo
        case GLUT_KEY_RIGHT:
            freeSpace = blocks.freeSpaceOnTheRightOfLastBlock();
            if(freeSpace > block->width)
                block->pos_x += block->width;
            else
                block->pos_x += freeSpace;
            break;
            
            // strzałka w dół
        case GLUT_KEY_DOWN:
            freeSpace = blocks.freeSpaceBelowLastBlock();
            if(freeSpace > block->height)
                block->pos_y -= block->height;
            else
                block->pos_y -= freeSpace;
            break;
    }
    
    // odrysowanie okna
    Reshape( glutGet( GLUT_WINDOW_WIDTH ), glutGet( GLUT_WINDOW_HEIGHT ) );
}

void Animate(int arg)
{
    // bierzemy ostatni klocek z bufora
    Block* block = &(blocks.back());

    if(!block->isOnTheBottom()){
        block->pos_y -= 1;
    }

    glutPostRedisplay();

    if(!gameOver)
        glutTimerFunc(animationSpeed, Animate, 0);
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
    glutTimerFunc(animationSpeed, Animate, 0);
    
    // wprowadzenie programu do obsługi pętli komunikatów
    glutMainLoop();
    return 0;
}