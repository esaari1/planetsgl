/**
 *  Scene.h
 */
#ifndef _SCENE_H_
#define _SCENE_H_

#include <SDL/SDL.h>

class Scene {
   
public:
   virtual void setup() = 0;
   virtual void draw() = 0;
   virtual void handleKeydown(SDL_KeyboardEvent &event) = 0;
   virtual void handleKeyup(SDL_KeyboardEvent &event) = 0;
};

#endif
