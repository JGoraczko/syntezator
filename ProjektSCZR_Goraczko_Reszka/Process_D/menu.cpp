#include "menu.h"
#include <limits>
#include <cstdarg>
#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <csignal>
#include <sys/resource.h>
#include <ctime>

using namespace std;

#define NO_SUCH_OPTION "(Such option doesn't exist - try again)"

void Menu::pressKeyToContinue (void)    //"pauses" the program
{
  cout <<"Enter any key to continue ...\n";
  cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int Menu::getInfo(bool &error)  //gets signs till meeting ENTER
{
  error = false;
  char sign;
  string buff;
  do {
    cin.get(sign);
    buff += sign;
  } while (sign !='\n');
  return atoi(buff.c_str());
}

void Menu::getInt (int &choice, bool &error)    //gets only int (which is also != 0)
{
   if (!(choice = getInfo(error)))
     error = true;
}

void Menu::getOption (int &choice, int options, bool &error)    //gets correct number of chosen option in menu
{
  getInt(choice, error);
  if (!error && (choice < 1 || choice > options))
    error = true;
}

void Menu::mainMenu(char *arg[])   //launches main menu
{
  for (int i=0; i<4; ++i)
    parameters[i]= arg[i];
  setpriority(PRIO_PROCESS, 0, atoi(parameters[D_PRIORITY]));
  int choice;
  bool error = false;
  do
  {
    cout << "*****MIDI SYNTHESIZER*****\nChoose one of the options:\n1 - Choose waveform\n2 - Play from MIDI file\n3 - Exit\nYour choice:\n";
    if (error)
      cout << NO_SUCH_OPTION << endl;
    getOption(choice, 3, error);
    switch (choice)
    {
       case 1:
       {
          system("clear");
          waveformMenu();
          break;
       }

       case 2:
       {
          system("clear");
          playMidiFile();
         break;
       }
    }
    system("clear");
  }
  while (choice != 3);
}

void Menu::waveformMenu()
{
  int choice;
  bool error = false;
  do
  {
    cout << "*****MIDI SYNTHESIZER*****\nChoose one of the waveforms:\n1 - Sine wave (default)\n2 - Square wave\n3 - Triangle wave\n4 - Sawtooth wave\n5 - Return to main menu\nYour choice:\n";
    if (error)
      cout << NO_SUCH_OPTION << endl;
    getOption(choice, 5, error);
    switch (choice)
    {
       case 1:
       {
          system("clear");
          cout << "Sine wave is chosen.\n";
          wave_form = 1;
          pressKeyToContinue();
          break;
       }

       case 2:
       {
          system("clear");
          cout << "Square wave is chosen.\n";
          wave_form = 2;
          pressKeyToContinue();
         break;
       }

       case 3:
       {
          system("clear");
          cout << "Triangle wave is chosen.\n";
          wave_form = 3;
          pressKeyToContinue();
         break;
       }

       case 4:
       {
          system("clear");
          cout << "Sawtooth wave is chosen.\n";
          wave_form = 4;
          pressKeyToContinue();
         break;
       }
    }
    system("clear");
  }
  while (choice != 5);
}


void Menu::playMidiFile()
{
    string file_name;
    pid_t pid_tab[3];
    cout << "Enter name of MIDI file:\n";
    cin >> file_name;
    char *f_name = (char *) file_name.c_str();
    string s_numb = to_string(wave_form);
    char *wave_numb = (char *) s_numb.c_str();
    pid_t pid0 = fork();
    if( pid0 == 0){
        char *argv[] = {(char *) "./processA", f_name, wave_numb, NULL};
        setpriority(PRIO_PROCESS, 0, atoi(parameters[A_PRIORITY]));
        execve("./processA", argv, NULL);
    } else pid_tab[0] = pid0;

    pid_t pid1 = fork();
    if( pid1 == 0){
        char *argv1[] = {(char *) "./processB", NULL};
        //setpriority(PRIO_PROCESS, 0, atoi(parameters[A_PRIORITY]));
        execve("./processB", argv1, NULL);
    } else pid_tab[1] = pid1;
    
    string test_file_name = "test";
    time_t current_time = time(NULL);
    test_file_name += std::asctime(std::localtime(&current_time));
    test_file_name.pop_back();
    test_file_name += ".txt";
    char *test_f_name = (char *) test_file_name.c_str();
    pid_t pid2 = fork();
    if( pid2 == 0){
        char *argv2[] = {(char *) "./processC", test_f_name, parameters[SAVING_PERIOD], NULL};
        setpriority(PRIO_PROCESS, 0, atoi(parameters[C_PRIORITY]));
        execve("./processC", argv2, NULL);
    } else pid_tab[2] = pid2;


    cout << "Playing file " << file_name << "\nEnter any key to quit ...\n";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    kill(pid_tab[2], SIGABRT);   
    kill(pid_tab[1], SIGABRT);
    kill(pid_tab[0], SIGABRT);
}
