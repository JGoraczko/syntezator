#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include <vector>

#define A_PRIORITY 0
#define C_PRIORITY 1
#define D_PRIORITY 2
#define BUFFER_SIZE 3
#define N_SAMPLES_SAVED 4
#define WAVE_FORM 5
#define SINEW 1

class Menu
{
   public:

     void mainMenu (int *arg);  //launches main menu - rest of explanations in "menu.cpp"

   private:

     int parameters[6];
     void pressKeyToContinue (void);
     int getInfo (bool &error);
     void getInt (int &choice, bool &error);
     void getOption (int &choice, int options, bool &error);
     void waveformMenu (void);
     void playMidiFile(void);
};

#endif // MENU_H_INCLUDED
