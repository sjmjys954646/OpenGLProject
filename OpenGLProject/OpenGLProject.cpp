#define _CRT_SECURE_NO_WARNINGS
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
static float px = 0.0f, py = 1.75f, pz = 0.0f;
static float lx = 0.0f, ly = 0.0f, lz = 1.0f;
static POINT    ptLastMousePosit;
static POINT    ptCurrentMousePosit;
static bool        bMousing;
int screennum = 0, winddir = 45;
GLfloat windspeed = 0.0005f;


float g_fDistance = -5.0f;
float g_fSpinX = 0.0f;
float g_fSpinY = 0.0f;
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

static char coor[255];
static char message[255];
static char diemessage[255];
static char name[255];
static char clearmessage[255];
static char warn[255];
static char gohome[255];
int    g_nWindowWidth;
int    g_nWindowHeight;
void DrawCube();
//void addMob(vec3 position, float size);
void addMob();
void crash();
void DrawCubeTex();
void drawtrap();
void renderScene(void);
void drawPicture(float leftX, float midZ, bool garosero, int pictureNum);
void texturedCylinder(float radius, int height, int slices);
void texturedSphere(float radius, int slices);

GLboolean die = false;    //사망 처리
GLboolean clear = false;  //클리어 처리
GLboolean text = false;   // 좌표 및 텍스트 on/off
GLboolean trap = false;   //장애물
GLboolean girl = false;
GLboolean gentleman = false;
GLboolean monalisa = false;
GLboolean realclear = false;
GLboolean start = false;

////////////////////////////////////////////////////////////////

struct particle {
	GLfloat x, y, z;
	GLfloat r, g, b;
	GLfloat xd, yd, zd;
	GLfloat cs;
} p[1000];
void texturedCube()
{
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);  //앞면
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);

	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);  //뒷면
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);

	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);  //윗면
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);

	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);  //아랫면
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);

	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);  //우측면
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);

	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);  //좌측면
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	glEnd();
}
void SetParticle(int i)
{
	p[i].xd = -(rand() / 32767.0f - 0.5f) / 200.0f;
	p[i].zd = 2.f;
	p[i].yd = -rand() / 32767.0f / 100.0f;
	p[i].x = 30.f * (rand() / 32767.0f - 0.5f);
	p[i].y = 4.0f;
	p[i].z = 30.f * (rand() / 32767.0f - 0.5f);
	p[i].b = rand() / 32767.0f;
	p[i].g = p[i].b;
	p[i].r = p[i].b;
}
void rain()
{
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 4.0f);
	for (int i = 0; i < 1000; i++) {
		p[i].x += cos(winddir * .0174532925f) * windspeed;
		p[i].y += p[i].yd;
		p[i].z += sin(winddir * .0174532925f) * windspeed;
		p[i].yd -= rand() / 32767.0f / 100000.0f;

		if (p[i].y <= -0.5f) {
			SetParticle(i);
		}
	}

	for (int i = 0; i < 1000; i++) {
		float difx = 0.0, dify = 0.0;
		if (i % 3 == 0) {
			difx = 0.01;
			dify = 0.01;
		}
		else if (i % 3 == 1) {
			difx = 0.01;
			dify = 0.03;
		}
		else {
			difx = 0.01;
			dify = 0.05;
		}

		glDisable(GL_TEXTURE_2D);
		glColor3f(1, 0, 0);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(p[i].x - difx, p[i].y - dify, p[i].z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(p[i].x, p[i].y - dify, p[i].z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(p[i].x, p[i].y, p[i].z);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(p[i].x - difx, p[i].y, p[i].z);
		glEnd();
		glEnable(GL_TEXTURE_2D);
	}
	glPopMatrix();
}
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
	monalisa = false;
	gentleman = false;
	girl = false;
	text = false;
	realclear = false;
	px = 0.0f;
	py = 1.75f;
	pz = 0.0f;
	ang = 0.0f;
	lx = 0.0f, ly = 0.0f, lz = 1.0f;
}
void clr()
{
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

GLuint	texture[50];
GLuint g_textureID = -1;

const string textureName[50] = { "Data/monalisa.bmp","Data/gentleman.bmp","Data/girlwithearing.bmp","Data/girlwithearing2.bmp","Data/Museum_Front.bmp" ,"Data/brick.bmp" 
,"Data/Museum_Left.bmp","Data/Museum_Right.bmp","Data/BlueSky.bmp",  "Data/museum_floor.bmp", "Data/museum_floor_bokdo.bmp", "Data/museum_wall.bmp", "Data/museum_ceiling.bmp" , 
"Data/Earth.bmp","Data/ib.bmp","Data/monalisa3.bmp","Data/gentleman2.bmp","Data/1.bmp","Data/2.bmp","Data/3.bmp","Data/4.bmp","Data/5.bmp","Data/6.bmp","Data/7.bmp","Data/8.bmp",
"Data/9.bmp","Data/10.bmp","Data/11.bmp","Data/12.bmp","Data/13.bmp","Data/14.bmp","Data/15.bmp","Data/16.bmp","Data/17.bmp","Data/18.bmp" ,"Data/pannel_image_Earth.bmp" ,
"Data/pannel_image_Gentleman.bmp" ,"Data/pannel_image_Aladin.bmp" ,"Data/pannel_image_SoccerBall.bmp" ,"Data/pannel_image_Monalisa.bmp" ,"Data/pannel_image_Girl.bmp",
"Data/pannel_stick_image2.bmp", "Data/NoPhotography.bmp", "Data/pannel_image_dont_touch.bmp"
};
const int TEXTURENUM = 44;

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
	gluLookAt(px, py, pz, px + lx, py + ly, pz + lz, 0.0f, 1.0f, 0.0f);
}

