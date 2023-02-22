#ifndef _MODEL_H_
#define _MODEL_H_

#include "Drawable.h"
#include <string>

using namespace std;

class Orbit;

class Model : public Drawable {
   
public:
   Model();
   virtual ~Model();

   virtual void initialize() {}
   virtual void draw(const Matrix4x4& m);
   virtual void setHash(Hash* h);
   virtual void createPath() {}
   virtual void drawPath() {}
   virtual void calculateLocation(double days);
   
private:
   string name;
   Orbit* orbit;
   string parent;
};

#endif
