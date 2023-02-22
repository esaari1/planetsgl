// body.h
//
// Copyright (C) 2009, Eric Saari
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.

#ifndef _BODY_H
#define _BODY_H

#include "Drawable.h"
#include <string>
#include <map>

using namespace std;

class Orbit;

typedef map<GLenum, GLuint>::iterator textureIter;

class Body : public Drawable {
   
public:
   Body();
   virtual ~Body();

   virtual void initialize();
   virtual void setHash(Hash* h);
   virtual void draw(const Matrix4x4& m);
   virtual void drawBasic();
   virtual void createPath();
   virtual void drawPath();
   virtual void calculateLocation(double days);

private:
   void drawAtmosphere();
   void drawRings(const Point3D& location);

   double axisTilt;
   Hash* atmosphere;
   Hash* rings;
   string shader;
   Orbit* orbit;
   string parent;

   GLuint mapTexture;
   GLuint ringTexture;
   GLuint pathList;
   map<GLenum, GLuint> textures;
};

#endif
