#define _CRT_SECURE_NO_WARNINGS
#include <gl/glut.h>
#include <math.h>
#include <gl/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <gl/freeglut.h>
#include <stdlib.h>
#include "glaux.h"
#include <GL/GLU.h>
#include <GL/glut.h>
#include <string>
#include <vector>
#include "bitmap_fonts.h"

using namespace std; 
using namespace glm;
static float ang = 0.0, ratio;
static float x = 0.0f, y = 1.75f -50.0f, z = 5.0f;
static float lx = 0.0f, ly = 0.0f, lz = 1.0f;

GLfloat dx = 0.2;
GLfloat x1;
static char coor[255];
static char message[255];
int    g_nWindowWidth;
int    g_nWindowHeight;
void DrawCube(float size);
void addMob(vec3 position, float size);
void AddContactForce();
void DrawCubeTex();

bool die = false;

typedef struct {
	vec3 p = vec3(x, y, z);
}player;
player Player;

struct Mob {
	vec3 p; //position
	vec3 v; //velocity
	vec3 force; //force
	float size; //size
	float m; //mass
};

vector<Mob> mobs;

GLuint	texture[30];
GLuint g_textureID = -1;
const string textureName[30] = { "Data\monalisa.bmp","Data\gentleman.bmp","Data\girlwithearing2.bmp" };
const int TEXTURENUM = 3;


void Timer(int value) { ///////////////////////////////
	x1 += dx;
	if (x1 > 4 || x1 < -4) {
		dx *= -1;
	}
	else if (x1 > 11 || x1 < -4) {
		dx *= -1;
	}

	glutPostRedisplay();
	glutTimerFunc(40, Timer, 1);

}/////////////////////////////////////////

AUX_RGBImageRec* LoadBMP(const char* Filename) {
	FILE* File = NULL;

	if (!Filename) {
		return NULL;
	}

	File = fopen(Filename, "r");

	if (File) {
		fclose(File);
		return auxDIBImageLoad(Filename);
	}

	return NULL;
}




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

void LoadGLTextures() {
	AUX_RGBImageRec* TextureImage[30];
	memset(TextureImage, 0, sizeof(void*) * 30);

	for (int i = 0; i < TEXTURENUM; i++)
	{
		char cstr[50];
		strcpy(cstr, textureName[i].c_str());
		for (int c = 0; c < strlen(cstr); c++)
		{
			printf("%c", cstr[c]);
		}
		printf("\n");

		if (TextureImage[i] = LoadBMP(cstr)) {
			glGenTextures(1, &texture[i]);

			glBindTexture(GL_TEXTURE_2D, texture[i]);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[i]->sizeX, TextureImage[i]->sizeY, 0,
				GL_RGB, GL_UNSIGNED_BYTE, TextureImage[i]->data);
		}

		if (TextureImage[i]) {
			if (TextureImage[i]->data) {
				free(TextureImage[i]->data);
			}
			free(TextureImage[i]);
		}
	}

}