void LoadGLTextures() {
	AUX_RGBImageRec* TextureImage[50];
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
void MyTimer(int value) {
	glutPostRedisplay();
	crash();
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


	if (x3 < 20) {
		x3 += 1.5 * dx3;
	}
	else if (x4 < 20) {
		x4 += 1.5 * dx4;
	}
	else if (x5 < 20) {
		x5 += 1.5 * dx5;
	}
	else if (x6 < 20) {
		x6 += 1.5 * dx6;
	}
	else if (x3 > 20) {
		x3 = 0;
		x4 = 0;
		x5 = 0;
		x6 = 0;
	}
	mobs[0].p = vec3(0.0f + x1, -50.0f, 51.0f);
	mobs[1].p = vec3(-5.0f, -47.0f + x2, 43.0f);
	mobs[2].p = vec3(33.0f, -50.0f, 20.0f + x1);
	mobs[3].p = vec3(23.0f, -50.0f, 19.5f - x2);
	mobs[4].p = vec3(-23.0f, -50.0f, 20.0f + x1);
	mobs[5].p = vec3(-33.0f, -50.0f, 20.0f - x1);
	mobs[6].p = vec3(-10.0f + x3 - x5, -48.5f, 90.0f - x4 + x6);
	
	if (die == true)
	{

		//x = 300.0f;
		pz = -500.0f;
		//newExplosion();

	}
	if (die == false && py == 1.75f && pz >= 29.f && pz <= 35.f)
	{
		px = 0.0f;
		py = -48.25f;
		pz = 0.0f;
	}
	if (girl == true && gentleman == true && monalisa == true)
	{
		clear = true;
	}

	if (girl == true || gentleman == true || monalisa == true)
		trap = true;

	if (clear == true && px <= 2.0f && px >= -2.0f && py == -48.25f && pz <= 2.0f && pz >= -2.0f)
		realclear = true;

	if (realclear == true)
	{
		clr();
	}
	if (py == 51.75f && start == true)
	{
		px = 0.0f;
		py = 1.75f;
		pz = 0.0f;
	}
	glutTimerFunc(40, MyTimer, 1);
}
void drawtrap() {
	glColor3f(1.0f, 1.0f, 1.0f);
	glPushMatrix(); //가운데 밖
	glTranslatef(0.0f + x1, -50.0f, 51.0f);
	glRotatef(-90, 1, 0, 0);
	glBindTexture(GL_TEXTURE_2D, texture[10]);
	texturedCylinder(0.5f, 5.0f, 32);
	glPopMatrix();

	glPushMatrix(); //가운데 안
	glTranslatef(-5.0f, -47.0f + x2, 43.0f);
	glRotatef(90, 1, 90, 0);
	glBindTexture(GL_TEXTURE_2D, texture[10]);
	texturedCylinder(0.5f, 10.0f, 32);
	glPopMatrix();

	glPushMatrix(); //왼쪽 안
	glTranslatef(33.0f, -50.0f, 20.0f + x1);
	glRotatef(-90, 1, 0, 0);
	glBindTexture(GL_TEXTURE_2D, texture[10]);
	texturedCylinder(0.5f, 5.0f, 32);
	glPopMatrix();

	glPushMatrix(); //왼쪽 밖
	glTranslatef(23.0f, -50.0f, 19.5f - x2);
	glRotatef(-90, 1, 0, 0);
	glBindTexture(GL_TEXTURE_2D, texture[10]);
	texturedCylinder(0.5f, 5.0f, 32);
	glPopMatrix();

	glPushMatrix();//오른쪽 밖
	glTranslatef(-23.0f, -50.0f, 20.0f + x1);
	glRotatef(-90, 1, 0, 0);
	glBindTexture(GL_TEXTURE_2D, texture[10]);
	texturedCylinder(0.5f, 5.0f, 32);
	glPopMatrix();

	glPushMatrix(); //오른쪽 안
	glTranslatef(-33.0f, -50.0f, 20.0f - x1);
	glRotatef(-90, 1, 0, 0);
	glBindTexture(GL_TEXTURE_2D, texture[10]);
	texturedCylinder(0.5f, 5.0f, 32);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-10.0f + x3 - x5, -48.5f, 90.0f - x4 + x6);
	glBindTexture(GL_TEXTURE_2D, texture[15]);
	texturedCube();
	
	glPopMatrix();
}

