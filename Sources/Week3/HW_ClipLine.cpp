// Program cliping line is clip line out of window
#include <GL/glut.h>
//#include <GL/glaux.h>
#include <stdio.h>
#include <ctype.h>

#define  ENTER    13
#define  plus     '+'
#define  minus    '-'
#define  SIZEW    600
#define  SIZEMX   600.0

struct p {
       GLfloat      x;
       GLfloat      y;
};

struct p            p1, p2;

int skip = 1;
// Initial moving window coordinate standard size 50 px
double Xmin = -100.0;
double Xmax = 100.0;
double Ymin = -100.0;
double Ymax = 100.0;
// Use to Encode function
GLubyte     Codewinleft    = 0x01;
GLubyte     Codewinright   = 0x02;
GLubyte     Codewinbottom  = 0x04;
GLubyte     Codewintop     = 0x08;

void init()
{
     glClearColor(0.0, 0.0, 0.0, 1.0);
     glPointSize(1.0);
     gluOrtho2D(-SIZEMX, SIZEMX, -SIZEMX, SIZEMX);
     p1.x = -400.0; p1.y = -400.0;
     p2.x = 400.0; p2.y = 400.0;
}

void myDrawLine()
{
     glClear(GL_COLOR_BUFFER_BIT);        // Clear window
     // Axis X and Y
     glColor3f(1.0, 1.0, 1.0);
     glBegin(GL_LINES);
          glVertex2f(-SIZEMX, 0.0);
          glVertex2f(SIZEMX, 0.0);
          glVertex2f(0.0, SIZEMX);
          glVertex2f(0.0, -SIZEMX);
     glEnd();
     // Rectangle
     glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
     glBegin(GL_POLYGON);
          glColor3f(0.0, 1.0, 0.0);
          glVertex2f(Xmin,Ymin);
          glVertex2f(Xmin,Ymax);
          glVertex2f(Xmax,Ymax);
          glVertex2f(Xmax,Ymin);
     glEnd();
     // Draw Line to cliping
     if (skip)
     {
        glPushMatrix();
             glBegin(GL_LINES);
                  glColor3f(1.0, 0.0, 0.0);
                  glVertex2f(p1.x, p1.y);
                  glVertex2f(p2.x, p2.y);
             glEnd();
        glPopMatrix();
     }

     glFlush();                         // Force update of screen
}

int Encode(GLfloat x, GLfloat y)
{
    GLubyte code = 0x00;
    
    if (x < Xmin)           // Point is on Left
       code = code | Codewinleft;
    if (x > Xmax)           // Point is on Right
       code = code | Codewinright;
    if (y < Ymin)           // Point is on Bottom
       code = code | Codewinbottom;
    if (y > Ymax)           // Point is on Top
       code = code | Codewintop;
    return code;
}

int Accept(GLubyte code1, GLubyte code2)
{
    if (code1 | code2)
       return 0;
    return 1;
}

int Reject(GLubyte code1, GLubyte code2)
{
    if (code1 & code2)
       return 1;
    return 0;
}

void SwapPoint(GLubyte code1, GLubyte code2)
{
     struct p     pointtmp;
     GLubyte      codetmp;
     
     // Swap coordinate p1 and p2
     pointtmp.x = p1.x;
     p1.x       = p2.x;
     p2.x       = pointtmp.x;
     
     // Swap coordinate p1 and p2 
     pointtmp.y = p1.y;
     p1.y       = p2.y;
     p2.y       = pointtmp.y;

     // Swap code p1 and p2
     codetmp = code1;
     code1   = code2;
     code2   = codetmp;
}

void ClipLine(void)
{
     int flag = 0;
     
     GLubyte  code1 = 0x00;
     GLubyte  code2 = 0x00;

     while (!flag)
     {
           // Initial 2 coordinate to code
           code1 = Encode(p1.x, p1.y);
           code2 = Encode(p2.x, p2.y);
           
//           printf("\n\nCode1: %4X \nCode2: %4X", code1, code2);
           if (Accept(code1, code2))                // Accept Line
              flag = 1;
           else if (Reject(code1, code2))           // Reject Line
                {
                   flag = 1;
                   skip = 0;
                }
                else                               // Intersection coordinate Line
                {
                    // Calculate new point
                    if (code1 == 0x00)
                       SwapPoint(code1, code2);
                    
                    // Calculate new coordinate of p1
                    GLfloat dy = p2.y - p1.y;
                    GLfloat dx = (p2.x != p1.x)? (p2.x - p1.x) : 0.00000000000000000001;
                    GLfloat m  = dy/dx;
                    
                    // Line is moving on axis X
                    if (code1 & Codewinleft)
                    {
                       if (dy != 0)
                          p1.y += (Xmin - p1.x) * m;
                       p1.x = Xmin;
                    }
                    if (code1 & Codewinright)
                    {
                       if (dy != 0)
                          p1.y += (Xmax - p1.x) * m;
                       p1.x = Xmax;
                    }
                    if (code1 & Codewinbottom)
                    {
                       if (dx != 0)
                          p1.x += (Ymin - p1.y) * m;
                       p1.y = Ymin;
                    }
                    if (code1 & Codewintop)
                    {
                       if (dx != 0)
                          p1.x += (Ymax - p1.y) * m;
                       p1.y = Ymax;
                    }                    
                }
     }
}

