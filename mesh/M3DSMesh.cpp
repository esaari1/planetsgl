#include "M3DSMesh.h"
#include <iomanip>
#include "3dschunk.h"
#include "utility/bytes.h"
#include "utility/DevILHelper.h"
#include "geometry/Point3D.h"

M3DSMesh::M3DSMesh(const string& _path) : path(_path), in()
{
}

M3DSMesh::~M3DSMesh()
{
}

bool M3DSMesh::loadModel(const string& filename) {
   string fname = path + filename;

   in.open(fname.c_str(), ios::in | ios::binary);
   if (!in.good()) {
      fprintf(stderr, "Read3DSFile: Error opening %s\n", filename.c_str());
      return false;
   }
   else {
      uint16 chunkType = readUshort();
      if (chunkType != M3DCHUNK_MAGIC) {
         fprintf(stderr, "Read3DSFile: Wrong magic number in header\n");
         return false;
      }

      int32 chunkSize = readInt();
      if (in.bad()) {
         fprintf(stderr, "Read3DSFile: Error reading 3DS file.\n");
         return false;
      }

      int contentSize = chunkSize - 6;
      processTopLevelChunk(contentSize);

      in.close();
      calculateNormals();

      return true;
   }
}

int M3DSMesh::processTopLevelChunk(int nBytes) {
   int bytesRead = 0;

   while (bytesRead < nBytes) {
      uint16 chunkType = readUshort();
      int32 chunkSize = readInt();
      int contentSize = chunkSize - 6;
      bytesRead += chunkSize;

      if (chunkType == M3DCHUNK_MESHDATA) {
         processSceneChunk(contentSize);
      }
      else {
         skipBytes(contentSize);
      }
   }

   if (bytesRead != nBytes) {
      cerr << "In processTopLevelChunk expected " << nBytes << " bytes but read " << bytesRead << '\n';
   }

   return bytesRead;
}

int M3DSMesh::processSceneChunk(int nBytes) {
   int bytesRead = 0;

   while (bytesRead < nBytes) {
      uint16 chunkType = readUshort();
      int32 chunkSize = readInt();
      int contentSize = chunkSize - 6;
      bytesRead += chunkSize;

      if (chunkType == M3DCHUNK_NAMED_OBJECT) {
         string name = readString();
         processModelChunk(contentSize - (name.length() + 1));
      }
      else if (chunkType == M3DCHUNK_MATERIAL_ENTRY) {
         Material* material = new Material();
         processMaterialChunk(contentSize, material);
         addMaterial(material);
      }
      else if (chunkType == M3DCHUNK_BACKGROUND_COLOR) {
         // Don't use background color so read it in and then discard it
         Color color;
         processColorChunk(contentSize, color);
      }
      else {
         skipBytes(contentSize);
      }
   }

   if (bytesRead != nBytes) {
      cerr << "In processSceneChunk expected " << nBytes << " bytes but read " << bytesRead << '\n';
   }

   return bytesRead;
}

int M3DSMesh::processModelChunk(int nBytes) {
   int bytesRead = 0;

   while(bytesRead < nBytes) {
      uint16 chunkType = readUshort();
      int32 chunkSize = readInt();
      int contentSize = chunkSize - 6;
      bytesRead += chunkSize;

      if (chunkType == M3DCHUNK_TRIANGLE_MESH) {
         processTriMeshChunk(contentSize);
      }
      else {
         skipBytes(contentSize);
      }
   }

   if(bytesRead != nBytes) {
      cerr << "In processModelChunk expected " << nBytes << " bytes but read " << bytesRead << '\n';
   }

   return bytesRead;
}

int M3DSMesh::processMaterialChunk(int nBytes, Material* material) {
   int bytesRead = 0;

   while(bytesRead < nBytes) {
      uint16 chunkType = readUshort();
      int32 chunkSize = readInt();
      int contentSize = chunkSize - 6;
      bytesRead += chunkSize;

      if (chunkType == M3DCHUNK_MATERIAL_NAME) {
         material->name = readString();
      }
      else if (chunkType == M3DCHUNK_MATERIAL_AMBIENT) {
         processColorChunk(contentSize, material->ambient);
      }
      else if (chunkType == M3DCHUNK_MATERIAL_DIFFUSE) {
         processColorChunk(contentSize, material->diffuse);
      }
      else if (chunkType == M3DCHUNK_MATERIAL_SPECULAR) {
         processColorChunk(contentSize, material->specular);
      }
      else if (chunkType == M3DCHUNK_MATERIAL_SHININESS) {
         processPercentageChunk(contentSize, material->shininess);
      }
      else if (chunkType == M3DCHUNK_MATERIAL_TRANSPARENCY) {
         processPercentageChunk(contentSize, material->opacity);
      }
      else if (chunkType == M3DCHUNK_MATERIAL_TEXMAP) {
         processTexmapChunk(contentSize, material->texID);
      }
      else {
         skipBytes(contentSize);
      }
   }

   if(bytesRead != nBytes) {
      cerr << "In processMaterialChunk expected " << nBytes << " bytes but read " << bytesRead << '\n';
   }

   return bytesRead;   
}

