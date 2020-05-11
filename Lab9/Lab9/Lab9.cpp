#include <iostream>
extern "C" double __cdecl IntegrateCotan(int N);
extern "C" int __cdecl Covid(int* death, int* days, int* death2);

void task1() {
    std::cout<<"_/`[pi/4; pi/2]ctgx dx = "<< IntegrateCotan(222);
}

void task2() {
    std::cout.precision(10);
    int _death = 0;
    int _death_not_ignor = 0;
    int _days = 0;
    int* death = &_death;
    int* days = &_days;
    int* death_not_ignor = &_death_not_ignor;
    Covid(death, days, death_not_ignor);
    std::cout << "\nDeath (30 days): " << _death;
    std::cout << "\nDays: " << _days<< " (death: "<<_death_not_ignor<<")";
}

int main()
{
    task1();   
    task2();
}
