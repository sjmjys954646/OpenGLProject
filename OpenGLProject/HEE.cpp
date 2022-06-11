﻿#define _CRT_SECURE_NO_WARNINGS
#include <gl/glut.h>
#include <time.h>
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
#include <iostream>
#define NUM_PARTICLES    10000          
#define NUM_DEBRIS       1000           

using namespace std;
using namespace glm;
static float ang = 0.0f, ratio;
static float px = 0.0f, py = 1.75f, pz = 70.0f;
static float lx = 0.0f, ly = 0.0f, lz = 1.0f;
static POINT    ptLastMousePosit;
static POINT    ptCurrentMousePosit;
static bool        bMousing;


float g_fDistance = -5.0f;
float g_fSpinX = 0.0f;
float g_fSpinY = 0.0f;
//////////////////////////////////////////////
GLfloat dx = 0.2;
GLfloat dx2 = 0.2;
GLfloat dx3 = 0.2;
GLfloat dx4 = 0.2;
GLfloat dx5 = 0.2;
GLfloat dx6 = 0.2;
GLfloat x1;
GLfloat x2;
GLfloat x3;
GLfloat x4;
GLfloat x5;
GLfloat x6;

float posix = 0; 
float posiy = 0;
/////////////////////////////////////////////

static char coor[255];
static char message[255];
static char diemessage[255];
static char name[255];
int    g_nWindowWidth;
int    g_nWindowHeight;
void DrawCube();
//void addMob(vec3 position, float size);
void addMob();
void crash();
void DrawCubeTex();
void drawtrap();
void renderScene(void);
GLboolean die = false;    //사망 처리
GLboolean clear = false;  //클리어 처리
GLboolean text = true;   // 좌표 및 텍스트 on/off
GLboolean trap = false;
GLboolean girl = false;
GLboolean gentleman = false;
GLboolean monalisa = false;
struct Mob {
	vec3 p; //position

	vec3 force; //force
	float size; //size
	float m; //mass
	float v; //velocity
};

vector<Mob> mobs;
struct particleData {
	float   position[3];
	float   speed[3];
	float   color[3];
};
typedef struct particleData    particleData;

struct debrisData {
	float   position[3];
	float   speed[3];
	float   orientation[3];        /* Rotation angles around x, y, and z axes */
	float   orientationSpeed[3];
	float   color[3];
	float   scale[3];
};
typedef struct debrisData    debrisData;

particleData	particles[NUM_PARTICLES];
debrisData		debris[NUM_DEBRIS];
int             fuel = 0;                /* "fuel" of the explosion */
int				wantNormalize = 0;   /* Speed vector normalization flag */

void rst()
{
	die = false;
	clear = false;
	trap = false;
	px = 0.0f;
	py = 1.75f;
	pz = 0.0f;
	ang = 0.0f;
	lx = 0.0f, ly = 0.0f, lz = 1.0f;
}

void newSpeed(float dest[3]) {
	float ax, ay, az, len;

	ax = (2.0 * ((GLfloat)rand()) / ((GLfloat)RAND_MAX)) - 1.0;
	ay = (2.0 * ((GLfloat)rand()) / ((GLfloat)RAND_MAX)) - 1.0;
	az = (2.0 * ((GLfloat)rand()) / ((GLfloat)RAND_MAX)) - 1.0;

	if (wantNormalize) {
		len = sqrt(ax * ax + ay * ay + az * az);

		if (len) {
			ax = ax / len;
			ay = ay / len;
			az = az / len;
		}
	}

	dest[0] = ax;
	dest[1] = ay;
	dest[2] = az;
}

