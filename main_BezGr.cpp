#include "Body.h"
#include "Case.h"
int main(){
    Case c1;
    Body t1(147.5e9,0,0,29.8e4,6e24);
    Body t2(0,0,0,0,1.989e30);
    c1.Add(t1);
    c1.Add(t2);
    c1.SolveEuSymp(1,50000,500);
}   