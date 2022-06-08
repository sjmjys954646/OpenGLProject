#define _CRT_SECURE_NO_WARNINGS
#include <gl/glut.h>
#include <math.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include "glaux.h"
#include <stdlib.h>
#include "bitmap_fonts.h"

#include <vector>
#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp>
using namespace std;
using namespace glm;
static float ang = 0.0, ratio;
static float x = 0.0f, y = 1.75f, z = 5.0f;
static float lx = 0.0f, ly = 0.0f, lz = -1.0f;

static char coor[255];
static char message[255];
GLfloat dx = 0.2;
GLfloat x1;
int    g_nWindowWidth;
int    g_nWindowHeight;

unsigned int MyTextureObject[1];
AUX_RGBImageRec* pTextureImage[1];
GLuint g_textureID = -1;

void DrawCube(float size);
void addMob(vec3 position, float size);
void AddContactForce();
void DrawCubeTex();

bool die = false;

typedef struct {
	float Left, Right;
	float Top, Bottom;
}Rect;
typedef struct {
	vec3 p = vec3(x, y, z);
}player;
player Player;

struct Mob{
	vec3 p; //position
	vec3 v; //velocity
	vec3 force; //force
	float size; //size
	float m; //mass
};

vector<Mob> mobs;

AUX_RGBImageRec* LoadBMP(const char* szFilename) {
	FILE* pFile = NULL;
	if (!szFilename) {
		return NULL;
	}

	pFile = fopen(szFilename, "r");
	if (pFile) {
		fclose(pFile);
		return auxDIBImageLoad(szFilename);     //파일로부터 메모리로
	}
	return NULL;
}
int LoadGLTextures(const char* szFilePath) {       //파일을 로드하고 텍스쳐로 변환
	int Status = FALSE;
	glClearColor(0.0, 0.0, 0.0, 0.5);
	memset(pTextureImage, 0, sizeof(void*) * 1);    //포인터를 널로

	if (pTextureImage[0] = LoadBMP(szFilePath)) {   //비트맵을 로드하고 오류확인
		Status = TRUE;                              //상태 플랙을 True로
		glGenTextures(1, &MyTextureObject[0]);      //텍스쳐 생성
		glBindTexture(GL_TEXTURE_2D, MyTextureObject[0]);
		glTexImage2D(GL_TEXTURE_2D, 0, 3,
			pTextureImage[0]->sizeX, pTextureImage[0]->sizeY,
			0, GL_RGB, GL_UNSIGNED_BYTE, pTextureImage[0]->data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glEnable(GL_TEXTURE_2D);
	}
	if (pTextureImage[0]) {                 //텍스쳐가 존재하면
		if (pTextureImage[0]->data) {       //텍스쳐 영상이 존재하면
			free(pTextureImage[0]->data);   //텍스쳐 영상공간 반납
		}
		free(pTextureImage[0]);             //텍스쳐 반납
	}
	return Status;
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
	glBegin(GL_QUADS);
	glVertex3f(-size, -size, size);		// 앞면
	glVertex3f(size, -size, size);
	glVertex3f(size, size, size);
	glVertex3f(-size, size, size);

	glVertex3f(size, -size, -size);
	glVertex3f(-size, -size, -size);		// 뒷면
	glVertex3f(-size, size, -size);
	glVertex3f(size, size, -size);

	glVertex3f(-size, size, size);		// 윗면
	glVertex3f(size, size, size);
	glVertex3f(size, size, -size);
	glVertex3f(-size, size, -size);

	glVertex3f(-size, -size, -size);		// 아랫면
	glVertex3f(size, -size, -size);
	glVertex3f(size, -size, size);
	glVertex3f(-size, -size, size);

	glVertex3f(size, -size, size);		// 우측면
	glVertex3f(size, -size, -size);
	glVertex3f(size, size, -size);
	glVertex3f(size, size, size);

	glVertex3f(-size, -size, -size);	// 좌측면
	glVertex3f(-size, -size, size);
	glVertex3f(-size, size, size);
	glVertex3f(-size, size, -size);
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

	//DrawCube(1.0f);
	//addMob(vec3(x,y,z-1),0.2);
	DrawCubeTex();
	
	glEnd();
	
	sprintf(coor,"x : %f y : %f z : %f",x,y,z);
	sprintf(message, "You DIED");
	beginRenderText(g_nWindowWidth, g_nWindowHeight);
	{
		glColor3f(1.f, 1.f, 1.0f);
		int u = (1.f - 0.75f) / 4.f * g_nWindowWidth;
		int v = (1.f - (-1.75f)) / 4.f * g_nWindowHeight;
		if (die==false)
			renderText(g_nWindowWidth - 0.95, g_nWindowHeight + 0.9, BITMAP_FONT_TYPE_HELVETICA_18, coor);
		else
			renderText(g_nWindowWidth-0.08 , g_nWindowHeight, BITMAP_FONT_TYPE_HELVETICA_18,message);
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
	x = x + i * (lx) * 0.1;
	z = z + i * (lz) * 0.1;
	glLoadIdentity();
	gluLookAt(x, y, z, x + lx, y + ly, z + lz, 0.0f, 1.0f, 0.0f);
	sprintf(coor, "x : %f y : %f z : %f", x, y, z);
}

void processNormalKeys(unsigned char key, int x, int y) {

	if (key == 27)
		exit(0);
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
	case 'w':
		moveMeFlat(1);
		break;
	case 's':
		moveMeFlat(-1);
		break;
	}
}
void addMob(vec3 position, float size)
{
	Mob newmob;

	vec3 _pos(0, 0, 0);
	_pos.x = position.x;
	_pos.y = position.y;
	_pos.z = position.z;
	float _size = size;
	newmob.p.x = _pos.x;
	newmob.p.y = _pos.y;
	newmob.p.z = _pos.z;
	newmob.size = _size;
	
	
	DrawCube(size);
	

	mobs.push_back(newmob);
}

void AddContactForce()
{
	for (int i = 0; i < mobs.size(); i++)
	{
		for (int j = i + 1; j < mobs.size(); j++)
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
}
void DrawCubeTex() {
	glBindTexture(GL_TEXTURE_2D, MyTextureObject[0]);
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


	glBindTexture(GL_TEXTURE_2D, g_textureID);

}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(900, 700);
	glutCreateWindow("project");
	

	glutKeyboardFunc(processNormalKeys);
	glutKeyboardFunc(inputKey);

	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glutTimerFunc(40, MyTimer, 1);
	glutReshapeFunc(changeSize);

	glutMainLoop();
	if (LoadGLTextures("Data/monalisa.bmp")) {
		glEnable(GL_TEXTURE_2D);
		glShadeModel(GL_SMOOTH);
		glClearDepth(1.0);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glutMainLoop();
	}
	return(0);
}