#include <array>
#include <vector>
#include <cmath>
#include <iostream>
#include <fstream>
#include "Teleso.h"

class Case {

//////////////////VARIABLES/////////////////////////////////////////////////////////
	public:
    const int dim = 2;
    std::vector<Teleso> Telesa;

//////////////////METHODS///////////////////////////////////////////////////////////
	void Add(Teleso t)
        {
            Telesa.push_back(t);
        }

	double Distance(int i, int j)
	{
		double s = 0;
		for (int k = 0; k < dim; k++)
		{
			s+=pow(abs(Telesa[j].x[k]-Telesa[i].x[k]),2);
		}
		double v = sqrt(s);
		return v;
	}
	double Dv(int telI,int coor){
		double dvi=0;
		double G=-6.6743e-11;
		// double G=-6.6743e2;
		for (int j = 0; j < Telesa.size(); j++)
		{
			if (j!=telI)
			{
				dvi-=(G*Telesa[j].m*(Telesa[j].x[coor]-Telesa[telI].x[coor]))/(pow(Distance(telI,j),3));
			}
		}
		return dvi;
		
	}
	void SolveEuSymp(double timeStep, double end, int saveInterval)
	{	
		std::ofstream file("solverResult.txt");
		int Nsteps=end/timeStep;
		for (int t = 0; t < Nsteps; t++)	//casova iterace
		{
			for (int telI = 0; telI < Telesa.size(); telI++)	//telesova iterace
			{
				for (int coor = 0; coor < dim; coor++)	//souradincova iterace rychlost
				{
					Telesa[telI].nv[coor]=Telesa[telI].v[coor]+Dv(telI,coor)*timeStep;
					Telesa[telI].v[coor]=Telesa[telI].nv[coor]; //rovnou prepise
				}																		
				
				for (int coor = 0; coor < dim; coor++)	//souradincova iterace poloha
				{
					if (t%saveInterval==0)
					{
						file << Telesa[telI].x[coor];
						if (coor<dim-1)
						{
							file << ",";
						}
					}
					Telesa[telI].nr[coor]=Telesa[telI].x[coor]+Telesa[telI].nv[coor]*timeStep;
					Telesa[telI].x[coor]=Telesa[telI].nr[coor];
				}
				if (telI!=Telesa.size()-1)
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
	}
};