void drawStartPoint()
{
	glBindTexture(GL_TEXTURE_2D, texture[14]);
	glBegin(GL_QUADS);
	glColor3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0, 0); glVertex3f(-10.0f, 48.0f, 15.0f);
	glTexCoord2f(0, 1); glVertex3f(10.0f, 48.0f, 15.0f);
	glTexCoord2f(1, 1); glVertex3f(10.0f, 56.0f, 15.0f);
	glTexCoord2f(1, 0); glVertex3f(-10.0f, 56.0f, 15.0f);
	glEnd();
	//도입길
	glBindTexture(GL_TEXTURE_2D, texture[5]);
	glBegin(GL_QUADS);
	glColor3f(0.9f, 0.9f, 0.9f);
	glTexCoord2f(0, 0); glVertex3f(-10.0f, 0.0f, 30.0f);
	glTexCoord2f(0, 1); glVertex3f(10.0f, 0.0f, 30.0f);
	glTexCoord2f(1, 1); glVertex3f(10.0f, 0.0f, 0.0f);
	glTexCoord2f(1, 0); glVertex3f(-10.0f, 0.0f, 0.0f);
	glEnd();

	//하늘
	glBindTexture(GL_TEXTURE_2D, texture[8]);
	glBegin(GL_QUADS);
	glColor3f(0.9f, 0.9f, 0.9f);
	glTexCoord2f(0, 0); glVertex3f(-10.0f, 10.0f, 35.0f);
	glTexCoord2f(0, 1); glVertex3f(10.0f, 10.0f, 35.0f);
	glTexCoord2f(1, 1); glVertex3f(10.0f, 10.0f, 0.0f);
	glTexCoord2f(1, 0); glVertex3f(-10.0f, 10.0f, 0.0f);
	glEnd();


	//문
	glBindTexture(GL_TEXTURE_2D, texture[4]);
	glBegin(GL_QUADS);
	glColor3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0, 0); glVertex3f(-10.0f, 0.0f, 30.0f);
	glTexCoord2f(0, 1); glVertex3f(10.0f, 0.0f, 30.0f);
	glTexCoord2f(1, 1); glVertex3f(10.0f, 10.0f, 30.0f);
	glTexCoord2f(1, 0); glVertex3f(-10.0f, 10.0f, 30.0f);
	glEnd();

	//문 Right
	glBindTexture(GL_TEXTURE_2D, texture[6]);
	glBegin(GL_QUADS);
	glColor3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(1, 1); glVertex3f(-10.0f, 10.0f, 0.0f);
	glTexCoord2f(1, 0); glVertex3f(-10.0f, 10.0f, 30.0f);
	glTexCoord2f(0, 0); glVertex3f(-10.0f, 0.0f, 30.0f);
	glTexCoord2f(0, 1); glVertex3f(-10.0f, 0.0f, 0.0f);
	glEnd();

	//문 Left
	glBindTexture(GL_TEXTURE_2D, texture[7]);
	glBegin(GL_QUADS);
	glColor3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(1, 1); glVertex3f(10.0f, 10.0f, 0.0f);
	glTexCoord2f(1, 0); glVertex3f(10.0f, 10.0f, 30.0f);
	glTexCoord2f(0, 0); glVertex3f(10.0f, 0.0f, 30.0f);
	glTexCoord2f(0, 1); glVertex3f(10.0f, 0.0f, 0.0f);
	glEnd();
}

void drawFullWall(float leftX, float midZ, bool garosero)
{
	if (garosero == 0)
	{
		glBindTexture(GL_TEXTURE_2D, texture[11]);
		glBegin(GL_QUADS);
		glColor3f(1.0f, 1.0f, 1.0f);
		glTexCoord2f(0, 0); glVertex3f(leftX, -50.0f, midZ);
		glTexCoord2f(0, 1); glVertex3f(leftX, -45.0f, midZ);
		glTexCoord2f(1, 1); glVertex3f(leftX + 40.0f, -45.0f, midZ);
		glTexCoord2f(1, 0); glVertex3f(leftX + 40.0f, -50.0f, midZ);
		glEnd();
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, texture[11]);
		glBegin(GL_QUADS);
		glColor3f(1.0f, 1.0f, 1.0f);
		glTexCoord2f(0, 0); glVertex3f(leftX, -50.0f, midZ);
		glTexCoord2f(0, 1); glVertex3f(leftX, -45.0f, midZ);
		glTexCoord2f(1, 1); glVertex3f(leftX, -45.0f, midZ + 40.0f);
		glTexCoord2f(1, 0); glVertex3f(leftX, -50.0f, midZ + 40.0f);
		glEnd();
	}
}

void drawWall(float leftX, float midZ, bool garosero)
{
	if (garosero == 0)
	{
		glBindTexture(GL_TEXTURE_2D, texture[11]);
		glBegin(GL_QUADS);
		glColor3f(1.0f, 1.0f, 1.0f);
		glTexCoord2f(0, 0); glVertex3f(leftX, -50.0f, midZ);
		glTexCoord2f(0, 1); glVertex3f(leftX, -45.0f, midZ);
		glTexCoord2f(1, 1); glVertex3f(leftX + 15.0f, -45.0f, midZ);
		glTexCoord2f(1, 0); glVertex3f(leftX + 15.0f, -50.0f, midZ);
		glEnd();
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, texture[11]);
		glBegin(GL_QUADS);
		glColor3f(1.0f, 1.0f, 1.0f);
		glTexCoord2f(0, 0); glVertex3f(leftX, -50.0f, midZ);
		glTexCoord2f(0, 1); glVertex3f(leftX, -45.0f, midZ);
		glTexCoord2f(1, 1); glVertex3f(leftX, -45.0f, midZ + 15.0f);
		glTexCoord2f(1, 0); glVertex3f(leftX, -50.0f, midZ + 15.0f);
		glEnd();
	}
}

