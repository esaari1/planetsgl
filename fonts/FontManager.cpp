#include "FontManager.h"
#include <SDL/SDL_ttf.h>
#include <fstream>

std::auto_ptr<FontManager> FontManager::s_instance;

FontManager::FontManager() {
   if(TTF_Init() == -1) {
      fprintf(stderr, "TTF_Init: %s\n", TTF_GetError());
   }
   initializeFonts();
}

FontManager::~FontManager() {
   TTF_Quit();
}

FontManager& FontManager::getInstance() {
   if(s_instance.get() == 0) {
      s_instance.reset(new FontManager);
   }
   return *s_instance;
}

void FontManager::initializeFonts() {
   ifstream is("fonts.txt");
   string name;
   string file;
   int size;
   
   while(!is.eof()) {
      is >> name;
      is >> file;
      is >> size;
      
      fonts[name] = new Font(file.c_str(), size);
   }
}

Font *FontManager::getFont(string name) {
   return fonts[name];
}
