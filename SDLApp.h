#ifndef _SDLAPP_H
#define _SDLAPP_H

#include <SDL/SDL.h>
#include "scene/Space.h"
#include "scene/Map.h"

class SDLApp {
   
public:
   SDLApp();
   ~SDLApp();
   void start();
   
private:
   void handleKeydown(SDL_KeyboardEvent &event);
   void handleKeyup(SDL_KeyboardEvent &event);
   void handleMouseMotion(SDL_MouseMotionEvent &event);
   void drawScene();
   void toggleMap();
   
   bool stopApp, _map;
   Scene* m_scene;
   Scene* scene;
   Map* map;
};

#endif