void drawFloor(float midX, float midZ, float size, int imageNum)
{
	glBindTexture(GL_TEXTURE_2D, texture[imageNum]);
	glBegin(GL_QUADS);
	glColor3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0, 0); glVertex3f(midX - size, -50.0f, midZ - size);
	glTexCoord2f(0, 1); glVertex3f(midX + size, -50.0f, midZ - size);
	glTexCoord2f(1, 1); glVertex3f(midX + size, -50.0f, midZ + size);
	glTexCoord2f(1, 0); glVertex3f(midX - size, -50.0f, midZ + size);
	glEnd();
}

void drawBokdoWall(float leftX, float midZ, bool garosero)
{
	if (garosero == 0)
	{
		glBindTexture(GL_TEXTURE_2D, texture[11]);
		glBegin(GL_QUADS);
		glColor3f(1.0f, 1.0f, 1.0f);
		glTexCoord2f(0, 0); glVertex3f(leftX, -50.0f, midZ);
		glTexCoord2f(0, 1); glVertex3f(leftX, -45.0f, midZ);
		glTexCoord2f(1, 1); glVertex3f(leftX + 20.0f, -45.0f, midZ);
		glTexCoord2f(1, 0); glVertex3f(leftX + 20.0f, -50.0f, midZ);
		glEnd();
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, texture[11]);
		glBegin(GL_QUADS);
		glColor3f(1.0f, 1.0f, 1.0f);
		glTexCoord2f(0, 0); glVertex3f(leftX, -50.0f, midZ);
		glTexCoord2f(0, 1); glVertex3f(leftX, -45.0f, midZ);
		glTexCoord2f(1, 1); glVertex3f(leftX, -45.0f, midZ + 20.0f);
		glTexCoord2f(1, 0); glVertex3f(leftX, -50.0f, midZ + 20.0f);
		glEnd();
	}
}

void texturedCylinder(float radius,int height, int slices) {
	GLUquadric* obj = gluNewQuadric();
	gluQuadricTexture(obj, GL_TRUE);
	gluQuadricNormals(obj, GLU_SMOOTH);
	gluCylinder(obj, radius, radius, height, slices, slices);
	gluDeleteQuadric(obj);
}

void texturedSphere(float radius, int slices) {
	GLUquadric* obj = gluNewQuadric();
	gluQuadricTexture(obj, GL_TRUE);
	gluQuadricNormals(obj, GLU_SMOOTH);
	gluSphere(obj, radius, slices, slices);
	gluDeleteQuadric(obj);
}

void drawPicture(float leftX, float midZ, bool garosero, int pictureNum)
{

	//leftX = leftX - 1.2f;
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

void drawPannelGround(float posX, float posZ, float angle, int pictureNum, int needReverse)
{
	glPushMatrix();
	{
		glTranslatef(posX, 0.0f, posZ);
		glRotatef(angle, 0, 1, 0);
	}
	glBindTexture(GL_TEXTURE_2D, texture[41]);
	glBegin(GL_QUADS);
	glColor3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0, 0); glVertex3f(-0.1f, -50.0f, 0.0f);
	glTexCoord2f(0, 1); glVertex3f(-0.1f, -49.0f, 0.0f);
	glTexCoord2f(1, 1); glVertex3f(0.1f, -49.0f, 0.0f);
	glTexCoord2f(1, 0); glVertex3f(0.1f, -50.0f, 0.0f);
	glEnd();
	if (needReverse)
	{
		glBindTexture(GL_TEXTURE_2D, texture[pictureNum]);
		glBegin(GL_QUADS);
		glColor3f(1.0f, 1.0f, 1.0f);
		glTexCoord2f(0, 0); glVertex3f(-0.8f, -49.0f, 0.0f);
		glTexCoord2f(0, 1); glVertex3f(-0.8f, -48.2f, 0.0f);
		glTexCoord2f(1, 1); glVertex3f(0.8f, -48.2f, 0.0f);
		glTexCoord2f(1, 0); glVertex3f(0.8f, -49.0f, 0.0f);
		glEnd();
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, texture[pictureNum]);
		glBegin(GL_QUADS);
		glColor3f(1.0f, 1.0f, 1.0f);
		glTexCoord2f(1, 0); glVertex3f(-0.8f, -49.0f, 0.0f);
		glTexCoord2f(1, 1); glVertex3f(-0.8f, -48.2f, 0.0f);
		glTexCoord2f(0, 1); glVertex3f(0.8f, -48.2f, 0.0f);
		glTexCoord2f(0, 0); glVertex3f(0.8f, -49.0f, 0.0f);
		glEnd();
	}
	glPopMatrix();
}

