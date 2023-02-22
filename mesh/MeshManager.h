// MeshManager.h
//
// Copyright (C) 2009, Eric Saari
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.

#ifndef _MESH_MANAGER_H_
#define _MESH_MANAGER_H_

#include <memory>
#include <map>
#include <string>

#ifdef WIN32
#include <GL/gl.h>
#else
#include <GLUT/glut.h>
#endif

using namespace std;

class MeshManager {
   
public:
   static MeshManager& instance();
   ~MeshManager();
   GLuint getMesh(const string& name);
   
private:
   MeshManager();
   void createSpheres();
   void loadModels();
   
   static auto_ptr<MeshManager> s_instance;
   map<string, GLuint> meshes;
};

#endif
