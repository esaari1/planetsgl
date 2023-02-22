//============================================================================
//  Johns Hopkins University Whiting School of Engineering
//  605.761 Applied Computer Graphics
//  Instructor: David W. Nesbitt
//
//  Author:  David W. Nesbitt
//  File:    Matrix.h
//  Purpose: Matrix class
//          Student should include "geometry.h" to get all 
//          class definitions included in proper order.
//
//============================================================================

#include "Matrix.h"
#include "Point3D.h"
#include "Vector3D.h"
#include "constants.h"

Matrix4x4::Matrix4x4() {
   SetIdentity();
}

Matrix4x4::Matrix4x4(double e0, double e1, double e2, double e3,
                     double e4, double e5, double e6, double e7,
                     double e8, double e9, double e10, double e11,
                     double e12, double e13, double e14, double e15)
{
   a[0]=e0;
   a[1]=e1;
   a[2]=e2;
   a[3]=e3;
   a[4]=e4;
   a[5]=e5;
   a[6]=e6;
   a[7]=e7;
   a[8]=e8;
   a[9]=e9;
   a[10]=e10;
   a[11]=e11;
   a[12]=e12;
   a[13]=e13;
   a[14]=e14;
   a[15]=e15;
}

Matrix4x4::Matrix4x4(const Matrix4x4& n) {
   m11() = n.m11(); m12() = n.m12(); m13() = n.m13(); m14() = n.m14();
   m21() = n.m21(); m22() = n.m22(); m23() = n.m23(); m24() = n.m24();
   m31() = n.m31(); m32() = n.m32(); m33() = n.m33(); m34() = n.m34();
   m41() = n.m41(); m42() = n.m42(); m43() = n.m43(); m44() = n.m44();
}

void Matrix4x4::SetIdentity() {
   a[0] = 1.0f; a[4] = 0.0f; a[8]  = 0.0f; a[12] = 0.0f;
   a[1] = 0.0f; a[5] = 1.0f; a[9]  = 0.0f; a[13] = 0.0f;
   a[2] = 0.0f; a[6] = 0.0f; a[10] = 1.0f; a[14] = 0.0f;
   a[3] = 0.0f; a[7] = 0.0f; a[11] = 0.0f; a[15] = 1.0f;
}

Matrix4x4& Matrix4x4::operator = (const Matrix4x4& n) {
   m11() = n.m11(); m12() = n.m12(); m13() = n.m13(); m14() = n.m14();
   m21() = n.m21(); m22() = n.m22(); m23() = n.m23(); m24() = n.m24();
   m31() = n.m31(); m32() = n.m32(); m33() = n.m33(); m34() = n.m34();
   m41() = n.m41(); m42() = n.m42(); m43() = n.m43(); m44() = n.m44();
   return *this;
}

bool Matrix4x4::operator == (const Matrix4x4& n) {
   return (m11() == n.m11() && m12() == n.m12() && m13() == n.m13() && m14() == n.m14() &&
           m21() == n.m21() && m22() == n.m22() && m23() == n.m23() && m24() == n.m24() &&
           m31() == n.m31() && m32() == n.m32() && m33() == n.m33() && m34() == n.m34() &&
           m41() == n.m41() && m42() == n.m42() && m43() == n.m43() && m44() == n.m44());
}

void Matrix4x4::Set(const double* m) {
   for (int i = 0; i < 16; i++) {
      a[i] = m[i];
   }
}

double Matrix4x4::m(const unsigned int row, const unsigned int col) const {
   return (row < 4 && col < 4) ? a[col * 4 + row] : 0.0f;
}

double& Matrix4x4::m(const unsigned int row, const unsigned int col) {
   return (row < 4 && col < 4) ? a[col * 4 + row]: a[0];
}

