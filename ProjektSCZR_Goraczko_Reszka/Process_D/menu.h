#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include <vector>

#define A_PRIORITY 0
#define C_PRIORITY 1
#define D_PRIORITY 2
#define SAVING_PERIOD 3
#define WAVE_FORM 4
#define SINEW 1

class Menu
{
   public:

     void mainMenu (int *arg);  //launches main menu - rest of explanations in "menu.cpp"

   private:

     int parameters[5];
     void pressKeyToContinue (void);
     int getInfo (bool &error);
     void getInt (int &choice, bool &error);
     void getOption (int &choice, int options, bool &error);
     void waveformMenu (void);
     void playMidiFile(void);
};

#endif // MENU_H_INCLUDED
