/*
(c) Janusz Ganczarski
http://www.januszg.hg.pl
JanuszG(małpeczka)enter.net.pl
*/

#include <GL/glut.h>
#include <stdlib.h>

// rozmiary bryły obcinania


const GLdouble width = 200.0;
const GLdouble height = 200.0;

const GLdouble left = -10.0;
const GLdouble right = width - left;
const GLdouble bottom = -10.0;
const GLdouble top = height - bottom;
const GLdouble near = 50.0;
const GLdouble far = 70.0;

// współczynnik skalowania

GLfloat scale = 1.0;

// kąty obrotu

GLfloat translatey = 0.0;

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

    // przesunięcie układu współrzędnych obiektu do środka bryły odcinania
    glTranslatef( 0, 0, -( near + far ) / 2 );
    
    // przesunięcie obiektu - ruch myszką
    // glTranslatef( translatex, translatey, 0.0 );
    
    // kolor krawędzi obiektu
    glColor3f( 0.0, 0.0, 0.0 );
    
    // rysowanie obiektu
    DrawFrame(width, height);

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
        glOrtho( left, right, bottom * height / width, top * height / width, near, far );
    else

    // szerokość okna większa lub równa wysokości okna
    if( width >= height && height > 0 )
        glOrtho( left * width / height, right * width / height, bottom, top, near, far );
    
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
    switch( key )
    {
        // kursor w lewo
    case GLUT_KEY_LEFT:
        break;
        
        // kursor w górę
    case GLUT_KEY_UP:
        break;
        
        // kursor w prawo
    case GLUT_KEY_RIGHT:
        break;
        
        // kursor w dół
    case GLUT_KEY_DOWN:
        break;
    }
    
    // odrysowanie okna
    Reshape( glutGet( GLUT_WINDOW_WIDTH ), glutGet( GLUT_WINDOW_HEIGHT ) );
}

void Animate(int arg)
{
    // rotatey += 1;
    glutPostRedisplay();
    glutTimerFunc(45, Animate, 0);
}

int main( int argc, char * argv[] )
{
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