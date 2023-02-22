#include "Mesh.h"
#include "geometry/Vector3D.h"
#include "geometry/Point3D.h"

typedef vector<TexCoord*>::const_iterator TexIter;
typedef map<string, Material*>::const_iterator MaterialIter;
typedef vector<Vector3D*>::const_iterator VectorIter;

// Color class
Color::Color(float _r, float _g, float _b, float _a) {
   c[0] = _r;
   c[1] = _g;
   c[2] = _b;
   c[3] = _a;
}

// Material class
Material::Material() :
   ambient(),
   diffuse(),
   specular(),
   shininess(1.0f)
{
}

// Face class
Face::Face(int idx1, int idx2, int idx3) {
   vertIdxs[0] = idx1;
   vertIdxs[1] = idx2;
   vertIdxs[2] = idx3;
}

Face::~Face() {
   for(TexIter ti = texCoords.begin(); ti != texCoords.end(); ++ti) {
      delete *ti;
   }
   texCoords.clear();
}

void Face::addTexCoord(TexCoord* t) {
   texCoords.push_back(t);
}

bool Face::hasTexCoord() const {
   return !texCoords.empty();
}

TexCoord* Face::getTexCoordAt(int loc) const {
   return texCoords[loc];
}

int Face::getPointIndexAt(int loc) const {
   return vertIdxs[loc];
}

void Face::setPointIndexAt(int loc, int idx) {
   assert(loc >= 0 && loc < 3);
   vertIdxs[loc] = idx;
}

// TriangleMesh class
TriangleMesh::~TriangleMesh() {
   for(unsigned int i = 0; i < points.size(); i++) {
      delete points[i];
   }
   points.clear();

   for(unsigned int i = 0; i < normals.size(); i++) {
      delete normals[i];
   }
   normals.clear();
}

int TriangleMesh::addPoint(Point3D* p) {
   points.push_back(p);
   return points.size() - 1;
}

void TriangleMesh::addNormal(Vector3D* n) {
   normals.push_back(n);
}

void TriangleMesh::addTexCoord(TexCoord* t) {
   texCoords.push_back(t);
}

void TriangleMesh::addFace(Face* f) {
   faces.push_back(f);
}

void TriangleMesh::setMaterialName(const string& name) {
   materialName = name;
}

uint16 TriangleMesh::getFaceCount() const {
   return faces.size();
}

FaceIter TriangleMesh::faceBegin() const {
   return faces.begin();
}

FaceIter TriangleMesh::faceEnd() const {
   return faces.end();
}

Point3D* TriangleMesh::getPointAt(int loc) const {
   return points[loc];
}

Vector3D* TriangleMesh::getNormalAt(int loc) const {
   return normals[loc];
}

string TriangleMesh::getMaterialName() const {
   return materialName;
}

bool TriangleMesh::hasTexCoord() const {
   return texCoords.size() > 0;
}

TexCoord* TriangleMesh::getTexCoordAt(int idx) const {
   return texCoords[idx];
}

void TriangleMesh::calculateNormals() {
   normals.reserve(points.size());
   for(unsigned int i = 0; i < points.size(); i++) {
      normals.push_back(new Vector3D());
   }
   
   for(FaceIter fi = faces.begin(); fi != faces.end(); fi++) {
      int idx1 = (*fi)->getPointIndexAt(0);
      int idx2 = (*fi)->getPointIndexAt(1);
      int idx3 = (*fi)->getPointIndexAt(2);
      
      Point3D* p0 = points[idx1];
      Point3D* p1 = points[idx2];
      Point3D* p2 = points[idx3];
      
      Vector3D n(*p1 - *p0);
      n = n.cross(*p2 - *p1);
      n.normalize();
      
      *normals[idx1] += n;
      *normals[idx2] += n;
      *normals[idx3] += n;
   }
   
   for(VectorIter it = normals.begin(); it != normals.end(); it++) {
      (*it)->normalize();
   }
}

// Mesh class
Mesh::~Mesh() {
   for(TriMeshIter it = triMeshes.begin(); it != triMeshes.end(); ++it) {
      delete *it;
   }
   triMeshes.clear();

   for(MaterialIter it = materials.begin(); it != materials.end(); ++it) {
      delete it->second;
   }
   materials.clear();
}

void Mesh::addTriangleMesh(TriangleMesh* mesh) {
   triMeshes.push_back(mesh);
}

void Mesh::addMaterial(Material* mat) {
   materials.insert(map<string, Material*>::value_type(mat->name, mat));
}

void Mesh::calculateNormals() {
   for(TriMeshIter tmi = triMeshes.begin(); tmi != triMeshes.end(); tmi++) {
      (*tmi)->calculateNormals();
   }
}

TriMeshIter Mesh::triBegin() const {
   return triMeshes.begin();
}

TriMeshIter Mesh::triEnd() const {
   return triMeshes.end();
}

GLuint Mesh::createDisplayList() {
   GLuint drawId = glGenLists(1);
   glNewList(drawId, GL_COMPILE);
   
   glPushAttrib(GL_LIGHTING_BIT);
   
   // Begin draw routine
   for(TriMeshIter tmi = triMeshes.begin(); tmi != triMeshes.end(); tmi++) {
      Material* mat = materials[(*tmi)->getMaterialName()];
      if(mat != NULL) {
         glMaterialfv(GL_FRONT, GL_AMBIENT, mat->ambient);
         glMaterialfv(GL_FRONT, GL_DIFFUSE, mat->diffuse);
         glMaterialfv(GL_FRONT, GL_SPECULAR, mat->specular);
         glMaterialfv(GL_FRONT, GL_EMISSION, mat->emission);
         glMaterialf(GL_FRONT, GL_SHININESS, mat->shininess);
         
         if((*tmi)->hasTexCoord()) {
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, mat->texID);
         }
      }

      glBegin(GL_TRIANGLES);

      for(FaceIter fi = (*tmi)->faceBegin(); fi != (*tmi)->faceEnd(); fi++) {
         for(unsigned int i = 0; i < 3; i++) {
            int idx = (*fi)->getPointIndexAt(i);
            Vector3D* n = (*tmi)->getNormalAt(idx);
            glNormal3f(n->x, n->y, n->z);

            if((*fi)->hasTexCoord()) {
               TexCoord* t = (*fi)->getTexCoordAt(i);
               glTexCoord2f(t->s, t->t);
            }
            else if((*tmi)->hasTexCoord()) {
               TexCoord* t = (*tmi)->getTexCoordAt(idx);
               glTexCoord2f(t->s, t->t);
            }
            
            Point3D* p = (*tmi)->getPointAt(idx);
            glVertex3f(p->x, p->y, p->z);
         }
      }
      
      if(mat != NULL && (*tmi)->hasTexCoord()) {
         glDisable(GL_TEXTURE_2D);
      }
      
      glEnd();
   }
   // End draw routine

   glPopAttrib();
   glEndList();
   
   return drawId;
}
