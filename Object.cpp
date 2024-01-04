#include <Eigen/Dense>
#include <stdio.h>
#include <iostream>
#include <cmath>
#include "Object.h"
#include "Window.h"
#include "mpm.h"

float generation_random(float min, float max);

const double PI = 3.14159265358979323846;

Object CreateObject(int particle_num)
{
	// 円の中心座標
	Eigen::Vector2f pos;
	pos << 0.5, 0.5;
	// 円の半径
	float radius = 0.1;

	Object object(pos, radius);
	srand(4);  // 疑似乱数のシード値設定

	for (int i = 0; i < particle_num; i++) {
		float angle1 = generation_random(0.0, 1.0) * PI * 2;  // 0から2πの範囲の角度
		float angle2 = generation_random(0.0, 1.0) * PI;  // 0からπの範囲の角度
		float angle = angle1 + angle2;  // 2つの角度を合わせる

		// 位置座標
		Eigen::Vector2f position;
		
		position <<
			pos.x() + std::cos(angle) * generation_random(0.0, 1.0) * radius / 1.0,
			pos.y() + std::sin(angle) * generation_random(0.0, 1.0) * radius / 1.0;
		// 1.0は画面比率

		// position << pos.x(), pos.y();

		// 速度
		Eigen::Vector2f velocity;
		velocity << 0.0, 0.0;

		// affine velocity field
		Eigen::Matrix2f C;
		C << 0.0, 0.0, 0.0, 0.0;

		// 変形勾配
		Eigen::Matrix2f F;
		F << 2.0, 0.0, 0.0, 1.0;

		// 塑性変形勾配
		float Jp = 1.0;

		Point p = Point(position, velocity, C, F, Jp);
		object.points.emplace_back(p);
	}

	return object;
};

//----------------------------------------------------
// 範囲内の乱数の生成
//----------------------------------------------------
float generation_random(float min, float max)
{
	return((max - min) * ((float)rand() / RAND_MAX)) + min;
};