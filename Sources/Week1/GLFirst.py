from OpenGL.GL import *
from OpenGL.GLUT import *
from OpenGL.GLU import *
import numpy as np
import sys
k = 0
def InitGL():
    glColor3f(1.0, 1.0, 1.0)
    glClearColor(0.0, 0.0, 0.0, 0.0)

def DrawGLScene():
    glClear(GL_COLOR_BUFFER_BIT)
    glBegin(GL_LINES)
    glVertex2f(-1.0,-1.0)
    glVertex2f( 1.0, 1.0)
    glVertex2f( 2.0, 3.0)
    glEnd()
    glFlush()
#    glutSwapBuffers()

def MyIdle():
    global k
    k = k + 1
    print(k)

def on_keyboard(key, x, y):
    global k
    k = k + 1
    print(k)

def main():
    glutInit(sys.argv)
    glutCreateWindow(b'cube')
    InitGL()
    print('hello')
    glutDisplayFunc(DrawGLScene)
#    glutIdleFunc(MyIdle)
    glutKeyboardFunc(on_keyboard)
    glutMainLoop()


if __name__ == "__main__":
    main()