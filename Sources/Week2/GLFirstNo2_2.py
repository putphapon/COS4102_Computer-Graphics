from OpenGL.GL import *
from OpenGL.GLUT import *
from OpenGL.GLU import *
import numpy as np
import sys
k = 0
theta = 0
sign = 1
cx = 10
cy = 10
def InitGL():
    glColor3f(1., 1., 1.)
    glClearColor(0.5, 0.5, 0.5, 0.0)
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE)
    glMatrixMode(GL_PROJECTION)
    glLoadIdentity()
    gluOrtho2D(-20,20,-20,20)

def DrawGLScene():
    glClear(GL_COLOR_BUFFER_BIT)
    glPushMatrix()
    glTranslatef(-cx, -cy, 0)
    glRotatef(theta, 0, 0, 1)
    glTranslatef(cx,cy,0)
        glBegin(GL_POLYGON)
            glVertex2f(-5 - cx,-5 - cy)
            glVertex2f(-5 - cx, 5 - cy)
            glVertex2f(5 - cx, 5 - cy)
            glVertex2f(5 - cx, -5 - cy)
        glEnd()
    glPopMatrix()

   

    glFlush()



def MyIdle():
    global theta
    global sign
    theta = theta + (sign*0.1)
    if theta > 360 or theta < 0:
        sign = sign * -1
    glutPostRedisplay() #use Repost for call registered function // glutDisplayFunc()
    


def on_keyboard(key, x, y):
    pass

def main():
    glutInit(sys.argv) #createWindow
    glutInitWindowPosition(0,0)
    glutInitWindowSize(400,400)
    glutCreateWindow(b'cube')
    InitGL()
    glutDisplayFunc(DrawGLScene) #use 1 time for registerd DrawGLScene
    glutIdleFunc(MyIdle)
    glutKeyboardFunc(on_keyboard)
    glutMainLoop() #loop for call event


main()
