#include "Teleso.h"
#include "Case.h"
int main(){
    Case c1;
    Teleso t1(-5,100,0,3,6e10);
    Teleso t2(5,-100,0,-3,4e10);
    c1.Add(t1);
    c1.Add(t2);
    c1.SolveEuSymp2(1,100,10);
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