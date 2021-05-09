#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include <vector>

class Menu
{
   public:

     void mainMenu (void);  //launches main menu - rest of explanations in "menu.cpp"

   private:


     void pressKeyToContinue (void);
     int getInfo (bool &error);
     void getInt (int &choice, bool &error);
     void getOption (int &choice, int options, bool &error);
     void waveformMenu (void);
     void playMidiFile(void);
};

#endif // MENU_H_INCLUDED
