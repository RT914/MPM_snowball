#include <GL/freeglut.h>
#include <stdio.h>
#include <iostream>
#include "Draw.h"
#include "Object.h"
#include "Window.h"

//----------------------------------------------------
// 点の描画
//----------------------------------------------------
void DrawPoint(float x, float y, float size) {
	glPointSize(size);

	glBegin(GL_POINTS);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glVertex2d(x, y);
	glEnd();
}


//----------------------------------------------------
// 物体の描画
//----------------------------------------------------
void DrawObject(Object object)
{
	float point_size = 3.0;

	for (int i = 0; i < object.points.size(); i++)
	{
		float pos_x = object.points[i].position.x() * WindowWidth;
		float pos_y = object.points[i].position.y() * WindowHeight;
		DrawPoint(pos_x, pos_y, point_size);
	}
};
//----------------------------------------------------
// 全体の描画
//----------------------------------------------------
void Draw()
{
	
}