// Sphere.cpp
//
// Copyright (C) 2009, Eric Saari
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.

#include "Sphere.h"
#include "constants.h"
#include "geometry/Vector3D.h"
#include "geometry/Point3D.h"

Sphere::Sphere(int slices, int stacks) {
   int vertexCount = (slices * (stacks-1)) + 2;

   TriangleMesh* mesh = new TriangleMesh();
   addTriangleMesh(mesh);
   
   // Define the north pole
   mesh->addPoint(new Point3D(0, 1, 0));
   mesh->addNormal(new Vector3D(0, 1, 0));

   // Define non-polar points and normals
   int vertexIdx = 1;
   for(int stackIdx = 1; stackIdx < stacks; stackIdx++) {
      double v = (M_PI / 2.0) - (M_PI * stackIdx / stacks);
      double cosV = cos(v);

      for(int sliceIdx = 0; sliceIdx < slices; sliceIdx++) {
         double u = 2.0 * M_PI * sliceIdx / slices;
         
         mesh->addPoint(new Point3D(cosV * cos(u), sin(v), -cosV * sin(u)));
         mesh->addNormal(new Vector3D(cosV * cos(u), sin(v), -cosV * sin(u)));
         vertexIdx++;
      }
   }

   // Define the south pole
   mesh->addPoint(new Point3D(0, -1, 0));
   mesh->addNormal(new Vector3D(0, -1, 0));

   double fstacks = (double) stacks;
   double fslices = (double) slices;

   // Define the north pole faces
   for(int i = 1; i <= slices; i++) {
      int idx2 = (i % slices) + 1;
      Face* f = new Face(0, i, idx2);
      f->addTexCoord(new TexCoord((double) (i-1.0) / (fslices-1.0), 0.0f));
      f->addTexCoord(new TexCoord((double) (i-1.0) / fslices, 1.0 / fstacks));
      f->addTexCoord(new TexCoord((double) i / fslices, 1.0 / fstacks));
      mesh->addFace(f);
   }

   // Define non-polar faces
   for(int stackIdx = 0; stackIdx < stacks-2; stackIdx++) {
      for(int sliceIdx = 1; sliceIdx <= slices; sliceIdx++) {
         int idx1 = (stackIdx * slices) + sliceIdx;
         int idx2 = (stackIdx * slices) + sliceIdx + slices;
         int idx3 = (stackIdx * slices) + (sliceIdx + slices + 1);
         int idx4 = (stackIdx * slices) + sliceIdx + 1;
         if(sliceIdx == slices) {
            idx3 = (stackIdx * slices) + sliceIdx + 1;
            idx4 = (stackIdx * slices) + 1;
         }

         double fstackIdx = (double) stackIdx;
         double fsliceIdx = (double) sliceIdx;
         
         Face* f1 = new Face(idx1, idx2, idx3);
         f1->addTexCoord(new TexCoord((fsliceIdx - 1.0) / fslices, (fstackIdx + 1.0) / fstacks));
         f1->addTexCoord(new TexCoord((fsliceIdx - 1.0) / fslices, (fstackIdx + 2.0) / fstacks));
         f1->addTexCoord(new TexCoord(fsliceIdx / fslices, (fstackIdx + 2.0) / fstacks));
         mesh->addFace(f1);
   
         Face* f2 = new Face(idx1, idx3, idx4);
         f2->addTexCoord(new TexCoord((fsliceIdx - 1.0) / fslices, (fstackIdx + 1.0) / fstacks));
         f2->addTexCoord(new TexCoord(fsliceIdx / fslices, (fstackIdx + 2.0) / fstacks));
         f2->addTexCoord(new TexCoord(fsliceIdx / fslices, (fstackIdx + 1.0) / fstacks));
         mesh->addFace(f2);
      }
   }

   // Define south pole faces
   for(int i = 1; i <= slices; i++) {      
      int idx2 = vertexCount-1 - slices+i;
      if(i == slices) {
         idx2 = vertexCount-1 - slices;
      }
      
      double fi = (double)i;

      Face* south = new Face(vertexCount - 1, idx2, vertexCount - 1 - slices + i - 1);
      south->addTexCoord(new TexCoord((fi - 1.0) / (fslices - 1.0), 1.0));
      south->addTexCoord(new TexCoord(fi / fslices, (fstacks - 1.0) / fstacks));
      south->addTexCoord(new TexCoord((fi - 1.0) / fslices, (fstacks - 1.0) / fstacks));
      mesh->addFace(south);
   }
}
