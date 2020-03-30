#include <iostream>

extern "C" int __cdecl SortString(char** ss, int n);
extern "C" int __cdecl BankingDiv(int a, int b);

void task1() {
    int n = 28;
    // слова из случайного текста
    char s1[] = "next";
    char s2[] = "to";
    char s3[] = "people";
    char s4[] = "whereas";
    char s5[] = "wild";
    char s6[] = "animal";
    char s7[] = "homes";
    char s8[] = "are";
    char s9[] = "forests";
    char s10[] = "jungles";
    char s11[] = "oceansand";
    char s12[] = "so";
    char s13[] = "on";
    char s14[] = "giraffes";
    char s15[] = "area";
    char s16[] = "very";
    char s17[] = "beautifuland";
    char s18[] = "unusual";
    char s19[] = "animals";
    char s20[] = "they";
    char s21[] = "are";
    char s22[] = "the";
    char s23[] = "tallest";
    char s24[] = "land";
    char s25[] = "animals";
    char s26[] = "in";
    char s27[] = "the";
    char s28[] = "world";

    char* ss[] = { 
        s1, s2, s3, s4, s5, s6, s7, s8, s9, s10,
        s11, s12, s13, s14, s15, s16, s17, s18, s19, s20,
        s21, s22, s23, s24, s25, s26, s27, s28
    };

    SortString(ss, n);

    for (int i = 0; i < n; i++) {
        std::cout << ss[i] << '\n';
    }
}

void task2(){
    std::cout<<'\n'<<BankingDiv(2,3)<<'\n';
}

int main()
{   
    task1();
    task2();
    return 0;
}
