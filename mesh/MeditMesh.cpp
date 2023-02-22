#include "MeditMesh.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include "geometry/Point3D.h"
#include "utility/OptionsManager.h"
#include "parser/Hash.h"

MeditMesh::MeditMesh(bool _clockwise, double _scale) : clockwise(_clockwise), scale(_scale) {
}

MeditMesh::~MeditMesh() {
}

bool MeditMesh::loadModel(const string& meditFile) {
   string meshFile = meditFile.substr(0, meditFile.length() - 5);
   meshFile += "mesh";
   
   loadMesh(meshFile);
   loadMaterials(meditFile);
   calculateNormals();
   return true;
}

bool MeditMesh::loadMesh(const string& filename) {
   ifstream io(filename.c_str(), ios::in);
   if(!io.good()) {
      return false;
   }
   
   // Skip all starting comments. Look for the vertex list header.
   string line = "";
   while(line != "Vertices") {
      io >> line;
   }

   // Read in number of points.
   int numPoints;
   io >> numPoints;
   
   // Read in all points to temprary vector.
   // Each point has format "x y z w"
   float x, y, z, w;
   vector<Point3D> points;
   for(int pidx = 0; pidx < numPoints; pidx++) {
      io >> x >> y >> z >> w;
      Point3D p(x, y, z);
      p *= scale * OptionsManager::instance().getOptions()->getDouble("scale");
      points.push_back(p);
   }
   
   // Skip shape header
   io >> line;
   readFaces(io);
   io.close();

   map<int, int> pointMap;
   for(TriMeshIter it = triBegin(); it != triEnd(); it++) {
      pointMap.clear();
      for(FaceIter fit = (*it)->faceBegin(); fit != (*it)->faceEnd(); fit++) {
         for(int pidx = 0; pidx < 3; pidx++) {
            int idx = (*fit)->getPointIndexAt(pidx) - 1;
            if(pointMap.count(idx) == 0) {
               int newIdx = (*it)->addPoint(new Point3D(points[idx]));
               pointMap[idx] = newIdx;
               (*fit)->setPointIndexAt(pidx, newIdx);
            }
            else {
               (*fit)->setPointIndexAt(pidx, pointMap[idx]);
            }
         }
      }
   }
   
   points.clear();
   return true;
}

void MeditMesh::readFaces(ifstream& io) {
   // Read in number of faces
   int numFaces;
   io >> numFaces;
   
   // Read in all faces.
   // Each face has format "pidx1 pidx2 pidx2 material_idx"
   // Create a triangle mesh for each unique material_idx and add them to the model.
   int pidx1, pidx2, pidx3, mat_idx;
   map<int, TriangleMesh*> meshes;
   for(int fidx = 0; fidx < numFaces; fidx++) {
      io >> pidx1 >> pidx2 >> pidx3 >> mat_idx;
      if(clockwise) {
         int tmp = pidx1;
         pidx1 = pidx2;
         pidx2 = tmp;
      }
      Face* face = new Face(pidx1, pidx2, pidx3);
      
      if(meshes.count(mat_idx) == 0) {
         TriangleMesh* mesh = new TriangleMesh();
         mesh->addFace(face);
         
         char mat_name[10];
         sprintf(mat_name, "%d", mat_idx);
         mesh->setMaterialName(mat_name);
         
         meshes[mat_idx] = mesh;
         addTriangleMesh(mesh);
      }
      else {
         meshes[mat_idx]->addFace(face);
      }
   }

   meshes.clear();
}

bool MeditMesh::loadMaterials(const string& filename) {
   ifstream io(filename.c_str(), ios::in);
   if(!io.good()) {
      return false;
   }
   
   // Skip to material definitions.
   char line[1024];
   while(strcmp(line, "NbMaterial") != 0) {
      io.getline(line, 1024);
   }
   
   // Read in number of materials
   int numMaterials;
   io >> numMaterials;
   
   // Read in all matrials
   for(int midx = 0; midx < numMaterials; midx++) {
      Material* material = new Material();
      
      // Use midx as material name
      char mat_name[10];
      sprintf(mat_name, "%d", midx);
      material->name = mat_name;
      
      // Skip two header lines.
      io.getline(line, 1024);
      io.getline(line, 1024);
      io.getline(line, 1024);
      
      // Read colors
      io >> material->ambient.c[0] >> material->ambient.c[1] >> material->ambient.c[2] >> material->ambient.c[3];
      io >> material->diffuse.c[0] >> material->diffuse.c[1] >> material->diffuse.c[2] >> material->diffuse.c[3];
      io >> material->specular.c[0] >> material->specular.c[1] >> material->specular.c[2] >> material->specular.c[3];
      io >> material->emission.c[0] >> material->emission.c[1] >> material->emission.c[2] >> material->emission.c[3];
      
      // Read shinniness
      io >> material->shininess;
      
      addMaterial(material);
   }
   
   io.close();
   return true;
}