void drawtrap() {////////////////////////////
	glColor3f(1.0f, 0.0f, 0.0f);
	glPushMatrix();
	glTranslatef(0.0f+x1, -50.0f, 48.0f);
	glRotatef(-90, 1, 0, 0);
	glutSolidCylinder(0.5f, 5.0f, 32, 4);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-26.0f, -50.0f, 20.0f+ x1);
	glRotatef(-90, 1, 0, 0);
	glutSolidCylinder(0.5f, 5.0f, 32, 4);
	glPopMatrix(); 

	glPushMatrix();
	glTranslatef(28.0f, -50.0f, 20.0f + x1);
	glRotatef(-90, 1, 0, 0);
	glutSolidCylinder(0.5f, 5.0f, 32, 4);
	glPopMatrix();

	//glPushMatrix();
	//glTranslatef(-10.0f, -45.0f +x1, 14.0f);
	//glRotatef(90, 1, 0, 0);
	//glutSolidCone(1.0f, 3.0f, 10, 2);
	//glPopMatrix();

	glPushMatrix();
	glBegin(GL_QUADS);
	glVertex3f(-5.0f, -48.5f+x1, 90.0f);
	glVertex3f(-5.0f, -49.0f+x1, 90.0f);
	glVertex3f(5.0f, -49.0f+x1, 90.0f);
	glVertex3f(5.0f, -48.5f+x1, 90.0f);
	
	glVertex3f(5.0f, -48.5f+x1 , 100.0f);
	glVertex3f(5.0f, -49.0f+x1 , 100.0f);
	glVertex3f(5.0f, -49.0f + x1, 90.0f);
	glVertex3f(5.0f, -48.5f + x1, 90.0f);

	glVertex3f(-5.0f, -48.5f + x1, 100.0f);
	glVertex3f(-5.0f, -49.0f + x1, 100.0f);
	glVertex3f(-5.0f, -49.0f + x1, 90.0f);
	glVertex3f(-5.0f, -48.5f + x1, 90.0f);

	glVertex3f(70.0f, -48.5f + x1, 25.0f);
	glVertex3f(70.0f, -49.0f + x1, 25.0f);
	glVertex3f(70.0f, -49.0f + x1, 15.0f);
	glVertex3f(70.0f, -48.5f + x1, 15.0f);

	glVertex3f(80.0f, -48.5f + x1, 15.0f);
	glVertex3f(80.0f, -49.0f + x1, 15.0f);
	glVertex3f(70.0f, -49.0f + x1, 15.0f);
	glVertex3f(70.0f, -48.5f + x1, 15.0f);

	glVertex3f(80.0f, -48.5f + x1, 25.0f);
	glVertex3f(80.0f, -49.0f + x1, 25.0f);
	glVertex3f(70.0f, -49.0f + x1, 25.0f);
	glVertex3f(70.0f, -48.5f + x1, 25.0f);

	glVertex3f(-70.0f, -48.5f + x1, 25.0f);
	glVertex3f(-70.0f, -49.0f + x1, 25.0f);
	glVertex3f(-70.0f, -49.0f + x1, 15.0f);
	glVertex3f(-70.0f, -48.5f + x1, 15.0f);

	glVertex3f(-80.0f, -48.5f + x1, 15.0f);
	glVertex3f(-80.0f, -49.0f + x1, 15.0f);
	glVertex3f(-70.0f, -49.0f + x1, 15.0f);
	glVertex3f(-70.0f, -48.5f + x1, 15.0f);

	glVertex3f(-80.0f, -48.5f + x1, 25.0f);
	glVertex3f(-80.0f, -49.0f + x1, 25.0f);
	glVertex3f(-70.0f, -49.0f + x1, 25.0f);
	glVertex3f(-70.0f, -48.5f + x1, 25.0f);
	glPopMatrix();
	glEnd();
	
} ///////////////////////////////////////////////////


void drawCircle() {
	glColor3f(1.0f, 0.7f, 1.0f);
	glTranslatef(0.0f, 5.0f, 0.0f);
	glutSolidSphere(0.75f, 20, 20);
}

void MyTimer(int value) {
	Player.p = vec3(x, y, z);
	glutPostRedisplay();
	AddContactForce();
	if (die == false)
		glutTimerFunc(40, MyTimer, 1);
	else
	{
		x = 300;

		z = 300;
	}

}

