from OpenGL.GLUT import *
from OpenGL.GLU import *
from OpenGL.GL import *
import sys
import math
name = b'ball_glut'
sz = 5
xe, ye, ze = 0, 0, 1
theta, sign = 0., 1. 
def init():
    glClearColor(0.,0.,0.,0.)
    #glPolygonMode(GL_FRONT_AND_BACK, GL_LINE)
    glEnable(GL_DEPTH_TEST)
    glMatrixMode(GL_PROJECTION)
    glLoadIdentity()
    glOrtho(-sz,sz,-sz,sz,-sz,sz)
    return
def main():
    glutInit(sys.argv)
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH)
    glutInitWindowSize(400,400)
    glutCreateWindow(name)
    init()
    glutDisplayFunc(display)    
    glutKeyboardFunc(On_keyboard)
    glutIdleFunc(mytime)
    glutMainLoop()
    return

def mytime():
    global theta, sign
    theta = theta + (0.009 * sign)
    if ((theta >= 360.0) | (theta <= 0.0)):
        sign = sign * (-1)
    glutPostRedisplay()

def On_keyboard(key, x, y):
    global ze
    while True:
        print('55')
        if key == b'\x1b':
            sys.exit()
        if key == b'f':
            ze = ze + 0.1
            break
        if key == b'k':
            ze = ze - 0.1
            break
        break
    glutPostRedisplay()
    return

def display():
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT)
    a = (theta * math.pi)/180
    glMatrixMode(GL_MODELVIEW)
    glLoadIdentity()
    gluLookAt(xe*math.cos(a) - ze*math.sin(a), ye, xe*math.sin(a) + ze*math.cos(a), 0, 0, 0, 0, 1, 0)

    glPushMatrix()
    glColor3f(1.,0.,0.)
    glTranslatef(3,0,-4)
    glutSolidCube(1.0)
    glPopMatrix()

    glPushMatrix()
    glColor3f(0.,1.,0.)
    glTranslatef(-3,0,4)
    glutSolidCube(1.0)
    glPopMatrix()

    glFlush()
    return

if __name__ == '__main__': 
    main()