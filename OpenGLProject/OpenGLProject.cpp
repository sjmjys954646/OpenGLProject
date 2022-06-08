#include <gl/glut.h>
#include <math.h>
#include <gl/gl.h>
#include <gl/glu.h>

#include <stdlib.h>

static float angle = 0.0, ratio;
static float x = 0.0f, y = 1.75f - 50.0f, z = 0.0f;
static float lx = 0.0f, ly = 0.0f, lz = 1.0f;

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
	if (x1 > 10 || x1 < -1) {
		dx *= -1;
	}
	glutPostRedisplay();
	glutTimerFunc(40, MyTimer, 1);

}

void drawStartPoint()
{
	//도입길
	glBegin(GL_QUADS);
	glColor3f(0.9f, 0.9f, 0.9f);
	glVertex3f(-5.0f, 0.0f, 30.0f);
	glVertex3f(5.0f, 0.0f, 30.0f);
	glVertex3f(5.0f, 0.0f, 0.0f);
	glVertex3f(-5.0f, 0.0f, 0.0f);
	glEnd();

	

	//문
	glBegin(GL_QUADS);
	glColor3f(0.4f, 0.4f, 0.4f);
	glVertex3f(-5.0f, 0.0f, 30.0f);
	glVertex3f(5.0f, 0.0f, 30.0f);
	glVertex3f(5.0f, 5.0f, 30.0f);
	glVertex3f(-5.0f, 5.0f, 30.0f);
	glEnd();
}

void drawFullWall(float leftX, float midZ, bool garosero)
{
	if (garosero == 0)
	{
		glBegin(GL_QUADS);
		glColor3f(0.2f, 0.0f, 0.1f);
		glVertex3f(leftX, -50.0f, midZ);
		glVertex3f(leftX, -45.0f, midZ);
		glVertex3f(leftX + 40.0f, -45.0f, midZ);
		glVertex3f(leftX + 40.0f, -50.0f, midZ);
		glEnd();
	}
	else
	{
		glBegin(GL_QUADS);
		glColor3f(0.2f, 0.0f, 0.1f);
		glVertex3f(leftX, -50.0f, midZ);
		glVertex3f(leftX, -45.0f, midZ);
		glVertex3f(leftX, -45.0f, midZ + 40.0f);
		glVertex3f(leftX, -50.0f, midZ + 40.0f);
		glEnd();
	}
}

void drawWall(float leftX, float midZ, bool garosero)
{
	if (garosero == 0)
	{
		glBegin(GL_QUADS);
		glColor3f(0.2f, 0.0f, 0.1f);
		glVertex3f(leftX, -50.0f, midZ);
		glVertex3f(leftX , -45.0f, midZ);
		glVertex3f(leftX + 15.0f, -45.0f, midZ);
		glVertex3f(leftX + 15.0f, -50.0f, midZ);
		glEnd();
	}
	else
	{
		glBegin(GL_QUADS);
		glColor3f(0.2f, 0.0f, 0.1f);
		glVertex3f(leftX, -50.0f, midZ);
		glVertex3f(leftX, -45.0f, midZ);
		glVertex3f(leftX, -45.0f, midZ + 15.0f);
		glVertex3f(leftX, -50.0f, midZ + 15.0f);
		glEnd();
	}
}

void drawFloor(float midX, float midZ, float size)
{
	glBegin(GL_QUADS);
	glColor3f(0.2f, 0.9f, 0.9f);
	glVertex3f(midX - size, -50.0f, midZ - size);
	glVertex3f(midX + size, -50.0f, midZ - size);
	glVertex3f(midX + size, -50.0f, midZ + size);
	glVertex3f(midX - size, -50.0f, midZ + size);
	glEnd();
}

void drawBokdoWall(float leftX, float midZ, bool garosero)
{
	if (garosero == 0)
	{
		glBegin(GL_QUADS);
		glColor3f(0.2f, 0.0f, 0.1f);
		glVertex3f(leftX, -50.0f, midZ);
		glVertex3f(leftX, -45.0f, midZ);
		glVertex3f(leftX + 20.0f, -45.0f, midZ);
		glVertex3f(leftX + 20.0f, -50.0f, midZ);
		glEnd();
	}
	else
	{
		glBegin(GL_QUADS);
		glColor3f(0.2f, 0.0f, 0.1f);
		glVertex3f(leftX, -50.0f, midZ);
		glVertex3f(leftX, -45.0f, midZ);
		glVertex3f(leftX, -45.0f, midZ + 20.0f);
		glVertex3f(leftX, -50.0f, midZ + 20.0f);
		glEnd();
	}
}