int M3DSMesh::processTriMeshChunk(int nBytes) {
   int bytesRead = 0;
   TriangleMesh* mesh = new TriangleMesh();
   addTriangleMesh(mesh);

   while(bytesRead < nBytes) {
      uint16 chunkType = readUshort();
      int32 chunkSize = readInt();
      int contentSize = chunkSize - 6;
      bytesRead += chunkSize;

      if (chunkType == M3DCHUNK_POINT_ARRAY) {
         readPointArray(mesh);
      }
      else if (chunkType == M3DCHUNK_MESH_TEXTURE_COORDS) {
         readTextureCoordArray(mesh);
      }
      else if (chunkType == M3DCHUNK_FACE_ARRAY) {
         readFaceArray(mesh, contentSize);
      }
      else if (chunkType == M3DCHUNK_MESH_MATRIX) {
         readMatrix();
      }
      else {
         skipBytes(contentSize);
      }
   }

   if(bytesRead != nBytes) {
      cerr << "In processTriMeshChunk expected " << nBytes << " bytes but read " << bytesRead << '\n';
   }

   return bytesRead;
}

int M3DSMesh::processFaceArrayChunk(int nBytes, TriangleMesh* mesh) {
   int bytesRead = 0;

   while(bytesRead < nBytes) {
      uint16 chunkType = readUshort();
      int32 chunkSize = readInt();
      int contentSize = chunkSize - 6;
      bytesRead += chunkSize;

      if (chunkType == M3DCHUNK_MESH_MATERIAL_GROUP) {
         // For now, we just assume that there is only one material group
         // per triangle mesh, and that the material applies to all faces in
         // the mesh.
         string materialName = readString();
         uint16 nFaces = readUshort();

         for (uint16 i = 0; i < nFaces; i++) {
            readUshort();
         }

         mesh->setMaterialName(materialName);
      }
      else if (chunkType == M3DCHUNK_MESH_SMOOTH_GROUP) {
         uint16 nFaces = mesh->getFaceCount();

         for (uint16 i = 0; i < nFaces; i++) {
            /* uint32 groups = (uint32) */ readInt();
//            triMesh->addSmoothingGroups(groups);
         }
      }
      else {
         skipBytes(contentSize);
      }
   }
   
   if(bytesRead != nBytes) {
      cerr << "In processFaceArrayChunk expected " << nBytes << " bytes but read " << bytesRead << '\n';
   }
   
   return bytesRead;
}

int M3DSMesh::processColorChunk(int nBytes, Color& color) {
   int bytesRead = 0;
   while (bytesRead < nBytes) {
      uint16 chunkType = readUshort();
      int32 chunkSize = readInt();
      int contentSize = chunkSize - 6;
      bytesRead += chunkSize;
   
      if (chunkType == M3DCHUNK_COLOR_24) {
         readColor(color);
      }
      else if (chunkType == (M3DCHUNK_COLOR_FLOAT)) {
         readFloatColor(color);
      }
      else {
         skipBytes(contentSize);
      }
   }
   
   if(bytesRead != nBytes) {
      cerr << "In processColorChunk expected " << nBytes << " bytes but read " << bytesRead << '\n';
   }
   
   return bytesRead;
}

int M3DSMesh::processPercentageChunk(int nBytes, float& percent) {
   int bytesRead = 0;
   while (bytesRead < nBytes) {
      uint16 chunkType = readUshort();
      int32 chunkSize = readInt();
      int contentSize = chunkSize - 6;
      bytesRead += chunkSize;

      if (chunkType == M3DCHUNK_INT_PERCENTAGE) {
         percent = readShort();
      }
      else if (chunkType == M3DCHUNK_FLOAT_PERCENTAGE) {
         percent = readFloat();
      }
      else {
         skipBytes(contentSize);
      }
   }
   
   if(bytesRead != nBytes) {
      cerr << "In processPercentageChunk expected " << nBytes << " bytes but read " << bytesRead << '\n';
   }
   
   return bytesRead;
}