void DrawCube(float size)
{
	glColor3f(0.0f, 0.9f, 0.9f);
	glutSolidCube(size);
	//glBegin(GL_QUADS);
	//glVertex3f(-size, -size, size);		// 앞면
	//glVertex3f(size, -size, size);
	//glVertex3f(size, size, size);
	//glVertex3f(-size, size, size);

	//glVertex3f(size, -size, -size);
	//glVertex3f(-size, -size, -size);		// 뒷면
	//glVertex3f(-size, size, -size);
	//glVertex3f(size, size, -size);

	//glVertex3f(-size, size, size);		// 윗면
	//glVertex3f(size, size, size);
	//glVertex3f(size, size, -size);
	//glVertex3f(-size, size, -size);

	//glVertex3f(-size, -size, -size);		// 아랫면
	//glVertex3f(size, -size, -size);
	//glVertex3f(size, -size, size);
	//glVertex3f(-size, -size, size);

	//glVertex3f(size, -size, size);		// 우측면
	//glVertex3f(size, -size, -size);
	//glVertex3f(size, size, -size);
	//glVertex3f(size, size, size);

	//glVertex3f(-size, -size, -size);	// 좌측면
	//glVertex3f(-size, -size, size);
	//glVertex3f(-size, size, size);
	//glVertex3f(-size, size, -size);
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
		glVertex3f(leftX, -45.0f, midZ);
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

void drawPicture(float leftX, float midZ, bool garosero, int pictureNum)
{
	leftX = leftX - 1.2f;
	if (garosero == 0)
	{
		glBindTexture(GL_TEXTURE_2D, texture[pictureNum]);
		glBegin(GL_QUADS);
		glColor3f(1.0f, 1.0f, 1.0f);
		glTexCoord2f(0, 0); glVertex3f(leftX, -49.0f, midZ);
		glTexCoord2f(0, 1); glVertex3f(leftX, -45.8f, midZ);
		glTexCoord2f(1, 1); glVertex3f(leftX + 2.4f, -45.8f, midZ);
		glTexCoord2f(1, 0); glVertex3f(leftX + 2.4f, -49.0f, midZ);
		glEnd();

	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, texture[pictureNum]);
		glBegin(GL_QUADS);
		glColor3f(1.0f, 1.0f, 1.0f);
		glTexCoord2f(0, 0);  glVertex3f(leftX, -49.0f, midZ);
		glTexCoord2f(0, 1); glVertex3f(leftX, -45.8f, midZ);
		glTexCoord2f(1, 1); glVertex3f(leftX, -45.8f, midZ + 2.4f);
		glTexCoord2f(1, 0); glVertex3f(leftX, -49.0f, midZ + 2.4f);
		glEnd();
	}
}

void drawPannelGround(float posX, float posZ, float angle)
{
	glPushMatrix();
	{
		glTranslatef(posX, 0.0f, posZ);
		glRotatef(angle, 0, 1, 0);
	}
	glBegin(GL_QUADS);
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-0.1f, -50.0f, 0.0f);
	glVertex3f(-0.1f, -49.0f, 0.0f);
	glVertex3f(0.1f, -49.0f, 0.0f);
	glVertex3f(0.1f, -50.0f, 0.0f);
	glEnd();
	glBegin(GL_QUADS);
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-0.8f, -49.0f, 0.0f);
	glVertex3f(-0.8f, -48.2f, 0.0f);
	glVertex3f(0.8f, -48.2f, 0.0f);
	glVertex3f(0.8f, -49.0f, 0.0f);
	glEnd();
	glPopMatrix();
}

void drawPannel(float leftX, float midZ, bool garosero)
{
	leftX = leftX - 1.2f;
	if (garosero == 0)
	{
		glBegin(GL_QUADS);
		glColor3f(1.0f, 1.0f, 1.0f);
		glTexCoord2f(0, 0); glVertex3f(leftX, -48.0f, midZ);
		glTexCoord2f(0, 1); glVertex3f(leftX, -47.2f, midZ);
		glTexCoord2f(1, 1); glVertex3f(leftX + 1.2f, -47.2f, midZ);
		glTexCoord2f(1, 0); glVertex3f(leftX + 1.2f, -48.0f, midZ);
		glEnd();

	}
	else
	{
		glBegin(GL_QUADS);
		glColor3f(1.0f, 1.0f, 1.0f);
		glTexCoord2f(0, 0);  glVertex3f(leftX, -48.0f, midZ);
		glTexCoord2f(0, 1); glVertex3f(leftX, -47.2f, midZ);
		glTexCoord2f(1, 1); glVertex3f(leftX, -47.2f, midZ + 1.2f);
		glTexCoord2f(1, 0); glVertex3f(leftX, -48.0f, midZ + 1.2f);
		glEnd();
	}
}

