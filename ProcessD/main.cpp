#include "menu.h"
#include <iostream>

using namespace std;

int main(int argc, char * argv[])
{
    if (argc != 6) return -1;

    int arg[5];
    for (int i=0; i<5; ++i)
        arg[i]= stoi(argv[i+1]);

    Menu menu;
    menu.mainMenu(arg);
    return 0;
}
