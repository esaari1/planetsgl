/**
 *  Map.h
 */

#ifndef _MAP_H_
#define _MAP_H_

#include "Scene.h"
#include "geometry/Point3D.h"

class Map : public Scene {
   
public:
   Map();
   virtual ~Map();
   
   virtual void setup();
   virtual void draw();
   virtual void handleKeydown(SDL_KeyboardEvent &event);
   virtual void handleKeyup(SDL_KeyboardEvent &event);
   
private:
   void gotoPlanet(int pidx);

   Point3D eye, center;
   double pitch, roll;
   double pitchVel, rollVel;
   short zoomMult;
};

#endif
