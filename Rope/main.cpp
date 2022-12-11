#include <iostream>
#include <cstring>
#include "Rope.h"

int main()
{
    const char* original  = new char[12]{'H', 'e', 'l', 'l', 'o', ' ', 'W', 'o', 'r', 'l', 'd', '!'};
    const char* newChunk  = new char[10]{'B','r','a','v','e',' ','N','e','w',' '};
    const char* starChunk = new char[2]{'*', 0};

    const phillno::ManagedArr<const char> buff1 = phillno::ManagedArr<const char>(original, 0, 12, phillno::AllocationType::DYNAMIC); 
    const phillno::ManagedArr<const char> buff2 = phillno::ManagedArr<const char>(buff1, 6, 6);
    const phillno::ManagedArr<const char> buff3 = phillno::ManagedArr<const char>(newChunk, 0, 10, phillno::AllocationType::DYNAMIC); 
    const phillno::ManagedArr<const char> buff4 = phillno::ManagedArr<const char>(starChunk, 0, 1, phillno::AllocationType::DYNAMIC);
    
    for (int i = 0; i < 23; i++)
    {
        phillno::Rope<const char> R1 = phillno::Rope<const char>(buff1);
        R1.insert(6, buff3);
        
        R1.insert(i, buff4);
        
        for (int j = 0; j < R1.len(); j++)
        {
            std::cout << R1[j];
        }
        std::cout << std::endl << std::flush;        
    }
    return 0;
}
