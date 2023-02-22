// body.cpp
//
// Copyright (C) 2009, Eric Saari
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.

#ifdef WIN32
#define GLEW_STATIC 1
#include <GL/glew.h>
#include <GL/gl.h>
#else
#include <GLUT/glut.h>
#endif

#include "Body.h"
#include "DrawableManager.h"
#include "parser/Hash.h"
#include "parser/Value.h"
#include "physics/PlanetOrbit.h"
#include "physics/MoonOrbit.h"
#include "mesh/MeshManager.h"
#include "utility/OptionsManager.h"
#include "utility/ShaderManager.h"
#include "utility/DevILHelper.h"
#include "physics/time_funcs.h"
#include "geometry/Vector3D.h"
#include "constants.h"

static const double SQRT_HALF = sqrt(0.5);

Body::Body() : 
   axisTilt(0.0),
   atmosphere(NULL),
   rings(NULL),
   shader("none")
{
}

Body::~Body() {
   delete orbit;
}

void Body::initialize() {
//   location = orbit->calculateLocation(daysSinceEpoch());
   createPath();
}

void Body::setHash(Hash* h) {
   if(h->contains("Shader")) {
      shader = h->getString("Shader");
   }
   ShaderManager::instance().setActiveShader(shader);
   
   name = h->getString("Name");

   if(h->contains("Texture")) {
      glActiveTexture(GL_TEXTURE0);
      textures[GL_TEXTURE0] = DevILHelper::instance().loadImage(h->getString("Texture").c_str());
      mapTexture = textures[GL_TEXTURE0];
   }
   
   if(h->contains("Textures")) {
      Array* uniforms = h->getValue("Textures")->getArray();
      for(unsigned int i = 0; i < uniforms->size(); i++) {
         string s = uniforms->at(i)->getString();
         GLint texLoc = ShaderManager::instance().getUniformLocation(shader.c_str(), s.c_str());
         glUniform1i(texLoc, i);
         
         string fname = h->getString(s.c_str());
         glActiveTexture(GL_TEXTURE0 + i);
         textures[GL_TEXTURE0 + i] = DevILHelper::instance().loadImage(fname.c_str());
      }
   }
   
   if(h->contains("MapTexture")) {
      ShaderManager::instance().setActiveShader("none");
      glActiveTexture(GL_TEXTURE0);
      mapTexture = DevILHelper::instance().loadImage(h->getString("MapTexture").c_str());
   }

   radius = h->getDouble("Radius") * OptionsManager::instance().getOptions()->getDouble("scale");
   
   if(h->contains("Parent")) {
      parent = h->getString("Parent");
   }

   if(h->contains("Orbit")) {
      string orbitName = h->getString("Orbit");
      if(orbitName == "MoonOrbit") {
         orbit = new MoonOrbit(h);
      }
   }
   else {
      orbit = new PlanetOrbit(h->getDouble("Period"),
                              h->getDouble("LongAtEpoch"),
                              h->getDouble("LongOfPerihelion"),
                              h->getDouble("Eccentricity"),
                              h->getDouble("SemiMajorAxis"),
                              h->getDouble("Inclination"),
                              h->getDouble("AscendingNode"));
   }
//   location = orbit->calculateLocation(daysSinceEpoch());

   if(h->contains("Atmosphere")) {
      atmosphere = h->getValue("Atmosphere")->getHash();
   }
   
   if(h->contains("Rings")) {
      ShaderManager::instance().setActiveShader("rings");
      rings = h->getValue("Rings")->getHash();

      GLint texLoc = ShaderManager::instance().getUniformLocation("rings", "Rings");
      glUniform1i(texLoc, 1);

      glActiveTexture(GL_TEXTURE1);
      ringTexture = DevILHelper::instance().loadImage(rings->getString("Texture").c_str());
   }
   
   if(h->contains("AxisTilt")) {
      axisTilt = h->getValue("AxisTilt")->getDouble();
   }
}

