#include <GL/glut.h>
#include <stdio.h>
#include <conio.h>
#include <math.h>

#define PI          3.1415926535897932384626433832795
#define SIZE_WIN    500
#define SIZE_ORT    500.0
#define RAD(X)      (X*PI)/180
#define TN          3
#define RN          4

typedef GLfloat Matrix3x[3][3];
typedef struct Coordinate{
        GLfloat x;
        GLfloat y;
        GLfloat w;
} Point;

Point Tri[TN], Tri_Trans[TN];
Point Tri2[TN], Tri_Trans2[TN];
Point Rec[RN], Rec_Trans[RN];

Point CenTri, CenTri2, CenRec;

Matrix3x TrnMTx;

GLfloat Sx = 1.0, Sy = 1.0;
GLfloat scale_sign = 0.001;
GLfloat theta = 0.0;
GLfloat Shear = 1.0;
GLfloat theta_sign = 0.1;
GLfloat shear_sign = 0.001;

GLint   Axis = 0;
GLint   flag = 1;

void ConfigPoint(void);
void Centroid(Point Obj[], GLint n, GLfloat &cx, GLfloat &cy);
void Init(void);
void myTransform2D(void);
void KeyboardKey(GLubyte Key, GLint x, GLint y);
void SpecialKey(GLint Key, GLint x, GLint y);
void Idle(void);
void iMatrix(Matrix3x IdenMx);
void mulMatrix(Matrix3x MTx1, const Matrix3x MTx2);
void myTranslatef(Matrix3x MTx1, GLfloat tx, GLfloat ty);
void myRotatef(Matrix3x MTx1, GLfloat theta);
void myScalef(Matrix3x MTx1, GLfloat sx, GLfloat sy);
void myShearf(Matrix3x MTx1, GLint Axis, GLfloat shear);

int main(int argc, char** argv)
{
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
    glutInitWindowSize(SIZE_WIN, SIZE_WIN);
    glutInitWindowPosition(SIZE_WIN, 0);
    glutCreateWindow("Transform2D");

    Init();

    glutDisplayFunc(myTransform2D);
    glutKeyboardFunc(KeyboardKey);
    glutSpecialFunc(SpecialKey);
    glutIdleFunc(Idle);

    glutMainLoop();

    return 0;
}

void ConfigPoint(void)
{
     Tri[0].x = -250.0;           Tri[0].y = -150.0;
     Tri[1].x = -350.0;           Tri[1].y = -350.0;
     Tri[2].x = -150.0;           Tri[2].y = -350.0;

     Tri2[0].x = -250.0;          Tri2[0].y = 350.0;
     Tri2[1].x = -350.0;          Tri2[1].y = 150.0;
     Tri2[2].x = -150.0;          Tri2[2].y = 150.0;

     Rec[0].x = 150.0;         Rec[0].y = 150.0;
     Rec[1].x = 350.0;         Rec[1].y = 150.0;
     Rec[2].x = 350.0;         Rec[2].y = 350.0;
     Rec[3].x = 150.0;         Rec[3].y = 350.0;

     for (int i=0; i<TN; i++)
     {
         Tri_Trans[i].x = Tri_Trans[i].y = 0.0;
         Tri[i].w = Tri_Trans[i].w = 1.0;
         Tri_Trans2[i].x = Tri_Trans2[i].y = 0.0;
         Tri2[i].w = Tri_Trans2[i].w = 1.0;
     }

     for (int i=0; i<RN; i++)
     {
         Rec_Trans[i].x = Rec_Trans[i].y = 0.0;
         Rec[i].w = Rec_Trans[i].w = 1.0;
     }
}

void Centroid(Point Obj[], GLint n, GLfloat &cx, GLfloat &cy)
{
     GLfloat sumCenterX, sumCenterY;

     sumCenterX = sumCenterY = 0.0;

     for (int i=0; i<n; i++)
     {
         sumCenterX += Obj[i].x;
         sumCenterY += Obj[i].y;
     }

     cx = sumCenterX / n;
     cy = sumCenterY / n;
}

void Init(void)
{
     ConfigPoint();
     // Calculate centroid of Triangle
     Centroid(Tri, TN, CenTri.x, CenTri.y);
     Centroid(Rec, RN, CenRec.x, CenRec.y);
     Centroid(Tri2, TN, CenTri2.x, CenTri2.y);

     glClearColor(0.0, 0.0, 0.0, 0.0);
     glPointSize(1.0);
     glMatrixMode(GL_PROJECTION);
     glLoadIdentity();
     gluOrtho2D(-SIZE_ORT, SIZE_ORT, -SIZE_ORT, SIZE_ORT);
}

