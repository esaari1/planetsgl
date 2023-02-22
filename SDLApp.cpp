#ifdef WIN32
#define GLEW_STATIC 1
#include <GL/glew.h>
#endif

#include "SDLApp.h"
#include "utility/OptionsManager.h"
#include "parser/Hash.h"
#include <math.h>
#include "constants.h"

static int frameCnt = 0;

Uint32 timer(Uint32 interval, void *param) {
   printf("FPS = %d, %d\n", frameCnt, interval);
   frameCnt = 0;
   return interval;
}

SDLApp::SDLApp() :
   stopApp(false),
   _map(false),
   scene(new Space),
   map(new Map)
{
   int w = OptionsManager::instance().getOptions()->getInteger("width");
   int h = OptionsManager::instance().getOptions()->getInteger("height");
   double near = OptionsManager::instance().getOptions()->getDouble("near");
   
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   
   double top = near * tan(22.5 * DEG_TO_RAD);
   double bottom = -top;
   double right = top * (double)w / (double)h;
   double left = -right;
   Matrix4x4 per(2.0 * near / (right - left), 0.0, 0.0, 0.0,
                 0.0, 2.0 * near / (top-bottom), 0.0, 0.0,
                 0.0, 0.0, -1.0, -1.0,
                 0.0, 0.0, -2.0 * near, 0.0);
   glLoadMatrixd(per.Get());
   
   glMatrixMode(GL_MODELVIEW);
   
   GLfloat ambient_light[] = {0.0, 0.0, 0.0, 0.0};
   glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light);
   glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_light);
   
   GLfloat diffuse_light[] = {1.0, 1.0, 1.0, 1.0};
   glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light);
   glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_light);
   
   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   
   //Depth states
   glClearDepth(1.0f);
   glEnable(GL_DEPTH_TEST);

   glEnable(GL_CULL_FACE);
   glShadeModel(GL_SMOOTH);   
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   
   m_scene = scene;
   
   SDL_AddTimer(1000, timer, NULL);
}

SDLApp::~SDLApp() {
   SDL_Quit();
}

void SDLApp::start() {
   SDL_Event event;
   while(!stopApp) {
      while ( SDL_PollEvent(&event)) {
         switch (event.type) {
            case SDL_KEYDOWN :
               handleKeydown(event.key);
               break;
            case SDL_KEYUP :
               handleKeyup(event.key);
               break;
            case SDL_MOUSEMOTION :
//               handleMouseMotion(event.motion);
               break;
            case SDL_QUIT:
               stopApp = true;
               break;
         }
      }
      
      drawScene();
   }
}

void SDLApp::drawScene() {   
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

   m_scene->draw();
   glFlush();
   SDL_GL_SwapBuffers();
   
   frameCnt++;
}

void SDLApp::toggleMap() {
   _map = !_map;

   if(_map) {
      m_scene = map;      
   } else {
      m_scene = scene;
   }
   m_scene->setup();
}

void SDLApp::handleKeydown(SDL_KeyboardEvent &event) {
   switch(event.keysym.sym) {
      case SDLK_TAB:
         toggleMap();
         break;
      default:
         m_scene->handleKeydown(event);
         break;
   }
}

void SDLApp::handleKeyup(SDL_KeyboardEvent &event) {   
   switch(event.keysym.sym) {
      case SDLK_ESCAPE :
         stopApp = true;
         break;
      default: 
         m_scene->handleKeyup(event);
         break;
   }
}

void SDLApp::handleMouseMotion(SDL_MouseMotionEvent &event) {
}
