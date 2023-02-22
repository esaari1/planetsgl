#ifndef _CMOD_MESH_H_
#define _CMOD_MESH_H_

#include "Mesh.h"
#include <string>
#include <fstream>

using namespace std;

class CmodMesh : public Mesh {
   
public:
   CmodMesh(const string& _path, double _scale);
   ~CmodMesh();
   
   bool loadModel(const string& cmodFile);
   
private:
   string path;
   double scale;
   ifstream io;
   
   void processMaterial(const string& name);
   void processMesh(const string& name);
   string strip(string s);
};

#endif
