#include "GeoSphere.h"
#include <math.h>
#include "geometry/Point3D.h"
#include "geometry/Vector3D.h"
#include "utility/ShaderManager.h"

const double GOLDEN = (1.0 + sqrt(5.0)) / 2.0;

const double DEG_TO_RAD = M_PI / 180.0;

typedef map<Edge, vector<int>* >::const_iterator EdgeMapIter;

GeoSphere::GeoSphere(int _divs) : divs(_divs) {
   TriangleMesh* mesh = new TriangleMesh();
   addTriangleMesh(mesh);
   
   // ALl points are rotated so that (0, GOLDEN, 1) is the north pole and is rotated to (0, 1, 0)
   // 90 degrees = height of a triangle + half the length of a triangle side
   // side = 2.0
   // traingle height = side * sin(60)
   double ninety_degrees = 2.0 * sin(60 * DEG_TO_RAD) + 1.0;
   double rotate = 1.0 / ninety_degrees * -90.0 * DEG_TO_RAD;
   
   // rotate_y = y * cos(rotate) - z * sin(rotate)
   // rotate_z = y * sin(rotate) + z * cos(rotate)
   // cache cos(rotate) and sin(rotate)
   double c_rotate = cos(rotate);
   double s_rotate = sin(rotate);
   
   // Define the north pole
   addPoint(mesh, new Vector3D(0, GOLDEN * c_rotate - s_rotate, GOLDEN * s_rotate + c_rotate), false); // (0, GOLDEN, 1)
   tangents.push_back(new Vector3D(1, 0, 0));
   binormals.push_back(new Vector3D(0, 0, -1));
   
   // Row 2
   addPoint(mesh, new Vector3D(-1, -GOLDEN * s_rotate, GOLDEN * c_rotate)); // (-1, 0, GOLDEN)
   addPoint(mesh, new Vector3D(1, -GOLDEN * s_rotate, GOLDEN * c_rotate)); // (1, 0, GOLDEN)
   addPoint(mesh, new Vector3D(GOLDEN, c_rotate, s_rotate)); // (GOLDEN, 1, 0)
   addPoint(mesh, new Vector3D(0, GOLDEN * c_rotate + s_rotate, GOLDEN * s_rotate - c_rotate)); // (0, GOLDEN, -1)
   addPoint(mesh, new Vector3D(-GOLDEN, c_rotate, s_rotate)); // (-GOLDEN, 1, 0)
   
   // Row 3
   addPoint(mesh, new Vector3D(-GOLDEN, -c_rotate, -s_rotate)); // (-GOLDEN, -1, 0)
   addPoint(mesh, new Vector3D(0, -GOLDEN * c_rotate - s_rotate, -GOLDEN * s_rotate + c_rotate)); // (0, -GOLDEN, 1)
   addPoint(mesh, new Vector3D(GOLDEN, -c_rotate, -s_rotate)); // (GOLDEN, -1, 0)
   addPoint(mesh, new Vector3D(1, GOLDEN * s_rotate, -GOLDEN * c_rotate)); // (1, 0, -GOLDEN)
   addPoint(mesh, new Vector3D(-1, GOLDEN * s_rotate, -GOLDEN * c_rotate)); // (-1. 0. -GOLDEN)
   
   // Define the south pole
   addPoint(mesh, new Vector3D(0, -GOLDEN * c_rotate + s_rotate, -GOLDEN * s_rotate - c_rotate), false); // (0, -GOLDEN, -1)
   tangents.push_back(new Vector3D(-1, 0, 0));
   binormals.push_back(new Vector3D(0, 0, 1));
   
   // North pole faces   
   subdivide(mesh, 0, 1, 2);
   subdivide(mesh, 0, 2, 3);
   subdivide(mesh, 0, 3, 4);
   subdivide(mesh, 0, 4, 5);
   subdivide(mesh, 0, 5, 1);

   // Mid row faces
   subdivide(mesh, 1, 6, 7);
   subdivide(mesh, 7, 2, 1);
   subdivide(mesh, 2, 7, 8);
   subdivide(mesh, 8, 3, 2);
   subdivide(mesh, 3, 8, 9);
   subdivide(mesh, 9, 4, 3);
   subdivide(mesh, 4, 9, 10);
   subdivide(mesh, 10, 5, 4);
   subdivide(mesh, 5, 10, 6);
   subdivide(mesh, 6, 1, 5);

   // South pole faces
   subdivide(mesh, 6, 11, 7);
   subdivide(mesh, 7, 11, 8);
   subdivide(mesh, 8, 11, 9);
   subdivide(mesh, 9, 11, 10);
   subdivide(mesh, 10, 11, 6);

//   printf("face count = %d\n", mesh->getFaceCount());
}

GeoSphere::~GeoSphere() {
   for(unsigned int i = 0; i < tangents.size(); i++) {
      delete tangents[i];
   }
   tangents.clear();
   
   for(unsigned int i = 0; i < binormals.size(); i++) {
      delete binormals[i];
   }
   binormals.clear();
}