//폭발시 파티클 및 파편 생성
void newExplosion(void) {
	for (int i = 0; i < NUM_PARTICLES; i++) {
		particles[i].position[0] = 0.0;
		particles[i].position[1] = 0.0;
		particles[i].position[2] = 0.0;

		particles[i].color[0] = 1.0;
		particles[i].color[1] = 1.0;
		particles[i].color[2] = 0.5;

		newSpeed(particles[i].speed);
	}

	for (int i = 0; i < NUM_DEBRIS; i++) {
		debris[i].position[0] = 0.0;
		debris[i].position[1] = 0.0;
		debris[i].position[2] = 0.0;

		debris[i].orientation[0] = 0.0;
		debris[i].orientation[1] = 0.0;
		debris[i].orientation[2] = 0.0;

		debris[i].color[0] = 0.7;
		debris[i].color[1] = 0.7;
		debris[i].color[2] = 0.7;

		debris[i].scale[0] = (2.0 * ((GLfloat)rand()) / ((GLfloat)RAND_MAX)) - 1.0;
		debris[i].scale[1] = (2.0 * ((GLfloat)rand()) / ((GLfloat)RAND_MAX)) - 1.0;
		debris[i].scale[2] = (2.0 * ((GLfloat)rand()) / ((GLfloat)RAND_MAX)) - 1.0;

		newSpeed(debris[i].speed);
		newSpeed(debris[i].orientationSpeed);
	}

	fuel = 1000;
}

GLuint	texture[30];
GLuint g_textureID = -1;
const string textureName[30] = { "Data/monalisa.bmp","Data/gentleman.bmp","Data/girlwithearing.bmp","Data/girlwithearing2.bmp" };
const int TEXTURENUM = 4;

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
///////////////////////////
void texturedSphere(float radius, int slices) {

	GLUquadric* obj = gluNewQuadric();
	gluQuadricTexture(obj, GL_TRUE);
	gluQuadricNormals(obj, GLU_SMOOTH);
	gluSphere(obj, radius, slices, slices);
	gluDeleteQuadric(obj);
}
/////////////////////////////
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
	gluLookAt(px, py, pz, px + lx, py + ly, pz + lz, 0.0f, 1.0f, 0.0f);
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


void drawCircle() {
	glColor3f(1.0f, 0.7f, 1.0f);
	glTranslatef(0.0f, 5.0f, 0.0f);
	glutSolidSphere(0.75f, 20, 20);
}
float testspeed;

