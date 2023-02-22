/**
 *  Sun.h
 */

#include "Drawable.h"

class Sun : public Drawable {
   
public:
   Sun();
   virtual ~Sun();

   virtual void initialize() {}
   virtual void draw(const Matrix4x4& m);
   virtual void setHash(Hash* h);
   virtual void createPath() {}
   virtual void drawPath() {}
   virtual void calculateLocation(double days) {}
   
private:
   GLfloat *sunColor;
   GLuint sunTex;
   double up[3], right[3];
   double coronaRadius;
   
   void billboard();
};
