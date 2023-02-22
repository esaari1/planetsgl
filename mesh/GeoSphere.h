#ifndef _GEOSPHERE_H_
#define _GEOSPHERE_H_

#include "Mesh.h"
#include <map>
#include <vector>

using namespace std;

class Edge {
   
public:
   Edge(int p1, int p2);
   Edge(const Edge& e);
   bool operator==(const Edge& e) const;
   bool operator<(const Edge& e) const;
   
   int pidx1, pidx2;
};


class GeoSphere : public Mesh {
   
public:
   GeoSphere(int _divs);
   ~GeoSphere();
   virtual GLuint createDisplayList();
   
private:
   void addPoint(TriangleMesh* mesh, Vector3D* norm, bool addTangents = true);
   void subdivide(TriangleMesh* mesh, int pidx1, int idxp2, int pidx3);
   vector<int>* getEdgePoints(TriangleMesh* mesh, const Edge& e);
   
   void calculateTexCoords(TriangleMesh* mesh, Face* f);
   
   int divs;
   map<Edge, vector<int>* > edgeMap;
   vector<Vector3D*> tangents;
   vector<Vector3D*> binormals;
};

#endif