void KeyboardKey(GLubyte Key, GLint X, GLint Y)
{
     switch(tolower(Key))
     {
           case 27 :
                exit(0);
                break;
           case ENTER :
                ClipLine();
                glutPostRedisplay();
                break;
           case 'n' :               // Press n to new
                skip = 1;
                p1.x = p1.y = -400.0;
                p2.x = p2.y = 400.0;
                glutPostRedisplay();
                break;
           case 'x' :                        // Horizontal Line
                skip = 1;
                p1.x = 100.0; p1.y = -400.0;
                p2.x = 100.0; p2.y = 400.0;
                glutPostRedisplay();
                break;
           case 'y' :                        // Vertical Line
                skip = 1;
                p1.x = -400.0; p1.y = 100.0;
                p2.x = 400.0; p2.y = 100.0;
                glutPostRedisplay();
                break;
           case 'w' :                        // UP
                 if (Ymax < SIZEMX)
                 {
                    Ymin += 2.0;
                    Ymax += 2.0;
                    glutPostRedisplay();
                 }
                break;
           case 's' :                        // DOWN
                 if (Ymin > -SIZEMX)
                 {
                    Ymin -= 2.0;
                    Ymax -= 2.0;
                    glutPostRedisplay();
                 }
                break;
           case 'a' :                        // LEFT
                 if (Xmin > -SIZEMX)
                 {
                    Xmin -= 2.0;
                    Xmax -= 2.0;
                    glutPostRedisplay();
                 }
                break;
           case 'd' :                        // RIGHT
                 if (Xmax < SIZEMX)
                 {
                    Xmin += 2.0;
                    Xmax += 2.0;
                    glutPostRedisplay();
                 }
                break;
           case minus :                      // Decrement Rectangle minimum size 10.00
                if ((Xmax - Xmin) > 30.0)
                {
                   Xmin += 1.0;
                   Xmax -= 1.0;
                   Ymin += 1.0;
                   Ymax -= 1.0;
                   glutPostRedisplay();
                }
                break;
           case plus :                       // Increment Rectangle maximun size 300.00
                if ((Xmax - Xmin) < 300.0)
                {
                   Xmin -= 1.0;
                   Xmax += 1.0;
                   Ymin -= 1.0;
                   Ymax += 1.0;
                   glutPostRedisplay();
                }
                break;
     }     // switch          
}

void SpecialKey(GLint Key, GLint X, GLint Y)
{
     switch (Key)
     {
            case GLUT_KEY_LEFT :
                 if (Xmin > -SIZEMX)
                 {
                    Xmin -= 2.0;
                    Xmax -= 2.0;
                    glutPostRedisplay();
                 }
                 break;
            case GLUT_KEY_RIGHT :
                 if (Xmax < SIZEMX)
                 {
                    Xmin += 2.0;
                    Xmax += 2.0;
                    glutPostRedisplay();
                 }
                 break;
            case GLUT_KEY_UP :
                 if (Ymax < SIZEMX)
                 {
                    Ymin += 2.0;
                    Ymax += 2.0;
                    glutPostRedisplay();
                 }
                 break;
            case GLUT_KEY_DOWN:
                 if (Ymin > -SIZEMX)
                 {
                    Ymin -= 2.0;
                    Ymax -= 2.0;
                    glutPostRedisplay();
                 }
                 break;
     }      // switch
}

int main(int argc, char** argv)
{
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);        // Initialize display mode
    glutInitWindowSize(SIZEW, SIZEW);                   // Initialize windows sizw
    glutInitWindowPosition(SIZEW, 0);                   // Initialize window position
    glutCreateWindow("Clip Line");                      // Create window and caption
    
    init();
    
    glutDisplayFunc(myDrawLine);
    glutKeyboardFunc(KeyboardKey);
    glutSpecialFunc(SpecialKey);

    glutMainLoop();
    
    return 0;
}