void myTransform2D(void)
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
          iMatrix(TrnMTx);
          myTranslatef(TrnMTx, CenTri.x, CenTri.y);
          myScalef(TrnMTx, Sx, Sy);
          myTranslatef(TrnMTx, -CenTri.x, -CenTri.y);
          glBegin(GL_POLYGON);
               for (int i=0; i<TN; i++)
               {
                   Tri_Trans[i].x = (Tri[i].x * TrnMTx[0][0]) + (Tri[i].y * TrnMTx[0][1]) + (Tri[i].w * TrnMTx[0][2]);
                   Tri_Trans[i].y = (Tri[i].x * TrnMTx[1][0]) + (Tri[i].y * TrnMTx[1][1]) + (Tri[i].w * TrnMTx[1][2]);
                   glVertex2f(Tri_Trans[i].x, Tri_Trans[i].y);
               }
          glEnd();
     glPopMatrix();

     glPushMatrix();
          glColor3f(0.0, 0.8, 0.8);
          iMatrix(TrnMTx);
          myTranslatef(TrnMTx, CenTri2.x, CenTri2.y);
          myShearf(TrnMTx, Axis, Shear);
          myTranslatef(TrnMTx, -CenTri2.x, -CenTri2.y);
          glBegin(GL_POLYGON);
               for (int i=0; i<TN; i++)
               {
                   Tri_Trans2[i].x = (Tri2[i].x * TrnMTx[0][0]) + (Tri2[i].y * TrnMTx[0][1]) + (Tri2[i].w * TrnMTx[0][2]);
                   Tri_Trans2[i].y = (Tri2[i].x * TrnMTx[1][0]) + (Tri2[i].y * TrnMTx[1][1]) + (Tri2[i].w * TrnMTx[1][2]);
                   glVertex2f(Tri_Trans2[i].x, Tri_Trans2[i].y);
               }
          glEnd();
     glPopMatrix();

     glPushMatrix();
          glColor3f(1.0, 0.0, 1.0);
          iMatrix(TrnMTx);
          myTranslatef(TrnMTx, CenRec.x, CenRec.y);
          myRotatef(TrnMTx, theta);
          myTranslatef(TrnMTx, -CenRec.x, -CenRec.y);
          glBegin(GL_POLYGON);
               for (int i=0; i<RN; i++)
               {
			       Rec_Trans[i].x = (Rec[i].x * TrnMTx[0][0]) + (Rec[i].y * TrnMTx[0][1]) + (Rec[i].w * TrnMTx[0][2]);
			       Rec_Trans[i].y = (Rec[i].x * TrnMTx[1][0]) + (Rec[i].y * TrnMTx[1][1]) + (Rec[i].w * TrnMTx[1][2]);
                   glVertex2f(Rec_Trans[i].x, Rec_Trans[i].y);
               }
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
                 if (theta >= 360.0) theta = 0.0;
                 theta += 10.0;
                 glutPostRedisplay();
                 break;
            case GLUT_KEY_RIGHT :
                 if (theta <= -360.0) theta = 0.0;
                 theta -= 10.0;
                 glutPostRedisplay();
                 break;
            case GLUT_KEY_UP :
                 if (Sx <= 2.0)
                 {
                    Sx += 0.1;
                    Sy += 0.1;
                 }
                 glutPostRedisplay();
                 break;
            case GLUT_KEY_DOWN :
                 if (Sx >= 0.1)
                 {
                    Sx -= 0.1;
                    Sy -= 0.1;
                 }
                 glutPostRedisplay();
                 break;
            default :
                    break;
     }
}

void Idle(void)
{
     if (theta >= 360.0)
     {
        Axis = 1;
        Shear = 0.0;
        theta_sign = -0.1;
     }
     if (theta <= -360.0)
     {
        Axis = 0;
        Shear = 0.0;
        theta_sign = 0.1;
     }

     theta += theta_sign;

     if (Sx <= 0.1)
        scale_sign = 0.001;
     if (Sx >= 2.0)
        scale_sign = -0.001;

     Sx += scale_sign;
     Sy += scale_sign;


     if (Shear >= 1.0)
        shear_sign = -0.001;
     if (Shear <= -1.0)
        shear_sign = 0.001;

     Shear += shear_sign;

     glutPostRedisplay();
}

void iMatrix(Matrix3x IdenMx)
{
     for (int i=0; i<3; i++)
         for (int j=0; j<3; j++)
             IdenMx[i][j] = (i == j);
}

void mulMatrix(Matrix3x MTx1, const Matrix3x MTx2)
{
     Matrix3x temp = {0};

     for (int i=0; i<3; i++)
         for (int j=0; j<3; j++)
             for (int k=0; k<3; k++)
                 temp[i][j] += MTx1[i][k] * MTx2[k][j];
     for (int i=0; i<3; i++)
         for (int j=0; j<3; j++)
             MTx1[i][j] = temp[i][j];
}

void myTranslatef(Matrix3x MTx1, GLfloat tx, GLfloat ty)
{
     Matrix3x MTx2;

     iMatrix(MTx2);
     MTx2[0][2] = tx;
     MTx2[1][2] = ty;

     mulMatrix(MTx1, MTx2);
}

void myRotatef(Matrix3x MTx1, GLfloat theta)
{
     Matrix3x MTx2;

     iMatrix(MTx2);
     MTx2[0][0] = cos(RAD(theta));
     MTx2[1][0] = -sin(RAD(theta));
     MTx2[0][1] = sin(RAD(theta));
     MTx2[1][1] = cos(RAD(theta));

     mulMatrix(MTx1, MTx2);
}

void myScalef(Matrix3x MTx1, GLfloat sx, GLfloat sy)
{
     Matrix3x MTx2;

     iMatrix(MTx2);
     MTx2[0][0] = sx;
     MTx2[1][1] = sy;

     mulMatrix(MTx1, MTx2);
}

void myShearf(Matrix3x MTx1, GLint Axis, GLfloat shear)
{
     Matrix3x MTx2;

     iMatrix(MTx2);

     switch (Axis)
     {
            case 0 :               // On X Axis
                 MTx2[1][0] = shear;
                 break;
            case 1:               // On Y Axis
                 MTx2[0][1] = shear;
                 break;
            default :
                    break;
     }

     mulMatrix(MTx1, MTx2);
}
