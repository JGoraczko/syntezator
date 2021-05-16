#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include <vector>
#include <string>
#include "test_definitions.h"

class Menu
{
   public:

     void mainMenu ();  //launches main menu - rest of explanations in "menu.cpp"

   private:

     int wave_form = 1;
     bool isFilterOn = false;
     int filterFrequency = 1000;
     void pressKeyToContinue (void);
     int getInfo (bool &error);
     void getInt (int &choice, bool &error);
     void getOption (int &choice, int options, bool &error);
     void waveformMenu (void);
     void filterMenu (void);
     void playMidiFile(void);
};

#endif // MENU_H_INCLUDED
