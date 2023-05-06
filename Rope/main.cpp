#include <iostream>
#include <cstring>
#include "Rope.h"

int main()
{
    char *const original   = new char[12]{'H', 'e', 'l', 'l', 'o', ' ', 'W', 'o', 'r', 'l', 'd', '!'};
    char *const new_chunk  = new char[10]{'B','r','a','v','e',' ','N','e','w',' '};
    char *const star_chunk = new char[2]{'*'};

    const phillno::ManagedArr<const char> buff1 = phillno::ManagedArr<const char>(original, 0, 12,  phillno::AllocationType::STATIC); 
    const phillno::ManagedArr<const char> buff2 = phillno::ManagedArr<const char>(buff1, 6, 6);
    const phillno::ManagedArr<const char> buff3 = phillno::ManagedArr<const char>(new_chunk, 0, 10, phillno::AllocationType::STATIC); 
    const phillno::ManagedArr<const char> buff4 = phillno::ManagedArr<const char>(star_chunk, 0, 1, phillno::AllocationType::STATIC);
    
    for (int i = 0; i < 23; i++)
    {
        phillno::Rope<const char> R1(buff1);
        R1.insert(6, buff3);
        
        //R1.insert(i, buff4);
        phillno::Rope<const char> star_rope(buff4);
        R1.insert(i, star_rope);

        std::cout << R1 << "*" << std::endl << std::flush;        
    }

    phillno::Rope<const char> R1(buff1);
    std::cout << "\n*" << R1 << "*" << std::endl;

    R1.remove(5, 1);
    std::cout << "*" << R1 << "*" << std::endl;


    R1 = phillno::Rope<const char>(buff1);
    //R1.insert(6, buff3);
    auto R3 = phillno::Rope<const char>(buff3);
    R1.insert(6, R3);

    //ensure no side effects in R1 due to changes in R3
    auto R4 = phillno::Rope<const char>(buff4);
    R3.insert(0, R4);
    R3.insert(R3.length(), R4);
    
    std::cout << "*" << R1 << "*" << std::endl;

    R1.remove(6, 10);
    std::cout << "*" << R1 << "*\n\n" << std::endl;

    return 0;

}
