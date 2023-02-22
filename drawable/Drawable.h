// Drawable.cpp
//
// Copyright (C) 2009, Eric Saari
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.

#ifndef _DRAWABLE_H_
#define _DRAWABLE_H_

#include "geometry/Point3D.h"
#include "geometry/Matrix.h"
#include <string>

using namespace std;

class Hash;

class Drawable {

public:
   virtual ~Drawable() {}
   
   virtual void initialize() = 0;
   virtual void draw(const Matrix4x4& m) = 0;
   virtual void drawBasic() { }
   virtual void setHash(Hash* h) = 0;
   virtual void createPath() = 0;
   virtual void drawPath() = 0;
   virtual void calculateLocation(double days) = 0;

   string getName() const { return name; }
   Point3D getLocation() const { return location; }
   double getRadius() const { return radius; }
   
protected:
   string name;
   double radius;
   Point3D location;
};

#endif
