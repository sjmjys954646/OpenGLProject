#define _CRT_SECURE_NO_WARNINGS
#include "map.h"
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "glaux.h"
#include "cal.h"
#include <stdio.h>

int Loadfile(int (&board)[50][50])
{
	FILE* fp;
	int input = 0;
	input = rand() % 4;
	
	fp = fopen("C:\\Users\\이윤수\\source\\repos\\OpenGLProject\\OpenGLProject\\Data\\board.txt","rt");

	if (fp == NULL)
	{
		printf("\n실패\n");
		return 0;
	}
	printf("\n완료\n");

	int cha;
	bool fin = false;
	memset(board, 0, sizeof(board));

	for (int i = 0; i < 50; i++)
	{
		for (int j = 0; j < 50; j++)
		{
			fscanf(fp, "%d", &cha);
			board[i][j] = cha;
			if (feof(fp))
			{
				i = 50;
				break;
			}
		}
	}

	fclose(fp);
	return 1;
}

void board_maker(int board[50][50])
{
	for (int i = 0; i < 50; i++)
	{
		for (int j = 0; j < 50; j++)
		{
			if (board[i][j] == 1)
			{
				glPushMatrix();
				{
					glTranslatef(i * 1.1 - 15, 0, j * 1.1 - 15);
					draw_block();
				}
				glPopMatrix(); // 블럭 표시
				glColor3f(1, 0, 0);
				glLineWidth(1);
				glEnd();
				glLineWidth(1); // 블럭 위치
			}
		}
	}
}

void draw_block()
{

	glColor3f(0.3, 0.5, 0.5);
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
