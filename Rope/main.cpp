#include <iostream>
#include <span>
#include "Rope.h"

using std::cout;
using std::endl;
using std::flush;

int main()
{
    auto buff1 = std::span<const char>(new char[12]{'H', 'e', 'l', 'l', 'o', ' ', 'W', 'o', 'r', 'l', 'd', '!'}, 12); 
    auto buff3 = std::span<const char>(new char[10]{'B','r','a','v','e',' ','N','e','w',' '}, 10); 
    auto buff4 = std::span<const char>(new char[1]{'*'}, 1);

    for (int i = 0; i < 23; i++)
    {
        phillno::Rope<const char> R1(buff1);
        R1.insert(6, buff3); //insert span
        
        phillno::Rope<const char> star_rope(buff4);
        R1.insert(i, star_rope); //insert rope

        cout << "*" << R1 << "*" << endl << flush;        
    }

    phillno::Rope<const char> R1(buff1);
    cout << "\n*" << R1 << "*" << endl;

    R1.remove(5, 1);
    cout << "*" << R1 << "*" << endl;

    R1 = phillno::Rope<const char>(buff1); //test copy constructor
    cout << "*" << R1 << "*" << endl;
    auto R3 = phillno::Rope<const char>(buff3);
    R1.insert(6, R3);
    cout << "*" << R1 << "*" << endl;

    //ensure no side effects in R1 due to changes in R3
    auto R4 = phillno::Rope<const char>(buff4);
    R3.insert(0, R4);
    R3.insert(R3.length(), R4);
    
    cout << "*" << R1 << "*" << endl;

    R1.remove(6, 10);
    cout << "*" << R1 << "*\n" << endl;
    
    //phillno::Rope<int> R5 {};// = phillno::Rope<int>(); // Rope<int> R5(); is not a constructor!
    //cout << "*" << R5 << "*" << endl;
    //int r5 = R5.at(0);
    //cout << "*" << r5 << "*" << endl;


    return 0;

}
