from OpenGL.GLUT import *
from OpenGL.GLU import *
from OpenGL.GL import *
import sys
import math as mt
import numpy as np

name = b'ball_glut'
Rcx = 5.;  Rcy = 5.
tcx = -5.;  tcy = -5.
xsc = 1.0
ysc = 1.0
sign_sc = 1.0
Rec = [[-3. + Rcx,-3.  + Rcy, 1], [-3. + Rcx, 3. + Rcy, 1], [3. + Rcx, 3. + Rcy, 1], [3. + Rcx,-3. + Rcy, 1]]
tri = [[-2. + tcx,-2.  + tcy, 1], [0. + tcx, 2. + tcy, 1], [2. + tcx, -2. + tcy, 1]]

theta = 0
sign = 1

def initial():
    global Rec, tri
    glClearColor(0., 0., 0., 1.)
    glColor3f(1., 1., 1.)
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE)
    glMatrixMode(GL_PROJECTION)
    glLoadIdentity()
    gluOrtho2D(-20, 20, -20 , 20)

def PushMatrix():
    return np.matrix(np.identity(3, dtype = float))  

def main():
    glutInit(sys.argv)
    glutInitWindowSize(800,800)
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE)
    glutCreateWindow(name)
    initial()
    glutDisplayFunc(display)
    glutIdleFunc(mytime)
    glutMainLoop()
    return

def MyTranslate(tran, tx, ty):
    ts = np.matrix(np.identity(3, dtype = float))
    ts[2,0] = tx
    ts[2,1] = ty
    return  ts*tran

def MyRotate(tran, theta):    
    theta = (theta * np.pi)/180 
    R = np.matrix(np.identity(3, dtype = float))
    R[0,0] = mt.cos(theta)
    R[1,0] = mt.sin(theta)
    R[0,1] = -mt.sin(theta)
    R[1,1] = mt.cos(theta)
    return  R*tran

def MyScale(tran, a, b):
    S = np.matrix(np.identity(3, dtype = float))
    S[0,0] = a
    S[1,1] = b
    return  S*tran

def mytime():
    global theta, sign
    global xsc, ysc, sign_sc
    theta = theta + (1.0 * sign)
    if ((theta >= 360.0) | (theta <= 0.0)):
        sign = sign * (-1)
    if ((xsc >= 2.0) | (xsc <= 0.0)):
        sign_sc = sign_sc * (-1)
    xsc = xsc + 0.09*sign_sc
    ysc = ysc + 0.09*sign_sc
    glutPostRedisplay()

def display():
    rM = np.matrix(np.array(Rec,float))
    tM = np.matrix(np.array(tri,float))
    r, c = rM.shape 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)    
    tran = PushMatrix()
    tran = MyTranslate(tran, 5., 5.)
    tran = MyRotate(tran, theta)
    tran = MyTranslate(tran, -5., -5.)
    glBegin(GL_POLYGON)
    t = rM*tran
    for i in range(r):
        glVertex2f(t[i,0], t[i,1])
    glEnd()

    tran = PushMatrix()
    tran = MyTranslate(tran, -5., -5.)
    tran = MyScale(tran, xsc, ysc)
    tran = MyTranslate(tran, 5., 5.)
    r, c = tM.shape 
    glBegin(GL_TRIANGLES)
    t = tM*tran
    for i in range(r):
        glVertex2f(t[i,0], t[i,1])
    glEnd()

    glutSwapBuffers()
    return

if __name__ == '__main__':
    print(tri)
    main()