int M3DSMesh::processTexmapChunk(int nBytes, GLuint& texID) {
   int bytesRead = 0;
   while (bytesRead < nBytes) {
      uint16 chunkType = readUshort();
      int32 chunkSize = readInt();
      int contentSize = chunkSize - 6;
      bytesRead += chunkSize;

      if (chunkType == M3DCHUNK_MATERIAL_MAPNAME) {
         string texmap = path + readString();
         texID = DevILHelper::instance().loadImage(texmap.c_str());
      }
      else {
         skipBytes(contentSize);
      }
   }
   
   if(bytesRead != nBytes) {
      cerr << "In processPercentageChunk expected " << nBytes << " bytes but read " << bytesRead << '\n';
   }
   
   return bytesRead;
}

int32 M3DSMesh::readInt() {
   int32 ret;
   in.read((char *) &ret, sizeof(int32));
   LE_TO_CPU_INT32(ret, ret);
   return ret;
}

int16 M3DSMesh::readShort() {
   int16 ret;
   in.read((char *) &ret, sizeof(int16));
   LE_TO_CPU_INT16(ret, ret);
   return ret;
}

uint16 M3DSMesh::readUshort() {
   uint16 ret;
   in.read((char *) &ret, sizeof(uint16));
   LE_TO_CPU_INT16(ret, ret);
   return ret;
}

float M3DSMesh::readFloat() {
   float f;
   in.read((char*) &f, sizeof(float));
   LE_TO_CPU_FLOAT(f, f);
   return f;
}

char M3DSMesh::readChar() {
   char c;
   in.read(&c, 1);
   return c;
}

string M3DSMesh::readString() {
   char s[1024];

   for (int count = 0; count < 1024; count++) {
      in.read(s + count, 1);
      if (s[count] == '\0') {
         break;
      }
   }

   return string(s);
}

void M3DSMesh::readColor(Color& color) {
   uint8 r = (uint8) readChar();
   uint8 g = (uint8) readChar();
   uint8 b = (uint8) readChar();
   
   color.c[0] = (float) r / 255.0f;
   color.c[1] = (float) g / 255.0f;
   color.c[2] = (float) b / 255.0f;
}


void M3DSMesh::readFloatColor(Color& color) {
   color.c[0] = readFloat() / 255.0f;
   color.c[1] = readFloat() / 255.0f;
   color.c[2] = readFloat() / 255.0f;
}

Matrix4x4 M3DSMesh::readMatrix() {
    float m00 = readFloat();
    float m01 = readFloat();
    float m02 = readFloat();
    float m10 = readFloat();
    float m11 = readFloat();
    float m12 = readFloat();
    float m20 = readFloat();
    float m21 = readFloat();
    float m22 = readFloat();
    float m30 = readFloat();
    float m31 = readFloat();
    float m32 = readFloat();

    return Matrix4x4(m00, m01, m02, 0,
                 m10, m11, m12, 0,
                 m20, m21, m22, 0,
                 m30, m31, m32, 1);
}

void M3DSMesh::readPointArray(TriangleMesh* mesh) {
   uint16 nPoints = readUshort();

   for (int i = 0; i < (int) nPoints; i++) {
      float x = readFloat();
      float y = readFloat();
      float z = readFloat();
      mesh->addPoint(new Point3D(x, y, z));
   }
}

void M3DSMesh::readTextureCoordArray(TriangleMesh* mesh) {
   uint16 nPoints = readUshort();

   for (int i = 0; i < (int) nPoints; i++) {
      float s = readFloat();
      float t = readFloat();
      mesh->addTexCoord(new TexCoord(s, t));
   }
}

void M3DSMesh::readFaceArray(TriangleMesh* mesh, int contentSize) {
   uint16 nFaces = readUshort();

   for (int i = 0; i < (int) nFaces; i++) {
      uint16 v0 = readUshort();
      uint16 v1 = readUshort();
      uint16 v2 = readUshort();
      /* uint16 flags = */readUshort();
      mesh->addFace(new Face(v0, v1, v2));
   }

   int bytesLeft = contentSize - (8 * nFaces + 2);
   if (bytesLeft > 0) {
      processFaceArrayChunk(bytesLeft, mesh);
   }
}

void M3DSMesh::skipBytes(int count) {
   char c;
   while (count-- > 0) {
      in.get(c);
   }
}
