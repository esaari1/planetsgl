// DrawableManager.h
//
// Copyright (C) 2009, Eric Saari
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.

#ifndef _DRAWABLE_MANAGER_H_
#define _DRAWABLE_MANAGER_H_

#include <memory>
#include <map>
#include <string>

class Drawable;

using namespace std;

typedef map<string, Drawable*>::const_iterator DrawableIter;

class DrawableManager {
   
public:
   static DrawableManager& instance();
   ~DrawableManager();
   
   DrawableIter begin() const { return drawables.begin(); }
   DrawableIter end() const { return drawables.end(); }
   Drawable* getDrawable(string name);
   
private:
   DrawableManager();
   void loadDrawables();
   
   static auto_ptr<DrawableManager> s_instance;
   map<string, Drawable*> drawables;
};

#endif
