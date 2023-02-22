#include "MenuItem.h"

MenuItem::MenuItem(string _text, boost::function<void ()> _callback, int x, int y) : text(_text), callback(_callback)
{
   int w, h;
   FontManager::getInstance().getFont("menuItem")->getTextSize(text.c_str(), &w, &h);
   pos.x = x;
   pos.y = y;
   pos.w = w;
   pos.h = h;
}

void MenuItem::draw() {
   FontManager::getInstance().getFont("menuItem")->drawText(text.c_str(), pos.x, pos.y);
}

void MenuItem::setPosition(int x, int y) {
   pos.x = x; pos.y = y; 
}

bool MenuItem::pointWithin(int x, int y) const {
   return (x >= pos.x && x <= pos.x + pos.w && y >= pos.y && y <= pos.y + pos.h);
}

void MenuItem::performAction() {
   if(callback) {
      callback();
   }
}
