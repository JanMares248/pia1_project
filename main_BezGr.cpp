#include "Teleso.h"
#include "Case.h"
int main(){
    Case c1;
    Teleso t1(100,100,10000,10000,100000);
    Teleso t2(10000,10000,-10000,-10000,100000);
    c1.Add(t1);
    c1.Add(t2);
    c1.SolveEuSymp(10,100000,100);
}   