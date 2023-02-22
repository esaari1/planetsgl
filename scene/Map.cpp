/**
 *  Map.cpp
 */

#include "utility/ShaderManager.h"
#include "Map.h"

#ifdef WIN32
#include <GL/gl.h>
#else
#include <GLUT/glut.h>
#endif

#include "drawable/DrawableManager.h"
#include "drawable/Drawable.h"
#include "mesh/MeshManager.h"
#include "physics/time_funcs.h"

const GLfloat lightPosition[] = {0, 0, 0, 1};
const GLfloat ambient_light[] = {0.1, 0.1, 0.1, 1.0};

void axis() {
   glDisable(GL_LIGHTING);
   
   glBegin(GL_LINES);
   glColor3f(1, 0, 0);
   glVertex3i(0, 0, 0);
   glVertex3i(100, 0, 0);

   glColor3f(0, 1, 0);
   glVertex3i(0, 0, 0);
   glVertex3i(0, 100, 0);
   
   glColor3f(0, 0, 1);
   glVertex3i(0, 0, 0);
   glVertex3i(0, 0, 100);
   glEnd();
   
   glEnable(GL_LIGHTING);
}

Map::Map() : eye(0, 0, 100000), center(0, 0, 0), pitch(90), roll(0), pitchVel(0), rollVel(0), zoomMult(0)
{
//   for(DrawableIter it = DrawableManager::instance().begin(); it != DrawableManager::instance().end(); it++) {
//      it->second->createPath();
//   }
}

Map::~Map() {
}

void Map::setup() {
   ShaderManager::instance().setActiveShader("none");
   glActiveTexture(GL_TEXTURE0);
   glDisable(GL_TEXTURE_2D);
   
   glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light);
   glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_light);
}

void Map::draw() {
   ShaderManager::instance().setActiveShader("none");

   pitch += pitchVel;
   roll += rollVel;
   double newZ = eye.z + (eye.z * zoomMult / 200.0);
   eye.z = max(newZ, 0.1);
   
   glColor3f(1, 1, 1);
   
   glLoadIdentity();
   gluLookAt(eye.x, eye.y, eye.z, 0, 0, 0, 0, 1, 0);
   glRotated(pitch, 1, 0, 0);
   glRotated(roll, 0, 1, 0);
   glTranslated(-center.x, -center.y, -center.z);

   double scale = 10.0; // eye.z / 100.0;
//   double days = daysSinceEpoch();
   
   glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
//   glEnable(GL_RESCALE_NORMAL);

   for(DrawableIter it = DrawableManager::instance().begin(); it != DrawableManager::instance().end(); it++) {
      glPushMatrix();
      glScaled(0.01, 0.01, 0.01);
      glDisable(GL_LIGHTING);
      it->second->drawPath();
      glEnable(GL_LIGHTING);
      glPopMatrix();
      
//      it->second->calculateLocation(days);
      Point3D loc = it->second->getLocation();
      loc *= 0.01;
      
      glPushMatrix();
      glTranslated(loc.x, loc.y, loc.z);
      glScaled(scale, scale, scale);
      it->second->drawBasic();
      glPopMatrix();
   }

   axis();
}

void Map::handleKeydown(SDL_KeyboardEvent &event) {
   switch(event.keysym.sym) {
      case SDLK_DOWN:
         pitchVel = 0.1;
         break;
      case SDLK_UP:
         pitchVel = -0.1;
         break;
      case SDLK_LEFT:
         rollVel = -0.1;
         break;
      case SDLK_RIGHT:
         rollVel = 0.1;
         break;
      case '-':
         zoomMult = 1;
         break;
      case '=':
         zoomMult = -1;
         break;
      default: break;
   }
}

void Map::handleKeyup(SDL_KeyboardEvent &event) {
   if(event.keysym.sym >= SDLK_0 && event.keysym.sym <= SDLK_9) {
      gotoPlanet(event.keysym.sym);
   }
   switch(event.keysym.sym) {
      case SDLK_DOWN:
      case SDLK_UP:
         pitchVel = 0;
         break;
      case SDLK_LEFT:
      case SDLK_RIGHT:
         rollVel = 0;
         break;
      case '-':
      case '=':
         zoomMult = 0;
         break;
      default: break;
   }
}

void Map::gotoPlanet(int pidx) {
   Drawable* d;
   if(pidx == SDLK_0) {
      d = DrawableManager::instance().getDrawable("Sun");
   }
   else if(pidx == SDLK_1) {
      d = DrawableManager::instance().getDrawable("Mercury");
   }
   else if(pidx == SDLK_2) {
      d = DrawableManager::instance().getDrawable("Venus");
   }
   else if(pidx == SDLK_3) {
      d = DrawableManager::instance().getDrawable("Earth");
   }
   else if(pidx == SDLK_4) {
      d = DrawableManager::instance().getDrawable("Mars");
   }
   else if(pidx == SDLK_5) {
      d = DrawableManager::instance().getDrawable("Jupiter");
   }
   else if(pidx == SDLK_6) {
      d = DrawableManager::instance().getDrawable("Saturn");
   }
   else if(pidx == SDLK_7) {
      d = DrawableManager::instance().getDrawable("Uranus");
   }
   else if(pidx == SDLK_8) {
      d = DrawableManager::instance().getDrawable("Neptune");
   }
   else if(pidx == SDLK_9) {
      d = DrawableManager::instance().getDrawable("Pluto");
   }
   else {
      return;
   }
   
   center = d->getLocation() * 0.01;
   eye.z = d->getRadius() * 100.0;
}
