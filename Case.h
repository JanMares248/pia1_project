#include <array>
#include <vector>
#include <cmath>
#include <iostream>
#include <fstream>
#include "Teleso.h"
class Case;
    int dim;
    std::vector<Teleso> Telesa;

    static void Add(double x, double y, double px, double py, double ms)//Teleso t
        {
            Teleso t(x, y, px, py, ms);
            Telesa.push_back(t);
        }

	static void SolverWrite()
	// zapisuje do solverResult.txt ve formátu r1;r2 \n...pro kazde teleso... dp \n\n
		{
			std::ofstream MyFile("solverResult.txt");
			for (int i = 0; i < Telesa.size(); i++)
			{
				for (int j = 0; j < dim; j++)
				{
					MyFile << Telesa[i].nr[j];
					if (j==dim-1)
						{
							MyFile << "\n";
						}
						else
						{
							MyFile << ";";
						}
					}
				// pro pripad, ze bychom chteli vypisovat i p
				for (int j = 0; j < dim; j++)
				{
					MyFile << Telesa[i].np[j];
					if (j==dim-1)
						{
							MyFile << "\n\n";
						}
						else
						{
							MyFile << ";";
						}
					}
			}
			MyFile << dp;
			MyFile << "\n\n";
		}
	static void SolveEuSymp(double timeStep, double end, int saveInterval)
	{
		int nSteps = ceil(end / timeStep); //vim bude zaokrohlovat
		std::cout <<"Create time"<< std::endl;
		for (int t = 0; t < nSteps; t++)  //mirne nechapu prevod z shift space do souradnic
			{
				Dp();
				Dr();  // update hodnot v každý čas
				for (int i = 0; i < Telesa.size(); i++) // pro každé těleso
					{
						for (int j = 0; j < dim; j++) // pro každou souřadnici
						{
							Telesa[i].np[j] = Telesa[i].p[j] + timeStep * dp[j]; // spočte nové p a nové r
							Telesa[i].nr[j] = Telesa[i].r[j] + timeStep * dr[j];
							if (t%saveInterval==0)
							{
								SolverWrite(); // zapíše do souboru
							} 
						}
					}
			}
		for (int i = 0; i < Telesa.size(); i++)//for (Teleso t : Telesa) //-> nahrat nove do stare vrstvy, asi chybi.
		{
			for (int j = 0; j < dim; j++) // pro každou souřadnici
						{
			Telesa[i].p[j] = Telesa[i].np[j];
			}
			//t.p = t.np;
		}

	//pokud budete chtit dodelat solvery, tak mohme tady -> mluvil o normalnim eulerovy
};
	void PolTransReversible(double r[dim], bool reverse)
	{
		if (reverse==true)
		{
			x[0] = r[0]*cos(r[1]);
			x[1] = r[0]*sin(r[1]);
		}
		else
		{
			r[0] = sqrt(pow(x[0],2)+pow(x[1],2));
			r[1] = atan2(x[1],x[0]);
		}
	}
};