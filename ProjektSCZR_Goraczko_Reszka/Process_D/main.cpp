#include "menu.h"
#include <iostream>
#include <ctime>

using namespace std;

int main(int argc, char * argv[])
{
    if (argc != 5) return -1;

    char* arg[4];
    for (int i=0; i<4; ++i)
        arg[i]= argv[i+1];

    Menu menu;
    menu.mainMenu(arg);
    return 0;
}
