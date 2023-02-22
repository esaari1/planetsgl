#include "Menu.h"

#ifdef WIN32
#include <GL/gl.h>
#else
#include <GLUT/glut.h>
#endif

#include "fonts/FontManager.h"

#define MENU_SPACING 30

Menu::Menu(string t) : title(t), mouseX(-1), mouseY(-1), titleTop(400), menuTop(370), menuLeft(100) 
{
   normalColor.r = 255;
   normalColor.g = 255;
   normalColor.b = 255;
   
   highlightColor.r = 255;
   highlightColor.g = 0;
   highlightColor.b = 0;
}

void Menu::draw() {
   glDisable(GL_LIGHTING);

   FontManager::getInstance().getFont("menuTitle")->setColor(normalColor);
   FontManager::getInstance().getFont("menuTitle")->drawText(title.c_str(), menuLeft, titleTop);
   
   vector<MenuItem>::iterator it;
   for(it = menuItems.begin(); it != menuItems.end(); it++) {
      if(it->pointWithin(mouseX, mouseY)) {
         FontManager::getInstance().getFont("menuItem")->setColor(highlightColor);
      } else {
         FontManager::getInstance().getFont("menuItem")->setColor(normalColor);
      }
      it->draw();
   }

   glEnable(GL_LIGHTING);   
}

void Menu::addMenuItem(MenuItem item) {
   int y = menuTop - (MENU_SPACING * menuItems.size());
   item.setPosition(menuLeft, y);
   menuItems.push_back(item);
}

void Menu::handleMouseMotion(const SDL_MouseMotionEvent& event) {
   this->mouseX = event.x;
   this->mouseY = 767 - event.y;
}

void Menu::handleMouseClick(const SDL_MouseButtonEvent& event) {
   vector<MenuItem>::iterator it;
   for(it = menuItems.begin(); it != menuItems.end(); it++) {
      if(it->pointWithin(mouseX, mouseY)) {
         it->performAction();
      }
   }
}

void Menu::setNormalColor(int r, int g, int b) {
   normalColor.r = r;
   normalColor.g = g;
   normalColor.b = b;
}

void Menu::setHighlightColor(int r, int g, int b) {
   highlightColor.r = r;
   highlightColor.g = g;
   highlightColor.b = b;
}
