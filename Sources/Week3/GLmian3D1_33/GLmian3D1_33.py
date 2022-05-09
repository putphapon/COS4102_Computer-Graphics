from OpenGL.GLUT import *
from OpenGL.GLU import *
from OpenGL.GL import *
import sys
import numpy as np
import math as m

name = b'ball_glut'
fill, view = True, True
width, height = 500, 500
theta, sign = 0.0, 1.0

def config():
    global sur
    sf1 = [[-1.0, -1.0, 1.0], [-1.0, 1.0, 1.0], [1.0, 1.0, 1.0], [1.0, -1.0, 1.0]]
    sf2 = [[-1.0, 1.0, 1.0], [-1.0, 1.0, -1.0], [1.0, 1.0, -1.0], [1.0, 1.0, 1.0]]
    sf3 = [[-1.0, -1.0, 1.0], [-1.0, 1.0, 1.0], [-1.0, 1.0, -1.0], [-1.0, -1.0, -1.0]]
    sf1 = np.array(sf1,float)
    sf2 = np.array(sf2,float)
    sf3 = np.array(sf3,float)
    (n,p) = sf1.shape
    sf4 = np.copy(sf1)
    sf5 = np.copy(sf2)
    sf6 = np.copy(sf3)
    for i in range(n):
        sf4[i,2] = sf1[i,2]*-1
        sf5[i,1] = sf2[i,1]*-1
        sf6[i,0] = sf3[i,0]*-1
    sur = np.array((sf1, sf2, sf3, sf4, sf5, sf6))

def InitGL():
    glClearColor(0.0, 0.0, 0.0, 0.0)
    glEnable(GL_DEPTH_TEST)

def SetGLScene():
    if fill:
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL)
    else:
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE)

    glViewport(0, 0, width, height)
    glMatrixMode(GL_PROJECTION)
    glLoadIdentity()
    if view:
        gluPerspective(90., 1., 0.5, 80.)
    else:
        glOrtho(-10, 10, -10, 10, -10, 10)
    #gluPerspective(90.0, float(width) / float(height), 0.1, 100.0)
    glMatrixMode(GL_MODELVIEW)
    glLoadIdentity()
    gluLookAt(0, 0, 8, 0, 0, 0, 0, 1, 0)

def Surface(sx,sy,sz,r,g,b):
    glBegin(GL_QUADS)
    (m,n,o) = sur.shape
    for i in range(m):
        r[0] = r[0] + 0.1; b[0] = b[0] - 0.1
        glColor3f(r[0], g[0], b[0])
        for j in range(n):
            glVertex3f(sur[i,j,0] + sx, sur[i,j,1] + sy, sur[i,j,2]+ sz)
    glEnd()

def DrawGLScene():
    glClearColor(0.0, 0.0, 0.0, 0.0)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
    SetGLScene()
    glPushMatrix()
    glRotatef(theta,0,1,0)
    Surface(0,0,0,[0.4],[0.3],[0.9])
    glPopMatrix()

    glPushMatrix()
    glRotatef(theta,1,0,0)
    Surface(-5, 0, -5, [0.1], [0.8], [0.7])
    glPopMatrix()

    glPushMatrix()
    glRotatef(-1*theta,1,0,0)
    Surface( 5, 0, -5, [0.3], [0.5], [0.6])
    glPopMatrix()
    glutSwapBuffers()

def On_keyboard(key, x, y):
    global fill, view
    while True:
        if key == b'\x1b':
            sys.exit()
        if key == b'f':
            fill = not fill
            break
        if key == b'v':
            view = not view
            break
        break

    glutPostRedisplay()

def MyIdle():
    global theta, sign
    theta = theta + (0.2*sign)
    if ((theta >= 360.0) | (theta <= 0.0)):
        sign = sign*(-1)
    glutPostRedisplay()

def On_reshape(w, h):
    global width, height
    width, height = w, h

def main():
    glutInit(sys.argv)
    glutInitWindowSize(width, height)
    glutInitWindowPosition(0, 0)
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH)
    glutCreateWindow(b'cube')
    InitGL()
    glutDisplayFunc(DrawGLScene)
    glutKeyboardFunc(On_keyboard)
    glutReshapeFunc(On_reshape)
    glutIdleFunc(MyIdle)
    glutMainLoop()

if __name__ == "__main__":
    config()
    main()