void drawGallary()
{
	//문
	glColorMaterial(GL_FRONT, GL_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
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
	drawPannelGround(10.0f, 28.0f, 30.0f);
	drawPannelGround(-10.0f, 30.0f, -30.0f);
	//미술관 땅1
	drawFloor(-60.0f, 20.0f, 20.0f);
	drawWall(20.0f - 60.0f, 0.0f, 1);
	drawWall(20.0f - 60.0f, 25.0f, 1);
	drawFullWall(-20.0f - 60.0f, 0.0f, 1);
	drawFullWall(-20.0f - 60.0f, 0.0f, 0);
	drawFullWall(-20.0f - 60.0f, 40.0f, 0);
	drawPicture(-78.5f, 20.0f, 1, 1);
	drawPannel(-78.5f, 20.0f - 1.6f, 1);
	drawPannelGround(-50.0f, 15.0f, 45.0f);
	//미술관 땅2
	drawFloor(60.0f, 20.0f, 20.0f);
	drawWall(-20.0f + 60.0f, 0.0f, 1);
	drawWall(-20.0f + 60.0f, 25.0f, 1);
	drawFullWall(20.0f + 60.0f, 0.0f, 1);
	drawFullWall(-20.0f + 60.0f, 0.0f, 0);
	drawFullWall(-20.0f + 60.0f, 40.0f, 0);
	drawPicture(79.5f, 20.0f, 1, 0);
	drawPannel(78.5f, 20.0f + 2.8f, 1);
	drawPannelGround(50.0f, 25.0f, 45.0f);
	//미술관 땅3
	drawFloor(0, 80.0f, 20.0f);
	drawWall(-20.0f, 60.0f, 0);
	drawWall(5.0f, 60.0f, 0);
	drawFullWall(20.0f, 60.0f, 1);
	drawFullWall(-20.0f, 100.0f, 0);
	drawFullWall(-20.0f, 60.0f, 1);
	drawPicture(0.0f, 99.5f, 0, 2);
	drawPannel(0.0f - 1.6f, 99.5f, 0);

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

void drawStructure()
{
	//조형물
	//메인방
	glColor3f(1.0f, 0.0f, 0.0f);
	glPushMatrix();
	{
		glTranslatef(-15.0f, 1.75f - 50.0f, 35.0f);
		glutWireDodecahedron();
	}
	glPopMatrix();
	glPushMatrix();
	{
		glTranslatef(15.0f, 5.0f - 50.0f, 35.0f);
		glRotatef(90, 1, 0, 0);
		glutSolidCone(2.0f, 5.0f, 32, 4);
	}
	glPopMatrix();
	//2번방
	glPushMatrix();
	{
		glTranslatef(-10.0f, -50.0f, 95.0f);
		glRotatef(-90, 1, 0, 0);
		glutSolidCylinder(0.5f, 5.0f, 32, 4);
	}
	glPopMatrix();
	glPushMatrix();
	{
		glTranslatef(-10.0f, -50.0f, 70.0f);
		glRotatef(-90, 1, 0, 0);
		glutSolidCylinder(0.5f, 5.0f, 32, 4);
	}
	glPopMatrix();
	glPushMatrix();
	{
		glTranslatef(10.0f, -50.0f, 95.0f);
		glRotatef(-90, 1, 0, 0);
		glutSolidCylinder(0.5f, 5.0f, 32, 4);
	}
	glPopMatrix();
	glPushMatrix();
	{
		glTranslatef(10.0f, -50.0f, 70.0f);
		glRotatef(-90, 1, 0, 0);
		glutSolidCylinder(0.5f, 5.0f, 32, 4);
	}
	glPopMatrix();
	//1번방
	glPushMatrix();
	{
		glDisable(GL_TEXTURE_2D);
		glColor3f(0.0f, 0.0f, 0.0f);
		glTranslatef(-60.0f + 10.0f, -50.0f + 2.0f, 20.0f - 10.0f);
		glRotatef(-90, 0, 1, 0);
		glutSolidTeapot(2);
		glEnable(GL_TEXTURE_2D);
	}
	glPopMatrix();
	//3번방
	glPushMatrix();
	{
		glDisable(GL_TEXTURE_2D);
		glColor3f(0.0f, 0.0f, 0.0f);
		glTranslatef(60.0f - 10.0f, -50.0f + 2.0f, 20.0f + 10.0f);
		glutSolidSphere(2, 42, 10);
		glEnable(GL_TEXTURE_2D);
	}
	glPopMatrix();

}

void drawMap()
{
	//천장

	glBegin(GL_QUADS);
	glColor3f(0.8f, 0.2f, 0.9f);
	glVertex3f(-120.0f, -45.0f, -20.0f);
	glVertex3f(120.0f, -45.0f, -20.0f);
	glVertex3f(120.0f, -45.0f, 120.0f);
	glVertex3f(-120.0f, -45.0f, 120.0f);
	glEnd();
	drawStartPoint();
	drawGallary();
	drawStructure();
	drawtrap();
}

void renderScene(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	drawMap();

	//DrawCube(1.0f);
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(x, y, z + 2);
	//addMob(vec3(x,y,z+2),0.5);
	glPopMatrix();
	//DrawCubeTex();


	glEnd();

	sprintf(coor, "x : %f y : %f z : %f", x, y, z);
	sprintf(message, "You DIED");
	beginRenderText(g_nWindowWidth, g_nWindowHeight);
	{
		glColor3f(1.f, 1.f, 1.0f);
		int u = (1.f - 0.75f) / 4.f * g_nWindowWidth;
		int v = (1.f - (-1.75f)) / 4.f * g_nWindowHeight;
		if (die == false)
			renderText(g_nWindowWidth - 0.95, g_nWindowHeight + 0.9, BITMAP_FONT_TYPE_HELVETICA_18, coor);
		else
			renderText(g_nWindowWidth - 0.08, g_nWindowHeight, BITMAP_FONT_TYPE_HELVETICA_18, message);
	}
	endRenderText();
	glutSwapBuffers();
}

void orientMe(float ang) {

	lx = sin(ang);
	lz = -cos(ang);
	glLoadIdentity();
	gluLookAt(x, y, z, x + lx, y + ly, z + lz, 0.0f, 1.0f, 0.0f);
	sprintf(coor, "x : %f y : %f z : %f", x, y, z);
}


void moveMeFlat(int i) {
	x = x + i * (lx) * 0.2;
	z = z + i * (lz) * 0.2;
	glLoadIdentity();
	gluLookAt(x, y, z, x + lx, y + ly, z + lz, 0.0f, 1.0f, 0.0f);
	sprintf(coor, "x : %f y : %f z : %f", x, y, z);
}

void processNormalKeys(unsigned char key, int x, int y) {

	if (key == 27)
		exit(0);
}

void addMob(vec3 position, float size)
{
	Mob newmob;

	vec3 _pos(0, 0, 0);
	_pos.x = position.x;
	_pos.y = position.y;
	_pos.z = position.z;
	float _size = size;
	//newmob.p = _pos;
	newmob.p.x = _pos.x;
	newmob.p.y = _pos.y;
	newmob.p.z = _pos.z;

	newmob.size = _size;

	/*glPushMatrix();
	glTranslatef(position.x, position.y, position.z);*/
	DrawCube(size);
	//glPopMatrix();
	mobs.push_back(newmob);
}

void AddContactForce()
{
	for (int i = 0; i < mobs.size(); i++)
	{
		vec3 dis = mobs[i].p - Player.p;       //dis=반지름 사이
			//compute length
		float L = length(dis);                    //L= 반지름 사이의 거리

		dis = normalize(dis);
		if (L <= mobs[i].size)          //L이 반지름 사이의 거리보다 작다? => 두 공이 겹쳤다
		{
			die = true;
			//compute and add force
			/*vec3 force = stiff * ((balls[i].r + balls[j].r) - L) * dis;
			balls[i].force += force;
			balls[j].force -= force;*/
		}
	}
}
void inputKey(unsigned char key, int x, int y) {

	switch (key) {

	case 'a':
		ang -= 0.03f;
		orientMe(ang);
		break;
	case 'd':
		ang += 0.03f;
		orientMe(ang);
		break;
	case 'f':
		ang += 110.0018f;
		orientMe(ang);
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
	LoadGLTextures();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
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
	glutTimerFunc(40, Timer, 1);//////////
	glutReshapeFunc(changeSize);

	glutMainLoop();

	return(0);
}