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

void Menu::mainMenu()   //launches main menu
{
  setpriority(PRIO_PROCESS, 0, D_PRIORITY);
  system("clear");
  int choice;
  bool error = false;
  do
  {
    cout << "*****MIDI SYNTHESIZER*****\nChoose one of the options:\n1 - Choose waveform\n2 - Show filter settings\n3 - Play from MIDI file\n4 - Exit\nYour choice:\n";
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
          filterMenu();
         break;
       }

       case 3:
       {
          system("clear");
          playMidiFile();
         break;
       }
    }
    system("clear");
  }
  while (choice != 4);
}

void Menu::filterMenu ()
{
  	int choice;
  	bool error = false;
  	do
  	{
    	cout << "*****MIDI SYNTHESIZER*****\n";
    	if (isFilterOn) 
    		cout << "Filtring is on - cut off frequency = " << filterFrequency << endl;
    	else
    		cout << "Filtring is off" << endl;
    	cout << "Choose one of the options:\n1 - Set different frequency\n2 - Turn on filtering\n3 - Turn off filtering\n4 - Return to main menu\nYour choice:\n";
    	if (error)
      		cout << NO_SUCH_OPTION << endl;
    	getOption(choice, 3, error);
    	switch (choice)
    	{
    	   case 1:
    	   {
    	      system("clear");
    	      cout << "Enter new frequency: ";
    	      string buf;
    	      cin >> buf;
    	      filterFrequency = atoi(buf.c_str());
    	      cout << "Set frequency value to " << filterFrequency << endl;
    	      pressKeyToContinue();
    	      break;
    	   }
            
            case 2:
    	   {
    	      system("clear");
    	      cout << "Filter is turned on. Filter frequency = " << filterFrequency << endl;
    	      isFilterOn = true;
    	      pressKeyToContinue();
    	      break;
    	   }       

           case 3:
    	   {
    	      system("clear");
    	      isFilterOn = false;
    	      cout << "Filter is turned off\n";
    	      pressKeyToContinue();
    	      break;
    	   }
          
    	}
    	system("clear");
  	} while (choice != 4);
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
    string testB_file_name = "testB", testC_file_name = "testC";
    time_t current_time = time(NULL);
    testB_file_name += std::asctime(std::localtime(&current_time));
    testB_file_name.pop_back();
    testB_file_name += ".txt";
    testC_file_name += std::asctime(std::localtime(&current_time));
    testC_file_name.pop_back();
    testC_file_name += ".txt";
    string filterStatus = to_string(isFilterOn), filterFreq = to_string(filterFrequency);
    char *testB_f_name = (char *) testB_file_name.c_str();
    char *testC_f_name = (char *) testC_file_name.c_str();
        
    //creating A process
    pid_t pid0 = fork();
    if( pid0 == 0){
        char *argv[] = {(char *) "./processA", f_name, wave_numb, NULL};
        setpriority(PRIO_PROCESS, 0, A_PRIORITY);
        execve("./processA", argv, NULL);
    } else pid_tab[0] = pid0;

	//creating B process
    pid_t pid1 = fork();
    if( pid1 == 0){
        char *argv1[] = {(char *) "./processB", (char*) filterStatus.c_str(), (char*) filterFreq.c_str(), testB_f_name, NULL};
        setpriority(PRIO_PROCESS, 0, B_PRIORITY);
        execve("./processB", argv1, NULL);
    } else pid_tab[1] = pid1;
    
    //creating C process
    pid_t pid2 = fork();
    if( pid2 == 0){
        char *argv2[] = {(char *) "./processC", testC_f_name, NULL};
        setpriority(PRIO_PROCESS, 0, C_PRIORITY);
        execve("./processC", argv2, NULL);
    } else pid_tab[2] = pid2;


    cout << "Playing file " << file_name << "\nEnter any key to quit ...\n";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    kill(pid_tab[2], SIGABRT);   
    kill(pid_tab[1], SIGABRT);
    kill(pid_tab[0], SIGABRT);
}
