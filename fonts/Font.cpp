#include "Font.h"

#ifdef WIN32
#include <GL/gl.h>
#else
#include <GLUT/glut.h>
#endif

#include <math.h>

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
const static Uint32 rmask = 0xff000000;
const static Uint32 gmask = 0x00ff0000;
const static Uint32 bmask = 0x0000ff00;
const static Uint32 amask = 0x000000ff;
#else
const static Uint32 rmask = 0x000000ff;
const static Uint32 gmask = 0x0000ff00;
const static Uint32 bmask = 0x00ff0000;
const static Uint32 amask = 0xff000000;
#endif

Font::Font(const char * name, int size) {
   font = TTF_OpenFont(name, size);
   if (!font) {
      printf("TTF_OpenFont: %s\n", TTF_GetError());
   }
   setColor(255, 255, 255);
}

Font::~Font() {
   TTF_CloseFont(font);
}

void Font::setColor(Uint8 red, Uint8 green, Uint8 blue) {
   color.r = red;
   color.g = green;
   color.b = blue;
}

void Font::setColor(const SDL_Color &c) {
   color.r = c.r;
   color.g = c.g;
   color.b = c.b;
}

void Font::drawText(const char *text, int locX, int locY) {
   GLint vPort[4];
   glGetIntegerv(GL_VIEWPORT, vPort);

   glMatrixMode(GL_PROJECTION);
   glPushMatrix();
   glLoadIdentity();
   glOrtho(0, vPort[2], 0, vPort[3], -1, 1);

   glMatrixMode(GL_MODELVIEW);
   glPushMatrix();
   glLoadIdentity();

   glEnable(GL_BLEND);
   glBlendFunc(GL_ONE, GL_ONE);
   
   SDL_GL_RenderText(text, locX, locY);
   
   glDisable(GL_BLEND);

   glMatrixMode(GL_PROJECTION);
   glPopMatrix();
   glMatrixMode(GL_MODELVIEW);
   glPopMatrix();
}

int nextpoweroftwo(int x) {
   double logbase2 = log(x) / log(2);
   return (int) round(pow(2, ceil(logbase2)));
}

void Font::SDL_GL_RenderText(const char *text, int locX, int locY) {
   GLuint texture;

   /* Use SDL_TTF to render our text */
   glColor3f(color.r, color.g, color.b);
   SDL_Surface *initial = TTF_RenderText_Blended(font, text, color);

   /* Convert the rendered text to a known format */
   int w = initial->w; // nextpoweroftwo(initial->w);
   int h = initial->h; // nextpoweroftwo(initial->h);

   SDL_Surface *intermediary = SDL_CreateRGBSurface(0, w, h, 32, 
         rmask, gmask, bmask, amask);

   SDL_BlitSurface(initial, 0, intermediary, 0);

   /* Tell GL about our new texture */
   glGenTextures(1, &texture);
   glBindTexture(GL_TEXTURE_2D, texture);
   glTexImage2D(GL_TEXTURE_2D, 0, 4, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, intermediary->pixels);

   /* GL_NEAREST looks horrible, if scaled... */
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

   /* prepare to render our texture */
   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D, texture);

   /* Draw a quad at location */
   glBegin(GL_QUADS);
   glTexCoord2f(0.0f, 1.0f);  glVertex2f(locX, locY);
   glTexCoord2f(1.0f, 1.0f);  glVertex2f(locX + w, locY);
   glTexCoord2f(1.0f, 0.0f);  glVertex2f(locX + w, locY + h);
   glTexCoord2f(0.0f, 0.0f);  glVertex2f(locX, locY + h);
   glEnd();

   /* Bad things happen if we delete the texture before it finishes */
   glFinish();

   /* Clean up */
   SDL_FreeSurface(initial);
   SDL_FreeSurface(intermediary);
   glDeleteTextures(1, &texture);
}
