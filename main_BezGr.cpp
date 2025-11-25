#include "Teleso.h"
int main(){
    Teleso t1(100,100,10000,10000,100000);
    Teleso t2(10000,10000,-10000,-10000,100000);
    t1.Telesa = {t1,t2};
    t1.SolveEuSymp(10,1000000000,1000);
}