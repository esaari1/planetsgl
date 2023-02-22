#ifndef _MESH_H_
#define _MESH_H_

#ifdef WIN32
#include <GL/gl.h>
#else
#include <GLUT/glut.h>
#endif

#include <string>
#include <vector>
#include <map>
#include "utility/basictypes.h"

using namespace std;

class Point3D;
class Vector3D;

struct TexCoord {
   TexCoord(float _s, float _t) : s(_s), t(_t) { }
   float s, t;
};

class Color {

public:
   Color(float _r = 0, float _g = 0, float _b = 0, float _a = 1);
   operator float* () { return c; }

   float c[4];
};

class Material {
   
public:
   Material();

   string name;
   GLuint texID;
   Color ambient;
   Color diffuse;
   Color specular;
   Color emission;
   float shininess;
   float opacity;
};

class Face {
   
public:
   Face(int idx1, int idx2, int idx3);
   ~Face();

   void addTexCoord(TexCoord* t);
   bool hasTexCoord() const;
   TexCoord* getTexCoordAt(int loc) const;
   int getPointIndexAt(int loc) const;
   void setPointIndexAt(int loc, int idx);

private:
   int vertIdxs[3];
   vector<TexCoord*> texCoords;
};

typedef vector<Face*>::const_iterator FaceIter;

class TriangleMesh {
   
public:
   TriangleMesh() { }
   ~TriangleMesh();

   int addPoint(Point3D* p);
   void addNormal(Vector3D* n);
   void addTexCoord(TexCoord* t);
   void addFace(Face* f);
   void setMaterialName(const string& name);
   
   uint16 getFaceCount() const;
   FaceIter faceBegin() const;
   FaceIter faceEnd() const;
   
   Point3D* getPointAt(int loc) const;
   int getPointCount() const { return points.size(); }
   Vector3D* getNormalAt(int loc) const;
   string getMaterialName() const;
   
   bool hasTexCoord() const;
   TexCoord* getTexCoordAt(int idx) const;
   
   void calculateNormals();

private:
   vector<Point3D*> points;
   vector<Vector3D*> normals;
   vector<TexCoord*> texCoords;
   vector<Face*> faces;
   string materialName;
};

typedef vector<TriangleMesh*>::const_iterator TriMeshIter;

class Mesh {
   
public:
   Mesh() { }
   virtual ~Mesh();
   
   TriMeshIter triBegin() const;
   TriMeshIter triEnd() const;

   GLuint createDisplayList();
   
protected:
   void addTriangleMesh(TriangleMesh* mesh);
   void addMaterial(Material* mat);
   void calculateNormals();
   
private:
   vector<TriangleMesh*> triMeshes;
   map<string, Material*> materials;
};

#endif
