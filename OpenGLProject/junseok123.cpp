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
#define NUM_PARTICLES    10000          /* Number of particles  */
#define NUM_DEBRIS       1000           /* Number of debris     */

using namespace std;
using namespace glm;
static float ang = 0.0, ratio;
static float x = 0.0f, y = 1.75f, z = 0.0f;
static float lx = 0.0f, ly = 0.0f, lz = 1.0f;

GLfloat dx = 0.2;
GLfloat x1;
static char coor[255];
static char message[255];
int    g_nWindowWidth;
int    g_nWindowHeight;
void DrawCube();
void addMob(vec3 position, float size);
void Add_dist();
void DrawCubeTex();
void mobinit();

GLboolean die = false;
GLboolean clear = false;


struct Mob {
	vec3 p; //position
	vec3 v; //velocity
	vec3 force; //force
	float size; //size
	float m; //mass
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

void newSpeed(float dest[3]) {
	float x, y, z, len;

	x = (2.0 * ((GLfloat)rand()) / ((GLfloat)RAND_MAX)) - 1.0;
	y = (2.0 * ((GLfloat)rand()) / ((GLfloat)RAND_MAX)) - 1.0;
	z = (2.0 * ((GLfloat)rand()) / ((GLfloat)RAND_MAX)) - 1.0;

	if (wantNormalize) {
		len = sqrt(x * x + y * y + z * z);

		if (len) {
			x = x / len;
			y = y / len;
			z = z / len;
		}
	}

	dest[0] = x;
	dest[1] = y;
	dest[2] = z;
}
GLboolean text = false;
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

void loadTexture(void) {
	AUX_RGBImageRec* pTextureImage = auxDIBImageLoad("Data/monalisa.bmp");

	if (pTextureImage != NULL) {
		glGenTextures(1, &g_textureID);

		glBindTexture(GL_TEXTURE_2D, g_textureID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, 3, pTextureImage->sizeX, pTextureImage->sizeY, 0,
			GL_RGB, GL_UNSIGNED_BYTE, pTextureImage->data);
	}

	if (pTextureImage) {
		if (pTextureImage->data)
			free(pTextureImage->data);

		free(pTextureImage);
	}
}


void drawCircle() {
	glColor3f(1.0f, 0.7f, 1.0f);
	glTranslatef(0.0f, 5.0f, 0.0f);
	glutSolidSphere(0.75f, 20, 20);
}
GLboolean test = false;
void MyTimer(int value) {
	
	glutPostRedisplay();
	Add_dist();
	if (die == false)
		glutTimerFunc(40, MyTimer, 1);
	if(die==true)
	{
		x = 300;
		z = 300;
		newExplosion();
	}
	

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
		glBindTexture(GL_TEXTURE_2D, g_textureID);
		glBegin(GL_QUADS);
		glTexCoord2f(0, 0); glVertex3f(leftX, -49.0f, midZ);
		glTexCoord2f(0, 1); glVertex3f(leftX, -45.8f, midZ);
		glTexCoord2f(1, 1); glVertex3f(leftX + 2.4f, -45.8f, midZ);
		glTexCoord2f(1, 0); glVertex3f(leftX + 2.4f, -49.0f, midZ);
		glEnd();

	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, g_textureID);
		glBegin(GL_QUADS);
		glTexCoord2f(0, 0);  glVertex3f(leftX, -49.0f, midZ);
		glTexCoord2f(0, 1); glVertex3f(leftX, -45.8f, midZ);
		glTexCoord2f(1, 1); glVertex3f(leftX, -45.8f, midZ + 2.4f);
		glTexCoord2f(1, 0); glVertex3f(leftX, -49.0f, midZ + 2.4f);
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
	drawFullWall(-20.0f - 60.0f, 0.0f, 0);
	drawFullWall(-20.0f - 60.0f, 40.0f, 0);
	drawPicture(-79.5f, 20.0f, 1, 0);
	//미술관 땅2
	drawFloor(60.0f, 20.0f, 20.0f);
	drawWall(-20.0f + 60.0f, 0.0f, 1);
	drawWall(-20.0f + 60.0f, 25.0f, 1);
	drawFullWall(20.0f + 60.0f, 0.0f, 1);
	drawFullWall(-20.0f + 60.0f, 0.0f, 0);
	drawFullWall(-20.0f + 60.0f, 40.0f, 0);
	drawPicture(79.5f, 20.0f, 1, 0);
	//미술관 땅3
	drawFloor(0, 80.0f, 20.0f);
	drawWall(-20.0f, 60.0f, 0);
	drawWall(5.0f, 60.0f, 0);
	drawFullWall(20.0f, 60.0f, 1);
	drawFullWall(-20.0f, 100.0f, 0);
	drawFullWall(-20.0f, 60.0f, 1);
	drawPicture(0.0f, 99.5f, 0, 0);

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
void DrawCube(vec3 p)
{
	glutSolidCube(1);
	addMob(p, 0.2);
}
void renderScene(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	drawMap();
	//DrawCubeTex();
	//DrawCube(vec3(x, y, z + 2), 0.2);

	glPushMatrix();
	{
		glTranslatef(0.0f, 1.0f, 10.0f);
		DrawCube(vec3(x, y, z + 10));
		
	}
	glPopMatrix();
	
	glEnd();

	sprintf(coor, "x : %f y : %f z : %f", x, y, z);
	sprintf(message, "You DIED");
	beginRenderText(g_nWindowWidth, g_nWindowHeight);
	{
		glColor3f(1.f, 1.f, 1.0f);
		int u = (1.f - 0.75f) / 4.f * g_nWindowWidth;
		int v = (1.f - (-1.75f)) / 4.f * g_nWindowHeight;
		if (die == false&&text==true)
			renderText(g_nWindowWidth - 0.95, g_nWindowHeight + 0.9, BITMAP_FONT_TYPE_HELVETICA_12, coor);
		if(die==true)
			renderText(g_nWindowWidth - 0.08, g_nWindowHeight, BITMAP_FONT_TYPE_HELVETICA_18, message);
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

	/*mat4 trans = translate(mat4(1), vec3(position.x, position.y, position.z));
	vec4 tempP = vec4(newmob.p, 0);
	newmob.p = tempP * trans;*/

	
	//glutSolidCube(size);
	
	mobs.push_back(newmob);
}
vector<float> dist;
void Add_dist()
{
	
	for (int i = 0; i < mobs.size(); i++)
	{
		vec3 p = vec3(x, y, z);
		vec3 dis = mobs[0].p - p;       //dis=반지름 사이
		//compute length
		float L = length(dis);                    //L= 반지름 사이의 거리
		//dist.push_back(L);
		//dis = normalize(dis);
		if (L <= mobs[0].size - 0.01)          //L이 반지름 사이의 거리보다 작다? => 두 공이 겹쳤다
		{
			die = true;
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
	case 'k':
		newExplosion();
		break;
	case 'p':
		text = !text;
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
	loadTexture();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
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
void mobinit()
{
	glPushMatrix();
	{
		glTranslatef(0.0f, 1.0f, 10.0f);
		glutSolidCube(1);
		addMob(vec3(x, y, z + 10), 0.2);

	}
	glPopMatrix();
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
	glutIdleFunc(MyIdle);
	glutTimerFunc(40, MyTimer, 1);
	glutReshapeFunc(changeSize);

	glutMainLoop();

	return(0);
}