void MyTimer(int value) {
	testspeed += 2 * dx;
	glutPostRedisplay();
	crash();
	//printf("%f %f %d %f\n", ptLastMousePosit.x, ptLastMousePosit.y, ptCurrentMousePosit.x, ptCurrentMousePosit.y);
	////////////////////////////////////////////////////
	x1 += dx;
	if (x1 > 4 || x1 < -5) {
		dx *= -1;
	}
	else if (x1 > 11 || x1 < -4) {
		dx *= -1;
	}

	x2 += dx;
	if (x2 > 5 || x2 < -5) {
		dx2 *= -2;
	}

	
	if (x3 <20) {
		x3 += dx3;
	}
	else if (x4 < 20) {
		x4 += dx4;
	}
	else if (x5 < 20) {
		x5 += dx5;
	}
	else if (x6 < 20) {
		x6 += dx6;
	}
	else if (x3 > 20) {
		x3 = 0;
		x4 = 0;
		x5 = 0;
		x6 = 0;
	}
	/////////////////////////////////////////////////////////////////////
	mobs[0].p = vec3(0.0f + x1, -50.0f, 48.0f);
	mobs[0].size = 0.1f;
	mobs[1].p = vec3(-26.0f, -50.0f, 20.0f + x1);
	mobs[1].size = 0.1f;
	mobs[2].p = vec3(28.0f, -50.0f, 20.0f + x1);
	mobs[2].size = 0.1f;
	//printf("%f %f %f\n", mobs[0].p.x, mobs[0].p.y, mobs[0].p.z);
	glutTimerFunc(40, MyTimer, 1);
	if (die == true)
	{

		//x = 300.0f;
		pz = 500.0f;
		//newExplosion();

	}
	if (die == false && py == 1.75f && pz >= 29.f)
	{
		px = 0.0f;
		py = -48.25f;
		pz = 0.0f;
	}

}
////////////////////////////////////////////////
void drawtrap() {
	glColor3f(1.0f, 0.0f, 0.0f);
	glPushMatrix(); //가운데 밖
	glTranslatef(0.0f + x1, -50.0f, 51.0f);
	glRotatef(-90, 1, 0, 0);
	glutSolidCylinder(0.5f, 5.0f, 32, 4);
	glPopMatrix();

	glPushMatrix(); //가운데 안
	glTranslatef(-5.0f, -47.0f +x2, 43.0f);
	glRotatef(90, 1, 90, 0);
	glutSolidCylinder(0.5f, 60.0f, 32, 4);
	glPopMatrix();

	glPushMatrix(); //왼쪽 안
	glTranslatef(33.0f, -50.0f, 20.0f + x1);
	glRotatef(-90, 1, 0, 0);
	glutSolidCylinder(0.5f, 5.0f, 32, 4);
	glPopMatrix();

	glPushMatrix(); //왼쪽 밖
	glTranslatef(23.0f, -50.0f, 19.5f - x2);
	glRotatef(-90, 1, 0, 0);
	glutSolidCylinder(0.5f, 5.0f, 32, 4);
	glPopMatrix();

	glPushMatrix();//오른쪽 밖
	glTranslatef(-23.0f, -50.0f, 20.0f + x1);
	glRotatef(-90, 1, 0, 0);
	glutSolidCylinder(0.5f, 5.0f, 32, 4);
	glPopMatrix();

	glPushMatrix(); //오른쪽 안
	glTranslatef(-33.0f, -50.0f, 20.0f -x1);
	glRotatef(-90, 1, 0, 0);
	glutSolidCylinder(0.5f, 5.0f, 32, 4);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-10.0f + x3 -x5, -48.5f, 90.0f-x4+x6);
	glutSolidSphere(1.5, 30, 30);
	glPopMatrix();

	//glPushMatrix();
	//glBegin(GL_QUADS);
	////가운데
	//glVertex3f(-5.0f, -48.5f + x1, 90.0f);
	//glVertex3f(-5.0f, -49.0f + x1, 90.0f);
	//glVertex3f(5.0f, -49.0f + x1, 90.0f);
	//glVertex3f(5.0f, -48.5f + x1, 90.0f);

	//glVertex3f(5.0f, -48.5f + x1, 100.0f);
	//glVertex3f(5.0f, -49.0f + x1, 100.0f);
	//glVertex3f(5.0f, -49.0f + x1, 90.0f);
	//glVertex3f(5.0f, -48.5f + x1, 90.0f);

	//glVertex3f(-5.0f, -48.5f + x1, 100.0f);
	//glVertex3f(-5.0f, -49.0f + x1, 100.0f);
	//glVertex3f(-5.0f, -49.0f + x1, 90.0f);
	//glVertex3f(-5.0f, -48.5f + x1, 90.0f);

	////왼쪽
	//glVertex3f(70.0f, -48.5f + x1, 25.0f);
	//glVertex3f(70.0f, -49.0f + x1, 25.0f);
	//glVertex3f(70.0f, -49.0f + x1, 15.0f);
	//glVertex3f(70.0f, -48.5f + x1, 15.0f);

	//glVertex3f(80.0f, -48.5f + x1, 15.0f);
	//glVertex3f(80.0f, -49.0f + x1, 15.0f);
	//glVertex3f(70.0f, -49.0f + x1, 15.0f);
	//glVertex3f(70.0f, -48.5f + x1, 15.0f);

	//glVertex3f(80.0f, -48.5f + x1, 25.0f);
	//glVertex3f(80.0f, -49.0f + x1, 25.0f);
	//glVertex3f(70.0f, -49.0f + x1, 25.0f);
	//glVertex3f(70.0f, -48.5f + x1, 25.0f);

	////오른쪽
	//glVertex3f(-70.0f, -48.5f + x1, 25.0f);
	//glVertex3f(-70.0f, -49.0f + x1, 25.0f);
	//glVertex3f(-70.0f, -49.0f + x1, 15.0f);
	//glVertex3f(-70.0f, -48.5f + x1, 15.0f);

	//glVertex3f(-80.0f, -48.5f + x1, 15.0f);
	//glVertex3f(-80.0f, -49.0f + x1, 15.0f);
	//glVertex3f(-70.0f, -49.0f + x1, 15.0f);
	//glVertex3f(-70.0f, -48.5f + x1, 15.0f);

	//glVertex3f(-80.0f, -48.5f + x1, 25.0f);
	//glVertex3f(-80.0f, -49.0f + x1, 25.0f);
	//glVertex3f(-70.0f, -49.0f + x1, 25.0f);
	//glVertex3f(-70.0f, -48.5f + x1, 25.0f);
	//glPopMatrix();

	//glEnd();
}

