//using coordinates = double[];  //mnenitelne typy
//using speeds = double[];
//using mass = double;
#include <array>
#include <vector>
#include <cmath>
#include <iostream>
#include <fstream>
#pragma once
class Teleso
{
public:
	static const int dim=2;//dimenze, bude potřeba ještě předělat/ dát jinam
	double x[dim];//coordinates polar;
	double v[dim];//momentum p;
	double m;//mass m;
	double nr[dim];//new coordinates x;
	double nv[dim];//new velocity v;

	//pripravene vysledne veliciny
	 double dr[dim];
	 double dp[dim];

	Teleso(double x_, double y_, double vx, double vy, double ms)
	{
		x[0] = x_;
		x[1] = y_;
		v[0] = vx;
		v[1] = vy;
		m = ms;
	}
};