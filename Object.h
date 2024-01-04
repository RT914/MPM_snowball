#ifndef __OBJECT_H__
#define __OBJECT_H__

#include <Eigen/Dense>
#include <vector>

using namespace std;

struct Point
{
	Point(Eigen::Vector2f& pos, Eigen::Vector2f& vel, Eigen::Matrix2f c, Eigen::Matrix2f f, float jp)
		: position(pos), velocity(vel), C(c), F(f), Jp(jp) {};

	Eigen::Vector2f position;
	Eigen::Vector2f velocity;
	Eigen::Matrix2f C;
	Eigen::Matrix2f F;
	double Jp;
};

struct Object
{
	Object(Eigen::Vector2f& pos, float rad) : position(pos), radius(rad) {};
	Eigen::Vector2f position;
	float radius;
	vector<Point> points;
};

Object CreateObject(int particle_num);

#endif