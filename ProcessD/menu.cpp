#include "menu.h"
#include <limits>
#include <cstdarg>
#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <csignal>
#include <sys/resource.h>

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

void Menu::mainMenu(int *arg)   //launches main menu
{
  for (int i=0; i<4; ++i)
    parameters[i]= arg[i];
  arg[WAVE_FORM] = SINEW;
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
          parameters[WAVE_FORM] = 1;
          pressKeyToContinue();
          break;
       }

       case 2:
       {
          system("clear");
          cout << "Square wave is chosen.\n";
          parameters[WAVE_FORM] = 2;
          pressKeyToContinue();
         break;
       }

       case 3:
       {
          system("clear");
          cout << "Triangle wave is chosen.\n";
          parameters[WAVE_FORM] = 3;
          pressKeyToContinue();
         break;
       }

       case 4:
       {
          system("clear");
          cout << "Sawtooth wave is chosen.\n";
          parameters[WAVE_FORM] = 4;
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
    //Tworzenie i usuwanie procesów
    //pid_t tab[3];
    cout << "Enter name of MIDI file:\n";
    cin >> file_name;
    /*char name [file_name.size()+1];
    file_name.copy(name, file_name.size()+1);
    name[file_name.size()] = '\0';
    // Create first process
    pid_t pid = fork();
    if( pid == 0){
        char *argv[] = {"processC", name, NULL};
        execve("./processC", argv, NULL);
    } else tab[0] = pid;
    //ustalanie priorytetów
    setpriority(PRIO_PROCESS, 0, 19);
    */
    cout << "Playing file " << file_name << "\nEnter any key to quit ...\n";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    //usuwanie odpowiednich procesów
    //kill(tab[0], SIGABRT);
}