/////////////////////////////////////////////////
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
	glInitNames();

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
	//미술관 땅1(오른쪽방 gentleman)
	drawFloor(-60.0f, 20.0f, 20.0f);
	drawWall(20.0f - 60.0f, 0.0f, 1);
	drawWall(20.0f - 60.0f, 25.0f, 1);
	drawFullWall(-20.0f - 60.0f, 0.0f, 1);
	drawFullWall(-20.0f - 60.0f, 0.0f, 0);
	drawFullWall(-20.0f - 60.0f, 40.0f, 0);
	glPushName(100);
	drawPicture(-78.5f, 20.0f, 1, 1);
	glPopName();
	drawPannel(-78.5f, 20.0f - 1.6f, 1);
	drawPannelGround(-50.0f, 15.0f, 45.0f);
	//미술관 땅2 (왼쪽 monalisa)
	drawFloor(60.0f, 20.0f, 20.0f);
	drawWall(-20.0f + 60.0f, 0.0f, 1);
	drawWall(-20.0f + 60.0f, 25.0f, 1);
	drawFullWall(20.0f + 60.0f, 0.0f, 1);
	drawFullWall(-20.0f + 60.0f, 0.0f, 0);
	drawFullWall(-20.0f + 60.0f, 40.0f, 0);
	glPushName(101);
	drawPicture(79.5f, 20.0f, 1, 0);
	glPopName();
	drawPannel(78.5f, 20.0f + 2.8f, 1);
	drawPannelGround(50.0f, 25.0f, 45.0f);
	//미술관 땅3(정면방 girl)
	drawFloor(0, 80.0f, 20.0f);
	drawWall(-20.0f, 60.0f, 0);
	drawWall(5.0f, 60.0f, 0);
	drawFullWall(20.0f, 60.0f, 1);
	drawFullWall(-20.0f, 100.0f, 0);
	drawFullWall(-20.0f, 60.0f, 1);
	glPushName(102);
	drawPicture(0.0f, 99.5f, 0, 2);
	glPopName();
	drawPannel(0.0f - 1.6f, 99.5f, 0);
	glEnd();


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
}


void SetTextMessage(GLuint index[64])
{
	switch (index[3]) {

	case 100:
		sprintf(name, "gentleman");
		trap = true;
		printf("gentleman\n");
		break;
	case 101: sprintf(name, "monalisa");
		trap = true;
		printf("monalisa\n");
		break;
	case 102: sprintf(name, "girl");
		trap = true;
		printf("girl\n");
		break;


		//default: sprintf_s(name, "None"); break;
	}
}
void SelectObjects(GLint x, GLint y) {
	GLuint selectBuff[64];
	GLint hits, viewport[4];

	glSelectBuffer(64, selectBuff);
	glGetIntegerv(GL_VIEWPORT, viewport);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glRenderMode(GL_SELECT);
	glLoadIdentity();
	gluPickMatrix(x, viewport[3] - y, 2, 2, viewport);
	renderScene();
	//gluPerspective(45.0f, (GLfloat)g_nWindowWidth / (GLfloat)g_nWindowHeight, 0.1f, 100.0f);
	//gluPerspective(45, ratio, 1, 1000);
	gluLookAt(px, py, pz, px + lx, py + ly, pz + lz, 0.0f, 1.0f, 0.0f);
	/*glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();*/

	renderScene();
	hits = glRenderMode(GL_RENDER);
	if (hits > 0)
	{
		//ProcessSelect(selectBuff);
		SetTextMessage(selectBuff);
	}
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}