void drawGallary()
{
	//문
	glBegin(GL_QUADS);
	glColor3f(0.4f, 0.2f, 0.2f);
	glVertex3f(-5.0f, -50.0f, -20.0f);
	glVertex3f(5.0f, -50.0f, -20.0f);
	glVertex3f(5.0f, -45.0f, -20.0f);
	glVertex3f(-5.0f, -45.0f, -20.0f);
	glEnd();

	glColor3f(0.4f, 0.0f, 0.0f);
	//미술관 복도 아래
	glBegin(GL_QUADS);
	glVertex3f(-5.0f, -50.0f, -20.0f);
	glVertex3f(5.0f, -50.0f, -20.0f);
	glVertex3f(5.0f, -50.0f, 0.0f);
	glVertex3f(-5.0f, -50.0f, 0.0f);
	glEnd();

	//미술관 복도 왼쪽
	glBegin(GL_QUADS);
	glVertex3f(-5.0f, -50.0f, -20.0f);
	glVertex3f(-5.0f, -50.0f, 0.0f);
	glVertex3f(-5.0f, -45.0f, 0.0f);
	glVertex3f(-5.0f, -45.0f, -20.0f);
	glEnd();

	//미술관 복도 오른쪽
	glBegin(GL_QUADS);
	glVertex3f(5.0f, -50.0f, -20.0f);
	glVertex3f(5.0f, -50.0f, 0.0f);
	glVertex3f(5.0f, -45.0f, 0.0f);
	glVertex3f(5.0f, -45.0f, -20.0f);
	glEnd();

	//미술관 땅
	drawFloor(0.0f, 20.0f, 20.0f);
	drawWall(-20.0f, 0.0f, 0);
	drawWall(5.0f, 0.0f, 0);
	drawWall(-20.0f, 40.0f, 0);
	drawWall(5.0f, 40.0f, 0);
	drawWall(-20.0f, 0.0f, 1);
	drawWall(-20.0f, 25.0f, 1);
	drawWall(20.0f, 0.0f, 1);
	drawWall(20.0f, 25.0f, 1);
	//미술관 땅1
	drawFloor(-60.0f, 20.0f, 20.0f);
	drawWall(20.0f - 60.0f, 0.0f, 1);
	drawWall(20.0f - 60.0f, 25.0f, 1);
	drawFullWall(-20.0f - 60.0f, 0.0f, 1);
	drawFullWall(- 20.0f - 60.0f, 0.0f, 0);
	drawFullWall(- 20.0f - 60.0f, 40.0f, 0); 
	//미술관 땅2
	drawFloor(60.0f, 20.0f, 20.0f);
	drawWall(-20.0f + 60.0f, 0.0f, 1);
	drawWall(-20.0f + 60.0f, 25.0f, 1);
	drawFullWall(20.0f + 60.0f, 0.0f, 1);
	drawFullWall(-20.0f + 60.0f, 0.0f, 0);
	drawFullWall(-20.0f + 60.0f, 40.0f, 0);
	//미술관 땅3
	drawFloor(0, 80.0f, 20.0f);
	drawWall(-20.0f , 60.0f, 0);
	drawWall(5.0f , 60.0f, 0);
	drawFullWall(20.0f , 60.0f, 1);
	drawFullWall(-20.0f , 100.0f, 0);
	drawFullWall(-20.0f , 60.0f, 1);

	//미술관 복도1
	glBegin(GL_QUADS);
	glColor3f(0.5f, 0.9f, 0.9f);
	glVertex3f(-40.0f, -50.0f, 15.0f);
	glVertex3f(-40.0f, -50.0f, 25.0f);
	glVertex3f(-20.0f, -50.0f, 25.0f);
	glVertex3f(-20.0f, -50.0f, 15.0f);
	glEnd();
	drawBokdoWall(-40.0f, 15.0f, 0);
	drawBokdoWall(-40.0f, 25.0f, 0);

	//미술관 복도2
	glBegin(GL_QUADS);
	glColor3f(0.5f, 0.9f, 0.9f);
	glVertex3f(40.0f, -50.0f, 15.0f);
	glVertex3f(40.0f, -50.0f, 25.0f);
	glVertex3f(20.0f, -50.0f, 25.0f);
	glVertex3f(20.0f, -50.0f, 15.0f);
	glEnd();
	drawBokdoWall(20.0f, 15.0f, 0);
	drawBokdoWall(20.0f, 25.0f, 0);

	//미술관 복도3
	glBegin(GL_QUADS);
	glColor3f(0.5f, 0.9f, 0.9f);
	glVertex3f(-5.0f, -50.0f, 40.0f);
	glVertex3f(5.0f, -50.0f, 40.0f);
	glVertex3f(5.0f, -50.0f, 60.0f);
	glVertex3f(-5.0f, -50.0f, 60.0f);
	glEnd();
	drawBokdoWall(-5.0f, 40.0f, 1);
	drawBokdoWall(5.0f, 40.0f, 1);
}

