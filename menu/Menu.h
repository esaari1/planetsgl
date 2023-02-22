#ifndef MENU_H_
#define MENU_H_

#include <vector>
#include <string>
#include <SDL/SDL.h>
#include "fonts/Font.h"
#include "MenuItem.h"

using std::vector;
using std::string;

class Menu {
   
public:
   Menu(string title);
   void draw();
   void handleMouseMotion(const SDL_MouseMotionEvent& event);
   void handleMouseClick(const SDL_MouseButtonEvent& event);
   void addMenuItem(MenuItem item);
   
   void setNormalColor(int r, int g, int b);
   void setHighlightColor(int r, int g, int b);
   
private:
   string title;
   int mouseX;
   int mouseY;
   int titleTop;
   int menuTop;
   int menuLeft;

   vector<MenuItem> menuItems;
   SDL_Color normalColor;
   SDL_Color highlightColor;
};

#endif /*MENU_H_*/
