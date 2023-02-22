#include "CmodMesh.h"
#include "utility/DevILHelper.h"
#include "geometry/Point3D.h"
#include "geometry/Vector3D.h"

CmodMesh::CmodMesh(const string& _path, double _scale) :path(_path), scale(_scale), io() {
}

CmodMesh::~CmodMesh() {
}

bool CmodMesh::loadModel(const string& cmodFile) {
   string fname = path + cmodFile;

   io.open(fname.c_str(), ios::in);
   if(!io.good()) {
      return false;
   }

   char data[1024];
   while(!io.eof()) {
      io.getline(data, 1024);
      if(data[0] == '#') {
         continue;
      }
      
      string line(data);
      if(line.find("material") != string::npos) {
         string matName = strip(line.substr(9));
         processMaterial(matName);
      }
      else if(line.find("mesh") != string::npos) {
         char meshName[100];
         sscanf(data, "%*s %*c %s", meshName);
         processMesh(meshName);
      }
   }
   
   io.close();
   calculateNormals();
   
   return true;
}

void CmodMesh::processMaterial(const string& name) {
   Material* mat = new Material();
   mat->name = name;

   char data[1024];
   
   io.getline(data, 1024);
   string line(data);
   
   while(line.find("end_material") == string::npos) {
      if(data[0] != '#') {
         if(line.find("diffuse") != string::npos) {
            sscanf(data, "%*s %f %f %f", &mat->diffuse.c[0], &mat->diffuse.c[1], &mat->diffuse.c[2]);
         }
         else if(line.find("specular") != string::npos) {
            sscanf(data, "%*s %f %f %f", &mat->specular.c[0], &mat->specular.c[1], &mat->specular.c[2]);
         }
         else if(line.find("emissive") != string::npos) {
            sscanf(data, "%*s %f %f %f", &mat->emission.c[0], &mat->emission.c[1], &mat->emission.c[2]);
         }
         else if(line.find("specpower") != string::npos) {
            sscanf(data, "%*s %f", &mat->shininess);
         }
         else if(line.find("opacity") != string::npos) {
            sscanf(data, "%*s %f", &mat->opacity);
         }
         else if(line.find("texture") != string::npos) {
            char texName[100];
            sscanf(data, "%*s %s", texName);
            string tn = texName;
            tn = tn.substr(1, tn.length() - 2);
            string texFile = path + tn;
            mat->texID = DevILHelper::instance().loadImage(texFile.c_str());
         }
      }

      io.getline(data, 1024);
      line = data;
   }
   
   addMaterial(mat);
}

void CmodMesh::processMesh(const string& name) {
   TriangleMesh* tri = new TriangleMesh();
   tri->setMaterialName(name);
   
   char data[1024];
   
   io.getline(data, 1024);
   string line(data);
   
   while(line.find("vertices") == string::npos) {
      io.getline(data, 1024);
      line = data;
   }
   
   int vcount;
   sscanf(line.c_str(), "%*s %d", &vcount);
   
   for(int i = 0; i < vcount; i++) {
      io.getline(data, 1024);

      float px, py, pz;
      float nx, ny, nz;
      float ts, tt;
      sscanf(data, "%f %f %f %f %f %f %f %f", &px, &py, &pz, &nx, &ny, &nz, &ts, &tt);

      Point3D* p = new Point3D(px, py, pz);
      (*p) *= scale;
      TexCoord* t = new TexCoord(ts, tt);

      tri->addPoint(p);
      tri->addTexCoord(t);
   }
   
   while(line.find("trilist") == string::npos) {
      io.getline(data, 1024);
      line = data;
   }
   
   while(line.find("end_mesh") == string::npos) {
      int p1, p2, p3;
      io.getline(data, 1024);
      line = data;
      
      sscanf(data, "%d %d %d", &p1, &p2, &p3);

      Face* face = new Face(p1, p2, p3);
      tri->addFace(face);
   }
   
   addTriangleMesh(tri);
}

string CmodMesh::strip(string s) {
   int idx = s.find_first_not_of(" \r\n");
   if(idx > 0) {
      s = s.substr(idx);
   }
   
   idx = s.find_last_not_of(" \r\n");
   if(idx > 0) {
      s = s.substr(0, idx+1);
   }
   return s;
}
