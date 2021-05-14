#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include <vector>
#include <string>

#define A_PRIORITY 0
#define C_PRIORITY 1
#define D_PRIORITY 2
#define SAVING_PERIOD 3

class Menu
{
   public:

     void mainMenu (char *arg[]);  //launches main menu - rest of explanations in "menu.cpp"

   private:

     int wave_form = 1;
     char *parameters[4];
     void pressKeyToContinue (void);
     int getInfo (bool &error);
     void getInt (int &choice, bool &error);
     void getOption (int &choice, int options, bool &error);
     void waveformMenu (void);
     void playMidiFile(void);
};

#endif // MENU_H_INCLUDED
