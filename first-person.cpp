#include <gl/glut.h>
#include <math.h>
#include <gl/gl.h>
#include <gl/glu.h>

#include <stdlib.h>

static float angle = 0.0, ratio;
static float x = 0.0f, y = 1.75f, z = 5.0f;
static float lx = 0.0f, ly = 0.0f, lz = -1.0f;

GLfloat dx = 0.2;
GLfloat x1;

void changeSize(int w, int h)
{
	if (h == 0)
		h = 1;

	ratio = 1.0f * w / h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	
	glViewport(0, 0, w, h);

	gluPerspective(45, ratio, 1, 1000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(x, y, z, x + lx, y + ly, z + lz, 0.0f, 1.0f, 0.0f);


}


void drawCircle() {


	glColor3f(1.0f, 0.7f, 1.0f);
	glTranslatef(0.0f, 5.0f, 0.0f);
	glutSolidSphere(0.75f, 20, 20);


}

void MyTimer(int value) {
	x1 += dx;                
	if (x1  > 10 || x1 < -1) {
		dx *= -1;         
	}
	glutPostRedisplay();
	glutTimerFunc(40, MyTimer, 1);

}


void renderScene(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



	glColor3f(0.9f, 0.9f, 0.9f);
	glBegin(GL_QUADS);
	glVertex3f(-100.0f, 0.0f, -100.0f);
	glVertex3f(-100.0f, 0.0f, 100.0f);
	glVertex3f(100.0f, 0.0f, 100.0f);
	glVertex3f(100.0f, 0.0f, -100.0f);
	glEnd();


	glPushMatrix();
	glColor3f(0.0f, 0.9f, 0.9f);
	glBegin(GL_QUADS);
	glVertex3f(-1.0f, -1.0f + x1, 1.0f);		// ¾Õ¸é
	glVertex3f(1.0f, -1.0f + x1, 1.0f);
	glVertex3f(1.0f, 1.0f + x1, 1.0f);
	glVertex3f(-1.0f, 1.0f + x1, 1.0f);

	glVertex3f(1.0f, -1.0f + x1, -1.0f);
	glVertex3f(-1.0f, -1.0f + x1, -1.0f);		// µÞ¸é
	glVertex3f(-1.0f, 1.0f + x1, -1.0f);
	glVertex3f(1.0f, 1.0f + x1, -1.0f);

	glVertex3f(-1.0f, 1.0f + x1, 1.0f);		// À­¸é
	glVertex3f(1.0f, 1.0f + x1, 1.0f);
	glVertex3f(1.0f, 1.0f + x1, -1.0f);
	glVertex3f(-1.0f, 1.0f + x1, -1.0f);

	glVertex3f(-1.0f, -1.0f + x1, -1.0f);		// ¾Æ·§¸é
	glVertex3f(1.0f, -1.0f + x1, -1.0f);
	glVertex3f(1.0f, -1.0f + x1, 1.0f);
	glVertex3f(-1.0f, -1.0f + x1, 1.0f);

	glVertex3f(1.0f, -1.0f + x1, 1.0f);		// ¿ìÃø¸é
	glVertex3f(1.0f, -1.0f + x1, -1.0f);
	glVertex3f(1.0f, 1.0f + x1, -1.0f);
	glVertex3f(1.0f, 1.0f + x1, 1.0f);

	glVertex3f(-1.0f, -1.0f + x1, -1.0f);	// ÁÂÃø¸é
	glVertex3f(-1.0f, -1.0f + x1, 1.0f);
	glVertex3f(-1.0f, 1.0f + x1, 1.0f);
	glVertex3f(-1.0f, 1.0f + x1, -1.0f);
	glEnd();
	glPopMatrix();

	glutSwapBuffers();
}

void orientMe(float ang) {


	lx = sin(ang);
	lz = -cos(ang);
	glLoadIdentity();
	gluLookAt(x, y, z, x + lx, y + ly, z + lz, 0.0f, 1.0f, 0.0f);
}


void moveMeFlat(int i) {
	x = x + i * (lx) * 0.1;
	z = z + i * (lz) * 0.1;
	glLoadIdentity();
	gluLookAt(x, y, z, x + lx, y + ly, z + lz, 0.0f, 1.0f, 0.0f);
}

void processNormalKeys(unsigned char key, int x, int y) {

	if (key == 27)
		exit(0);
}


void inputKey(int key, int x, int y) {

	switch (key) {

	case GLUT_KEY_LEFT: 
		angle -= 0.03f;
		orientMe(angle);
		break;
	case GLUT_KEY_RIGHT: 
		angle += 0.03f;
		orientMe(angle);
		break;
	case GLUT_KEY_UP:
		moveMeFlat(1);
		break;
	case GLUT_KEY_DOWN: 
		moveMeFlat(-1);
		break;
	}
}


int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(900, 700);
	glutCreateWindow("project");


	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(inputKey);

	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glutTimerFunc(40, MyTimer, 1);
	glutReshapeFunc(changeSize);

	glutMainLoop();

	return(0);
}