// Scene.h
//
// Copyright (C) 2009, Eric Saari
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.

#ifndef _SCENE_H
#define _SCENE_H

#ifdef WIN32
#include <GL/gl.h>
#else
#include <GLUT/glut.h>
#endif

#include <vector>
#include "Camera.h"
#include "Scene.h"
#include <SDL/SDL.h>

using namespace std;

class Drawable;

class Space : public Scene {
    
public:
   Space();
   virtual ~Space();

   virtual void setup();
   virtual void draw();
   virtual void handleKeydown(SDL_KeyboardEvent &event);
   virtual void handleKeyup(SDL_KeyboardEvent &event);
   
private:
   void initializeBackground();
   void gotoSun();
   void gotoPlanet(int pidx);

   void stars(int a, int b, int c);

   Camera cam;
   
   float delU, delV, delN;
   float roll, pitch, yaw;
   GLuint starsId;
   double rollVel, slideVel;
   int currDays;
};

#endif