void drawPannel(float leftX, float midZ, bool garosero, int pictureNum, int needReverse, float yUp)
{
	leftX = leftX - 1.2f;
	if (garosero == 0)
	{
		glBindTexture(GL_TEXTURE_2D, texture[pictureNum]);
		glBegin(GL_QUADS);
		glColor3f(1.0f, 1.0f, 1.0f);
		glTexCoord2f(1, 0); glVertex3f(leftX, -48.0f + yUp, midZ);
		glTexCoord2f(1, 1); glVertex3f(leftX, -47.2f + yUp, midZ);
		glTexCoord2f(0, 1); glVertex3f(leftX + 1.2f, -47.2f + yUp, midZ);
		glTexCoord2f(0, 0); glVertex3f(leftX + 1.2f, -48.0f + yUp, midZ);
		glEnd();

	}
	else
	{
		if (needReverse)
		{
			glBindTexture(GL_TEXTURE_2D, texture[pictureNum]);
			glBegin(GL_QUADS);
			glColor3f(1.0f, 1.0f, 1.0f);
			glTexCoord2f(0, 0);  glVertex3f(leftX, -48.0f + yUp, midZ);
			glTexCoord2f(0, 1); glVertex3f(leftX, -47.2f + yUp, midZ);
			glTexCoord2f(1, 1); glVertex3f(leftX, -47.2f + yUp, midZ + 1.2f);
			glTexCoord2f(1, 0); glVertex3f(leftX, -48.0f + yUp, midZ + 1.2f);
			glEnd();
		}
		else
		{

			glBindTexture(GL_TEXTURE_2D, texture[pictureNum]);
			glBegin(GL_QUADS);
			glColor3f(1.0f, 1.0f, 1.0f);
			glTexCoord2f(1, 0);  glVertex3f(leftX, -48.0f + yUp, midZ);
			glTexCoord2f(1, 1); glVertex3f(leftX, -47.2f + yUp, midZ);
			glTexCoord2f(0, 1); glVertex3f(leftX, -47.2f + yUp, midZ + 1.2f);
			glTexCoord2f(0, 0); glVertex3f(leftX, -48.0f + yUp, midZ + 1.2f);
			glEnd();
		}
	}
}

