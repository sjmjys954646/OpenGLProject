#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <stdio.h>
#include <GL/glut.h>
#include <GL/glu.h>

//for drawing fonts
#include "bitmap_fonts.h"

#include <vector>
#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp>

int Loadfile()
{
	FILE* fp;
	int input = 0;
	input = rand() % 4;
	switch (input)
	{
	case 0:
		fp = fopen("C:\Users\이윤수\source\repos\OpenGLProject\OpenGLProject\Data\Map\bring_1.txt", "rt");
		break;
	case 1:
		fp = fopen("C:\Users\이윤수\source\repos\OpenGLProject\OpenGLProject\Data\Map\bring_2.txt", "rt");
		break;
	case 2:
		fp = fopen("C:\Users\이윤수\source\repos\OpenGLProject\OpenGLProject\Data\Map\bring_3.txt", "rt");
		break;
	case 3:
		fp = fopen("C:\Users\이윤수\source\repos\OpenGLProject\OpenGLProject\Data\Map\bring_4.txt", "rt");
		break;
	}

	if (fp == NULL)
	{
		printf("\n실패\n");
		return 1;
	}
	printf("\n완료\n");

	int cha;

	while (feof(fp) == 0)
	{
		for (int i = 0; i < 30; i++)
		{
			for (int j = 0; j < 30; j++)
			{
				fscanf(fp, "%d", &cha);
				makeboard[i][j] = cha;
			}
		}
	}
	fclose(fp);
	return 1;
}

void board_maker()
{
	for (int i = 0; i < 30; i++)
	{
		for (int j = 0; j < 30; j++)
		{
			if (makeboard[i][j] == 1)
			{
				glPushMatrix();
				{
					glTranslatef(i * 1.1 - 15, 0, j * 1.1 - 15);
					draw_block();
				}
				glPopMatrix(); // 블럭 표시
				glColor3f(1, 0, 0);
				glLineWidth(1);
				glBegin(GL_LINE_LOOP);
				{
					glVertex3f(static_block[i][j].max_x, 1, static_block[i][j].max_z);
					glVertex3f(static_block[i][j].min_x, 1, static_block[i][j].max_z);
					glVertex3f(static_block[i][j].min_x, 1, static_block[i][j].min_z);
					glVertex3f(static_block[i][j].max_x, 1, static_block[i][j].min_z);
				}
				glEnd();
				glLineWidth(1); // 블럭 위치
			}
			else if (makeboard[i][j] == 2)
			{
				glPushMatrix();
				{
					glTranslatef(i * 1.1 - 15, 0, j * 1.1 - 15);
					item();
				}
				glPopMatrix(); // 아이템 표시
				glColor3f(0, 1, 0);
				glLineWidth(1);
				glBegin(GL_LINE_LOOP);
				{
					glVertex3f(static_block[i][j].max_x, 1, static_block[i][j].max_z);
					glVertex3f(static_block[i][j].min_x, 1, static_block[i][j].max_z);
					glVertex3f(static_block[i][j].min_x, 1, static_block[i][j].min_z);
					glVertex3f(static_block[i][j].max_x, 1, static_block[i][j].min_z);
				}
				glEnd();
				glLineWidth(1); // 아이템 위치
			}
			else if (makeboard[i][j] == 3)
			{
				glTranslatef(i * 1.1  15, 1.5, j * 1.1 - 15);
				glColor3f(0.0, 0.0, 0.0);

				glBindTexture(GL_TEXTURE_2D, textures[1]);
				glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
				glBegin(GL_QUADS);
				{
					glTexCoord2f(1, 1);
					glVertex3f(-1.0f, 2.0f, 1.0f);
					glTexCoord2f(0, 1);
					glVertex3f(-1.0f, -2.0f, 1.0f);
					glTexCoord2f(0, 0);
					glVertex3f(1.0f, -2.0f, 1.0f);
					glTexCoord2f(1, 0);
					glVertex3f(1.0f, 2.0f, 1.0f);
				}
				glEnd();
				//오른쪽
				glBindTexture(GL_TEXTURE_2D, textures[1]);
				glBegin(GL_QUADS);
				{
					glTexCoord2f(1, 1);
					glVertex3f(1.0f, 2.0f, 1.0f);
					glTexCoord2f(0, 1);
					glVertex3f(1.0f, -2.0f, 1.0f);
					glTexCoord2f(0, 0);
					glVertex3f(1.0f, -2.0f, -1.0f);
					glTexCoord2f(1, 0);
					glVertex3f(1.0f, 2.0f, -1.0f);
				}
				glEnd();
				//뒷쪽
				glBindTexture(GL_TEXTURE_2D, textures[1]);
				glBegin(GL_QUADS);
				{
					glTexCoord2f(1, 1);
					glVertex3f(1.0f, 2.0f, -1.0f);
					glTexCoord2f(0, 1);
					glVertex3f(1.0f, -2.0f, -1.0f);
					glTexCoord2f(0, 0);
					glVertex3f(-1.0f, -2.0f, -1.0f);
					glTexCoord2f(1, 0);
					glVertex3f(-1.0f, 2.0f, -1.0f);
				}
				glEnd();
				////왼쪽
				glBindTexture(GL_TEXTURE_2D, textures[1]);
				glBegin(GL_QUADS);
				{
					glTexCoord2f(1, 1);
					glVertex3f(-1.0f, 2.0f, -1.0f);
					glTexCoord2f(0, 1);
					glVertex3f(-1.0f, -2.0f, -1.0f);
					glTexCoord2f(0, 0);
					glVertex3f(-1.0f, -2.0f, 1.0f);
					glTexCoord2f(1, 0);
					glVertex3f(-1.0f, 2.0f, 1.0f);
				}
				glEnd();
				////아랫쪽
				glBindTexture(GL_TEXTURE_2D, textures[1]);
				glBegin(GL_QUADS);
				{
					glTexCoord2f(1, 1);
					glVertex3f(-1.0f, -2.0f, 1.0f);
					glTexCoord2f(0, 1);
					glVertex3f(-1.0f, -2.0f, -1.0f);
					glTexCoord2f(0, 0);
					glVertex3f(1.0f, -2.0f, -1.0f);
					glTexCoord2f(1, 0);
					glVertex3f(1.0f, -2.0f, 1.0f);
				}
				glEnd();
				////윗쪽
				glBindTexture(GL_TEXTURE_2D, textures[1]);
				glBegin(GL_QUADS);
				{
					glTexCoord2f(1, 1);
					glVertex3f(-1.0f, 2.0f, 1.0f);
					glTexCoord2f(0, 1);
					glVertex3f(-1.0f, 2.0f, -1.0f);
					glTexCoord2f(0, 0);
					glVertex3f(1.0f, 2.0f, -1.0f);
					glTexCoord2f(1, 0);
					glVertex3f(1.0f, 2.0f, 1.0f);
				}
				glEnd();
			}
		}
	}
}

