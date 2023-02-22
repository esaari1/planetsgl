#ifndef DEVILHELPER_H_
#define DEVILHELPER_H_

#include <memory>

#ifdef WIN32
#include <GL/gl.h>
#else
#include <GLUT/glut.h>
#endif

using namespace std;

class DevILHelper {
   
public:
   static DevILHelper& instance();
   ~DevILHelper();
   
   GLuint loadImage(const char *filename, bool convertToRGBA = false);
   GLuint loadImageWithMask(const char *imageF, const char *maskF);
   
private:
   DevILHelper();
   void reportErrors();
   static auto_ptr<DevILHelper> s_instance;
};

#endif /*DEVILHELPER_H_*/
