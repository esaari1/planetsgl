// MeshManager.cpp
//
// Copyright (C) 2009, Eric Saari
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.

#include "MeshManager.h"
#include "GeoSphere.h"
#include "M3DSMesh.h"
#include "MeditMesh.h"
#include "CmodMesh.h"
#include "parser/Parser.h"

auto_ptr<MeshManager> MeshManager::s_instance;

MeshManager& MeshManager::instance() {
   if(s_instance.get() == 0) {
      s_instance.reset(new MeshManager);
   }
   return *s_instance;
}

MeshManager::MeshManager() {
   createSpheres();
   loadModels();
}

MeshManager::~MeshManager() {
}

GLuint MeshManager::getMesh(const string& name) {
   return meshes[name];
}

void MeshManager::createSpheres() {
   GeoSphere sphere15(15);
   meshes["sphere15"] = sphere15.createDisplayList();
   
   GeoSphere sphere5(10);
   meshes["sphere5"] = sphere5.createDisplayList();
}

void MeshManager::loadModels() {
   std::ifstream fp("resources/models.txt");
   Tokenizer tok(&fp);
   Parser parser(&tok);

   while(tok.nextToken() != Tokenizer::TokenEnd) {
      if(tok.getTokenType() != Tokenizer::TokenName) {
         return ;
      }
      
      string modelName = tok.getNameValue();
      
      Value *value = parser.readValue();
      if(value == NULL || value->getType() != Value::HashType) {
         return ;
      }
      
      Hash* hash = value->getHash();
      string type = hash->getValue("Type")->getString();
      string fname = hash->getValue("File")->getString();
      
      if(type == "3ds") {
         M3DSMesh mesh(hash->getString("Path"));
         mesh.loadModel(fname);
         meshes[modelName.c_str()] = mesh.createDisplayList();
      }
      else if(type == "medit") {
         bool clockwise = false;
         double scale = 1.0;

         Value* cvalue = hash->getValue("Clockwise");
         if(cvalue != NULL) {
            string val = cvalue->getString();
            if(val == "true") {
               clockwise = true;
            }
         }
         
         Value *svalue = hash->getValue("Scale");
         if(svalue != NULL) {
            scale = svalue->getDouble();
         }

         MeditMesh mesh(clockwise, scale);
         mesh.loadModel(fname.c_str());
         meshes[modelName.c_str()] = mesh.createDisplayList();
      }
      else if(type == "cmod") {
         double scale = 1.0;
         Value *svalue = hash->getValue("Scale");
         if(svalue != NULL) {
            scale = svalue->getDouble();
         }

         CmodMesh mesh(hash->getString("Path"), scale);
         mesh.loadModel(fname);
         meshes[modelName.c_str()] = mesh.createDisplayList();
      }
   }
   
   fp.close();
}
