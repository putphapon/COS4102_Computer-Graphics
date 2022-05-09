#include<GL/gl.h>
#include<GL/glu.h>
#include<GL/glut.h>
#include<cmath>


#define MAX         100
#define SIZE_W      500
#define SIZEMX      500.0

typedef struct p {
        float  x;
        float  y;
} Point;

Point   p[MAX], p_tmp[MAX];

double  Xmin = -100.0;
double  Xmax = 100.0;
double  Ymin = -100.0;
double  Ymax = 100.0;
double  center_xy = (Xmax - Xmin) / 2.0;
double  mouse_x, mouse_y;

int     n;
int     mouse_State = 1;

char    *side_list[] = {"Left", "Right", "Bootom", "Top"};

void ClearAll(void)
{
     for (int i = 0; i < MAX; i++)
         p[i].x = p[i].y = p_tmp[i].x = p_tmp[i].y = 0.0;
}

void SetPoint(void)
{
     p[0].x = 0.0;         p[0].y = 0.0;
     p[1].x = -200.0;      p[1].y = 0.0;
     p[2].x = 200.0;       p[2].y = 150.0;
     p[3].x = 150.0;       p[3].y = 0.0;
     p[4].x = 200.0;       p[4].y = -150.0;

     n = 5;
}

void init(void)
{
     glClearColor(0.0, 0.0, 0.0, 1.0);
     glPointSize(1.0);
     glMatrixMode(GL_PROJECTION);
     glLoadIdentity();
     gluOrtho2D(-SIZEMX, SIZEMX, -SIZEMX, SIZEMX);

     SetPoint();
}

void myDrawObject(void)
{
     glClear(GL_COLOR_BUFFER_BIT);
     glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

     glColor3f(1.0, 1.0, 1.0);
     glBegin(GL_LINES);
          glVertex2f(-SIZEMX, 0.0);
          glVertex2f(SIZEMX, 0.0);
          glVertex2f(0.0, -SIZEMX);
          glVertex2f(0.0, SIZEMX);
     glEnd();

     glPushMatrix();
     glColor3f(0.0, 0.8, 0.0);
     glBegin(GL_POLYGON);
          glVertex2f(Xmin, Ymin);
          glVertex2f(Xmin, Ymax);
          glVertex2f(Xmax, Ymax);
          glVertex2f(Xmax, Ymin);
     glEnd();
     glPopMatrix();

     glPushMatrix();
          glColor3f(1.0, 0.0, 0.0);
          glBegin(GL_POLYGON);
               for (int i = 0; i < n; i++)
                   glVertex2f(p[i].x, p[i].y);
          glEnd();
     glPopMatrix();

     glFlush();
}

void ClipSide(Point s, Point p, int &j, int side)
{
     GLfloat dy = p.y - s.y;
     GLfloat dx = (p.x != s.x)? p.x - s.x : 0.0000000000000001;

     switch (side)
     {
            case 0 :                // Left side
                 if (p.x >= Xmin)
                 {
                    if (s.x < Xmin)
                    {
                       p_tmp[j].y = p.y + (Xmin - p.x) * (dy / dx);
                       p_tmp[j++].x = Xmin;
                    }
                    p_tmp[j++] = p;
                 }
                 else
                     if (s.x > Xmin)
                     {
                          p_tmp[j].y = s.y + (Xmin - s.x) * (dy / dx);
                          p_tmp[j++].x = Xmin;
                     }
                 break;
            case 1 :                // Right side
                 if (p.x <= Xmax)
                 {
                    if (s.x > Xmax)
                    {
                       p_tmp[j].y = p.y + (Xmax - p.x) * (dy / dx);
                       p_tmp[j++].x = Xmax;
                    }
                    p_tmp[j++] = p;
                 }
                 else
                     if (s.x < Xmax)
                     {
                        p_tmp[j].y = s.y + (Xmax - s.x) * (dy / dx);
                        p_tmp[j++].x = Xmax;
                     }
                 break;
            case 2 :                // Bottom side
                 if (p.y >= Ymin)
                 {
                    if (s.y < Ymin)
                    {
                       p_tmp[j].x = p.x + (Ymin - p.y) / (dy / dx);
                       p_tmp[j++].y = Ymin;
                    }
                    p_tmp[j++] = p;
                 }
                 else
                     if (s.y > Ymin)
                     {
                        p_tmp[j].x = s.x + (Ymin - s.y) / (dy / dx);
                        p_tmp[j++].y = Ymin;
                     }
                 break;
            case 3 :                // Top side
                 if (p.y <= Ymax)
                 {
                    if (s.y > Ymax)
                    {
                       p_tmp[j].x = p.x + (Ymax - p.y) / (dy / dx);
                       p_tmp[j++].y = Ymax;
                    }
                    p_tmp[j++] = p;
                 }
                 else
                     if (s.y < Ymax)
                     {
                        p_tmp[j].x = s.x + (Ymax - s.y) / (dy / dx);
                        p_tmp[j++].y = Ymax;
                     }
                 break;
     }
}