Matrix4x4 Matrix4x4::operator * (const Matrix4x4& n) const
{
   // Unroll the loop, do 1 row at a time.
   Matrix4x4 t;
   double a0 = m11();
   double a1 = m12();
   double a2 = m13();
   double a3 = m14();
   t.m11() = a0 * n.m11() + a1 * n.m21() + a2 * n.m31() + a3 * n.m41();
   t.m12() = a0 * n.m12() + a1 * n.m22() + a2 * n.m32() + a3 * n.m42();
   t.m13() = a0 * n.m13() + a1 * n.m23() + a2 * n.m33() + a3 * n.m43();
   t.m14() = a0 * n.m14() + a1 * n.m24() + a2 * n.m34() + a3 * n.m44();
   a0 = m21();
   a1 = m22();
   a2 = m23();
   a3 = m24();
   t.m21() = a0 * n.m11() + a1 * n.m21() + a2 * n.m31() + a3 * n.m41();
   t.m22() = a0 * n.m12() + a1 * n.m22() + a2 * n.m32() + a3 * n.m42();
   t.m23() = a0 * n.m13() + a1 * n.m23() + a2 * n.m33() + a3 * n.m43();
   t.m24() = a0 * n.m14() + a1 * n.m24() + a2 * n.m34() + a3 * n.m44();
   a0 = m31();
   a1 = m32();
   a2 = m33();
   a3 = m34();
   t.m31() = a0 * n.m11() + a1 * n.m21() + a2 * n.m31() + a3 * n.m41();
   t.m32() = a0 * n.m12() + a1 * n.m22() + a2 * n.m32() + a3 * n.m42();
   t.m33() = a0 * n.m13() + a1 * n.m23() + a2 * n.m33() + a3 * n.m43();
   t.m34() = a0 * n.m14() + a1 * n.m24() + a2 * n.m34() + a3 * n.m44();
   a0 = m41();
   a1 = m42();
   a2 = m43();
   a3 = m44();
   t.m41() = a0 * n.m11() + a1 * n.m21() + a2 * n.m31() + a3 * n.m41();
   t.m42() = a0 * n.m12() + a1 * n.m22() + a2 * n.m32() + a3 * n.m42();
   t.m43() = a0 * n.m13() + a1 * n.m23() + a2 * n.m33() + a3 * n.m43();
   t.m44() = a0 * n.m14() + a1 * n.m24() + a2 * n.m34() + a3 * n.m44();
   return t;
}

Matrix4x4& Matrix4x4::operator *= (const Matrix4x4& n) {
   *this = *this * n;
   return *this;
}

Matrix4x4 & Matrix4x4::operator *= (const double s) {
   for (int r = 0; r < 16; r++)
      a[r] *= s;
   
   return *this;
}

Point3D Matrix4x4::operator *(const Point3D& v) const {
   return Point3D((a[0] * v.x + a[4] * v.y + a[8]  * v.z + a[12]),
                 (a[1] * v.x + a[5] * v.y + a[9]  * v.z + a[13]),
                 (a[2] * v.x + a[6] * v.y + a[10] * v.z + a[14]));
}

Vector3D Matrix4x4::operator *(const Vector3D& v) const {
   return Vector3D((a[0] * v.x + a[4] * v.y + a[8]  * v.z),
                   (a[1] * v.x + a[5] * v.y + a[9]  * v.z),
                   (a[2] * v.x + a[6] * v.y + a[10] * v.z));
}

Matrix4x4& Matrix4x4::Transpose() {
   Matrix4x4 t;
   t.m11() = m11();
   t.m12() = m21();
   t.m13() = m31();
   t.m14() = m41();
   t.m21() = m12();
   t.m22() = m22();
   t.m23() = m32();
   t.m24() = m42();
   t.m31() = m13();
   t.m32() = m23();
   t.m33() = m33();
   t.m34() = m43();
   t.m41() = m14();
   t.m42() = m24();
   t.m43() = m34();
   t.m44() = m44();
   *this = t;
   return *this;
}

