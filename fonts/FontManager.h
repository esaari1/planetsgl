#ifndef _FONT_MANAGER_H_
#define _FONT_MANAGER_H_

#include <memory>
#include <string>
#include "Font.h"
#include <map>

using namespace std;

class FontManager {

public:
   static FontManager& getInstance();
   ~FontManager();
   Font *getFont(string name);

private:
   FontManager();
   void initializeFonts();

   static auto_ptr<FontManager> s_instance;
   map<string, Font *> fonts;
};

#endif
