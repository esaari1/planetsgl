#ifndef M3DSMESH_H_
#define M3DSMESH_H_

#include "Mesh.h"

#include <iostream>
#include <fstream>
#include <string>

#include "utility/basictypes.h"
#include "geometry/Matrix.h"

using namespace std;

class M3DSMesh : public Mesh {

public:
	M3DSMesh(const string& _path);
	virtual ~M3DSMesh();
	
	bool loadModel(const string& filename);
	
private:
   int processTopLevelChunk(int nBytes);
   int processSceneChunk(int nBytes);
   int processModelChunk(int nBytes);
   int processMaterialChunk(int nBytes, Material* material);
   int processTriMeshChunk(int nBytes);
   int processFaceArrayChunk(int nBytes, TriangleMesh* mesh);
   int processColorChunk(int nBytes, Color& color);
   int processPercentageChunk(int nBytes, float& percent);
   int processTexmapChunk(int nBytes, GLuint& texID);
   
   int32 readInt();
   int16 readShort();
   uint16 readUshort();
   float readFloat();
   char readChar();
   string readString();
   void readColor(Color& color);
   void readFloatColor(Color& color);
   Matrix4x4 readMatrix();

   void readPointArray(TriangleMesh* mesh);
   void readTextureCoordArray(TriangleMesh* mesh);
   void readFaceArray(TriangleMesh* mesh, int contentSize);

   void skipBytes(int count);
   
   string path;
   ifstream in;
};

#endif /*M3DSMESH_H_*/
