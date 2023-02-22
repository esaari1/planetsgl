#ifndef MEDIT_H_
#define MEDIT_H_

#include "Mesh.h"

class MeditMesh : public Mesh {
   
public:
   MeditMesh(bool _clockwise, double _scale);
   ~MeditMesh();
   
   bool loadModel(const string& meditFile);
   
private:
   bool loadMesh(const string& filename);
   void readFaces(ifstream& io);
   bool loadMaterials(const string& filename);
   
   bool clockwise;
   double scale;
};

#endif /*MEDIT_H_*/
