from OpenGL.GLUT import *
from OpenGL.GLU import *
from OpenGL.GL import *
import sys

name = b'ball_glut'

def init():
    glClearColor(0.,0.,0.,0.)
    glEnable(GL_DEPTH_TEST)
    return

def display():
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT)
    glPushMatrix()
    glColor3f(1.,1.,1.)
    glutWireSphere(0.9,10,10)
    glPopMatrix()
    glFlush()
    return

def main():
    glutInit(sys.argv)
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH)
    glutInitWindowSize(400,400)
    glutCreateWindow(name)
    init()
    glutDisplayFunc(display)    
    glutMainLoop()
    return

if __name__ == '__main__': 
    main()
