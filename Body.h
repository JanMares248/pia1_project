#pragma managed(push, off)
//using coordinates = double[];  //mnenitelne typy
//using speeds = double[];
//using mass = double;
#include <array>
#include <vector>
#include <cmath>
#include <iostream>
#include <fstream>
#pragma once
class Body
{
public:
	static const int dim=2; // the number of dimensions of the system
	double x[dim]; // carthesian coordinates
	double v[dim]; // velocity
	double m; // mass
	double nr[dim]; // new coordinates
	double nv[dim]; // new velocity

	Body(double x_, double y_, double vx, double vy, double ms)
	{
		x[0] = x_;
		x[1] = y_;
		v[0] = vx;
		v[1] = vy;
		m = ms;
	}
};

#pragma managed(pop)