void GeoSphere::addPoint(TriangleMesh* mesh, Vector3D* norm, bool addTangents) {
   norm->normalize();
   mesh->addPoint(new Point3D(norm->x, norm->y, norm->z));
   mesh->addNormal(norm);
   
   if(addTangents) {
      Vector3D *t = new Vector3D(norm->z, 0, -norm->x);
      tangents.push_back(t);
      binormals.push_back(new Vector3D(norm->cross(*t)));
   }
}

void GeoSphere::subdivide(TriangleMesh* mesh, int pidx1, int pidx2, int pidx3) {
   vector<int>* side12 = getEdgePoints(mesh, Edge(pidx1, pidx2));
   vector<int>* side13 = getEdgePoints(mesh, Edge(pidx1, pidx3));
   vector<int>* side23 = getEdgePoints(mesh, Edge(pidx2, pidx3));
   int** span = new int*[divs+1];
   for(int i = 0; i < divs+1; i++) {
      span[i] = new int[divs];
   }
   
   int pidx = mesh->getPointCount();

   for(int i = 1; i < (divs - 1); i++) {
      Point3D* p1 = mesh->getPointAt(side12->at(divs - i));
      Point3D* p2 = mesh->getPointAt(side13->at(divs - i));

      for(int j = 1; j < (divs - i); j++) {
         double d = (double)j / (double)(divs - i);
         double x = p1->x + d * (p2->x - p1->x);
         double y = p1->y + d * (p2->y - p1->y);
         double z = p1->z + d * (p2->z - p1->z);

         addPoint(mesh, new Vector3D(x, y, z));
         span[i][j] = pidx++;
      }
   }
   
   // Top face
   Face* f = new Face(pidx1, side12->at(1), side13->at(1));
   calculateTexCoords(mesh, f);
   mesh->addFace(f);
   
   if(divs == 2) {
      f = new Face(side12->at(1), side12->at(2), side23->at(1));
      mesh->addFace(f);
   
      f = new Face(side12->at(1), side23->at(1), side13->at(1));
      mesh->addFace(f);
   
      f = new Face(side13->at(1), side23->at(1), side13->at(2));
      mesh->addFace(f);
      return;
   }
   
   // Row two faces
   f = new Face(side12->at(1), side12->at(2), span[divs-2][1]);
   calculateTexCoords(mesh, f);
   mesh->addFace(f);
   
   f = new Face(side12->at(1), span[divs-2][1], side13->at(1));
   calculateTexCoords(mesh, f);
   mesh->addFace(f);
   
   f = new Face(side13->at(1), span[divs-2][1], side13->at(2));
   calculateTexCoords(mesh, f);
   mesh->addFace(f);

   // Center faces
   for(int i = 2; i < divs-1; i++) {      
      f = new Face(side12->at(i), side12->at(i+1), span[divs-i-1][1]);
      calculateTexCoords(mesh, f);
      mesh->addFace(f);
      
      f = new Face(side12->at(i), span[divs-i-1][1], span[divs-i][1]);
      calculateTexCoords(mesh, f);
      mesh->addFace(f);
      
      for(int j = 1; j < i; j++) {
         f = new Face(span[divs-i][j], span[divs-i-1][j], span[divs-i-1][j+1]);
         calculateTexCoords(mesh, f);
         mesh->addFace(f);

         if(j < i-1) {
            f = new Face(span[divs-i][j], span[divs-i-1][j+1], span[divs-i][j+1]);
            calculateTexCoords(mesh, f);
            mesh->addFace(f);
         }
      }

      f = new Face(side13->at(i), span[divs-i][i-1], span[divs-i-1][i]);
      calculateTexCoords(mesh, f);
      mesh->addFace(f);
      
      f = new Face(side13->at(i), span[divs-i-1][i], side13->at(i+1));
      calculateTexCoords(mesh, f);
      mesh->addFace(f);
   }
   
   // Bottom row faces
   f = new Face(side12->at(divs-1), pidx2, side23->at(1));
   calculateTexCoords(mesh, f);
   mesh->addFace(f);

   f = new Face(side12->at(divs-1), side23->at(1), span[1][1]);
   calculateTexCoords(mesh, f);
   mesh->addFace(f);
   
   for(int j = 2; j < divs; j++) {
      f = new Face(span[1][j-1], side23->at(j-1), side23->at(j));
      calculateTexCoords(mesh, f);
      mesh->addFace(f);
      
      if(j < divs-1) {
         f = new Face(span[1][j-1], side23->at(j), span[1][j]);         
         calculateTexCoords(mesh, f);
         mesh->addFace(f);
      }
   }
   
   f = new Face(span[1][divs-2], side23->at(divs-1), side13->at(divs-1));
   calculateTexCoords(mesh, f);
   mesh->addFace(f);
   
   f = new Face(side13->at(divs-1), side23->at(divs-1), pidx3);
   calculateTexCoords(mesh, f);
   mesh->addFace(f);
}