void Body::calculateLocation(double days) {
   location = orbit->calculateLocation(days);
   if(!parent.empty()) {
      Drawable* d = DrawableManager::instance().getDrawable(parent);
      location.x += d->getLocation().x;
      location.y += d->getLocation().y;
      location.z += d->getLocation().z;      
   }
}

void Body::draw(const Matrix4x4& m) {
   ShaderManager::instance().setActiveShader(shader);

   if(shader == "none") {
      glActiveTexture(GL_TEXTURE0);
      glEnable(GL_TEXTURE_2D);
      glBindTexture(GL_TEXTURE_2D, textures[GL_TEXTURE0]);
      glEnable(GL_RESCALE_NORMAL);
   }
   else {
      for(textureIter it = textures.begin(); it != textures.end(); it++) {
         glActiveTexture((*it).first);
         glBindTexture(GL_TEXTURE_2D, (*it).second);
      }
   }

   glPushMatrix();
   Matrix4x4 t;
   t.Translate(location.x, location.y, location.z);
   t = m * t;
   glLoadMatrixd(t.Get());

   glPushMatrix();
   glScalef(radius, radius, radius);
   glRotated(axisTilt, 1, 0, 0);
   glCallList(MeshManager::instance().getMesh("sphere15"));
   glPopMatrix();
   
   if(atmosphere != NULL) {
      drawAtmosphere();
   }
   
   if(shader == "none") {
      glDisable(GL_TEXTURE_2D);
      glDisable(GL_RESCALE_NORMAL);
   }
   
   if(rings != NULL) {
      glRotated(axisTilt, 1, 0, 0);
      drawRings(location);
   }
   
   glPopMatrix();
}

void Body::drawBasic() {
   ShaderManager::instance().setActiveShader("none");
   
   glActiveTexture(GL_TEXTURE0);
   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D, mapTexture);

   glEnable(GL_RESCALE_NORMAL);
   
   glPushMatrix();
//   glScalef(radius, radius, radius);
   glCallList(MeshManager::instance().getMesh("sphere5"));
   glPopMatrix();
   
   glDisable(GL_TEXTURE_2D);
   glDisable(GL_RESCALE_NORMAL);

   if(rings != NULL) {
      drawRings(location);
   }
}

void Body::drawPath() {
   glCallList(pathList);
}

void Body::drawAtmosphere() {
   ShaderManager::instance().setActiveShader("atmosphere");
   
   Array* color = atmosphere->getValue("Color")->getArray();
   GLint loc = ShaderManager::instance().getUniformLocation("atmosphere", "color");
   glUniform3f(loc, color->at(0)->getDouble(), color->at(1)->getDouble(), color->at(2)->getDouble());
   
   glEnable(GL_BLEND);
   
   double atmoRadius = radius + atmosphere->getDouble("Height") * OptionsManager::instance().getOptions()->getDouble("scale");
   glScalef(atmoRadius, atmoRadius, atmoRadius);
   glCallList(MeshManager::instance().getMesh("sphere15"));
   
   glDisable(GL_BLEND);
}

