#include <array>
#include <vector>
#include <cmath>
#include <iostream>
#include <fstream>
#include "Body.h"

class Case {

//////////////////VARIABLES/////////////////////////////////////////////////////////

	double G=-6.6743e-11; // Gravitational constant 

	public:

		const int dim = 2; // the number of dimensions of the system
		std::vector<Body> Bodies; // the list of bodies
	

//////////////////METHODS///////////////////////////////////////////////////////////

//adds a body into the Telesa List  
	void Add(Body t)
        {
            Bodies.push_back(t);
        }

//calculates distance of two bodies in l2 norm
	double Distance(int i, int j) 
	{
		double s = 0;

		for (int k = 0; k < dim; k++) // sums over all dimensions
		{
			s+=pow(abs(Bodies[j].x[k]-Bodies[i].x[k]),2);
		}
		double v = sqrt(s);
		return v;
	}

//calculates the change of velocity to the next iteration
	double Dv(int telI,int coor)
	{
		double dvi=0;
		for (int j = 0; j < Bodies.size(); j++) // sum over all other bodies
		{
			if (j!=telI)
			{
				dvi-=(G*Bodies[j].m*(Bodies[j].x[coor]-Bodies[telI].x[coor]))/(pow(Distance(telI,j),3)); // calculate the force on this body
			}
		}
		return dvi;
		
	}

//Semi-implicit Euler method solver 
	void SolveEuSymp(double timeStep, double end, int saveInterval)
	{	
		std::ofstream file("polohy.txt"); // output file

		int Nsteps=end/timeStep; // number of steps

		for (int t = 0; t < Nsteps; t++)	// time iteration
		{
			for (int telI = 0; telI < Bodies.size(); telI++)	// body iteration
			{
				for (int coor = 0; coor < dim; coor++)	//dimension iteration for velocity
				{
					Bodies[telI].nv[coor]=Bodies[telI].v[coor]+Dv(telI,coor)*timeStep; // calculates new velocity
					Bodies[telI].v[coor]=Bodies[telI].nv[coor]; // updates velocity
				}																		
				
				for (int coor = 0; coor < dim; coor++)	//dimension iteration for position
				{
					if (t%saveInterval==0) // print to file
					{
						file << Bodies[telI].x[coor];
						if (coor<dim-1)
						{
							file << ",";
						}
					}
					Bodies[telI].nr[coor]=Bodies[telI].x[coor]+Bodies[telI].nv[coor]*timeStep;// calculates new position
					Bodies[telI].x[coor]=Bodies[telI].nr[coor]; //updates position
				}
				if (telI!=Bodies.size()-1) // print to file
				{
					if (t%saveInterval==0){
						file << ";";
					}
				}
			}
			if (t%saveInterval==0){
				file << "\n";
			}
		}
		file.close();
	}
};