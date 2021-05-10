#include "menu.h"
#include <iostream>
#include <ctime>

using namespace std;

int main(int argc, char * argv[])
{
    string test_file_name = "test";
    time_t current_time = time(NULL);
    test_file_name += current_time;
    test_file_name += ".txt";
    cout << test_file_name;
    if (argc != 5) return -1;

    char* arg[4];
    for (int i=0; i<4; ++i)
        arg[i]= argv[i+1];

    Menu menu;
    menu.mainMenu(arg);
    return 0;
}