Matrix4x4 Matrix4x4::Inverse(void) const {
   int j, k;
   int ind;
   double v1, v2;
   Matrix4x4 t = *this;
   Matrix4x4 b;
   for (int i = 0; i < 4; i++)
   {
      // Find pivot
      v1 = t.m(i, i);
      ind = i;
      for (j = i + 1; j < 4; j++)
      {
         if (fabs(t.m(j, i)) > fabs(v1))
         {
            ind = j;
            v1 = t.m(j, i);
         }
      }
      
      // Swap columns
      if (ind != i)
      {
         for (j = 0; j < 4; j++)
         {
            v2 = b.m(i, j);
            b.m(i, j) = b.m(ind, j);
            b.m(ind, j) = v2;
            v2 = t.m(i, j);
            t.m(i, j) = t.m(ind, j);
            t.m(ind, j) = v2;
         }
      }
      
      // The matrix is singular (has no inverse), set the inverse
      //    to the identity matrix.
      if (v1 == 0.0f)
      {
         b.SetIdentity();
         return b;
      }
      
      for (j = 0; j < 4; j++)
      {
         t.m(i, j) /= v1;
         b.m(i, j) /= v1;
      }
      
      // Eliminate column
      for (j = 0; j < 4; j++)
      {
         if (j == i)
            continue;
         
         v1 = t.m(j, i);
         for (k = 0; k < 4; k++)
         {
            t.m(j, k) -= t.m(i, k) * v1;
            b.m(j, k) -= b.m(i, k) * v1;
         }
      }
   }
   return b;
}

void Matrix4x4::Translate(const double x, const double y, const double z) {
   // Set the translation matrix elements.
   m14() += m11() * x + m12() * y + m13() * z;
   m24() += m21() * x + m22() * y + m23() * z;
   m34() += m31() * x + m32() * y + m33() * z;
   m44() += m41() * x + m42() * y + m43() * z;
}

void Matrix4x4::Scale(const double x, const double y, const double z) {
   // Set the scale matrix elements.
   m11() *= x;
   m21() *= x;	
   m31() *= x;	
   m41() *= x;
   m12() *= y;	
   m22() *= y;	
   m32() *= y;	
   m42() *= y;	
   m13() *= z;
   m23() *= z;
   m33() *= z;	
   m43() *= z;
}

void Matrix4x4::Rotate(const double angle, const double x, const double y, const double z) {
   // The rotation matrix
   Matrix4x4 r;
   
   // Handle simple cases of rotation about a single axis
   if (y == 0.0f && z == 0.0f)
   {
      double radians = angle * DEG_TO_RAD;
      double cosa = (double)cos(radians);
      double sina = (double)sin(radians);
      r.m22() = cosa;		r.m23() = -sina;
      r.m32() = sina;		r.m33() = cosa;
   }
   if (x == 0.0f && y == 0.0f)
   {
      double radians = angle * DEG_TO_RAD;
      double cosa = (double)cos(radians);
      double sina = (double)sin(radians);
      r.m11() = cosa;		r.m12()  = -sina;
      r.m21() = sina;		r.m22()  = cosa;
   }
   if (x == 0.0f && z == 0.0f)
   {
      double radians = angle * DEG_TO_RAD;
      double cosa = (double)cos(radians);
      double sina = (double)sin(radians);
      r.m11() =  cosa;	r.m13() = sina;
      r.m31() = -sina;	r.m33() = cosa;
   }
   else
   {
      // Set up the standard rotation using quaternions
      // Scalar part
      double s = (double)cos(angle * 0.5f * DEG_TO_RAD);
      
      // Vector part: a normalized direction vector
      Vector3D v(x, y, z);
      v.normalize();
      v *= (double)sin(angle * 0.5f * DEG_TO_RAD);
      double a = v.x;
      double b = v.y;
      double c = v.z;
      
      // Set the composite rotation matrix
      r.m11() = 1.0f - 2.0f * b * b - 2.0f * c * c;
      r.m12() = 2.0f * a * b - 2.0f * s * c;
      r.m13() = 2.0f * a * c + 2.0f * s * b;
      r.m21() = 2.0f * a * b + 2.0f * s * c;
      r.m22() = 1.0f - 2.0f * a * a - 2.0f * c * c;
      r.m23() = 2.0f * b * c - 2.0f * s * a;
      r.m31() = 2.0f * a * c - 2.0f * s * b;
      r.m32() = 2.0f * b * c + 2.0f * s * a;
      r.m33() = 1.0f - 2.0f * a * a - 2.0f * b * b;
      r.m44() = 1.0f;
   }
   
   // Postmultiply the current matrix
   *this *= r;
}