void renderScene(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	drawMap();
	if (trap == true)
		drawtrap();
	glPushMatrix();
	{
		glTranslatef(0.0f, 1.0f, 10.0f);
		//addMob(vec3(x, y, z + 10),0.2);
	}
	glPopMatrix();



	sprintf(coor, "x : %f y : %f z : %f", px, py, pz);
	sprintf(name, "Picture");
	sprintf(message, "You DIED");
	sprintf(diemessage, "Press 'r' to restart");
	beginRenderText(g_nWindowWidth, g_nWindowHeight);
	{
		glColor3f(1.f, 1.f, 1.0f);
		if (die == false && text == true)
		{
			renderText(g_nWindowWidth - 0.95, g_nWindowHeight + 0.9, BITMAP_FONT_TYPE_HELVETICA_12, coor);
			renderText(g_nWindowWidth - 0.95, g_nWindowHeight + 0.8, BITMAP_FONT_TYPE_HELVETICA_12, name);
		}
		if (die == true)
		{
			renderText(g_nWindowWidth - 0.08, g_nWindowHeight + 0.05, BITMAP_FONT_TYPE_HELVETICA_18, message);
			renderText(g_nWindowWidth - 0.15, g_nWindowHeight - 0.05, BITMAP_FONT_TYPE_HELVETICA_18, diemessage);
		}
	}
	endRenderText();


	if (fuel > 0) {
		glPushMatrix();
		glDisable(GL_DEPTH_TEST);
		glBegin(GL_POINTS);
		for (int i = 0; i < NUM_PARTICLES; i++) {
			glColor3fv(particles[i].color);
			glVertex3fv(particles[i].position);
		}
		glEnd();
		glPopMatrix();



		for (int i = 0; i < NUM_DEBRIS; i++) {
			glColor3fv(debris[i].color);

			glPushMatrix();
			glTranslatef(debris[i].position[0],
				debris[i].position[1],
				debris[i].position[2]);

			glRotatef(debris[i].orientation[0], 1.0, 0.0, 0.0);
			glRotatef(debris[i].orientation[1], 0.0, 1.0, 0.0);
			glRotatef(debris[i].orientation[2], 0.0, 0.0, 1.0);

			glScalef(debris[i].scale[0], debris[i].scale[1], debris[i].scale[2]);

			glBegin(GL_TRIANGLES);
			glNormal3f(0.0, 0.0, 1.0);
			glVertex3f(0.0, 0.5, 0.0);
			glVertex3f(-0.25, 0.0, 0.0);
			glVertex3f(0.25, 0.0, 0.0);
			glEnd();
			glPopMatrix();
		}
	}
	glutSwapBuffers();
}

void orientMe(float ang) {

	lx = sin(ang);
	lz = -cos(ang);
	glLoadIdentity();
	gluLookAt(px, py, pz, px + lx, py + ly, pz + lz, 0.0f, 1.0f, 0.0f);
	sprintf(coor, "x : %f y : %f z : %f", px, py, pz);
}


void moveMeFlat(int i) {
	px = px + i * (lx) * 0.2;
	pz = pz + i * (lz) * 0.2;
	glLoadIdentity();
	gluLookAt(px, py, pz, px + lx, py + ly, pz + lz, 0.0f, 1.0f, 0.0f);
	sprintf(coor, "x : %f y : %f z : %f", px, py, pz);
}

void processNormalKeys(unsigned char key, int x, int y) {

	if (key == 27)
		exit(0);
}

