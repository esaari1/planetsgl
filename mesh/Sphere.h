// Sphere.h
//
// Copyright (C) 2009, Eric Saari
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.

#ifndef SPHERE_H_
#define SPHERE_H_

#include "Mesh.h"

class Sphere : public Mesh {
   
public:
   Sphere(int slices, int stacks);
};

#endif /*SPHERE_H_*/
