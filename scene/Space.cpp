// Space.cpp
//
// Copyright (C) 2009, Eric Saari
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.

#ifdef WIN32
#define GLEW_STATIC 1
#include <GL/glew.h>
#endif

#include "Space.h"
#include "mesh/MeshManager.h"
#include "utility/DevILHelper.h"
#include "utility/OptionsManager.h"
#include "utility/ShaderManager.h"
#include "drawable/DrawableManager.h"
#include "parser/Hash.h"
#include "drawable/Body.h"
#include "drawable/Model.h"
#include <math.h>
#include "physics/time_funcs.h"

const float SUN_TO_EARTH = 149597.890;
const GLfloat lightPosition[] = {0, 0, 0, 1};
const GLfloat ambient_light[] = {0.0, 0.0, 0.0, 0.0};

Space::Space() : 
   cam(), 
   delU(0), delV(0), delN(0),
   roll(0), pitch(0), yaw(0),
   currDays(0)
{
   cam.set(Point3D(SUN_TO_EARTH, 0, 25), Point3D(SUN_TO_EARTH, 0, 0), Vector3D(0, 1, 0));
   
   slideVel = pow(2.0, 2.0) / 100.0;
   rollVel = pow(3.0, 2.0) / 100.0;

   initializeBackground();

   DrawableManager::instance();
   MeshManager::instance();
}

Space::~Space() {
}

void Space::setup() {
   glActiveTexture(GL_TEXTURE0);
   glEnable(GL_TEXTURE_2D);
   
   glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light);
   glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_light);
}

void Space::draw() {
   glLoadIdentity();

   cam.roll(roll);
   cam.pitch(pitch);
   cam.yaw(yaw);
   
   glPushMatrix();
   cam.undoTranslate();
   cam.loadGLMatrix();

   glUseProgramObjectARB(0);
   glCallList(starsId);
   glPopMatrix();

   cam.slide(delU, delV, delN);
   cam.loadGLMatrix();

   glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
   double days = daysSinceEpoch();
   
//   if(currDays != days) {
//      currDays = days;
//      for(DrawableIter it = DrawableManager::instance().begin(); it != DrawableManager::instance().end(); it++) {
//         it->second->calculateLocation(days);
//      }
//   }
   
   for(DrawableIter it = DrawableManager::instance().begin(); it != DrawableManager::instance().end(); it++) {
      it->second->calculateLocation(days);
      Point3D distance = it->second->getLocation() - cam.getLocation();
      double size = it->second->getRadius();
      double ratio = atan2(size, distance.distanceFromOrigin());
      
      if(ratio > 0.001 || it->second->getName() == "Sun") {
         it->second->draw(cam.m);
      }
   }
}

void Space::stars(int a, int b, int c) {
   float x = (float) rand() / (float) RAND_MAX * 10000.0;
   float y = (float) rand() / (float) RAND_MAX * 10000.0;
   float z = (float) rand() / (float) RAND_MAX * 10000.0;
   
   float r = (float) rand() / (float) RAND_MAX;
   glColor3f(r - 0.2, r - 0.2, r);
   
   float s = (float) rand() / (float) RAND_MAX * 2.0;
   glPointSize(s);
   
   Vector3D v(x * a, y * b, z * c);
   v.normalize();
   v *= OptionsManager::instance().getOptions()->getDouble("far") - 10000;
   glVertex3f(v.x, v.y, v.z);
}

void Space::initializeBackground() {
   starsId = glGenLists(1);
   glNewList(starsId, GL_COMPILE);

   srand(0);

   glDisable(GL_LIGHTING);   
   glBegin(GL_POINTS);
   
   for(int i = 0; i < 10000; i++) {
      stars(1, 1, 1);
      stars(1, 1, -1);
      stars(1, -1, 1);
      stars(1, -1, -1);
      stars(-1, 1, 1);
      stars(-1, 1, -1);
      stars(-1, -1, 1);
      stars(-1, -1, -1);
   }

   glEnd();
   glEnable(GL_LIGHTING);
   
   glEndList();
}

void Space::gotoSun() {
   cam.set(Point3D(0, 0, 700*7.0), Point3D(0, 0, 0), Vector3D(0, 1, 0));
   cam.loadGLMatrix();
}

void Space::gotoPlanet(int pidx) {
   Drawable* d;
   if(pidx == 0) {
      d = DrawableManager::instance().getDrawable("Mercury");
   }
   else if(pidx == 1) {
      d = DrawableManager::instance().getDrawable("Venus");
   }
   else if(pidx == 2) {
      d = DrawableManager::instance().getDrawable("Earth");
   }
   else if(pidx == 3) {
      d = DrawableManager::instance().getDrawable("Mars");
   }
   else if(pidx == 4) {
      d = DrawableManager::instance().getDrawable("Jupiter");
   }
   else if(pidx == 5) {
      d = DrawableManager::instance().getDrawable("Saturn");
   }
   else if(pidx == 6) {
      d = DrawableManager::instance().getDrawable("Uranus");
   }
   else if(pidx == 7) {
      d = DrawableManager::instance().getDrawable("Neptune");
   }
   else if(pidx == 8) {
      d = DrawableManager::instance().getDrawable("Pluto");
   }
   else {
      return;
   }

   Point3D loc = d->getLocation();
   loc.x += d->getRadius() * 2.0;
   cam.set(loc, d->getLocation(), Vector3D(0, 1, 0));
   cam.loadGLMatrix();
}

void Space::handleKeydown(SDL_KeyboardEvent &event) {
   switch(event.keysym.sym) {
      case 'w' :
         if(event.keysym.mod & KMOD_LCTRL) {
            delV = slideVel;
         } else {
            delN = -slideVel;
         }
         break;
      case 's' :
         if(event.keysym.mod & KMOD_LCTRL) {
            delV = -slideVel;
         } else {
            delN = slideVel;
         }
         break;
      case 'a' :
         if(event.keysym.mod & KMOD_LCTRL) {
            yaw = -rollVel;
         } else {
            delU = -slideVel;
         }
         break;
      case 'd' :
         if(event.keysym.mod & KMOD_LCTRL) {
            yaw = rollVel;
         } else {
            delU = slideVel;
         }
         break;
      case SDLK_LEFT:
         roll = rollVel;
         break;
      case SDLK_RIGHT:
         roll = -rollVel;
         break;
      case SDLK_UP:
         pitch = -rollVel;
         break;
      case SDLK_DOWN:
         pitch = rollVel;
         break;
      default: break;
   }
}

void Space::handleKeyup(SDL_KeyboardEvent &event) {
   SDLMod mod = SDL_GetModState();
   
   if(event.keysym.sym >= 49 && event.keysym.sym <= 57) {
      if(mod & KMOD_LCTRL) {
         gotoPlanet(event.keysym.sym - 49);
      }
      else {
         slideVel = pow(2.0, (event.keysym.sym - 48)) / 100.0;
         rollVel = pow(2.3, (event.keysym.sym - 48)) / 100.0;
      }
   }
   else {
      switch(event.keysym.sym) {
         case '0':
            slideVel = pow(2.0, 10) / 10.0;
            break;
         case 'w' :
         case 's' :
            if(mod & KMOD_LSHIFT) {
               gotoSun();
            }
            delV = delN = 0;
            break;
         case 'a' :
         case 'd' :
            yaw = delU = 0;
            break;
         case SDLK_LEFT:
         case SDLK_RIGHT:
            roll = 0;
            break;
         case SDLK_UP:
         case SDLK_DOWN:
            pitch = 0;
            break;
         default: break;
      }
   }
}
