#include "utility/ShaderManager.h"
#include "Model.h"
#include "mesh/MeshManager.h"
#include "DrawableManager.h"
#include "physics/PlanetOrbit.h"
#include "parser/Hash.h"
#include "physics/time_funcs.h"
#include "geometry/Matrix.h"

Model::Model() : parent("")
{
   radius = 5.0;
}

Model::~Model() {
   delete orbit;
}

void Model::calculateLocation(double days) {
   location = orbit->calculateLocation(days);
   if(!parent.empty()) {
      Drawable* d = DrawableManager::instance().getDrawable(parent);
      location.x += d->getLocation().x;
      location.y += d->getLocation().y;
      location.z += d->getLocation().z;      
   }
}

void Model::draw(const Matrix4x4& m) {
   ShaderManager::instance().setActiveShader("none");
   Point3D location  = getLocation();

   glPushMatrix();
   Matrix4x4 t;
   t.Translate(location.x, location.y, location.z);
   t = m * t;
   glLoadMatrixd(t.Get());
   
   glActiveTexture(GL_TEXTURE0);
   glCallList(MeshManager::instance().getMesh(name));

   glPopMatrix();
}

void Model::setHash(Hash* h) {
   name = h->getString("Name");

   orbit = new PlanetOrbit(h->getDouble("Period"),
                     h->getDouble("LongAtEpoch"),
                     h->getDouble("LongOfPerihelion"),
                     h->getDouble("Eccentricity"),
                     h->getDouble("SemiMajorAxis"),
                     h->getDouble("Inclination"),
                     h->getDouble("AscendingNode"));
   
   if(h->contains("Parent")) {
      parent = h->getString("Parent");
   }
}