vector<int>* GeoSphere::getEdgePoints(TriangleMesh* mesh, const Edge& e) {
   int pidx = mesh->getPointCount();

   Point3D* p1 = mesh->getPointAt(e.pidx1);
   Point3D* p2 = mesh->getPointAt(e.pidx2);

   EdgeMapIter iter = edgeMap.find(e);
   vector<int>* points;

   if(iter == edgeMap.end()) {
      points = new vector<int>();
      points->push_back(e.pidx1);

      for(int i = 1; i < divs; i++) {
         double d = (double)i / (double)divs;
         double x = p1->x + d * (p2->x - p1->x);
         double y = p1->y + d * (p2->y - p1->y);
         double z = p1->z + d * (p2->z - p1->z);
         
         addPoint(mesh, new Vector3D(x, y, z));
         points->push_back(pidx++);
      }
      
      points->push_back(e.pidx2);
      edgeMap[e] = points;
   }
   else {
      points = edgeMap[e];
   }
   return points;
}

void GeoSphere::calculateTexCoords(TriangleMesh* mesh, Face* f) {
   double s[3];
   double t[3];
   
   Point3D xaxis(1, 0, 0);
   
   for(int i = 0; i < 3; i++) {
      double x = mesh->getPointAt(f->getPointIndexAt(i))->x;
      double y = mesh->getPointAt(f->getPointIndexAt(i))->y;
      double z = mesh->getPointAt(f->getPointIndexAt(i))->z;
      
      s[i] = (atan2(x, z) + M_PI) / (2.0 * M_PI);
      t[i] = acos(y) / M_PI;
   }
   
   // Need to compensate for triangles that span the texture boundary.
   if(s[1] == 1.0) {
      s[1] = 0.0;
   }
   if(s[1] <= (s[0] - 0.5)) {
      s[1] += 1.0;
   }
   if(s[2] <= (s[0] - 0.5)) {
      s[2] += 1.0;
   }
   if(s[2] == 1.0) {
      if(s[0] < 0.5) {
         s[0] += 1.0;
      }
      if(s[1] < 0.5) {
         s[1] += 1.0;
      }
   }

   for(int i = 0; i < 3; i++) {
      f->addTexCoord(new TexCoord(s[i], t[i]));
   }
}

GLuint GeoSphere::createDisplayList() {
   GLuint drawId = glGenLists(1);
   glNewList(drawId, GL_COMPILE);
   
   // Begin draw routine
   TriMeshIter tmi = triBegin();
   string name = (*tmi)->getMaterialName();
   
   GLint tloc = ShaderManager::instance().getAttribLocation("tangent");
   GLint bloc = ShaderManager::instance().getAttribLocation("binormal");
   
   glBegin(GL_TRIANGLES);
   
   for(FaceIter fi = (*tmi)->faceBegin(); fi != (*tmi)->faceEnd(); fi++) {
      for(unsigned int i = 0; i < 3; i++) {
         int idx = (*fi)->getPointIndexAt(i);
         Vector3D* n = (*tmi)->getNormalAt(idx);
         glNormal3f(n->x, n->y, n->z);
         
         if((*fi)->hasTexCoord()) {
            TexCoord* t = (*fi)->getTexCoordAt(i);
            glTexCoord2d(t->s, t->t);
         }
         else if((*tmi)->hasTexCoord()) {
            TexCoord* t = (*tmi)->getTexCoordAt(idx);
            glTexCoord2d(t->s, t->t);
         }
         
         if(tloc != -1 && bloc != -1) {
            Vector3D* t = tangents[idx];
            Vector3D* b = binormals[idx];
         
            glVertexAttrib3d(tloc, t->x, t->y, t->z);
            glVertexAttrib3d(bloc, b->x, b->y, b->z);
         }
         
         Point3D* p = (*tmi)->getPointAt(idx);
         glVertex3f(p->x, p->y, p->z);
      }
   }
   
   glEnd();
   // End draw routine
   
   glEndList();
   
   return drawId;
}

Edge::Edge(int p1, int p2) : pidx1(p1), pidx2(p2)
{
}

Edge::Edge(const Edge& e) {
   pidx1 = e.pidx1;
   pidx2 = e.pidx2;
}

bool Edge::operator==(const Edge& e) const {
   if(e.pidx1 != pidx1) {
      return false;
   }
   if(e.pidx2 != pidx2) {
      return false;
   }
   return true;
}

bool Edge::operator<(const Edge& e) const {
   if(pidx1 < e.pidx1) {
      return true;
   }
   else if(pidx1 > e.pidx1) {
      return false;
   }
   
   if(pidx2 < e.pidx2) {
      return true;
   }
   return false;
}