void drawGallary()
{
	glBindTexture(GL_TEXTURE_2D, texture[11]);
	glBegin(GL_QUADS);
	glColor3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0, 0); glVertex3f(-5.0f, -50.0f, -20.0f);
	glTexCoord2f(0, 1); glVertex3f(5.0f, -50.0f, -20.0f);
	glTexCoord2f(1, 1); glVertex3f(5.0f, -45.0f, -20.0f);
	glTexCoord2f(1, 0); glVertex3f(-5.0f, -45.0f, -20.0f);
	glEnd();

	//미술관 복도 아래
	glBindTexture(GL_TEXTURE_2D, texture[10]);
	glBegin(GL_QUADS);
	glColor3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0, 0); glVertex3f(-5.0f, -50.0f, -20.0f);
	glTexCoord2f(0, 1); glVertex3f(5.0f, -50.0f, -20.0f);
	glTexCoord2f(1, 1); glVertex3f(5.0f, -50.0f, 0.0f);
	glTexCoord2f(1, 0); glVertex3f(-5.0f, -50.0f, 0.0f);
	glEnd();

	//미술관 복도 왼쪽
	glBindTexture(GL_TEXTURE_2D, texture[11]);
	glBegin(GL_QUADS);
	glColor3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0, 0); glVertex3f(-5.0f, -50.0f, -20.0f);
	glTexCoord2f(0, 1); glVertex3f(-5.0f, -50.0f, 0.0f);
	glTexCoord2f(1, 1); glVertex3f(-5.0f, -45.0f, 0.0f);
	glTexCoord2f(1, 0); glVertex3f(-5.0f, -45.0f, -20.0f);
	glEnd();

	//미술관 복도 오른쪽
	glBindTexture(GL_TEXTURE_2D, texture[11]);
	glBegin(GL_QUADS);
	glColor3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0, 0); glVertex3f(5.0f, -50.0f, -20.0f);
	glTexCoord2f(0, 1); glVertex3f(5.0f, -50.0f, 0.0f);
	glTexCoord2f(1, 1); glVertex3f(5.0f, -45.0f, 0.0f);
	glTexCoord2f(1, 0); glVertex3f(5.0f, -45.0f, -20.0f);
	glEnd();

	//미술관 땅
	glInitNames();

	drawFloor(0.0f, 20.0f, 20.0f, 9);
	drawWall(-20.0f, 0.0f, 0);
	drawWall(5.0f, 0.0f, 0);
	drawWall(-20.0f, 40.0f, 0);
	drawWall(5.0f, 40.0f, 0);
	drawWall(-20.0f, 0.0f, 1);
	drawWall(-20.0f, 25.0f, 1);
	drawWall(20.0f, 0.0f, 1);
	drawWall(20.0f, 25.0f, 1);
	drawPannel(-20.0f + 1.5f, 12.0f, 1, 36,0, 0.0f);
	drawPannel(-20.0f + 1.5f, 12.0f, 1, 42, 0, 1.0f);

	drawPannel(20.0f + 1.1f , 26.0f, 1, 39,1, 0.0f);
	drawPannel(20.0f + 1.1f, 26.0f, 1, 42, 1, 1.0f);

	drawPannel(-5.6f, 40.0f - 0.5f, 0, 40, 0, 0.0f);
	drawPannel(-5.6f, 40.0f - 0.5f, 0, 42, 0, 1.0f);

	//drawPannelGround(10.0f, 28.0f, 30.0f);
	drawPannelGround(0.0f, 10.0f, 0.0f, 43, 0);
	drawPannelGround(-10.0f, 30.0f, -30.0f, 38,0);
	//미술관 땅1(오른쪽방 gentleman)
	drawFloor(-60.0f, 20.0f, 20.0f, 9);
	drawWall(20.0f - 60.0f, 0.0f, 1);
	drawWall(20.0f - 60.0f, 25.0f, 1);
	drawFullWall(-20.0f - 60.0f, 0.0f, 1);
	drawFullWall(-20.0f - 60.0f, 0.0f, 0);
	drawFullWall(-20.0f - 60.0f, 40.0f, 0);
	glPushName(100);
	drawPicture(-78.5f, 30.0f, 1, 1);
	glPopName();
	drawPannelGround(-50.0f, 15.0f, 45.0f, 37,1);
	glPopMatrix();
	//정면
	drawPicture(-78.5f, 20.0f, 1, 29);
	drawPicture(-78.5f, 10.0f, 1, 30);
	//왼쪽
	drawPicture(-55.f, 39.8f, 0, 31);
	drawPicture(-65.f, 39.8f, 0, 32);
	//오른쪽
	drawPicture(-55.f, 0.2f, 0, 33);
	drawPicture(-65.f, 0.2f, 0, 34);

	//미술관 땅2 (왼쪽 monalisa)
	drawFloor(60.0f, 20.0f, 20.0f, 9);
	drawWall(-20.0f + 60.0f, 0.0f, 1);
	drawWall(-20.0f + 60.0f, 25.0f, 1);
	drawFullWall(20.0f + 60.0f, 0.0f, 1);
	drawFullWall(-20.0f + 60.0f, 0.0f, 0);
	drawFullWall(-20.0f + 60.0f, 40.0f, 0);
	glPushName(101);
	drawPicture(65.f, 1.0f, 0, 0);
	glPopName();
	drawPannelGround(50.0f, 25.0f, 45.0f, 35,0);
	//정면
	drawPicture(79.5f, 30.0f, 1, 17);
	drawPicture(79.5f, 10.0f, 1, 18);
	//왼쪽
	drawPicture(55.f, 1.0f, 0, 19);
	drawPicture(79.5f, 20.0f, 1, 20);
	//오른쪽
	drawPicture(55.f, 39.8f, 0, 21);
	drawPicture(65.f, 39.8f, 0, 22);

	//미술관 땅3(정면방 girl)
	drawFloor(0, 80.0f, 20.0f, 9);
	drawWall(-20.0f, 60.0f, 0);
	drawWall(5.0f, 60.0f, 0);
	drawFullWall(20.0f, 60.0f, 1);
	drawFullWall(-20.0f, 100.0f, 0);
	drawFullWall(-20.0f, 60.0f, 1);
	glPushName(102);
	drawPicture(-19.6f, 85.0f, 1, 2);
	glPopName();
	//정면
	drawPicture(10.0f, 99.5f, 0, 23);
	drawPicture(-10.0f, 99.5f, 0, 24);
	//왼쪽벽
	drawPicture(19.6f, 85.0f,1, 25);
	drawPicture(19.6f, 75.0f, 1, 26);
	//오른쪽벽
	drawPicture(-19.6f, 75.0f, 1, 27);
	drawPicture(0.0f, 99.5f, 0, 28);
	glEnd();


	//미술관 복도1
	glBindTexture(GL_TEXTURE_2D, texture[10]);
	glBegin(GL_QUADS);
	glColor3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0, 0); glVertex3f(-40.0f, -50.0f, 15.0f);
	glTexCoord2f(0, 1); glVertex3f(-40.0f, -50.0f, 25.0f);
	glTexCoord2f(1, 1); glVertex3f(-20.0f, -50.0f, 25.0f);
	glTexCoord2f(1, 0); glVertex3f(-20.0f, -50.0f, 15.0f);
	glEnd();
	drawBokdoWall(-40.0f, 15.0f, 0);
	drawBokdoWall(-40.0f, 25.0f, 0);

	//미술관 복도2
	glBindTexture(GL_TEXTURE_2D, texture[10]);
	glBegin(GL_QUADS);
	glColor3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0, 0); glVertex3f(40.0f, -50.0f, 15.0f);
	glTexCoord2f(0, 1); glVertex3f(40.0f, -50.0f, 25.0f);
	glTexCoord2f(1, 1); glVertex3f(20.0f, -50.0f, 25.0f);
	glTexCoord2f(1, 0); glVertex3f(20.0f, -50.0f, 15.0f);
	glEnd();
	drawBokdoWall(20.0f, 15.0f, 0);
	drawBokdoWall(20.0f, 25.0f, 0);

	//미술관 복도3
	glBindTexture(GL_TEXTURE_2D, texture[10]);
	glBegin(GL_QUADS);
	glColor3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0, 0); glVertex3f(-5.0f, -50.0f, 40.0f);
	glTexCoord2f(0, 1); glVertex3f(5.0f, -50.0f, 40.0f);
	glTexCoord2f(1, 1); glVertex3f(5.0f, -50.0f, 60.0f);
	glTexCoord2f(1, 0); glVertex3f(-5.0f, -50.0f, 60.0f);
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
	//2번방
	glPushMatrix();
	{
		glColor3f(0.8f, 0.8f, 0.8f);
		glTranslatef(-10.0f, -50.0f, 95.0f);
		glRotatef(-90, 1, 0, 0);
		glBindTexture(GL_TEXTURE_2D, texture[10]);
		texturedCylinder(0.5f, 5.0f, 32);
	}
	glPopMatrix();
	glPushMatrix();
	{
		glColor3f(0.8f, 0.8f, 0.8f);
		glTranslatef(-10.0f, -50.0f, 70.0f);
		glRotatef(-90, 1, 0, 0);
		glBindTexture(GL_TEXTURE_2D, texture[10]);
		texturedCylinder(0.5f, 5.0f, 32);
	}
	glPopMatrix();
	glPushMatrix();
	{
		glColor3f(0.8f, 0.8f, 0.8f);
		glTranslatef(10.0f, -50.0f, 95.0f);
		glRotatef(-90, 1, 0, 0);
		glBindTexture(GL_TEXTURE_2D, texture[10]);
		texturedCylinder(0.5f, 5.0f, 32);
	}
	glPopMatrix();
	glPushMatrix();
	{
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(10.0f, -50.0f, 70.0f);
		glRotatef(-90, 1, 0, 0);
		glBindTexture(GL_TEXTURE_2D, texture[10]);
		texturedCylinder(0.5f, 5.0f, 32);
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
		glColor3f(1.0f,1.0f, 1.0f);
		glTranslatef(60.0f - 10.0f, -50.0f + 2.0f, 20.0f + 10.0f);
		glRotatef(180, 0, 1, 0);
		glBindTexture(GL_TEXTURE_2D, texture[13]);
		texturedSphere(2.0f, 42);
	}
	glPopMatrix();

}

