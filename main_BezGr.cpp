#include "Teleso.h"
#include "Case.h"
int main(){
    Case c1;
    Teleso t1(147.5e9,0,0,29.8e3,6e24);
    Teleso t2(0,0,0,0,1.989e30);
    c1.Add(t1);
    c1.Add(t2);
    c1.SolveEuSymp(1,50000,500);
    // for (size_t i = 0; i < 2; i++)
    // {
    //     std::cout <<t1.r[i]<< std::endl;
    //     // std::cout <<"\n"<< std::endl;
    //     std::cout <<t1.dr[i]<< std::endl;
    //     // std::cout <<"\n"<< std::endl;
    //     t1.Dr();
    //     std::cout <<t1.dr[i]<< std::endl;
    //     std::cout <<"\n"<< std::endl;
    // }
}   