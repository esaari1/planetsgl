// DrawableManager.cpp
//
// Copyright (C) 2009, Eric Saari
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.

#include "DrawableManager.h"
#include "Drawable.h"
#include "Body.h"
#include "Model.h"
#include "parser/Parser.h"
#include "sun.h"
#include <fstream>

auto_ptr<DrawableManager> DrawableManager::s_instance;

DrawableManager& DrawableManager::instance() {
   if(s_instance.get() == 0) {
      s_instance.reset(new DrawableManager());
   }
   return *s_instance;
}

DrawableManager::DrawableManager() {
   loadDrawables();
   for(DrawableIter it = begin(); it != end(); it++) {
      it->second->initialize();
   }
}

DrawableManager::~DrawableManager() {
   for(DrawableIter it = begin(); it != end(); it++) {
      delete it->second;
   }
   drawables.clear();
}

Drawable* DrawableManager::getDrawable(string name) {
   return drawables[name];
}

void DrawableManager::loadDrawables() {
   std::ifstream fp("resources/planets.txt");
   Tokenizer tok(&fp);
   Parser parser(&tok);
   
   Drawable* drawable;
   
   while(tok.nextToken() != Tokenizer::TokenEnd) {
      if(tok.getTokenType() != Tokenizer::TokenName) {
         return ;
      }
      
      if(tok.getStringValue() == "Body") {
         drawable = new Body();
      }
      else if(tok.getStringValue() == "Model") {
         drawable = new Model();
      }
      else if(tok.getStringValue() == "Sun") {
         drawable = new Sun();
      }
      else {
         continue;
      }
      
      Value *value = parser.readValue();
      if(value == NULL || value->getType() != Value::HashType) {
         delete drawable;
         return ;
      }
      
      Hash* hash = value->getHash();
      drawable->setHash(hash);
      drawables[hash->getString("Name")] = drawable;
   }
   
   fp.close();
}
