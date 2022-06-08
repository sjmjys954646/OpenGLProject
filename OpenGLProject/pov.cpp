#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <gl/glu.h>
#include <gl/glut.h>
#include <glaux.h>


float g_fDistance = -2.5f;
float g_fSpinX = 0.0f;
float g_fSpinY = 0.0f;
float eyex = 0.0, eyey = 50.0, eyez = 100.0, fovy = 40.0;

static POINT ptLastMousePosit;
static POINT ptCurrentMousePosit;
static bool bMousing;

void DrawFloor(float size, float level) {
    glBegin(GL_QUADS);
    glVertex3f(-size, -size, level);
    glVertex3f(size, -size, level);
    glVertex3f(size, size, level);
    glVertex3f(-size, size, level);
    glEnd();
}

void MyDisplay() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(0.0f, 0.0f, g_fDistance);
    glRotatef(-g_fSpinY, 1.0f, 0.0f, 0.0f);
    glRotatef(-g_fSpinX, 0.0f, 0.0f, 1.0f);

    gluLookAt(eyex, eyey, eyez, 0.0, 5.0, 0.0, -1.0, 1.0, 0.0);  // 카메라의 위치를 제어한다.

    glPushMatrix();
    glColor3f(0.0, 0.0, 0.8);
    DrawFloor(300, -10);
    glPopMatrix();

    glPushMatrix();
    glColor3f(1.0, 0.0, 0.8);
    glutWireSphere(1, 20, 16);
    glPopMatrix();


    glutSwapBuffers();
}

void MySpecial(int key, int x, int y) {
    if (key == GLUT_KEY_UP) {
        g_fDistance += 2.0f;
    }
    else if (key == GLUT_KEY_DOWN) {
        g_fDistance -= 2.0f;
    }
    else if (key == GLUT_KEY_LEFT) {
        eyex += 1.0f;
    }
    else if (key == GLUT_KEY_RIGHT) {
        eyex -= 1.0f;
    }

    glutPostRedisplay();
}

void MyMouse(int button, int state, int x, int y) {
    switch (button) {
    case GLUT_LEFT_BUTTON:
        if (state == GLUT_DOWN) {
            ptLastMousePosit.x = ptCurrentMousePosit.x = x;
            ptLastMousePosit.y = ptCurrentMousePosit.y = y;
            bMousing = true;
        }
        else
            bMousing = false;
        break;
    case GLUT_MIDDLE_BUTTON:
    case GLUT_RIGHT_BUTTON:
        break;
    default:
        break;
    }

    glutPostRedisplay();
}

void MyMotion(int x, int y) {
    ptCurrentMousePosit.x = x;
    ptCurrentMousePosit.y = y;

    if (bMousing)
    {
        g_fSpinX -= (ptCurrentMousePosit.x - ptLastMousePosit.x);
        g_fSpinY -= (ptCurrentMousePosit.y - ptLastMousePosit.y);
    }

    ptLastMousePosit.x = ptCurrentMousePosit.x;
    ptLastMousePosit.y = ptCurrentMousePosit.y;

    glutPostRedisplay();
}

void MyReshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(fovy, (GLfloat)w / (GLfloat)h, 1.0, 2000.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Skybox");

    glutDisplayFunc(MyDisplay);
    glutReshapeFunc(MyReshape);
    glutMouseFunc(MyMouse);
    glutMotionFunc(MyMotion);
    glutSpecialFunc(MySpecial);

    glutMainLoop();
}