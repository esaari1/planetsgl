#ifndef MENUITEM_H_
#define MENUITEM_H_

#include <string>
#include "fonts/FontManager.h"
#include <SDL/SDL.h>
#include <boost/function.hpp>

using std::string;

class MenuItem {

public:
   MenuItem(string _text, boost::function<void ()> _callback = 0, int x = 0, int y = 0);
   
   void setPosition(int x, int y);
   void draw();   
   bool pointWithin(int x, int y) const;
   void performAction();
   
private:
   string text;
   boost::function<void ()> callback;
   SDL_Rect pos;
};

#endif /*MENUITEM_H_*/
