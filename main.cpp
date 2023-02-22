#ifdef WIN32
#define GLEW_STATIC 1
#include <GL/glew.h>
#endif

#include "SDLApp.h"
#include <SDL/SDL.h>
#include "utility/OptionsManager.h"
#include "utility/ShaderManager.h"
#include "parser/Hash.h"

void initSDL() {
   int w = OptionsManager::instance().getOptions()->getInteger("width");
   int h = OptionsManager::instance().getOptions()->getInteger("height");
   
   SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
   SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 8 );
   SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 8 );
   SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 8 );
   SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 8 );
   SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
   SDL_GL_SetAttribute( SDL_GL_STENCIL_SIZE, 8 );
   SDL_SetVideoMode(w, h, 0, SDL_OPENGL | SDL_HWSURFACE | SDL_DOUBLEBUF); // | SDL_FULLSCREEN);
}

int main(int argc, char **argv) {
   initSDL();

#ifdef WIN32
   GLenum err = glewInit();
   if(err != GLEW_OK) {
      fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
      exit(err);
   }
#endif

   ShaderManager::instance().initialize();
   SDLApp app;
   app.start();
   return 0;
}