//void addMob(vec3 position, float size,float velocity)
//{
//	Mob newmob;
//
//	vec3 _pos(0, 0, 0);
//	_pos.x = position.x;
//	_pos.y = position.y;
//	_pos.z = position.z;
//	float _size = size;
//	//newmob.p = _pos;
//	newmob.p.x = _pos.x;
//	newmob.p.y = _pos.y;
//	newmob.p.z = _pos.z;
//	newmob.v = velocity;
//	newmob.size = _size;
//	
//
//	mobs.push_back(newmob);
//}
void addMob()
{
	Mob newmob;
	mobs.push_back(newmob);
}
vector<float> dist;
void crash()
{
	for (int i = 0; i < mobs.size(); i++)
	{
		vec3 p = vec3(px, py, pz);
		vec3 dis = mobs[i].p - p;
		float L = length(dis);
		//printf("%f %f %f\n", px, mobs[0].p.x, L);
		if (L <= 2.0f)
		{
			die = true;
			printf("닿았따\n");
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
		moveMeFlat(2);
		break;
	case 's':
		moveMeFlat(-2);
		break;
	case 'k':
		die = true;
		break;
	case 'p':
		text = !text;
		break;
	case 'r':
		rst();
		break;
	case 'q':
		trap = true;
		break;
	}
}
///////////////////////////////////
void MyMouse(int but, int state, int x, int y) {
	switch (but) {
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN) {
			posix = x;
			posiy = y;

			if (10 < x < 20 && -80 < y < 80 && 0 <= pz && pz <= 5) {
				trap = true;
			}
		}

	}
}
/////////////////////////////////////////////////
void MyIdle(void) {
	if (fuel > 0) {
		for (int i = 0; i < NUM_PARTICLES; i++) {
			particles[i].position[0] += particles[i].speed[0] * 0.2;
			particles[i].position[1] += particles[i].speed[1] * 0.2;
			particles[i].position[2] += particles[i].speed[2] * 0.2;

			particles[i].color[0] -= 1.0 / 500.0;
			if (particles[i].color[0] < 0.0) {
				particles[i].color[0] = 0.0;
			}

			particles[i].color[1] -= 1.0 / 100.0;
			if (particles[i].color[1] < 0.0) {
				particles[i].color[1] = 0.0;
			}

			particles[i].color[2] -= 1.0 / 50.0;
			if (particles[i].color[2] < 0.0) {
				particles[i].color[2] = 0.0;
			}
		}

		for (int i = 0; i < NUM_DEBRIS; i++) {
			debris[i].position[0] += debris[i].speed[0] * 0.1;
			debris[i].position[1] += debris[i].speed[1] * 0.1;
			debris[i].position[2] += debris[i].speed[2] * 0.1;

			debris[i].orientation[0] += debris[i].orientationSpeed[0] * 10;
			debris[i].orientation[1] += debris[i].orientationSpeed[1] * 10;
			debris[i].orientation[2] += debris[i].orientationSpeed[2] * 10;
		}

		--fuel;
	}

	glutPostRedisplay();
}

void glInit()
{
	LoadGLTextures();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	for (int i = 0; i < 3; i++)
		addMob();
	if (fuel == 0) {
		glEnable(GL_DEPTH_TEST);
		glPushMatrix();
		{
			glTranslatef(300.0f, 0.0f, 300.0f);
			glutSolidCube(1.0);
		}
		glPopMatrix();
	}

}
int main(int argc, char** argv) {
		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
		glutInitWindowPosition(100, 100);
		glutInitWindowSize(900, 700);
		glutCreateWindow("yunsoo");
		glInit();
		glutKeyboardFunc(inputKey);

		glutDisplayFunc(renderScene);

		glutIdleFunc(MyIdle);

		glutTimerFunc(40, MyTimer, 1);
		glutReshapeFunc(changeSize);
		glutMouseFunc(MyMouse);
		glutMainLoop();

		return(0);
	}