void Body::drawRings(const Point3D& location) {
   ShaderManager::instance().setActiveShader("rings");

   double inner = rings->getValue("Inner")->getDouble() * OptionsManager::instance().getOptions()->getDouble("scale");
   double outer = rings->getValue("Outer")->getDouble() * OptionsManager::instance().getOptions()->getDouble("scale");
   
   double opp = sqrt(pow(inner, 2.0) - pow(radius, 2.0));
   double thetaInner = -opp / inner;
   
   opp = sqrt(pow(outer, 2.0) - pow(radius, 2.0));
   double thetaOuter = -opp / outer;

   glActiveTexture(GL_TEXTURE1);
   glBindTexture(GL_TEXTURE_2D, ringTexture);
   
   string ringPlane = rings->getValue("Plane")->getString();
   
   GLint loc = ShaderManager::instance().getUniformLocation("rings", "inner");
   glUniform1f(loc, inner * 0.5 / outer);
   
   loc = ShaderManager::instance().getUniformLocation("rings", "planetCenter");
   glUniform4f(loc, location.x, location.y, location.z, 1.0);

   loc = ShaderManager::instance().getUniformLocation("rings", "thetaInner");
   glUniform1f(loc, thetaInner);

   loc = ShaderManager::instance().getUniformLocation("rings", "thetaOuter");
   glUniform1f(loc, thetaOuter);

   loc = ShaderManager::instance().getUniformLocation("rings", "radius");
   glUniform1f(loc, radius * 0.5 / outer);

   loc = ShaderManager::instance().getUniformLocation("rings", "shadow");
   glUniform1i(loc, (ringPlane == "yz") ? 0 : 1);

   glEnable(GL_BLEND);
   glDisable(GL_CULL_FACE);
   glBegin(GL_QUADS);
   
   if(ringPlane == "xz") {
      glTexCoord2i(0, 0);  glNormal3f(-SQRT_HALF, 0, -SQRT_HALF);  glVertex3d(-outer, 0.0, -outer);
      glTexCoord2i(1, 0);  glNormal3f(SQRT_HALF, 0, -SQRT_HALF);   glVertex3d(outer, 0.0, -outer);
      glTexCoord2i(1, 1);  glNormal3f(SQRT_HALF, 0, SQRT_HALF);    glVertex3d(outer, 0.0, outer);
      glTexCoord2i(0, 1);  glNormal3f(-SQRT_HALF, 0, SQRT_HALF);   glVertex3d(-outer, 0.0, outer);
   }
   else if(ringPlane == "xy") {
      glTexCoord2i(0, 0);  glNormal3f(-SQRT_HALF, 0, -SQRT_HALF);  glVertex3d(-outer, -outer, 0);
      glTexCoord2i(1, 0);  glNormal3f(SQRT_HALF, 0, -SQRT_HALF);   glVertex3d(outer, -outer, 0);
      glTexCoord2i(1, 1);  glNormal3f(SQRT_HALF, 0, SQRT_HALF);    glVertex3d(outer, outer, 0);
      glTexCoord2i(0, 1);  glNormal3f(-SQRT_HALF, 0, SQRT_HALF);   glVertex3d(-outer, outer, 0);
   }
   else if(ringPlane == "yz") {
      glTexCoord2i(0, 0);  glNormal3f(-SQRT_HALF, 0, -SQRT_HALF);  glVertex3d(0, -outer, -outer);
      glTexCoord2i(1, 0);  glNormal3f(SQRT_HALF, 0, -SQRT_HALF);   glVertex3d(0, outer, -outer);
      glTexCoord2i(1, 1);  glNormal3f(SQRT_HALF, 0, SQRT_HALF);    glVertex3d(0, outer, outer);
      glTexCoord2i(0, 1);  glNormal3f(-SQRT_HALF, 0, SQRT_HALF);   glVertex3d(0, -outer, outer);
   }

   glEnd();
   glEnable(GL_CULL_FACE);
   glDisable(GL_BLEND);
   
   ShaderManager::instance().setActiveShader("none");
}

void Body::createPath() {
   int numPoints = (int)(orbit->getPeriod() * TROPICAL_YEAR);
   double day = daysSinceEpoch();

   pathList = glGenLists(1);
   glNewList(pathList, GL_COMPILE);

   glBegin(GL_LINE_LOOP);
   for(int i = 0; i < numPoints; i++) {
      Point3D curr = orbit->calculateLocation(day);
      if(!parent.empty()) {
         Drawable* d = DrawableManager::instance().getDrawable(parent);
         curr.x += d->getLocation().x;
         curr.y += d->getLocation().y;
         curr.z += d->getLocation().z;      
      }

      glVertex3d(curr.x, curr.y, curr.z);
      day++;
   }
   glEnd();

   glEndList();
}