void drawMap()
{
	//천장

	glBindTexture(GL_TEXTURE_2D, texture[12]);
	glBegin(GL_QUADS);
	glColor3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0, 0); glVertex3f(-120.0f, -45.0f, -20.0f);
	glTexCoord2f(0, 1); glVertex3f(120.0f, -45.0f, -20.0f);
	glTexCoord2f(1, 1); glVertex3f(120.0f, -45.0f, 120.0f);
	glTexCoord2f(1, 0); glVertex3f(-120.0f, -45.0f, 120.0f);
	glEnd();

	drawStartPoint();
	drawGallary();
	drawStructure();
}


void SetTextMessage(GLuint index[64])
{
	switch (index[3]) {

	case 100:
		trap = true;
		gentleman = true;
		printf("gentleman\n");
		break;
	case 101:
		trap = true;
		monalisa = true;
		printf("monalisa\n");
		break;
	case 102:
		trap = true;
		girl = true;
		printf("girl\n");
		break;


	default: break;
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

	//gluPerspective(45.0f, (GLfloat)g_nWindowWidth / (GLfloat)g_nWindowHeight, 0.1f, 100.0f);
	//gluPerspective(45, ratio, 1, 1000);
	gluLookAt(px, py, pz, px + lx, py + ly, pz + lz, 0.0f, 1.0f, 0.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

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

void MyMouse(int button, int state, int x, int y) {
	switch (button) {
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN) {
			ptLastMousePosit.x = x; ptCurrentMousePosit.x = x;
			ptLastMousePosit.y = y; ptCurrentMousePosit.y = y;
			SelectObjects(ptCurrentMousePosit.x, ptCurrentMousePosit.y);
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
void renderScene(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	drawMap();
	if (trap == true)
		drawtrap();
	if (realclear == true)
		rain();
	if (girl == true)
	{
		drawPicture(-19.55f, 85.0f, 1, 3);
		//정면
		drawPicture(10.0f, 99.45f, 0, 3);
		drawPicture(-10.0f, 99.45f, 0, 3);
		//왼쪽벽
		drawPicture(19.55f, 85.0f, 1, 3);
		drawPicture(19.55f, 75.0f, 1, 3);
		//오른쪽벽
		drawPicture(-19.55f, 75.0f, 1, 3);
		drawPicture(0.0f, 99.45f, 0, 3);
	}
	if (monalisa == true)
	{
		drawPicture(65.0f, 1.05f, 0, 15);
		//정면
		drawPicture(79.3f, 30.0f, 1, 15);
		drawPicture(79.3f, 10.0f, 1, 15);
		//왼쪽
		drawPicture(55.f, 1.05f, 0, 15);
		drawPicture(79.3f, 20.0f, 1, 15);
		//오른쪽
		drawPicture(55.f, 39.7f, 0, 15);
		drawPicture(65.f, 39.7f, 0, 15);
	}
	if (gentleman == true)
	{
		//정면
		drawPicture(-78.45f, 20.0f, 1, 16);
		drawPicture(-78.45f, 10.0f, 1, 16);
		//왼쪽
		drawPicture(-55.f, 39.75f, 0, 16);
		drawPicture(-65.f, 39.75f, 0, 16);
		//오른쪽
		drawPicture(-55.f, 0.25f, 0, 16);
		drawPicture(-65.f, 0.25f, 0, 16);
		drawPicture(-78.45f, 30.0f, 1, 16);
	}
	glPushMatrix();
	{
		glTranslatef(0.0f, 1.0f, 10.0f);
		//addMob(vec3(x, y, z + 10),0.2);
	}
	glPopMatrix();



	sprintf(coor, "x : %f z : %f", px, pz);
	sprintf(warn, "You RUINED the masterpiece!!!");
	sprintf(name, "FIND <the GIRL> <Woman with sad eyes> <Gentleman with apple face>");
	sprintf(message, "You DIED");
	sprintf(diemessage, "Press 'r' to restart");
	sprintf(clearmessage, "CLEAR!!");
	sprintf(gohome, "Go back to the Entrance");
	beginRenderText(g_nWindowWidth, g_nWindowHeight);
	{

		glColor3f(1.0f, 1.0f, 1.0f);

		if (die == false && text == true)
		{
			renderText(g_nWindowWidth - 0.95, g_nWindowHeight + 0.9, BITMAP_FONT_TYPE_HELVETICA_12, coor);

		}
		if (monalisa == true || gentleman == true || girl == true)
		{
			renderText(g_nWindowWidth - 0.95, g_nWindowHeight + 0.8, BITMAP_FONT_TYPE_HELVETICA_12, warn);
			renderText(g_nWindowWidth - 0.95, g_nWindowHeight + 0.7, BITMAP_FONT_TYPE_HELVETICA_12, name);
		}
		if (clear == true)
		{
			renderText(g_nWindowWidth - 0.95, g_nWindowHeight + 0.6, BITMAP_FONT_TYPE_HELVETICA_12, gohome);
		}
		if (die == true)
		{
			renderText(g_nWindowWidth - 0.08, g_nWindowHeight + 0.05, BITMAP_FONT_TYPE_HELVETICA_18, message);
			renderText(g_nWindowWidth - 0.15, g_nWindowHeight - 0.05, BITMAP_FONT_TYPE_HELVETICA_18, diemessage);
		}
		if (realclear == true)
		{
			glColor3f(1.0f, 0.0f, 0.0f);
			renderText(g_nWindowWidth - 0.08, g_nWindowHeight + 0.05, BITMAP_FONT_TYPE_HELVETICA_18, clearmessage);
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
	sprintf(coor, "x : %f z : %f", px, pz);
}


void moveMeFlat(int i) {
	px = px + i * (lx) * 0.2;
	pz = pz + i * (lz) * 0.2;
	glLoadIdentity();
	gluLookAt(px, py, pz, px + lx, py + ly, pz + lz, 0.0f, 1.0f, 0.0f);
	sprintf(coor, "x : %f z : %f", px, pz);
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
	if (trap == true)
	{
		for (int i = 0; i < mobs.size(); i++)
		{
			vec3 p = vec3(px, py, pz);
			vec3 dis = mobs[i].p - p;
			float L = length(dis);
			//printf("%f %f %f\n", px, mobs[0].p.x, L);
			if (i == 6 && L <= 2.5f)
				die = true;
			if (L <= 2.0f)
			{
				die = true;
				printf("닿았따\n");
			}
		}
	}


}
void inputKey(unsigned char key, int x, int y) {

	switch (key) {

	case 'a': case 'A':
		if (start == true)
		{
			ang -= 0.03f;
			orientMe(ang);
		}
		break;
	case 'd':case 'D':
		if (start == true)
		{
			ang += 0.03f;
			orientMe(ang);
		}
		break;
	case 'f': case 'F':
		if (start == true)
		{
			ang += 110.0018f;
			orientMe(ang);
		}
		break;
	case 'w': case'W':
		if (start == true)
			moveMeFlat(4);
		break;
	case 's': case'S':
		if (start == true)
			moveMeFlat(-4);
		break;
	case 'p': case'P':
		text = !text;
		break;
	case 'r': case'R':
		if (start == true)
			rst();
		break;
	case 'k': case'K':
		start = true;
		break;
	/// 아래는 테스트용
	case 'q':
		trap = true;
		break;
	case 'o':
		die = true;
		break;
	case 'z':
		girl = true;
		break;
	case 'x':
		monalisa = true;
		break;
	case 'c':
		gentleman = true;
		break;
	}
}

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
	py += 50.0f;
	for (int i = 0; i < 7; i++)
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

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(200, 200);
	glutInitWindowSize(900, 700);
	glutCreateWindow("Project");
	glInit();
	glutKeyboardFunc(inputKey);

	glutDisplayFunc(renderScene);

	glutIdleFunc(MyIdle);

	glutTimerFunc(40, MyTimer, 1);
	glutReshapeFunc(changeSize);
	glutMouseFunc(MyMouse);
	glutMotionFunc(MyMotion);
	glutMainLoop();

	return(0);
}