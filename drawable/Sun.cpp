/**
 *  Sun.cpp
 */

#include "utility/ShaderManager.h"
#include "Sun.h"
#include "utility/DevILHelper.h"
#include "mesh/MeshManager.h"
#include "parser/Hash.h"
#include "parser/Value.h"
#include "utility/OptionsManager.h"

Sun::Sun() {
   location.set(0, 0, 0);
}

Sun::~Sun() {
   delete[] sunColor;
}

void Sun::setHash(Hash* h) {
   if(h->contains("Texture")) {
      glActiveTexture(GL_TEXTURE0);
      sunTex = DevILHelper::instance().loadImage(h->getString("Texture").c_str());
   }
   
   Array* color = h->getValue("Color")->getArray();
   sunColor = new GLfloat[color->size()];
   for(unsigned int i = 0; i < color->size(); i++) {
      sunColor[i] = color->at(i)->getDouble();
   }
   
   name = h->getString("Name");
   radius = h->getDouble("Radius") * OptionsManager::instance().getOptions()->getDouble("scale");
   coronaRadius = h->getDouble("CoronaRadius") * OptionsManager::instance().getOptions()->getDouble("scale");
}

void Sun::draw(const Matrix4x4& m) {
   // Render the main sphere
   glPushAttrib(GL_LIGHTING_BIT | GL_CURRENT_BIT);
   glMaterialfv(GL_FRONT, GL_EMISSION, sunColor);
   
   glActiveTexture(GL_TEXTURE0);
   glBindTexture(GL_TEXTURE_2D, sunTex);
   glEnable(GL_TEXTURE_2D);
   
   ShaderManager::instance().setActiveShader("none");
   
   glPushMatrix();
   glScalef(radius, radius, radius);
   glCallList(MeshManager::instance().getMesh("sphere15"));
   glPopMatrix();
   
   glDisable(GL_TEXTURE_2D);
      
   // Render the corona
   ShaderManager::instance().setActiveShader("corona");
   GLint loc = ShaderManager::instance().getUniformLocation("corona", "color");
   glUniform3f(loc, sunColor[0], sunColor[1], sunColor[2]);
   
   glDisable(GL_LIGHTING);
   glEnable(GL_BLEND);
   
   billboard();

   glBegin(GL_QUADS);
   glTexCoord2i(0, 0);  glVertex3f(-(right[0] + up[0]) * coronaRadius, -(right[1] + up[1]) * coronaRadius, -(right[2] + up[2]) * coronaRadius);
   glTexCoord2i(1, 0);  glVertex3f((right[0] - up[0]) * coronaRadius, (right[1] - up[1]) * coronaRadius, (right[2] - up[2]) * coronaRadius);
   glTexCoord2i(1, 1);  glVertex3f((right[0] + up[0]) * coronaRadius, (right[1] + up[1]) * coronaRadius, (right[2] + up[2]) * coronaRadius);
   glTexCoord2i(0, 1);  glVertex3f(-(right[0] - up[0]) * coronaRadius, -(right[1] - up[1]) * coronaRadius, -(right[2] - up[2]) * coronaRadius);
   glEnd();
   
   // Render the corona sphere
   ShaderManager::instance().setActiveShader("sun");
   loc = ShaderManager::instance().getUniformLocation("sun", "color");
   glUniform3f(loc, sunColor[0], sunColor[1], sunColor[2]);
   
   glColor4f(sunColor[0], sunColor[1], sunColor[2], 0.5);

   glPushMatrix();
   glScalef(radius+100, radius+100, radius+100);
   glCallList(MeshManager::instance().getMesh("sphere15"));
   glPopMatrix();
   
   sunColor[3] = 1.0;
   glDisable(GL_BLEND);
   glEnable(GL_LIGHTING);
   
   glPopAttrib();
}

void Sun::billboard() {
	double modelview[16];
   
   glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
   
	right[0] = modelview[0];
	right[1] = modelview[4];
	right[2] = modelview[8];
   
	up[0] = modelview[1];
	up[1] = modelview[5];
	up[2] = modelview[9];
}
