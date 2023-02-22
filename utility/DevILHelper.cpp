#include "DevILHelper.h"

typedef void ILvoid;

#include <IL/ilut.h>
#include <IL/ilu.h>

std::auto_ptr<DevILHelper> DevILHelper::s_instance;

DevILHelper& DevILHelper::instance() {
   if(s_instance.get() == 0) {
      s_instance.reset(new DevILHelper);
   }
   return *s_instance;
}

DevILHelper::DevILHelper() {
   ilInit();
   iluInit();
   ilutRenderer(ILUT_OPENGL);
   
   reportErrors();
   
   GLint  texSize;
   glGetIntegerv( GL_MAX_TEXTURE_SIZE, &texSize );
   fprintf(stderr, "GL_MAX_TEXTURE_SIZE %d\n", (int)texSize );
}

DevILHelper::~DevILHelper() {
   
}

void DevILHelper::reportErrors() {
   ILenum errorID = ilGetError();
   while(errorID != IL_NO_ERROR) {
      printf("Error %d : %s\n", errorID, iluErrorString(errorID));
      errorID = ilGetError();
   }
}

GLuint DevILHelper::loadImage(const char *filename, bool convertToRGBA) {
   ILuint id;
   ilGenImages(1, &id);
   ilBindImage(id);
   ilLoadImage(filename);

   int width = ilGetInteger(IL_IMAGE_WIDTH);
   int height = ilGetInteger(IL_IMAGE_HEIGHT);
   int format = ilGetInteger(IL_IMAGE_FORMAT);
   
   if(convertToRGBA) {
      ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
      format = ilGetInteger(IL_IMAGE_FORMAT);
      
      ILubyte *outData = ilGetData();
      ILint outBpp = ilGetInteger(IL_IMAGE_BYTES_PER_PIXEL);
      
      int numPixels = width * height;
      for(int i = 0; i < numPixels; i++) {
         outData[3] = outData[0];
         outData += outBpp;
      }
   }
   
   GLuint glid;
   glGenTextures(1, &glid);
   glBindTexture(GL_TEXTURE_2D, glid);
   glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, ilGetData());

   glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   
   iluDeleteImage(id);

   reportErrors();
   fprintf(stderr, "Image %s loaded. ID = %d\n", filename, glid);

   return glid;
}

GLuint DevILHelper::loadImageWithMask(const char *imageF, const char *maskF) {
   ILuint ids[3];
   ilGenImages(3, ids);
   ilBindImage(ids[0]);
   ilLoadImage(imageF);

   int width = ilGetInteger(IL_IMAGE_WIDTH);
   int height = ilGetInteger(IL_IMAGE_HEIGHT);
   int format = ilGetInteger(IL_IMAGE_FORMAT);
   
   ilBindImage(ids[1]);
   ilLoadImage(maskF);
   
   width = ilGetInteger(IL_IMAGE_WIDTH);
   height = ilGetInteger(IL_IMAGE_HEIGHT);
   format = ilGetInteger(IL_IMAGE_FORMAT);

   ILubyte *alphaData = ilGetData();
   ILint alphaBpp = ilGetInteger(IL_IMAGE_BYTES_PER_PIXEL);

   ilBindImage(ids[2]);
   ilCopyImage(ids[0]);
   ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

   ILubyte *outData = ilGetData();
   ILint outBpp = ilGetInteger(IL_IMAGE_BYTES_PER_PIXEL);
   
   int numPixels = width * height;
   for(int i = 0; i < numPixels; i++) {
      outData[3] = alphaData[0];
      alphaData += alphaBpp;
      outData += outBpp;
   }
   
   GLuint glid;
   glGenTextures(1, &glid);
   glBindTexture(GL_TEXTURE_2D, glid);
   glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, ilGetData());
   
   glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

   ilDeleteImages(3, ids);
   
   fprintf(stderr, "Image with mask %s loaded. ID = %d\n", imageF, glid);

   return glid;
}
