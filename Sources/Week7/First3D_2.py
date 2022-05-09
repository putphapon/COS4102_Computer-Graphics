from OpenGL.GLUT import *
from OpenGL.GLU import *
from OpenGL.GL import *
import sys

name = b'ball_glut'
sz = 9

def init():
    glClearColor(0.,0.,0.,0.)
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE)
    glEnable(GL_DEPTH_TEST)
    glMatrixMode(GL_PROJECTION)
    glLoadIdentity()
    glOrtho(-sz,sz,-sz,sz,-sz,sz)

    glMatrixMode(GL_MODELVIEW)
    glLoadIdentity()
    gluLookAt(2,2,-2,0,0,0,0,1.5,0.8)
    

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

def display():
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT)
    glPushMatrix()
    glColor3f(1.,0.,0.)
    glutSolidCube(0.5)
    glPopMatrix()

    glPushMatrix()
    glColor3f(0.,1.,0.)
    glTranslatef(0,0,-3)
    glutSolidCube(1.0)
    glPopMatrix()


    glFlush()
    return

if __name__ == '__main__': 
    main()
