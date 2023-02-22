#ifndef FONT_H_
#define FONT_H_

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

class Font {

public:
   Font(const char * name, int size);
   ~Font();
   
   void drawText(const char *text, int locX, int locY);
   void setColor(Uint8 red, Uint8 green, Uint8 blue);
   void setColor(const SDL_Color &c);
   inline void getTextSize(const char *text, int *w, int *h) {
      TTF_SizeText(font, text, w, h);
   }
   
private:
   TTF_Font *font;
   SDL_Color color;
   void SDL_GL_RenderText(const char *text, int locX, int locY);
};

#endif /*FONT_H_*/