void ClipObj(void)
{
     for (int side = 0; side < 4; side++)
     {
         int j = 0;

         for (int iterate = 0; iterate < n; iterate++)
         {
            if (iterate < (n - 1))
               ClipSide(p[iterate], p[iterate + 1], j, side);
            else
               ClipSide(p[iterate], p[0], j, side);
         }   // For iterate

         for (int i = 0; i < j; i++)
         {
             p[i] = p_tmp[i];
             p_tmp[i].x = p_tmp[i].y = 0.0;
         }
         n = j;
     }   // For side
}

// Keyboard is control the object ex. moving object, clipping, reset object etc.
void KeyboardKey(GLubyte Key, GLint x, GLint y)
{
     switch (Key)
     {
            case 0x1B :               // Press Esc to exit
                 exit(0);
                 break;
            case 0x0D :               // Press Enter to Clipping Object
                 ClipObj();
                 break;
            case 0x6E :              // Press 'n' or 'N' to Reset Object
                 ClearAll();
                 SetPoint();
                 Xmin = -100.0;
                 Xmax = 100.0;
                 Ymin = -100.0;
                 Ymax = 100.0;
                 center_xy = (Xmax - Xmin) / 2.0;
                 break;
            case 0x2B :              // Press '+' to Increment size the moving window
                 if ((Xmax - Xmin) < (SIZEMX * 2.0))
                 {
                      Xmin -= 1.0;
                      Xmax += 1.0;
                      Ymin -= 1.0;
                      Ymax += 1.0;
                      center_xy = (Xmax - Xmin) / 2.0;
                 }
                 break;
            case 0x2D :              // Press '-' to Decrement size the moving window
                 if ((Xmax - Xmin) > 5.0)
                 {
                      Xmin += 1.0;
                      Xmax -= 1.0;
                      Ymin += 1.0;
                      Ymax -= 1.0;
                      center_xy = (Xmax - Xmin) / 2.0;
                 }
                 break;
     }      // Switch
     glutPostRedisplay();
}

// Key direction is control the moving window
void SpecialKey(GLint Key, GLint x, GLint y)
{
     switch (Key)
     {
            case GLUT_KEY_LEFT :
                 if (Xmin > -SIZEMX)
                 {
                    Xmin -= 1.0;
                    Xmax -= 1.0;
                 }
                 break;
            case GLUT_KEY_RIGHT :
                 if (Xmax < SIZEMX)
                 {
                    Xmin += 1.0;
                    Xmax += 1.0;
                 }
                 break;
            case GLUT_KEY_DOWN :
                 if (Ymin > -SIZEMX)
                 {
                    Ymin -= 1.0;
                    Ymax -= 1.0;
                 }
                 break;
            case GLUT_KEY_UP :
                 if (Ymax < SIZEMX)
                 {
                    Ymin += 1.0;
                    Ymax += 1.0;
                 }
                 break;
            default :
                    break;
     }           // Switch
     glutPostRedisplay();
}

void MouseAssign(GLint button, GLint state, GLint x, GLint y)
{

     switch (button)
     {
            case GLUT_LEFT_BUTTON :
                 if (state == GLUT_DOWN)
                 {
                    ClipObj();
                 }
                 break;
            case GLUT_RIGHT_BUTTON :
                 if (state == GLUT_DOWN)
                 {
                    ClearAll();
                    SetPoint();
                    Xmin = -100.0;
                    Xmax = 100.0;
                    Ymin = -100.0;
                    Ymax = 100.0;
                    center_xy = (Xmax - Xmin) / 2.0;
                 }
                 break;
            default :
                    break;
     }              // Switch

     mouse_State = (state == GLUT_ENTERED)? GLUT_ENTERED : GLUT_LEFT;
     mouse_x = (2.0 * x) - SIZEMX;
     mouse_y = SIZEMX - (2.0 * y);

     glutPostRedisplay();
}

void PassiveMouseMotion(GLint x, GLint y)
{
     if (mouse_State)
     {
        if (((mouse_x - center_xy) > -SIZEMX) && ((mouse_x + center_xy) < SIZEMX))
        {
           Xmin = mouse_x - center_xy;
           Xmax = mouse_x + center_xy;
        }
        if (((mouse_y - center_xy) > -SIZEMX) && ((mouse_y + center_xy) < SIZEMX))
        {
           Ymin = mouse_y - center_xy;
           Ymax = mouse_y + center_xy;
        }
        mouse_x = (2.0 * x) - SIZEMX;
        mouse_y = SIZEMX - (2.0 * y);
     }
     glutPostRedisplay();
}

int main(int argc, char** argv)
{
    ClearAll();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(SIZE_W, SIZE_W);
    glutInitWindowPosition(SIZE_W, 100);
    glutCreateWindow("My Clipping Object");

    init();

    glutDisplayFunc(myDrawObject);
    glutKeyboardFunc(KeyboardKey);
    glutSpecialFunc(SpecialKey);
    glutMouseFunc(MouseAssign);
    glutPassiveMotionFunc(PassiveMouseMotion);

    glutMainLoop();

    return 0;
}
