#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#define PI          3.1415926535897932384626433832795
#define SIZE_WIN    500
#define SIZE_ORT    500.0
#define TN          3
#define RN          4

typedef struct Point {
        GLfloat      x;
        GLfloat      y;
} Point;

Point Tri[TN], Rec[RN];
Point COTri, CORec;

GLfloat theta = 0.0;
GLfloat xsc = 1.0, ysc = 1.0;
GLfloat scale_sign = 0.001;

void Init(void);
void Transform(void);
void KeyboardKey(GLubyte Key, GLint x, GLint y);
void SpecialKey(GLint Key, GLint x, GLint y);
void Idle(void);
void ConfigPoint(void);

int main(int argc, char** argv)
{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(SIZE_WIN, SIZE_WIN);
	glutInitWindowPosition(SIZE_WIN, 0);
	glutCreateWindow("Transform2D");
	
	Init();
	
	glutDisplayFunc(Transform);
	glutKeyboardFunc(KeyboardKey);
	glutSpecialFunc(SpecialKey);
	glutIdleFunc(Idle);

	glutMainLoop();
	
    return 0;
}

void Init(void)
{
     Point center;

     center.x = center.y = 0.0;
     
     ConfigPoint();
     
     // Calculate centre
     for (int i=0; i<TN; i++)
     {
         center.x += Tri[i].x;
         center.y += Tri[i].y;
     }
     COTri.x = center.x / TN;
     COTri.y = center.y / TN;

     center.x = center.y = 0.0;
     
     for (int i=0; i<RN; i++)
     {
         center.x += Rec[i].x;
         center.y += Rec[i].y;
     }
     CORec.x = center.x / RN;
     CORec.y = center.y / RN;
     
     glClearColor(0.0, 0.0, 0.0, 0.0);
     glPointSize(1.0);
     glMatrixMode(GL_PROJECTION);
     glLoadIdentity();
     gluOrtho2D(-SIZE_ORT, SIZE_ORT, -SIZE_ORT, SIZE_ORT);
     
}

void Transform(void)
{
     glClear(GL_COLOR_BUFFER_BIT);
     glColor3f(1.0, 1.0, 1.0);
     glBegin(GL_LINES);
          glVertex2f(-SIZE_ORT, 0.0);
          glVertex2f(SIZE_ORT, 0.0);
          glVertex2f(0.0, -SIZE_ORT);
          glVertex2f(0.0, SIZE_ORT);
     glEnd();
     
     glPushMatrix();
          glColor3f(0.0, 0.8, 0.0);
          glTranslatef(COTri.x, COTri.y, 0.0);
          glScalef(xsc, ysc, 1.0);
          glTranslatef(-COTri.x, -COTri.y, 0.0);
          glBegin(GL_POLYGON);
               for (int i=0; i<TN; i++)
                   glVertex2f(Tri[i].x, Tri[i].y);
          glEnd();
     glPopMatrix();
     
     glPushMatrix();
          glColor3f(1.0, 0.0, 1.0);
          glTranslatef(CORec.x, CORec.y, 0.0);
          glRotatef(theta, 0, 0, 1);
          glTranslatef(-CORec.x, -CORec.y, 0.0);
          glBegin(GL_POLYGON);
               for (int i=0; i<RN; i++)
                   glVertex2f(Rec[i].x, Rec[i].y);
          glEnd();          
     glPopMatrix();
     
     glFlush();
     
     glutSwapBuffers();
}

void KeyboardKey(GLubyte Key, GLint x, GLint y)
{
     switch (Key)
     {
            case 0x1B :
                 exit(0);
                 break;
            default :
                    break;
     }
}

void SpecialKey(GLint Key, GLint x, GLint y)
{
     switch (Key)
     {
            case GLUT_KEY_LEFT :
                 theta += 10.0;
                 glutPostRedisplay();
                 break;
            case GLUT_KEY_RIGHT :
                 theta -= 10.0;
                 glutPostRedisplay();
                 break;
            case GLUT_KEY_DOWN :
                 xsc += 0.1;
                 ysc += 0.1;
                 glutPostRedisplay();
                 break;
            case GLUT_KEY_UP :
                 xsc -= 0.1;
                 ysc -= 0.1;
                 glutPostRedisplay();
                 break;
            case GLUT_KEY_HOME :
                 xsc -= 0.1;
				 glutPostRedisplay();
				 break;
            case GLUT_KEY_END :
                 xsc += 0.1;
                 glutPostRedisplay();
                 break;
            default :
                    break;
     }
}

void Idle(void)
{
     if (theta >= 360)
        theta = 0;
     else
         theta += 0.1;
     
     if (xsc < 0.1)
        scale_sign = 0.001;
     if (xsc > 2.0)
        scale_sign = -0.001;
     xsc += scale_sign;
     ysc += scale_sign;

	glutPostRedisplay();
}

void ConfigPoint(void)
{
     Tri[0].x = -250.0;         Tri[0].y = -150.0;
     Tri[1].x = -350.0;         Tri[1].y = -350.0;
     Tri[2].x = -150.0;         Tri[2].y = -350.0;
     
     Rec[0].x = 150.0;         Rec[0].y = 150.0;
     Rec[1].x = 350.0;         Rec[1].y = 150.0;
     Rec[2].x = 350.0;         Rec[2].y = 350.0;
     Rec[3].x = 150.0;         Rec[3].y = 350.0;   
}
