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
	// �~�̒��S���W
	Eigen::Vector2f pos;
	pos << 0.5, 0.5;
	// �~�̔��a
	float radius = 0.1;

	Object object(pos, radius);
	srand(4);  // �^�������̃V�[�h�l�ݒ�

	for (int i = 0; i < particle_num; i++) {
		float angle1 = generation_random(0.0, 1.0) * PI * 2;  // 0����2�΂͈̔͂̊p�x
		float angle2 = generation_random(0.0, 1.0) * PI;  // 0����΂͈̔͂̊p�x
		float angle = angle1 + angle2;  // 2�̊p�x�����킹��

		// �ʒu���W
		Eigen::Vector2f position;
		
		position <<
			pos.x() + std::cos(angle) * generation_random(0.0, 1.0) * radius / 1.0,
			pos.y() + std::sin(angle) * generation_random(0.0, 1.0) * radius / 1.0;
		// 1.0�͉�ʔ䗦

		// position << pos.x(), pos.y();

		// ���x
		Eigen::Vector2f velocity;
		velocity << 0.0, 0.0;

		// affine velocity field
		Eigen::Matrix2f C;
		C << 0.0, 0.0, 0.0, 0.0;

		// �ό`���z
		Eigen::Matrix2f F;
		F << 2.0, 0.0, 0.0, 1.0;

		// �Y���ό`���z
		float Jp = 1.0;

		Point p = Point(position, velocity, C, F, Jp);
		object.points.emplace_back(p);
	}

	return object;
};

//----------------------------------------------------
// �͈͓��̗����̐���
//----------------------------------------------------
float generation_random(float min, float max)
{
	return((max - min) * ((float)rand() / RAND_MAX)) + min;
};