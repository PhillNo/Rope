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
    auto buff5 = std::span<const char>(new char[1]{' '}, 1);

    cout << "Single character insertions at indices [0,n] of a \"monolithic\" Rope:\n"
        "============================================================================" << endl;

    for (int i = 0; i < 23; i++)
    {
        phillno::Rope<const char> R1(buff1);
        R1.insert(6, buff3); //insert span
        
        phillno::Rope<const char> star_rope(buff4);
        R1.insert(i, star_rope); //insert rope

        cout << R1 << endl << flush;        
    }


    cout << "\n\nRemove single character from \"monolithic\" Rope:\n"
        "============================================================================" << endl;

    phillno::Rope<const char> R1(buff1);
    cout << R1 << endl;

    R1.remove(5, 1);
    cout << R1 << endl;


    cout << "\n\nInstantiating via copy contstructor:\n"
        "============================================================================" << endl;

    R1 = phillno::Rope<const char>(R1);
    cout << R1 << endl;


    cout << "\n\nInserting Rope into Rope:\n"
        "============================================================================" << endl;
    auto R3 = phillno::Rope<const char>(buff3);
    R1.insert(5, phillno::Rope<const char>(buff5));
    R1.insert(6, R3);
    
    cout << R1 << endl;


    cout << "\n\nInserting Ropes at ends of previously used Rope:\n"
        "============================================================================" << endl;
    auto R4 = phillno::Rope<const char>(buff4);
    R1.insert(0, R4);
    R1.insert(R1.length(), R4);
    cout << R1 << endl;


    cout << "\n\nRemoving multiple characters from \"composite\" Rope:\n"
        "============================================================================" << endl;
    R1.remove(6, 10);
    cout << R1 << endl;
    

    return 0;

}