void draw_block()
{
	if (map == 0)
	{
		glColor3f(0.3, 0.5, 0.5);
	}
	if (map == 1)
	{
		glColor3f(0.7, 0.1, 0.7);
	}
	if (map == 2)
	{
		glColor3f(0.0, 0.8, 0.3);
	}
	if (map == 3)
	{
		glColor3f(0.7, 0.2, 0.3);
	}
	glPushMatrix();
	{
		glNormal3f(0, 1, 0);
		glTranslatef(0, 1.1, 0);
		glutSolidCube(1);

		glTranslatef(0, 1.1, 0);
		glutSolidCube(1);

		glTranslatef(0, 1.1, 0);
		glutSolidCube(1);

		glTranslatef(0, 1.1, 0);
		glutSolidCube(1);

		glTranslatef(0, 1.1, 0);
		glutSolidCube(1);
	}
	glPopMatrix();
}

void item()
{
	glPushMatrix();
	{
		glEnable(GL_BLEND);
		glPushMatrix();
		{
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
			//glColor3f(0.7, 0.2, 0.2);
			glTranslatef(0, 2, 0);
			glColor4f(0, 0.8, 0, 0.8);
			glRotatef(obj_rot, 0, 1, 0);
			glutSolidCube(0.3);
		}
		glPopMatrix();
		glPushMatrix();
		{
			glColor4f(0.7, 0, 0, 0.5);
			glTranslatef(0, 2, 0);
			glRotatef(obj_rot, 0, 1, 0);
			glRotatef(90, -1, 0, -1);
			glutSolidCube(0.6);
		}
		glPopMatrix();

		glDisable(GL_BLEND);
	}
	glPopMatrix();
}
int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(640, 480);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Selection Mode - Picking");
	InitGL();
	glutDisplayFunc(DrawGLScene);
	glutReshapeFunc(MyReshape);
	glutMouseFunc(MyMouse);
	glutMotionFunc(MyMotion);

	glutMainLoop();
	return 0;
}