void drawMap()
{
	drawStartPoint();
	drawGallary();
}



void renderScene(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	drawMap();

	glPushMatrix();
	glColor3f(0.0f, 0.9f, 0.9f);
	glBegin(GL_QUADS);
	glVertex3f(-1.0f, -1.0f + x1, 1.0f);		// 앞면
	glVertex3f(1.0f, -1.0f + x1, 1.0f);
	glVertex3f(1.0f, 1.0f + x1, 1.0f);
	glVertex3f(-1.0f, 1.0f + x1, 1.0f);

	glVertex3f(1.0f, -1.0f + x1, -1.0f);
	glVertex3f(-1.0f, -1.0f + x1, -1.0f);		// 뒷면
	glVertex3f(-1.0f, 1.0f + x1, -1.0f);
	glVertex3f(1.0f, 1.0f + x1, -1.0f);

	glVertex3f(-1.0f, 1.0f + x1, 1.0f);		// 윗면
	glVertex3f(1.0f, 1.0f + x1, 1.0f);
	glVertex3f(1.0f, 1.0f + x1, -1.0f);
	glVertex3f(-1.0f, 1.0f + x1, -1.0f);

	glVertex3f(-1.0f, -1.0f + x1, -1.0f);		// 아랫면
	glVertex3f(1.0f, -1.0f + x1, -1.0f);
	glVertex3f(1.0f, -1.0f + x1, 1.0f);
	glVertex3f(-1.0f, -1.0f + x1, 1.0f);

	glVertex3f(1.0f, -1.0f + x1, 1.0f);		// 우측면
	glVertex3f(1.0f, -1.0f + x1, -1.0f);
	glVertex3f(1.0f, 1.0f + x1, -1.0f);
	glVertex3f(1.0f, 1.0f + x1, 1.0f);

	glVertex3f(-1.0f, -1.0f + x1, -1.0f);	// 좌측면
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
	x = x + i * (lx) * 0.2;
	z = z + i * (lz) * 0.2;
	glLoadIdentity();
	gluLookAt(x, y, z, x + lx, y + ly, z + lz, 0.0f, 1.0f, 0.0f);
}

void processNormalKeys(unsigned char key, int x, int y) {

	if (key == 27)
		exit(0);
}


void inputKey(unsigned char key, int x, int y) {

	switch (key) {

	case 'a':
		angle -= 0.03f;
		orientMe(angle);
		break;
	case 'd':
		angle += 0.03f;
		orientMe(angle);
		break;
	case 'w':
		moveMeFlat(1);
		break;
	case 's':
		moveMeFlat(-1);
		break;
	}
}


void glInit()
{
	glEnable(GL_DEPTH_TEST);
}


int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(900, 700);
	glutCreateWindow("project");

	glInit();
	glutKeyboardFunc(inputKey);

	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glutTimerFunc(40, MyTimer, 1);
	glutReshapeFunc(changeSize);

	glutMainLoop();

	return(0);
}