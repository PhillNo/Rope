#include <iostream>
#include <cstring>
#include <span>
#include "ManagedArr.h"
#include "Rope.h"

using std::cout;
using std::endl;
using std::flush;

int main()
{
    char *const original   = new char[12]{'H', 'e', 'l', 'l', 'o', ' ', 'W', 'o', 'r', 'l', 'd', '!'};
    char *const new_chunk  = new char[10]{'B','r','a','v','e',' ','N','e','w',' '};
    char *const star_chunk = new char[1]{'*'};

    auto buff1 = std::span<const char>(original, 12); 
    auto buff3 = std::span<const char>(new_chunk, 10); 
    auto buff4 = std::span<const char>(star_chunk, 1);

    for (int i = 0; i < 23; i++)
    {
        phillno::Rope<const char> R1(buff1);
        R1.insert(6, buff3); //insert span
        
        phillno::Rope<const char> star_rope(buff4);
        R1.insert(i, star_rope); //insert rope

        std::cout << "*" << R1 << "*" << std::endl << std::flush;        
    }

    phillno::Rope<const char> R1(buff1);
    std::cout << "\n*" << R1 << "*" << std::endl;

    R1.remove(5, 1);
    std::cout << "*" << R1 << "*" << std::endl;

    R1 = phillno::Rope<const char>(buff1);
    std::cout << "*" << R1 << "*" << std::endl;
    auto R3 = phillno::Rope<const char>(buff3);
    R1.insert(6, R3);
    std::cout << "*" << R1 << "*" << std::endl;

    //ensure no side effects in R1 due to changes in R3
    auto R4 = phillno::Rope<const char>(buff4);
    R3.insert(0, R4);
    R3.insert(R3.length(), R4);
    
    std::cout << "*" << R1 << "*" << std::endl;

    R1.remove(6, 10);
    std::cout << "*" << R1 << "*\n" << std::endl;
    
    //phillno::Rope<int> R5 {};// = phillno::Rope<int>(); // Rope<int> R5(); is not a constructor!
    //std::cout << "*" << R5 << "*" << std::endl;
    //int r5 = R5.at(0);
    //std::cout << "*" << r5 << "*" << std::endl;


    return 0;

}
