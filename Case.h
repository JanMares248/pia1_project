#include <array>
#include <vector>
#include <cmath>
#include <iostream>
#include <fstream>
#include "Teleso.h"
class Case {
	public:
    const int dim = 2;
    std::vector<Teleso> Telesa;

    // static void Add(double x, double y, double px, double py, double ms)//Teleso t
	void Add(Teleso t)
        {
            // Teleso t(x, y, px, py, ms);
            Telesa.push_back(t);
        }

	// void SolverWrite(std::ofstream file)
	// // zapisuje do solverResult.txt ve formátu r1;r2 \n...pro kazde teleso... dp \n\n
	// 	{
			
	// 		for (int i = 0; i < Telesa.size(); i++)
	// 		{
	// 			for (int j = 0; j < dim; j++)
	// 			{
	// 				file << Telesa[i].nr[j];
	// 				if (j==dim-1)
	// 					{
	// 						file << "\n";
	// 					}
	// 					else
	// 					{
	// 						file << ";";
	// 					}
	// 				}
	// 			// pro pripad, ze bychom chteli vypisovat i p
	// 		// 	for (int j = 0; j < dim; j++)
	// 		// 	{
	// 		// 		MyFile << Telesa[i].np[j];
	// 		// 		if (j==dim-1)
	// 		// 			{
	// 		// 				file << "\n\n";
	// 		// 			}
	// 		// 			else
	// 		// 			{
	// 		// 				file << ";";
	// 		// 			}
	// 		// 		}
	// 		// file << Telesa[i].dp;
	// 		}
	// 		file << "\n\n";
	// 	}
	// double Hamiltonian()//vypocet hamiltonianu
	// {
	// 	double hamiltonian;
	// 	double grav = 0;
	// 	for (int i = 0; i < dim; i++)
	// 	{
	// 		for (int j = 0; j < dim;j++)
	// 		{
	// 			if (i < j)
	// 			{
	// 				grav = 6.6743e-11 * Telesa[i].m * Telesa[j].m / Distance(i, j);
	// 			}
	// 			else
	// 			{
	// 				grav = 0;
	// 			}
	// 			hamiltonian += (pow(2.0, Telesa[i].p[0]) + pow(2.0, Telesa[i].p[1])) / (2 * Telesa[i].m) + grav;
	// 		}
	// 	}
	// 	return hamiltonian;
	// };
	
	double Distance(int i, int j)
	{
		double s = 0;
		for (int k = 0; k < dim; k++)
		{
			// std::cout<<s<<std::endl;
			s+=pow(abs(Telesa[j].r[k]-Telesa[i].r[k]),2);
		}
		// std::cout<<"end"<<std::endl;
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
				dvi-=(G*Telesa[j].m*(Telesa[j].r[coor]-Telesa[telI].r[coor]))/(pow(Distance(telI,j),3));
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
					Telesa[telI].np[coor]=Telesa[telI].p[coor]+Dv(telI,coor)*timeStep;	// bacha, np a p jsou ted rychlosti aby se
					Telesa[telI].p[coor]=Telesa[telI].np[coor]; //rovnou prepise		// ty rovnice trochu zjednodusily,
				}																		// eventualne prejmenuju
				
				for (int coor = 0; coor < dim; coor++)	//souradincova iterace poloha
				{
					Telesa[telI].PolTransReversible(true);
					if (t%saveInterval==0)
					{
						file << Telesa[telI].x[coor];
						if (coor<dim-1)
						{
							file << ",";
						}
					}
					Telesa[telI].nr[coor]=Telesa[telI].r[coor]+Telesa[telI].np[coor]*timeStep;
					Telesa[telI].r[coor]=Telesa[telI].nr[coor];
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