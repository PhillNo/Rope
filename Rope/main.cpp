#include <iostream>
#include <cstring>
#include <span>
#include "Rope.h"

int main()
{
    char *const original   = new char[12]{'H', 'e', 'l', 'l', 'o', ' ', 'W', 'o', 'r', 'l', 'd', '!'};
    char *const new_chunk  = new char[10]{'B','r','a','v','e',' ','N','e','w',' '};
    char *const star_chunk = new char[2]{'*'};

    std::span<const char> original_span   = std::span(original, 12);
    std::span<const char> new_chunk_span  = std::span(original, 10);
    std::span<const char> star_chunk_span = std::span(original, 2);

    //for (auto el : mySpan)
    //{
    //    std::cout << el;
    //}
    //std::cout << std::endl;


    auto buff1 = phillno::ManagedArr<const char>(original, 0, 12,  phillno::AllocationType::STATIC); 
    auto buff2 = phillno::ManagedArr<const char>(buff1, 6, 6);
    auto buff3 = phillno::ManagedArr<const char>(new_chunk, 0, 10, phillno::AllocationType::STATIC); 
    auto buff4 = phillno::ManagedArr<const char>(star_chunk, 0, 1, phillno::AllocationType::STATIC);
    
    for (int i = 0; i < 23; i++)
    {
        phillno::Rope<const char> R1(buff1);
        R1.insert(6, buff3);
        
        //R1.insert(i, buff4);
        phillno::Rope<const char> star_rope(buff4);
        R1.insert(i, star_rope);

        std::cout << "*" << R1 << "*" << std::endl << std::flush;        
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
    std::cout << "*" << R1 << "*\n" << std::endl;

    phillno::Rope<int> R5 {};// = phillno::Rope<int>(); // Rope<int> R5(); is not a constructor!
    std::cout << "*" << R5 << "*" << std::endl;
    //int r5 = R5.at(0);
    //std::cout << "*" << r5 << "*" << std::endl;

    return 0;

}
