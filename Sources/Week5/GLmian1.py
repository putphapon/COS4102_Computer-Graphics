from OpenGL.GLUT import *
from OpenGL.GLU import *
from OpenGL.GL import *
import sys
import numpy as np

name = b'ball_glut'
Rcx = 5.;  Rcy = 5.

xsc = 1.0
ysc = 1.0
sign_sc = 1.0
Rec = [[2, 2], [2,5], [5,5],[5,2]] 

theta = 0
sign = 1

def initial():
    glClearColor(0., 0., 0., 1.)
    glColor3f(1., 1., 1.)
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE)
    glMatrixMode(GL_PROJECTION)
    glLoadIdentity()
    gluOrtho2D(-10, 10, -10 , 10)

def main():
    glutInit(sys.argv)
    glutInitWindowSize(800,800)
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE)
    glutCreateWindow(name)
    initial()
    glutDisplayFunc(display)
    glutMainLoop()
    return

def display():
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
    glPushMatrix()
    r,c = Rec.shape

    
    glBegin(GL_POLYGON)
    for i in range(r):
        glVertex2f(Rec[i,0], Rec[i,1])
    glEnd()
    glPopMatrix()


    glutSwapBuffers()
    return

def mytime():
    global theta, sign
    global xsc, ysc, sign_sc
    theta = theta + (0.2 * sign)
    if ((theta >= 360.0) | (theta <= 0.0)):
        sign = sign * (-1)
    if ((xsc >= 2.0) | (xsc <= 0.0)):
        sign_sc = sign_sc * (-1)
    xsc = xsc + 0.01*sign_sc
    ysc = ysc + 0.01*sign_sc
    glutPostRedisplay()

def keyboard(key, x, y):
    pass

def numpad(key, x, y):
    pass

if __name__ == '__main__': 
    Rec = np.matrix(np.array(Rec,float))
    main()